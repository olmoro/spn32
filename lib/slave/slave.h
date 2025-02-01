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

#ifdef __cplusplus
}
#endif

#endif  // _SLAVE_H_
