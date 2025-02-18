/*
 * SPDX-FileCopyrightText: 2016-2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "slave.h"
#include "staff.h"


// void app_main(void)
// {
//   // Запуск службы modbus_slave
//   slaveTaskStart();
//   vTaskDelay(1);

//   // Запуск службы staff
// //  staffTaskStart();
// //  vTaskDelay(1);
// }

/*
   Модуль преобразования интерфейса
   2025 январь
*/

#include "stdlib.h"
#include "esp_log.h"
#include "project_config.h"
#include "def_consts.h"
#include "rLog.h"
#include "rStrings.h"
#include "reLed.h"
#include "reNvs.h"
#include "reStates.h"
#include "reParams.h"
#include "reEvents.h"
#include "reEsp32.h"
#include "reWiFi.h"
#include "reSNTP.h"
#include "reMqtt.h"
#include "reSysInfo.h"
#include "reScheduler.h"
#include "meCerts.h"
#if CONFIG_PINGER_ENABLE
  #include "rePinger.h"
#endif // CONFIG_PINGER_ENABLE
#if CONFIG_TELEGRAM_ENABLE
  #include "reTgSend.h"
#endif // CONFIG_TELEGRAM_ENABLE
  #if CONFIG_DATASEND_ENABLE
#include "reDataSend.h"
  #endif // CONFIG_DATASEND_ENABLE
#if defined(CONFIG_GPIO_BUZZER) && (CONFIG_GPIO_BUZZER > -1)
  #include "reBeep.h"
#endif // CONFIG_GPIO_BUZZER
// #include "meRmt.h"
#include "sensors.h"
//#include "mb_slave.h"
#include "slave.h"
#include "security.h"

// Главная функция
extern "C"
{ 
  void app_main(void) 
  {
    // Инициализируем логи и выводим версию прошивки
    rlog_empty();
    disbleEspIdfLogs();
    rloga_i("Firmware initialization, version %s", APP_VERSION);
    vTaskDelay(1);

    /* Регистрируем обработчики перезагрузки (всего можно добавить до 5 обработчиков, 
       1 - системный (отладка), остается 4 для приложений) */
    espRegisterSystemShutdownHandler(); // #1
    vTaskDelay(1);

    // Инициализация глобального хранилища сертификатов
    initTlsGlobalCAStore();
    vTaskDelay(1);

    // Инициализация системы хранения параметров
    paramsInit();
    vTaskDelay(1);

    // Инициализация светодиодов
    ledSysInit(CONFIG_GPIO_SYSTEM_LED, true, CONFIG_LED_TASK_STACK_SIZE, nullptr);
    ledSysOn(false);
    vTaskDelay(1);

    // Запускаем главный цикл событий
    eventLoopCreate();
    vTaskDelay(1);

    // Инициализация системы отслеживания состояния устройства
    statesInit(true);
    vTaskDelay(1);

    // Регистрируем службу "минутного таймера" и расписаний, но не запускаем
    schedulerEventHandlerRegister();
    vTaskDelay(1);

    #if CONFIG_PINGER_ENABLE
      // Регистрация службы периодической проверки внешних серверов и доступа к сети интернет
      pingerEventHandlerRegister();
      vTaskDelay(1);
    #endif // CONFIG_PINGER_ENABLE

    // Запуск и регистрация службы синхронизации времени
    sntpTaskCreate(true);
    vTaskDelay(1);

    // Запуск и регистрация MQTT клиента
    mqttTaskStart(true);
    vTaskDelay(1);

    // Регистрациция обработчиков событий для параметров
    paramsEventHandlerRegister();
    vTaskDelay(1);

    // Запуск и регистрация службы уведомлений в Telegram
    #if CONFIG_TELEGRAM_ENABLE
      tgTaskCreate();
      vTaskDelay(1);
    #endif // CONFIG_TELEGRAM_ENABLE

    // // Запуск службы отправки данных на внешние сервисы
    // #if CONFIG_DATASEND_ENABLE
    //   dsTaskCreate(false);
    //   vTaskDelay(1);
    // #endif // CONFIG_DATASEND_ENABLE

    // // Запуск службы пищалки
    // #if defined(CONFIG_GPIO_BUZZER) && (CONFIG_GPIO_BUZZER > -1)
    //   beepTaskCreate(CONFIG_GPIO_BUZZER);
    //   vTaskDelay(1);
    // #endif // CONFIG_GPIO_BUZZER

    // Запуск службы контроля температуры
    sensorsTaskStart();
    vTaskDelay(1);

    // // Запуск службы сигнализации
    // alarmStart();
    // vTaskDelay(1);

  // Запуск службы modbus_slave
  slaveTaskStart();
  vTaskDelay(1);

  // Запуск службы staff
//  staffTaskStart();
//  vTaskDelay(1);



    // Подключение к WiFi AP
    if (!wifiStart()) 
    {
      ledSysBlinkOn(1, 100, 250);
    };
  }
}
