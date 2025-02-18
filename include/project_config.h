/*
   ------------------------------------------------------------------------
   Файл конфигурации проекта, доступен из всех файлов проекта, в том числе 
   и библиотек
   ------------------------------------------------------------------------
*/

#pragma once

#include <stdint.h>
#include "esp_task.h"

// ------------------------------------------------------------------------
//                              Версии 
// ------------------------------------------------------------------------
#define APP_VERSION "SPN32-20250208.009"
// 20250208.009: 
// 20250208.08: Возврат sensors RAM: 21.4% Flash: 72.4%
// 20250204.05: tsop. 
// 20250131.05: tsop, TSOP_STATIC_ALLOCATION=1
// 20250131.04: CONFIG_NO_SENSORS 1


#define PCB54           // Плата прототипа

#ifdef PCB54
  // Номер выводов периферии, версия платы URC-2E.54
  #define CONFIG_GPIO_LED_RED        4    // Красный, катод на GND (7mA)
  #define CONFIG_GPIO_LED_GREEN      2    // Зелёный, катод на GND (5mA)
  #define CONFIG_GPIO_LED_BLUE      12    // Синий,   катод на GND (4mA)

  // Входы
  #define CONFIG_GPIO_RXIR          35    // Вход ИК датчика

  // #define CONFIG_GPIO_TXD1          16    // UART
  // #define CONFIG_GPIO_RXD1          17    // UART

  // #define CONFIG_GPIO_TEST_PAD      14    // Test 

  // Отсутствующие на плате ресурсы
  //#define CONFIG_GPIO_I2C           -1
  //#define CONFIG_GPIO_RX433         -1
  //#define CONFIG_GPIO_BUZZER        -1
  //#define CONFIG_GPIO_BUZZER_ACTIVE -1

#else       // Плата SPN.55
  // Светодиоды
  #define CONFIG_GPIO_LED_RED        4    // Красный, катод на GND (7mA)
  #define CONFIG_GPIO_LED_GREEN      2    // Зелёный, катод на GND (5mA)
  #define CONFIG_GPIO_LED_BLUE      27    // Синий,   катод на GND (4mA)

  // Входы
  #define CONFIG_GPIO_IR            19    // Вход ИК датчика

  // UART1
  #define CONFIG_UART1_RXD          25
  #define CONFIG_UART1_TXD          26
  #define CONFIG_UART1_RTS          33

  // UART2
  #define CONFIG_UART2_RXD          21
  #define CONFIG_UART2_TXD          23
  #define CONFIG_UART2_RTS          22

#endif

// ------------------------------------------------------------------------
//                             GPIO
// ------------------------------------------------------------------------
  // Светодиоды
  #define CONFIG_GPIO_SYSTEM_LED    CONFIG_GPIO_LED_BLUE    // Системный Синий
  #define CONFIG_GPIO_EXEC_LED      CONFIG_GPIO_LED_GREEN   // зел
  #define CONFIG_GPIO_ALARM_LED     CONFIG_GPIO_LED_RED     // Красный (мигающий?) сигнал аварии

  // Входы
  #define CONFIG_GPIO_TSOP          CONFIG_GPIO_RXIR        // Вход ИК датчика

  // // MODBUS
  // #define CONFIG_MB_UART_RXD        CONFIG_UART1_RXD      // 
  // #define CONFIG_MB_UART_TXD        CONFIG_UART1_TXD      // 
  // #define CONFIG_MB_UART_RTS        CONFIG_UART1_RTS      // 

  // // SP_NET
  // #define CONFIG_SP_UART_RXD        CONFIG_UART2_RXD      // 
  // #define CONFIG_SP_UART_TXD        CONFIG_UART2_TXD      // 
  // #define CONFIG_SP_UART_RTS        CONFIG_UART2_RTS      // 

// Перезапустить устройство при ошибках выделения памяти
#define CONFIG_HEAP_ALLOC_FAILED_RESTART 0
#define CONFIG_HEAP_ALLOC_FAILED_RETRY 0

/* Разрешить "тихий" режим. Тихий режим - это период времени суток, когда 
   блокируются вспышки светодиодов и звуки. */
