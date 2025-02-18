/*
 * SPDX-FileCopyrightText: 2021-2022 Espressif Systems (Shanghai) CO LTD
 * SPDX-License-Identifier: Unlicense OR CC0-1.0
 * https://github.com/espressif/esp-idf/tree/master
 * https://github.com/espressif/esp-idf/tree/master/examples/peripherals/rmt/ir_nec_transceiver
 * Заменен на
 * https://github.com/huseyin-yildiz/ESP32-Idf-NEC-IR-Receiver
 */

#include "tsop.h"
#include "project_config.h"
  //#include "meAlarm.h"
#include "def_tasks.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "driver/rmt_rx.h"
#include "mTypes.h"
#include <driver/gpio.h>
#include "rLog.h"

QueueHandle_t _queueProc;
input_data_t _data;

static const char *TAG_RECEIVER = "TSOP";
#define ERR_CHECK(err, str) if (err != ESP_OK) ESP_LOGE(TAG_RECEIVER, "%s: #%d %s", str, err, esp_err_to_name(err));
#define ERR_GPIO_SET_MODE "Failed to set GPIO mode"
#define ERR_GPIO_SET_ISR  "Failed to set ISR handler"

static gpio_num_t _gpioRx = GPIO_NUM_MAX;

/**
 * @brief Saving NEC decode results
 */
static uint16_t s_nec_code_address;
static uint16_t s_nec_code_command;

/**
 * @brief Check whether a duration is within expected range
 */
static inline bool nec_check_in_range(uint32_t signal_duration, uint32_t spec_duration)
{
    return (signal_duration < (spec_duration + IR_NEC_DECODE_MARGIN)) &&
           (signal_duration > (spec_duration - IR_NEC_DECODE_MARGIN));
}

/**
 * @brief Check whether a RMT symbol represents NEC logic zero
 */
static bool nec_parse_logic0(rmt_symbol_word_t *rmt_nec_symbols)
{
    return nec_check_in_range(rmt_nec_symbols->duration0, NEC_PAYLOAD_ZERO_DURATION_0) &&
           nec_check_in_range(rmt_nec_symbols->duration1, NEC_PAYLOAD_ZERO_DURATION_1);
}

/**
 * @brief Check whether a RMT symbol represents NEC logic one
 */
static bool nec_parse_logic1(rmt_symbol_word_t *rmt_nec_symbols)
{
    return nec_check_in_range(rmt_nec_symbols->duration0, NEC_PAYLOAD_ONE_DURATION_0) &&
           nec_check_in_range(rmt_nec_symbols->duration1, NEC_PAYLOAD_ONE_DURATION_1);
}

/**
 * @brief Decode RMT symbols into NEC address and command
 */
static bool nec_parse_frame(rmt_symbol_word_t *rmt_nec_symbols)
{
    rmt_symbol_word_t *cur = rmt_nec_symbols;
    uint16_t address = 0;
    uint16_t command = 0;
    bool valid_leading_code = nec_check_in_range(cur->duration0, NEC_LEADING_CODE_DURATION_0) &&
                              nec_check_in_range(cur->duration1, NEC_LEADING_CODE_DURATION_1);
    if (!valid_leading_code) {
        return false;
    }
    cur++;
    for (int i = 0; i < 16; i++) {
        if (nec_parse_logic1(cur)) {
            address |= 1 << i;
        } else if (nec_parse_logic0(cur)) {
            address &= ~(1 << i);
        } else {
            return false;
        }
        cur++;
    }
    for (int i = 0; i < 16; i++) {
        if (nec_parse_logic1(cur)) {
            command |= 1 << i;
        } else if (nec_parse_logic0(cur)) {
            command &= ~(1 << i);
        } else {
            return false;
        }
        cur++;
    }
    // save address and command
    s_nec_code_address = address;
    s_nec_code_command = command;
    return true;
}

// /**
//  * @brief Check whether the RMT symbols represent NEC repeat code
//  */
// static bool nec_parse_frame_repeat(rmt_symbol_word_t *rmt_nec_symbols)
// {
//     return nec_check_in_range(rmt_nec_symbols->duration0, NEC_REPEAT_CODE_DURATION_0) &&
//            nec_check_in_range(rmt_nec_symbols->duration1, NEC_REPEAT_CODE_DURATION_1);
// }

