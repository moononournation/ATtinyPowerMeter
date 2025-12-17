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

#define DETECT_HIGH_CURRENT_THRESHOLD 360 // 90% of 400 mA

/*
 * Configuration Register (address = 00h)
 * Data Sheet Page 19 Table 3
 * Bit     13: Bus Voltage Range, 0 = 16V FSR, 1 = 32V FSR (default value)
 * Bits 12-11: PGA (Shunt Voltage Only)
 *             00: +- 40 mV, Highest current resolution
 *             01: +- 80 mV
 *             10: +-160 mV
 *             11: +-320 mV, largest measurable current range
 * Bits 10- 7: Bus ADC Resolution, same as Shunt ADC
 * Bits  6- 3: Shunt ADC Resolution
 *             0X00: 9 bit 84 μs
 *             0X01: 10 bit 148 μs
 *             0X10: 11 bit 276 μs
 *             0X11: 12 bit 532 μs
 *             1000: 12 bit 532 μs
 *             1001: 2 samples, 1.06 ms
 *             1010: 4 samples, 2.13 ms
 *             1011: 8 samples, 4.26 ms
 *             1100: 16 samples, 8.51 ms
 *             1101: 32 samples, 17.02 ms
 *             1110: 64 samples, 34.05 ms
 *             1111: 128 samples, 68.10 ms
 * Bits  2- 0: Operating Mode
 *             000: Power-down
 *             001: Shunt voltage, triggered
 *             010: Bus voltage, triggered
 *             011: Shunt and bus, triggered
 *             100: ADC off (disabled)
 *             101: Shunt voltage, continuous
 *             110: Bus voltage, continuous
 *             111: Shunt and bus, continuous
 */
#define CONFIG_REGISTER_ADDR 0x00
#define CONFIG_UPPER 0b00000100
#define CONFIG_UPPER_3A 0b00111100
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
 * Maximum Expected Current 800 mA
 * Current_LSB = 400 mA / 2^15 ~= 12.8 uA
 * R(SHUNT) = 0.1 Ohm
 * Cal = 0.04096 / 0.0000128 / 0.1 = 32000 (0x7D00)
 * Maximum Expected Current 3200 mA
 * Current_LSB = 3200 mA / 2^15 ~= 102.4 uA
 * R(SHUNT) = 0.1 Ohm
 * Cal = 0.04096 / 0.0001024 / 0.1 = 4000 (0x0FA0)
 */
#define CALIBRATION_REGISTER_ADDR 0x05
#define CALIBRATION_UPPER 0x7D
#define CALIBRATION_LOWER 0x00
#define CURRENT_LSB 0.0128
#define CALIBRATION_UPPER_3A 0x0F
#define CALIBRATION_LOWER_3A 0xA0
#define CURRENT_LSB_3A 0.1024
#define POWER_CONSTANT 20

/*
 * Conversion Time
 * Data Sheet Page 20 Table 5
 */
// #define WAIT_CONVERSION delayMicroseconds(532) // 12 bit triggered Mode require 532 us
// #define WAIT_CONVERSION delay(69) // 128 samples Mode triggered require 68.10 ms
#define WAIT_CONVERSION // no need to wait conversion in continuous mode

class INA219
{
public:
  INA219(void);
  void begin(void);
  void to_high_value_mode(void);
  bool is_high_value_mode(void);
  float read_shunt_voltage(void);
  float read_bus_voltage(void);
  float read_power(void);
  float read_current(void);
};

#endif //INA219_H
