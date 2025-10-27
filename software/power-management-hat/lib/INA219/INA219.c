#include "INA219.h"

uint8_t ina219_i2caddr;
uint32_t ina219_calValue;
// The following multipliers are used to convert raw current and power
// values to mA and mW, taking into account the current config settings
uint32_t ina219_currentDivider_mA;
float ina219_powerMultiplier_mW = 0;

/*!
 *  @brief  Sends a single command byte over I2C
 *  @param  reg
 *          register address
 *  @param  value
 *          value to write
 */
void INA219_wireWriteRegister(uint8_t reg, uint16_t value)
{
  DEV_I2C_Write_Register(INA219_ADDRESS, reg, value);
}

/*!
 *  @brief  Reads a 16 bit values over I2C
 *  @param  reg
 *          register address
 *  @param  *value
 *          read value
 */
void INA219_wireReadRegister(uint8_t reg, uint16_t *value)
{
  DEV_I2C_Read_Register(INA219_ADDRESS, reg, value);
}

/*!
 *  @brief  Configures to INA219 to be able to measure up to 32V and 2A
 *          of current.  Each unit of current corresponds to 100uA, and
 *          each unit of power corresponds to 2mW. Counter overflow
 *          occurs at 3.2A.
 *  @note   These calculations assume a 0.1 ohm resistor is present
 */
void INA219_setCalibration_32V_2A()
{
  // By default we use a pretty huge range for the input voltage,
  // which probably isn't the most appropriate choice for system
  // that don't use a lot of power.  But all of the calculations
  // are shown below if you want to change the settings.  You will
  // also need to change any relevant register settings, such as
  // setting the VBUS_MAX to 16V instead of 32V, etc.

  // VBUS_MAX = 32V             (Assumes 32V, can also be set to 16V)
  // VSHUNT_MAX = 0.32          (Assumes Gain 8, 320mV, can also be 0.16, 0.08, 0.04)
  // RSHUNT = 0.1               (Resistor value in ohms)

  // 1. Determine max possible current
  // MaxPossible_I = VSHUNT_MAX / RSHUNT
  // MaxPossible_I = 3.2A

  // 2. Determine max expected current
  // MaxExpected_I = 2.0A

  // 3. Calculate possible range of LSBs (Min = 15-bit, Max = 12-bit)
  // MinimumLSB = MaxExpected_I/32767
  // MinimumLSB = 0.000061              (61uA per bit)
  // MaximumLSB = MaxExpected_I/4096
  // MaximumLSB = 0,000488              (488uA per bit)

  // 4. Choose an LSB between the min and max values
  //    (Preferrably a roundish number close to MinLSB)
  // CurrentLSB = 0.0001 (100uA per bit)

  // 5. Compute the calibration register
  // Cal = trunc (0.04096 / (Current_LSB * RSHUNT))
  // Cal = 4096 (0x1000)

  ina219_calValue = 4096;

  // 6. Calculate the power LSB
  // PowerLSB = 20 * CurrentLSB
  // PowerLSB = 0.002 (2mW per bit)

  // 7. Compute the maximum current and shunt voltage values before overflow
  //
  // Max_Current = Current_LSB * 32767
  // Max_Current = 3.2767A before overflow
  //
  // If Max_Current > Max_Possible_I then
  //    Max_Current_Before_Overflow = MaxPossible_I
  // Else
  //    Max_Current_Before_Overflow = Max_Current
  // End If
  //
  // Max_ShuntVoltage = Max_Current_Before_Overflow * RSHUNT
  // Max_ShuntVoltage = 0.32V
  //
  // If Max_ShuntVoltage >= VSHUNT_MAX
  //    Max_ShuntVoltage_Before_Overflow = VSHUNT_MAX
  // Else
  //    Max_ShuntVoltage_Before_Overflow = Max_ShuntVoltage
  // End If

  // 8. Compute the Maximum Power
  // MaximumPower = Max_Current_Before_Overflow * VBUS_MAX
  // MaximumPower = 3.2 * 32V
  // MaximumPower = 102.4W

  // Set multipliers to convert raw current/power values
  ina219_currentDivider_mA = 1.0; // Current LSB = 100uA per bit (1000/100 = 10)
  ina219_powerMultiplier_mW = 20; // Power LSB = 1mW per bit (2/1)

  // Set Calibration register to 'Cal' calculated above
  INA219_wireWriteRegister(INA219_REG_CALIBRATION, ina219_calValue);

  // Set Config register to take into account the settings above
  uint16_t config = INA219_CONFIG_BVOLTAGERANGE_32V |
                    INA219_CONFIG_GAIN_8_320MV | INA219_CONFIG_BADCRES_12BIT |
                    INA219_CONFIG_SADCRES_12BIT_32S_17MS |
                    INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS;
  INA219_wireWriteRegister(INA219_REG_CONFIG, config);
}

