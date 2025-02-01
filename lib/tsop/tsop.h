/* 
  Модуль для приема данных с ИК пульта по протоколу NEC. 
  https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/peripherals/rmt.html 
*/

#ifndef _TSOP_H_
#define _TSOP_H_

#include <stdbool.h>
//#include "reLed.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

#ifdef __cplusplus
extern "C" {
#endif

void tsopInit(const uint8_t gpioRx, QueueHandle_t queueProc);
void tsopEnable();

// void tsopDisable();
// bool tsopIsAvailable();
// void tsopResetAvailable();
uint32_t tsopGetReceivedValue();
uint16_t tsopGetReceivedBitLength();
uint16_t tsopGetReceivedDelay();
uint16_t tsopGetReceivedProtocol();

#ifdef __cplusplus
}
#endif

#endif // _TSOP_H_