#define CONFIG_SILENT_MODE_ENABLE 1
#define CONFIG_SILENT_MODE_EXTENDED 0
/* Интервал в формате H1M1H2M2. То есть интервал 21:00 -> 06:00 это 21000600 */
#define CONFIG_SILENT_MODE_INTERVAL 22000600UL

// ------------------------------------------------------------------------
//                                MODBUS 
// ------------------------------------------------------------------------
// Здесь можно указать вообще любые параметры, связанные с прикладной задачей устройства

// Интервал чтения данных с сенсоров в миллисекундах
#define CONFIG_SLAVE_TASK_CYCLE 30000
/* Использовать статическое выделение памяти под задачу и очередь. 
     Должен быть включен параметр CONFIG_FREERTOS_SUPPORT_STATIC_ALLOCATION! */
#define CONFIG_SLAVE_STATIC_ALLOCATION 1
// Размер стека для главной задачи
#define CONFIG_SLAVE_TASK_STACK_SIZE   4*1024
// Приоритеты прикладных задач
#define CONFIG_TASK_PRIORITY_SLAVE     10U
// Процессорное ядро прикладных задач
#define CONFIG_TASK_CORE_SLAVE         1




// ------------------------------------------------------------------------
//                          ИК пульт (TSOP4838)
// ------------------------------------------------------------------------
// Интервал чтения данных с пульта в миллисекундах
// #define CONFIG_RX_TASK_CYCLE 300 // уточнить
// /* Использовать статическое выделение памяти под задачу и очередь. 
//      Должен быть включен параметр CONFIG_FREERTOS_SUPPORT_STATIC_ALLOCATION! */
#define CONFIG_TSOP_STATIC_ALLOCATION 1 
// Размер стека для задачи
#define CONFIG_TSOP_TASK_STACK_SIZE   4098
// Размер очереди для задачи чтения данных с пульта
// #define CONFIG_ALARM_QUEUE_SIZE 32

#define IR_RESOLUTION_HZ     1000000 // 1MHz resolution, 1 tick = 1us
//#define IR_TX_GPIO_NUM       16
#define IR_RX_GPIO_NUM       CONFIG_GPIO_TSOP       //35
#define IR_NEC_DECODE_MARGIN 200     // Tolerance for parsing RMT symbols into bit stream

/**
 * @brief NEC timing spec
 */
#define NEC_LEADING_CODE_DURATION_0  9000
#define NEC_LEADING_CODE_DURATION_1  4500
#define NEC_PAYLOAD_ZERO_DURATION_0  560
#define NEC_PAYLOAD_ZERO_DURATION_1  560
#define NEC_PAYLOAD_ONE_DURATION_0   560
#define NEC_PAYLOAD_ONE_DURATION_1   1690
#define NEC_REPEAT_CODE_DURATION_0   9000
#define NEC_REPEAT_CODE_DURATION_1   2250


// -----------------------------------------------------------------------------------------------------------------------
// --------------------------------------------- EN - Temperature control ------------------------------------------------
// --------------------------------------------- RU - Контроль температуры -----------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------
// EN: Here you can specify any parameters related to the main task of the device
// RU: Здесь можно указать вообще любые параметры, связанные с прикладной задачей устройства

// EN: Interval of reading data from sensors in milliseconds
// RU: Интервал чтения данных с сенсоров в миллисекундах
#define CONFIG_SENSORS_TASK_CYCLE 30000
// EN: Use static memory allocation for the task and queue. CONFIG_FREERTOS_SUPPORT_STATIC_ALLOCATION must be enabled!
// RU: Использовать статическое выделение памяти под задачу и очередь. Должен быть включен параметр CONFIG_FREERTOS_SUPPORT_STATIC_ALLOCATION!
#define CONFIG_SENSORS_STATIC_ALLOCATION 1
// EN: Stack size for main task
// RU: Размер стека для главной задачи
#define CONFIG_SENSORS_TASK_STACK_SIZE 4*1024

