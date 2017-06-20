/*
 * Data Sheet: http://www.ti.com/lit/ds/symlink/ina219.pdf
 */
#ifndef INA219_H
#define INA219_H

#include <TinyWireM.h>

/*
 * INA219 I2C address
 * Data Sheet Page 14 Table 1
 */
#define INA219_I2C_ADDR 0b1000000

/*
 * Configuration Register (address = 00h)
 * Data Sheet Page 19 Table 3
 * Bit     13: Bus Voltage Range, 0 = 16V FSR
 * Bits 11-12: PGA (Shunt Voltage Only), 11 = +-320 mV, largest measurable current range
 * Bits  7-10: Bus ADC Resolution, 1000 = 12 bit, 532 us
 * Bits  3- 6: Shunt ADC Resolution, 1000 = 12 bit, 532 us
 * Bits  0- 2: Operating Mode, Shunt and bus, continuous
 */
#define CONFIG_REGISTER_ADDR 0x00
#define CONFIG_UPPER 0b00000100
#define CONFIG_LOWER 0b01000111

/*
 * Shunt Voltage Register (address = 01h)
 * Data Sheet Page 23 Figure 20
 */
#define SHUNT_VOLTAGE_REGISTER_ADDR 0x01

/*
 * Bus Voltage Register (address = 02h)
 * Data Sheet Page 23 Figure 24
 */
#define BUS_VOLTAGE_REGISTER_ADDR 0x02

/*
 * Power Register (address = 03h)
 * Data Sheet Page 23 Figure 25
 */
#define POWER_REGISTER_ADDR 0x03

/*
 * Current Register (address = 04h)
 * Data Sheet Page 23 Figure 26
 */
#define CURRENT_REGISTER_ADDR 0x04

/*
 * Calibration Register (address = 05h)
 * Data Sheet Page 12 Chapter 8.5.1, Page 25 Figure 27
 * Maximum Expected Current 800 mA (2N2222 max current)
 * Current_LSB = 800 mA / 2^15 ~= 25.6 uA
 * R(SHUNT) = 0.1 Ohm
 * Cal = 0.04096 / 0.0000256 / 0.1 = 16000 (0x3E80)
 */
#define CALIBRATION_REGISTER_ADDR 0x05
#define CALIBRATION_UPPER 0x3E
#define CALIBRATION_LOWER 0x80
#define CURRENT_LSB 0.0256
#define POWER_LSB 20 * CURRENT_LSB

/*
 * Conversion Time
 * Data Sheet Page 20 Table 5
 * 12 bit Mode require 532 us
 */
#define CONVERSION_TIME 532

class INA219 {
 public:
  INA219(void);
  void begin(void);
  float read_shunt_voltage(void);
  float read_bus_voltage(void);
  float read_power(void);
  float read_current(void);
};

#endif //INA219_H

