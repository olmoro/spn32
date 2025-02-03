/* 
  Модуль для приема данных с ИК пульта по протоколу NEC. 
  https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/peripherals/rmt.html 
*/

#ifndef _TSOP_H_
#define _TSOP_H_

#include <stdbool.h>
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

#ifdef __cplusplus
extern "C" {
#endif

void tsopStart(const uint8_t gpioRx, QueueHandle_t queueProc);

#ifdef __cplusplus
}
#endif

#endif // _TSOP_H_