// EN: Allow publishing of raw RAW data (no correction or filtering): 0 - only processed value, 1 - always both values, 2 - only when there is processing
// RU: Разрешить публикацию необработанных RAW-данных (без коррекции и фильтрации): 0 - только обработанное значение, 1 - всегда оба значения, 2 - только когда есть обработка
#define CONFIG_SENSOR_RAW_ENABLE 1
// EN: Allow publication of sensor status
// RU: Разрешить публикацию форматированных данных в виде строки
#define CONFIG_SENSOR_STRING_ENABLE 0
// EN: Allow the publication of the time stamp of reading data from the sensor
// RU: Разрешить публикацию отметки времени чтения данных с сенсора
#define CONFIG_SENSOR_TIMESTAMP_ENABLE 1
// EN: Allow the publication of formatted data as "value + time"
// RU: Разрешить публикацию форматированных данных в виде "значение + время"
#define CONFIG_SENSOR_TIMESTRING_ENABLE 1
// EN: Allow dew point calculation and publication
// RU: Разрешить вычисление и публикацию точки росы
#define CONFIG_SENSOR_DEWPOINT_ENABLE 0
// EN: Allow publishing of mixed value, for example "temperature + humidity"
// RU: Разрешить публикацию смешанного значения, например "температура + влажность"
#define CONFIG_SENSOR_DISPLAY_ENABLED 1
// EN: Allow publication of absolute minimum and maximum
// RU: Разрешить публикацию абсолютного минимума и максимума
#define CONFIG_SENSOR_EXTREMUMS_ENTIRELY_ENABLE 1
// EN: Allow publication of daily minimum and maximum
// RU: Разрешить публикацию ежедневного минимума и максимума
#define CONFIG_SENSOR_EXTREMUMS_DAILY_ENABLE 1
// EN: Allow publication of weekly minimum and maximum
// RU: Разрешить публикацию еженедельного минимума и максимума
#define CONFIG_SENSOR_EXTREMUMS_WEEKLY_ENABLE 1
// EN: Publish extremums only when they are changed
// RU: Публиковать экстеремумы только при их изменении
#define CONFIG_SENSOR_EXTREMUMS_OPTIMIZED 1



// ------------------------------------------------------------------------
//                           RU - Сигнализация 
// ------------------------------------------------------------------------
// EN: Use static memory allocation for the fire alarm task
// RU: Использовать статическое выделение памяти для задачи охранно-пожарной сигнализации
#define CONFIG_ALARM_STATIC_ALLOCATION 1

// EN: Stack size for the fire alarm task
// RU: Размер стека для задачи охранно-пожарной сигнализации
#define CONFIG_ALARM_STACK_SIZE 4098 * 2

// EN: Queue size for the fire alarm task
// RU: Размер очереди для задачи охранно-пожарной сигнализации
#define CONFIG_ALARM_QUEUE_SIZE 32

// RU: Топик устройства для ОПС
// #define CONFIG_ALARM_MQTT_DEVICE_TOPIC "home"

/* Публиковать состояние сенсоров ОПС в локальных топиках 
   для передачи на другие устройства */
#define CONFIG_ALARM_LOCAL_PUBLISH true

/* Схема топиков ОПС: 0 - %location%/config/security/mode; 
                      1 - %location%/%device%/config/security/mode */
#define CONFIG_ALARM_MQTT_DEVICE_MODE 0

/* Схема топиков ОПС: 0 - %location%/security/events/%zone%; 
                      1 - %location%/%device%/security/events/%zone% */
#define CONFIG_ALARM_MQTT_DEVICE_EVENTS 0

/* Схема топиков ОПС: 0 - %location%/security/status/%device%; 
                      1 - %location%/%device%/security/status */
#define CONFIG_ALARM_MQTT_DEVICE_STATUS 0

/* При отключении тревоги с пульта сразу же снять с охраны, 
   иначе отключить тревогу без снятия с охраны */
#define CONFIG_ALARM_TOGETHER_DISABLE_SIREN_AND_ALARM 1

// ------------------------------------------------------------------------
//                               WiFi сети 
// ------------------------------------------------------------------------
/* 
 *  Режим для одной сети
 * --------------------
 * Раскомментируйте CONFIG_WIFI_SSID и CONFIG_WIFI_PASS, чтобы отключить 
 * автоматическое переключение между wifi-сетями
 * */
