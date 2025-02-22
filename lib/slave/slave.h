/*=====================================================================================
 * Description:
 *   FreeModbus Slave ESP32
 *  Kconfig - настройки MODBUS
 *  E:\PlatformIO\logika\mb_slave\managed_components\espressif__esp-modbus\Kconfig
 *====================================================================================*/
#ifndef _SLAVE_H_
#define _SLAVE_H_

#define MB_PORT_NUM     (static_cast<uart_port_t>(CONFIG_MB_UART_PORT_NUM))   // Number of UART port used for Modbus connection
#define MB_SLAVE_ADDR   (CONFIG_MB_SLAVE_ADDR)      // The address of device in Modbus network
#define MB_DEV_SPEED    (CONFIG_MB_UART_BAUD_RATE)  // The communication speed of the UART

/* Note: Some pins on target chip cannot be assigned for UART communication.
   Please refer to documentation for selected board and target to configure 
   pins using Kconfig.
*/ 

#ifdef __cplusplus
  extern "C" {
#endif

// Defines below are used to define register start address for each type of Modbus registers
#define HOLD_OFFSET(field) ((uint16_t)(offsetof(holding_reg_params_t, field) >> 1))
#define INPUT_OFFSET(field) ((uint16_t)(offsetof(input_reg_params_t, field) >> 1))
#define MB_REG_DISCRETE_INPUT_START         (0x0000)
#define MB_REG_COILS_START                  (0x0000)
#define MB_REG_INPUT_START_AREA0            (INPUT_OFFSET(input_data0)) // register offset input area 0
#define MB_REG_INPUT_START_AREA1            (INPUT_OFFSET(input_data4)) // register offset input area 1
#define MB_REG_HOLDING_START_AREA0          (HOLD_OFFSET(holding_data0))
#define MB_REG_HOLDING_START_AREA1          (HOLD_OFFSET(holding_data4))

#define MB_PAR_INFO_GET_TOUT                (10) // Timeout for get parameter info
#define MB_CHAN_DATA_MAX_VAL                (6)
#define MB_CHAN_DATA_OFFSET                 (0.2f)
#define MB_READ_MASK                        (MB_EVENT_INPUT_REG_RD \
                                                | MB_EVENT_HOLDING_REG_RD \
                                                | MB_EVENT_DISCRETE_RD \
                                                | MB_EVENT_COILS_RD)
#define MB_WRITE_MASK                       (MB_EVENT_HOLDING_REG_WR \
                                                | MB_EVENT_COILS_WR)
#define MB_READ_WRITE_MASK  static_cast<mb_event_group_t>(MB_READ_MASK | MB_WRITE_MASK)

void slaveTaskStart();

// Session Request
/* В этот момент интерфейс переходит в начальное состояние согласования скоростей, как это
требуется по стандарту МЭК1107. Программа компьютера должна вывести и принять ряд сообщений, 
которые описанных ниже.
  Формат сообщений соответствует двухточечному протоколу по стандарту МЭК1107. В процессе 
этого диалога, который выполняется на скорости 300 бит/с, устанавливается скорость последующего 
обмена и происходит переключение на магистральный формат сообщений.
  Передача ведется в символьном формате. Формат символа соответствует стандарту ИСО 646 
(1 стартовый бит, 7 битов данных, 1 бит четности, 1 стоповый бит). Данные кодируются в 
соответствии 7-битной таблицей кодов ASCII.
  Для начала обмена данными со сторон внешнего оборудования в прибор со скоростью передачи 300 
бит/с посылается сообщение  запрос сеанса  /   ?   !  CR  LF 
                                          2Fh 3Fh 21h 0Dh 0Ah
Начальному символу "/" соответствует шестнадцатеричный код 2Fh. Символ "?", код 3Fh, 
предлагает прибору начать сеанс обмена данными. Конечный символ "!", код 21h, ограничивает текстовую 
часть запроса. Завершают сообщение символы CR (возврат каретки, код 0Dh) и LF (перевод строки, 
код 0Ah). */

void sessionRequest();

/* В ответ на запрос сеанса прибор также со скоростью 300 бит/с передает идентифицирующее
 сообщение  /   LGK  Z   T1...T6   P1   P2   I1   I2   I3   CR   LF
Здесь:
"/" - стартовый символ;
LGK - три буквы, идентифицирующие изготовителя прибора, например, АО НПФ ЛОГИКА;
Z - принимает значения от нуля до девяти и указывает скорость, с которой прибор готов 
    вести обмен данными: 0  300 бит/с, 1  600 бит/с, ... 9 – 115200 бит/с;
T1...T6 - шесть символов, указывающих тип прибора; например, для прибора СПТ961  "SPT961";
P1 P2 - два символа, описывающие возможности прибора при переходе на этом этапе к 
    магистральному протоколу обмена. Первый символ, P1, указывает, закрыт или доступен 
    магистральный протокол (0/1). Второй, P2, указывает, какая скорость установлена для 
    магистрального протокола в параметре "Спецификация внешнего оборудования";
I1 I2 I3 - три младших цифры идентификатора прибора (параметр 008);
CR - завершающий символ возврата каретки;
LF - завершающий символ перевода строки.

*/

/* Далее со стороны внешнего оборудования в прибор со скоростью 300 бит/с должно быть передано 
подтверждение выбора скорости передачи:   ACK   0  P2   3  CR  LF
Здесь АСК - символ подтверждения, его код 06h.*/




#ifdef __cplusplus
}
#endif

#endif  // _SLAVE_H_
