/*
 * Data Sheet: http://www.ti.com/lit/ds/symlink/ina219.pdf
 */

#include <TinyWireM.h>
#include "ina219.h"

INA219::INA219(void) {}

void write_register(uint8_t reg_addr, uint8_t upper, uint8_t lower)
{
  TinyWireM.beginTransmission(INA219_I2C_ADDR);
  TinyWireM.send(reg_addr);
  TinyWireM.send(upper);
  TinyWireM.send(lower);
  TinyWireM.endTransmission();
}

uint16_t read_register(uint8_t reg_addr)
{
  TinyWireM.beginTransmission(INA219_I2C_ADDR);
  TinyWireM.send(reg_addr);
  TinyWireM.endTransmission();
#ifdef WAIT_CONVERSION
  WAIT_CONVERSION; // wait conversion time
#endif
  TinyWireM.requestFrom(INA219_I2C_ADDR, 2);
  return ((TinyWireM.read() << 8) | TinyWireM.read());
}

static bool high_value_mode;

void INA219::begin(void)
{
  high_value_mode = false;
  write_register(CONFIG_REGISTER_ADDR, CONFIG_UPPER, CONFIG_LOWER);
  write_register(CALIBRATION_REGISTER_ADDR, CALIBRATION_UPPER, CALIBRATION_LOWER);
}

void INA219::to_high_value_mode(void)
{
  high_value_mode = true;
  write_register(CONFIG_REGISTER_ADDR, CONFIG_UPPER_3A, CONFIG_LOWER);
  write_register(CALIBRATION_REGISTER_ADDR, CALIBRATION_UPPER_3A, CALIBRATION_LOWER_3A);
}

bool INA219::is_high_value_mode(void)
{
  return high_value_mode;
}

/*
 * Shunt Voltage Register (address = 01h)
 * Data Sheet Page 23 Figure 20
 */
float INA219::read_shunt_voltage(void)
{
  return ((int16_t)read_register(SHUNT_VOLTAGE_REGISTER_ADDR)) * 0.00001; // Shunt voltage
}

/*
 * Bus Voltage Register (address = 02h)
 * Data Sheet Page 23 Figure 24
 */
float INA219::read_bus_voltage(void)
{
  int16_t value = read_register(BUS_VOLTAGE_REGISTER_ADDR);
  if ((!high_value_mode) && (value & 0x01)) // overflow
  {
    to_high_value_mode();
    value = read_register(BUS_VOLTAGE_REGISTER_ADDR);
  }
  if (value & 0x01) // overflow
  {
    return 99.99;
  }
  return (value >> 3) * 0.004; // shift Bits 3-15 and LSB = 4 mV
}

/*
 * Power Register (address = 03h)
 * Data Sheet Page 23 Figure 25
 */
float INA219::read_power(void)
{
  if (high_value_mode)
  {
    return read_register(POWER_REGISTER_ADDR) * CURRENT_LSB_3A * POWER_CONSTANT;
  }
  return read_register(POWER_REGISTER_ADDR) * CURRENT_LSB * POWER_CONSTANT;
}

/*
 * Current Register (address = 04h)
 * Data Sheet Page 23 Figure 26
 */
float INA219::read_current(void)
{
  if (high_value_mode)
  {
    return ((int16_t)read_register(CURRENT_REGISTER_ADDR)) * CURRENT_LSB_3A;
  }
  else
  {
    float current = ((int16_t)read_register(CURRENT_REGISTER_ADDR)) * CURRENT_LSB;
    if (current > DETECT_HIGH_CURRENT_THRESHOLD)
    {
      to_high_value_mode();
      current = ((int16_t)read_register(CURRENT_REGISTER_ADDR)) * CURRENT_LSB_3A;
    }
    return current;
  }
}