// #define CONFIG_WIFI_SSID "HUAWEI-a6mB"
// #define CONFIG_WIFI_PASS "rmbDHxYK"
// #define CONFIG_WIFI_SSID "RG-WiFi-a272"
// #define CONFIG_WIFI_PASS "XPr2UkuhP6"
/** 
 * Режим для нескольких сетей
 * --------------------
 * Вы можете определенить от одной до пяти сетей. При невозможности 
 * подключиться к одной из сетей, ESP попытается поключиться к следующей. 
 * Номер успешного подключения запоминается и используется в дальнейшем
 *  (до следущего сбоя). 
 * Это позволяет переносить устройство из одного здания в другое, не 
 * перепрошивая и перенастраивая его. 
 * Просто заранее определите все возможные сети.
 * */
#define CONFIG_WIFI_2_SSID "RG-WiFi-a272"
#define CONFIG_WIFI_2_PASS "XPr2UkuhP6"
#define CONFIG_WIFI_SSID "HUAWEI-a6mB"
#define CONFIG_WIFI_PASS "rmbDHxYK"
// #define CONFIG_WIFI_3_SSID "WIFI3"
// #define CONFIG_WIFI_3_PASS "222222222"
// #define CONFIG_WIFI_4_SSID "WIFI4"
// #define CONFIG_WIFI_4_PASS "333333333"
// #define CONFIG_WIFI_5_SSID "WIFI5"
// #define CONFIG_WIFI_5_PASS "444444444"

/* Параметры WiFi подключения. Если закомментировать эти строки, будут 
    использованы параметры по умолчанию ESP-IDF */
// #define CONFIG_WIFI_STORAGE   WIFI_STORAGE_RAM
// #define CONFIG_WIFI_BANDWIDTH WIFI_BW_HT20

/* Перезапустить устройство, если нет подключения к WiFi более указанного
    времени в минутах. 
   Закомментируйте строку, если не нужно перезапускать устройство при длительном 
   отсутствии подключения к сети */
#define CONFIG_WIFI_TIMER_RESTART_DEVICE 60*24

/* Разрешить периодическую проверку доступности сети интернет с помошью пинга. 
    Иногда доступ в сеть может пропасть, но подключение к WiFi при этом работает. 
    В этом случае устройство приостановит все сетевые процессы. */
#define CONFIG_PINGER_ENABLE 1

/* Отключить иникацию сетевых ошибок (wifi, inetnet, openmon, tg...), так как 
    устройство не всегда подключено к сети */
#define CONFIG_OFFLINE_MODE 0



  

// ------------------------------------------------------------------------
//                            Общие параметры 
// ------------------------------------------------------------------------
// 
#define CONFIG_NO_SENSORS 0




// ------------------------------------------------------------------------
//                          MQTT клиент 
// ------------------------------------------------------------------------
/* Параметры MQTT-брокеров. Можно определить два брокера: основной и резервный
    CONFIG_MQTTx_TYPE :: 0 - public, 
                         1 - local, 
                         2 - gateway (CONFIG_MQTT1_HOST not used) */

/********************* primary server ************************/
#define CONFIG_MQTT1_TYPE 0
#define CONFIG_MQTT1_HOST "m8.wqtt.ru"
#define CONFIG_MQTT1_PING_CHECK 1
#define CONFIG_MQTT1_PING_CHECK_DURATION 250
#define CONFIG_MQTT1_PING_CHECK_LOSS 50.0
#define CONFIG_MQTT1_PING_CHECK_LIMIT 3
#define CONFIG_MQTT1_PORT_TCP 18668
#define CONFIG_MQTT1_PORT_TLS 18669
#define CONFIG_MQTT1_USERNAME "u_TMSO6A"
#define CONFIG_MQTT1_PASSWORD "xczIvuDx"
#define CONFIG_MQTT1_TLS_ENABLED 1
#define CONFIG_MQTT1_TLS_STORAGE CONFIG_DEFAULT_TLS_STORAGE
#define CONFIG_MQTT1_TLS_PEM_START CONFIG_DEFAULT_TLS_PEM_START
#define CONFIG_MQTT1_TLS_PEM_END CONFIG_DEFAULT_TLS_PEM_END
#define CONFIG_MQTT1_CLEAN_SESSION 1
#define CONFIG_MQTT1_AUTO_RECONNECT 1
#define CONFIG_MQTT1_KEEP_ALIVE 60
#define CONFIG_MQTT1_TIMEOUT 10000
#define CONFIG_MQTT1_RECONNECT 10000
#define CONFIG_MQTT1_CLIENTID "esp32_sp"
// #define CONFIG_MQTT1_LOC_PREFIX ""
// #define CONFIG_MQTT1_PUB_PREFIX ""
#define CONFIG_MQTT1_LOC_LOCATION "local/Olmoro"
#define CONFIG_MQTT1_PUB_LOCATION "Olmoro"
#define CONFIG_MQTT1_LOC_DEVICE "sp-net"
#define CONFIG_MQTT1_PUB_DEVICE "sp-net"