/*!
 *  @brief  Set power save mode according to parameters
 *  @param  on
 *          boolean value
 */
void INA219_powerSave(bool on)
{
  uint16_t current;
  INA219_wireReadRegister(INA219_REG_CONFIG, &current);
  uint8_t next;
  if (on)
  {
    next = current | INA219_CONFIG_MODE_POWERDOWN;
  }
  else
  {
    next = current & ~INA219_CONFIG_MODE_POWERDOWN;
  }
  INA219_wireWriteRegister(INA219_REG_CONFIG, next);
}

/*!
 *  @brief  Setups the HW (defaults to 32V and 2A for calibration values)
 *  @param theWire the TwoWire object to use
 */
// void INA219_begin(TwoWire *theWire) {
void INA219_begin()
{
  INA219_setCalibration_32V_2A();
}

/*!
 *  @brief  Gets the shunt voltage in mV (so +-327mV)
 *  @return the shunt voltage converted to millivolts
 */
float INA219_getShuntVoltage_mV()
{
  uint16_t value;
  INA219_wireReadRegister(INA219_REG_SHUNTVOLTAGE, &value);
  return (int16_t)value * 0.01;
}

/*!
 *  @brief  Gets the shunt voltage in volts
 *  @return the bus voltage converted to volts
 */
float INA219_getBusVoltage_V()
{

  uint16_t value;
  INA219_wireReadRegister(INA219_REG_BUSVOLTAGE, &value);
  // Shift to the right 3 to drop CNVR and OVF and multiply by LSB
  return (int16_t)((value >> 3) * 4) * 0.001;
}

/*!
 *  @brief  Gets the current value in mA, taking into account the
 *          config settings and current LSB
 *  @return the current reading convereted to milliamps
 */
float INA219_getCurrent_mA()
{
  uint16_t value;

  // Sometimes a sharp load will reset the INA219, which will
  // reset the cal register, meaning CURRENT and POWER will
  // not be available ... avoid this by always setting a cal
  // value even if it's an unfortunate extra step
  INA219_wireWriteRegister(INA219_REG_CALIBRATION, ina219_calValue);

  // Now we can safely read the CURRENT register!
  INA219_wireReadRegister(INA219_REG_CURRENT, &value);
  float valueDec = (int16_t)value;
  valueDec /= ina219_currentDivider_mA;
  return valueDec;
}

/*!
 *  @brief  Gets the power value in mW, taking into account the
 *          config settings and current LSB
 *  @return power reading converted to milliwatts
 */
float INA219_getPower_mW()
{

  uint16_t value;

  // Sometimes a sharp load will reset the INA219, which will
  // reset the cal register, meaning CURRENT and POWER will
  // not be available ... avoid this by always setting a cal
  // value even if it's an unfortunate extra step
  INA219_wireWriteRegister(INA219_REG_CALIBRATION, ina219_calValue);

  // Now we can safely read the POWER register!
  INA219_wireReadRegister(INA219_REG_POWER, &value);

  float valueDec = (int16_t)value;
  valueDec *= ina219_powerMultiplier_mW;
  return valueDec;
}