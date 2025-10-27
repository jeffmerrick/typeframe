#ifndef _INA219_H
#define _INA219_H

#include "DEV_Config.h"
#include <stdint.h>
#include <stdlib.h> //itoa()
#include <stdio.h>

/** default I2C address **/
#define INA219_ADDRESS (0x42)

/** config register address **/
#define INA219_REG_CONFIG (0x00)

/** shunt voltage register **/
#define INA219_REG_SHUNTVOLTAGE (0x01)

/** bus voltage register **/
#define INA219_REG_BUSVOLTAGE (0x02)

/** power register **/
#define INA219_REG_POWER (0x03)

/** current register **/
#define INA219_REG_CURRENT (0x04)

/** calibration register **/
#define INA219_REG_CALIBRATION (0x05)

/** reset bit **/
#define INA219_CONFIG_RESET (0x8000) // Reset Bit

/** mask for bus voltage range **/
#define INA219_CONFIG_BVOLTAGERANGE_MASK (0x2000) // Bus Voltage Range Mask

/** bus voltage range values **/
enum
{
    INA219_CONFIG_BVOLTAGERANGE_16V = (0x0000), // 0-16V Range
    INA219_CONFIG_BVOLTAGERANGE_32V = (0x2000), // 0-32V Range
};

/** mask for gain bits **/
#define INA219_CONFIG_GAIN_MASK (0x1800) // Gain Mask

/** values for gain bits **/
enum
{
    INA219_CONFIG_GAIN_1_40MV = (0x0000),  // Gain 1, 40mV Range
    INA219_CONFIG_GAIN_2_80MV = (0x0800),  // Gain 2, 80mV Range
    INA219_CONFIG_GAIN_4_160MV = (0x1000), // Gain 4, 160mV Range
    INA219_CONFIG_GAIN_8_320MV = (0x1800), // Gain 8, 320mV Range
};

/** mask for bus ADC resolution bits **/
#define INA219_CONFIG_BADCRES_MASK (0x0780)

/** values for bus ADC resolution **/
enum
{
    INA219_CONFIG_BADCRES_9BIT = (0x0000),  // 9-bit bus res = 0..511
    INA219_CONFIG_BADCRES_10BIT = (0x0080), // 10-bit bus res = 0..1023
    INA219_CONFIG_BADCRES_11BIT = (0x0100), // 11-bit bus res = 0..2047
    INA219_CONFIG_BADCRES_12BIT = (0x0180), // 12-bit bus res = 0..4097
};

/** mask for shunt ADC resolution bits **/
#define INA219_CONFIG_SADCRES_MASK \
    (0x0078) // Shunt ADC Resolution and Averaging Mask

/** values for shunt ADC resolution **/
enum
{
    INA219_CONFIG_SADCRES_9BIT_1S_84US = (0x0000),   // 1 x 9-bit shunt sample
    INA219_CONFIG_SADCRES_10BIT_1S_148US = (0x0008), // 1 x 10-bit shunt sample
    INA219_CONFIG_SADCRES_11BIT_1S_276US = (0x0010), // 1 x 11-bit shunt sample
    INA219_CONFIG_SADCRES_12BIT_1S_532US = (0x0018), // 1 x 12-bit shunt sample
    INA219_CONFIG_SADCRES_12BIT_2S_1060US =
        (0x0048), // 2 x 12-bit shunt samples averaged together
    INA219_CONFIG_SADCRES_12BIT_4S_2130US =
        (0x0050), // 4 x 12-bit shunt samples averaged together
    INA219_CONFIG_SADCRES_12BIT_8S_4260US =
        (0x0058), // 8 x 12-bit shunt samples averaged together
    INA219_CONFIG_SADCRES_12BIT_16S_8510US =
        (0x0060), // 16 x 12-bit shunt samples averaged together
    INA219_CONFIG_SADCRES_12BIT_32S_17MS =
        (0x0068), // 32 x 12-bit shunt samples averaged together
    INA219_CONFIG_SADCRES_12BIT_64S_34MS =
        (0x0070), // 64 x 12-bit shunt samples averaged together
    INA219_CONFIG_SADCRES_12BIT_128S_69MS =
        (0x0078), // 128 x 12-bit shunt samples averaged together
};

/** mask for operating mode bits **/
#define INA219_CONFIG_MODE_MASK (0x0007) // Operating Mode Mask

/** values for operating mode **/
enum
{
    INA219_CONFIG_MODE_POWERDOWN,
    INA219_CONFIG_MODE_SVOLT_TRIGGERED,
    INA219_CONFIG_MODE_BVOLT_TRIGGERED,
    INA219_CONFIG_MODE_SANDBVOLT_TRIGGERED,
    INA219_CONFIG_MODE_ADCOFF,
    INA219_CONFIG_MODE_SVOLT_CONTINUOUS,
    INA219_CONFIG_MODE_BVOLT_CONTINUOUS,
    INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS
};

void INA219_begin();
void INA219_setCalibration_32V_2A();
float INA219_getBusVoltage_V();
float INA219_getShuntVoltage_mV();
float INA219_getCurrent_mA();
float INA219_getPower_mW();
void INA219_powerSave(bool on);
void INA219_wireWriteRegister(uint8_t reg, uint16_t value);
void INA219_wireReadRegister(uint8_t reg, uint16_t *value);

#endif