/****************** MQTT : pinger ********************/
// Разрешить публикацию результатов пинга на MQTT брокере
#define CONFIG_MQTT_PINGER_ENABLE 1
#if CONFIG_MQTT_PINGER_ENABLE
  // Название топика результатов пинга
  #define CONFIG_MQTT_PINGER_TOPIC "ping"
  #define CONFIG_MQTT_PINGER_LOCAL 0
  #define CONFIG_MQTT_PINGER_QOS 0
  #define CONFIG_MQTT_PINGER_RETAINED 1
  #define CONFIG_MQTT_PINGER_AS_PLAIN 0
  #define CONFIG_MQTT_PINGER_AS_JSON 1
#endif // CONFIG_MQTT_PINGER_ENABLE

/*************** MQTT : remote control ***************/
// Разрешить обработку входящих команд устройством
#define CONFIG_MQTT_COMMAND_ENABLE 1
// Разрешить OTA обновления через сторонний сервер
#define CONFIG_MQTT_OTA_ENABLE 1

/***************** MQTT : sensors ********************/
// Интервал публикации данных с сенсоров в секундах
#define CONFIG_MQTT_SENSORS_SEND_INTERVAL 600
// QOS для данных с сенсоров
#define CONFIG_MQTT_SENSORS_QOS 1
#define CONFIG_MQTT_SENSORS_LOCAL_QOS 2
// Сохранять на брокере последние отправленные данные
#define CONFIG_MQTT_SENSORS_RETAINED 1
#define CONFIG_MQTT_SENSORS_LOCAL_RETAINED 0


// ------------------------------------------------------------------------
//                  Отправка на свой хостинг 
// ------------------------------------------------------------------------
// Включить отправку данных на свой хостинг
#define CONFIG_HOSTING_ENABLE 0

// ------------------------------------------------------------------------
//                      Уведомления в Telegram 
// ------------------------------------------------------------------------
// Разрешить уведомления в Telegram (общий флаг)
#define CONFIG_TELEGRAM_ENABLE 1
// Токен бота API Telegram
#define CONFIG_TELEGRAM_TOKEN "7731157620:AAFO8eSNRMuGyqeMp5jPhU5mlQZpr_iHgoQ"
// Идентификатор чата или группы
#define CONFIG_TELEGRAM_CHAT_ID_MAIN     "-4568554589"
#define CONFIG_TELEGRAM_CHAT_ID_SERVICE  "-4539244144"
#define CONFIG_TELEGRAM_CHAT_ID_PARAMS   CONFIG_TELEGRAM_CHAT_ID_SERVICE
#define CONFIG_TELEGRAM_CHAT_ID_SECURITY CONFIG_TELEGRAM_CHAT_ID_MAIN
// Отправлять заголовок сообщения (название устройства, см. CONFIG_TELEGRAM_DEVICE)
#define CONFIG_TELEGRAM_TITLE_ENABLED 0
// Название устройства (публикуется в качестве заголовка сообщения)
#define CONFIG_TELEGRAM_DEVICE "🏡 ДОМ"