/**
 * @brief Decode RMT symbols into NEC scan code and print the result
 */
static void parse_nec_frame(rmt_symbol_word_t *rmt_nec_symbols, size_t symbol_num)
{
    // printf("NEC frame start---\r\n");
    // for (size_t i = 0; i < symbol_num; i++) {
    //     printf("{%d:%d},{%d:%d}\r\n", rmt_nec_symbols[i].level0, rmt_nec_symbols[i].duration0,
    //            rmt_nec_symbols[i].level1, rmt_nec_symbols[i].duration1);
    // }
    // printf("---NEC frame end: ");

    /* decode RMT symbols */
    // switch (symbol_num) {
    // case 34: /* NEC normal frame */
    //     // if (nec_parse_frame(rmt_nec_symbols)) {
    //     //     printf("Address=%04X, Command=%04X\r\n", s_nec_code_address, s_nec_code_command);
    //     // }
    //   nec_parse_frame(rmt_nec_symbols);
    //   break;
    // case 2: /* NEC repeat frame */
    //     // if (nec_parse_frame_repeat(rmt_nec_symbols)) {
    //     //     printf("Address=%04X, Command=%04X, repeat\r\n", s_nec_code_address, s_nec_code_command);
    //     // }
    //   nec_parse_frame_repeat(rmt_nec_symbols);
    //   break;
    // default:
    //   printf("Unknown NEC frame\r\n");
    //   break;
    // }
  if (symbol_num == 34 || symbol_num == 2) nec_parse_frame(rmt_nec_symbols);
  else printf("Unknown NEC frame\r\n");
}

static bool rmt_rx_done_callback(rmt_channel_handle_t channel, const rmt_rx_done_event_data_t *edata, void *user_data)
{
    BaseType_t high_task_wakeup = pdFALSE;
    QueueHandle_t receive_queue = (QueueHandle_t)user_data;
    // send the received RMT symbols to the parser task
    xQueueSendFromISR(receive_queue, edata, &high_task_wakeup);
    return high_task_wakeup == pdTRUE;
}


