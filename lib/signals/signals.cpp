/*
  (security.cpp)

*/

#include "signals.h"  //#include "security.h"
#include "tsop.h"
#include <stdbool.h>
#include "project_config.h"
#include "def_alarm.h"
#include "def_consts.h"
#include "mTypes.h"   //#include "rTypes.h"
#include "reGpio.h"
#include "reLed.h"
#include "reEvents.h"
#include "reParams.h"
#include "rLog.h"

static const char* logTAG = "SIGNALS";



void alarmInitDevices()
{
  rlog_i(logTAG, "Initialization of AFS devices");

  // Создаем светодиоды
  ledQueue_t ledAlarm = nullptr;
  #if defined(CONFIG_GPIO_ALARM_LED) && (CONFIG_GPIO_ALARM_LED > -1)
    ledAlarm = ledTaskCreate(CONFIG_GPIO_ALARM_LED, true, true, "led_alarm", CONFIG_LED_TASK_STACK_SIZE, nullptr);
    ledTaskSend(ledAlarm, lmOff, 0, 0, 0);
  #endif // CONFIG_GPIO_ALARM_LED

  ledQueue_t ledExec = nullptr;
  #if defined(CONFIG_GPIO_EXEC_LED) && (CONFIG_GPIO_EXEC_LED > -1)
    ledExec = ledTaskCreate(CONFIG_GPIO_ALARM_LED, true, true, "led_exec", CONFIG_LED_TASK_STACK_SIZE, nullptr);
    ledTaskSend(ledExec, lmOff, 0, 0, 0);
  #endif // CONFIG_GPIO_EXEC_LED

  // Создаем сирену
  ledQueue_t siren = nullptr;
  #if defined(CONFIG_GPIO_ALARM_SIREN) && (CONFIG_GPIO_ALARM_SIREN > -1)
    siren = ledTaskCreate(CONFIG_GPIO_ALARM_SIREN, true, false, "siren", CONFIG_LED_TASK_STACK_SIZE, nullptr);
    ledTaskSend(siren, lmOff, 0, 0, 0);
  #endif // CONFIG_GPIO_ALARM_SIREN

   // Создаем флешер
  ledQueue_t flasher = nullptr;
  #if defined(CONFIG_GPIO_ALARM_FLASH) && (CONFIG_GPIO_ALARM_FLASH > -1)
    flasher = ledTaskCreate(CONFIG_GPIO_ALARM_FLASH, true, true, "flasher", CONFIG_LED_TASK_STACK_SIZE, nullptr);
    ledTaskSend(flasher, lmBlinkOn, 1, 100, 5000);
  #endif // CONFIG_GPIO_ALARM_FLASH
  
  // // Замена пассивной пищалки на активную
  ledQueue_t buzzer = nullptr;
  // #if defined(CONFIG_GPIO_BUZZER_ACTIVE) && (CONFIG_GPIO_BUZZER_ACTIVE > -1)
  //   buzzer = ledTaskCreate(CONFIG_GPIO_BUZZER_ACTIVE, true, false, "buzzer", CONFIG_LED_TASK_STACK_SIZE, nullptr);
  //   ledTaskSend(buzzer, lmOff, 0, 0, 0);
  // #endif // CONFIG_GPIO_ALARM_FLASH
  
  // Запускаем задачу
  // bool bbb = alarmTaskCreate(siren, flasher, buzzer, ledAlarm, ledExec, nullptr);
  // rlog_i(logTAG, " alarmTaskCreate=%" PRIX32 "\n\n", bbb);
  bool _alarmTaskCreate = alarmTaskCreate(siren, flasher, buzzer, ledAlarm, ledExec, nullptr);
  rlog_i(logTAG, " alarmTaskCreate=%" PRIX32 "\n\n", _alarmTaskCreate);
  if (!_alarmTaskCreate) {
    rlog_e(logTAG, "Failed to start alarmTask");
  };

  //rlog_e(logTAG, )
  // // Запускаем приемник RX 433 MHz
  // #ifdef CONFIG_GPIO_RX433
  //   rx433_Init(CONFIG_GPIO_RX433, alarmTaskQueue());
  //   rx433_Enable();
  // #endif // CONFIG_GPIO_RX433

  #ifdef CONFIG_GPIO_TSOP
    rlog_i(logTAG, " alarmTaskQueue=%" PRIX32 "\n\n", alarmTaskQueue());
    tsopStart(CONFIG_GPIO_TSOP, alarmTaskQueue());
  #endif // CONFIG_GPIO_TSOP

}



void alarmStart()
{
  alarmInitDevices();
  // alarmExternalSensorsInit();
  // alarmInitSensors();
}