// ------------------------------------------------------------------------
//                              Уведомления 
// ------------------------------------------------------------------------
// Мигать системным светодиодом при отправке данных
#define CONFIG_SYSLED_MQTT_ACTIVITY 0
#define CONFIG_SYSLED_SEND_ACTIVITY 0
#define CONFIG_SYSLED_TELEGRAM_ACTIVITY 0
// Разрешить удаленную включение или отключение уведомлений без перепрошивки устройства
#define CONFIG_NOTIFY_TELEGRAM_CUSTOMIZABLE 1
// Уведомления в Telegram при переключении режима охраны
#define CONFIG_NOTIFY_TELEGRAM_ALARM_MODE_CHANGE 1
#define CONFIG_NOTIFY_TELEGRAM_ALARM_ALERT_MODE_CHANGE 1
// Разрешить уведомления в Telegram, если поступил сигнал от датчика ОПС, который не зарегистрирован в системе
#define CONFIG_NOTIFY_TELEGRAM_ALARM_SENSOR_UNDEFINED 0
#define CONFIG_NOTIFY_TELEGRAM_ALARM_ALERT_SENSOR_UNDEFINED 0
// Разрешить уведомления в Telegram, если поступила незарегистрованная команда от датчика ОПС
#define CONFIG_NOTIFY_TELEGRAM_ALARM_COMMAND_UNDEFINED 0
#define CONFIG_NOTIFY_TELEGRAM_ALARM_ALERT_COMMAND_UNDEFINED 0
// Уведомления в Telegram при тревогах охраны
#define CONFIG_NOTIFY_TELEGRAM_ALARM_ALARM 1
#define CONFIG_NOTIFY_TELEGRAM_ALARM_ALERT_ALARM 1

// ------------------------------------------------------------------------
//                                Сенсоры 
// ------------------------------------------------------------------------
// Разрешить публикацию данных сенсора в простом виде (каждое значение в отдельном субтопике)
#define CONFIG_SENSOR_AS_PLAIN 0
// Разрешить публикацию данных сенсора в виде JSON в одном топике
#define CONFIG_SENSOR_AS_JSON 1
// Разрешить публикацию статуса сенсора
#define CONFIG_SENSOR_STATUS_ENABLE 1

// ------------------------------------------------------------------------
//                                ИК пульт 
// ------------------------------------------------------------------------
// Разрешить работу по протоколу NEC
#define CONFIG_IR_PROTOCOL_NEC 1
// Разрешить работу по протоколу RC5
#define CONFIG_IR_PROTOCOL_RC5 0

// ------------------------------------------------------------------------
//                            Log - Отладка 
// ------------------------------------------------------------------------
// Уровень отладочных сообщений. Всё, что выше этого уровня, будет исключено из кода
#define CONFIG_RLOG_PROJECT_LEVEL RLOG_LEVEL_DEBUG
// Добавлять коды цвета текста к сообщениям. Не работает на Win7
#define CONFIG_RLOG_PROJECT_COLORS 1
// Добавлять отметку времени к сообщениям
#define CONFIG_RLOG_SHOW_TIMESTAMP 1
// Добавлять информацию о файле и строке к сообщениям
#define CONFIG_RLOG_SHOW_FILEINFO 0


// Сохранять отладочную информацию при программном перезапуске устройства
#define CONFIG_RESTART_DEBUG_INFO 1
/* Разрешить периодическое сохранение информации о куче / памяти с 
    последующем выводом при перезапуске */
#define CONFIG_RESTART_DEBUG_HEAP_SIZE_SCHEDULE 1
// Глубина сохранения стека процессора при перезапуске. 0 - не сохранять
#define CONFIG_RESTART_DEBUG_STACK_DEPTH 28
// Разрешить публикацию отладочной информации из модуля WiFi
#define CONFIG_WIFI_DEBUG_ENABLE 0
// Разрешить периодическую публикацию системной информации
#define CONFIG_MQTT_SYSINFO_ENABLE 1
#define CONFIG_MQTT_SYSINFO_INTERVAL 1000*60*5
#define CONFIG_MQTT_SYSINFO_SYSTEM_FLAGS 1           
/* Разрешить периодическую публикацию информации о задачах. Должен быть 
    разрешен CONFIG_FREERTOS_USE_TRACE_FACILITY / configUSE_TRACE_FACILITY */
#define CONFIG_MQTT_TASKLIST_ENABLE 0
#define CONFIG_MQTT_TASKLIST_INTERVAL 1000*60*60