// ------------------------------------------------------------------------
//                          Задача
// ------------------------------------------------------------------------
// static void IRAM_ATTR tsopHandler(void* arg)
static void tsopHandler( void *pvParameters )
{
  portBASE_TYPE xStatus;
  const TickType_t xTicksToWait = 300 / portTICK_PERIOD_MS;

  ESP_LOGI(TAG_RECEIVER, "create RMT RX channel");
  rmt_rx_channel_config_t rx_channel_cfg = 
  {
    .gpio_num = _gpioRx,
    .clk_src = RMT_CLK_SRC_DEFAULT,
    .resolution_hz = IR_RESOLUTION_HZ,
    .mem_block_symbols = 64, /* amount of RMT symbols that the channel can store at a time */
  };
  rmt_channel_handle_t rx_channel = NULL;
  ESP_ERROR_CHECK(rmt_new_rx_channel(&rx_channel_cfg, &rx_channel));

  ESP_LOGI(TAG_RECEIVER, "register RX done callback");
  QueueHandle_t receive_queue = xQueueCreate(1, sizeof(rmt_rx_done_event_data_t));
  assert(receive_queue);
  rmt_rx_event_callbacks_t cbs = { .on_recv_done = rmt_rx_done_callback };
  ESP_ERROR_CHECK(rmt_rx_register_event_callbacks(rx_channel, &cbs, receive_queue));

  /* the following timing requirement is based on NEC protocol */
  rmt_receive_config_t receive_config = 
  {
    .signal_range_min_ns = 1250,     /* the shortest duration for NEC signal is 
                      560us, 1250ns < 560us, valid signal won't be treated as noise */
    .signal_range_max_ns = 12000000, /* the longest duration for NEC signal is 
                      9000us, 12000000ns > 9000us, the receive won't stop early */
  };

  ESP_LOGI(TAG_RECEIVER, "enable RMT RX channel");
  // ESP_ERROR_CHECK(rmt_enable(tx_channel));
  ESP_ERROR_CHECK(rmt_enable(rx_channel));

  /* save the received RMT symbols */
  rmt_symbol_word_t raw_symbols[64]; /* 64 symbols should be sufficient for a standard NEC frame */
  rmt_rx_done_event_data_t rx_data;
  /* ready to receive */
  ESP_ERROR_CHECK(rmt_receive(rx_channel, raw_symbols, sizeof(raw_symbols), &receive_config));
  while (1) 
  {
    /* wait for RX done signal */
    if (xQueueReceive(receive_queue, &rx_data, pdMS_TO_TICKS(1000)) == pdPASS) 
    {
      /* parse the receive symbols and print the result */
      parse_nec_frame(rx_data.received_symbols, rx_data.num_symbols);
      //rlog_i(TAG_RECEIVER, "Address=%04X, Command=%04X\r\n", s_nec_code_address, s_nec_code_command);

      /* Завершено успешно, отправка данных во внешнюю очередь */
            rlog_i(TAG_RECEIVER, " _queueProc=%" PRIX32 "\n", _queueProc);
      if ( _queueProc )
      {
        _data.source = IDS_TSOP;                       /* Идентификатор источника */
        _data.rxIR.value = (s_nec_code_address << 16) | s_nec_code_command;  /* uint32_t value */
        
        rlog_i(TAG_RECEIVER, " value=%" PRIX32 "\n", _data.rxIR.value);

          //      // Очистка буфера ??

            /* Отправка в очередь.
            Второй параметр - адрес отправляемой структуры. Адрес передается
            как параметр задачи, для чего напрямую используется pvParameters.
            Третий параметр время блокировки - время, которое задача должна быть
            в состоянии Blocked, ожидая появления доступного места на очереди,
            если очередь уже полна. Время блокировки указано потому, что
            отправляющие задачи имеют более высокий приоритет, чем принимающая
            задача, так что ожидается, что очередь сразу заполнится. Принимающая
            задача будет запускаться и удалять элементы данных из очереди, когда
            обе отправляющие задачи находятся в состоянии Blocked. */
        xStatus = xQueueSend( _queueProc, pvParameters, xTicksToWait );

        if( xStatus != pdPASS )
        {
            /* Операция отправки не завершена, даже после истечения 100 мс.
                Это должно означать ошибку, так как принимающая задача должна
                освобождать место в очереди, как только обе отправляющие задачи
                войдут в состояние Blocked. */
           rlog_e(TAG_RECEIVER, "Could not send to the queue.\r\n" );
        }

        /* Разрешаем выполниться другой такой же задаче, отправляющей данные
            в очередь. */
        taskYIELD();

        //  xQueueSend(_queueProc, pvParameters, portMAX_DELAY);
        //if (xQueueSend( alarmTaskQueue, &data, ( TickType_t ) 0 ) == pdTRUE) {      }

      /* start receive again */
      ESP_ERROR_CHECK(rmt_receive(rx_channel, raw_symbols, sizeof(raw_symbols), &receive_config));
      }
    }
  }
  vTaskDelete(NULL);
}



void tsopStart(const uint8_t gpioRx, QueueHandle_t queueProc)
{
  _gpioRx = gpioRx;
  _queueProc = queueProc;

  //rlog_i(TAG_RECEIVER, "Initialization of IR receiver on gpio #%d", _gpioRx);
  rlog_i(TAG_RECEIVER, " queueProc=%" PRIX32 "\n", queueProc);

  #ifdef CONFIG_TSOP_STATIC_ALLOCATION
  static StackType_t tsopTaskStack[CONFIG_TSOP_TASK_STACK_SIZE];
  static StaticTask_t tsopTaskBuffer;

       xTaskCreateStatic(tsopHandler,
                         "tsop_task",
         CONFIG_TSOP_TASK_STACK_SIZE,
                              &_data,    // адрес структуры
         CONFIG_TASK_PRIORITY_ALARM + 1, //     CONFIG_TASK_PRIORITY_TSOP,
                       tsopTaskStack,
                     &tsopTaskBuffer);
  #else
              xTaskCreate(tsopHandler, 
                          "tsop_task", 
          CONFIG_TSOP_TASK_STACK_SIZE, 
                                NULL, 
            CONFIG_TASK_PRIORITY_TSOP, 
                                NULL);
  #endif
}
