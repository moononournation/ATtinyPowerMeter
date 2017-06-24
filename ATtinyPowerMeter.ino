#include <TinyWireM.h>
#include "ssd1306.h"
#include "ina219.h"

// variables
SSD1306 oled;
INA219 power_mon;

#define CEILING_BUFFER 0.1F
#define CHART_LEFT 0
#define CHART_RIGHT 78

static uint8_t i = CHART_LEFT;
static float voltage = 0;
static float current = 0;
static float power = 0;
static float mWh = 0;
static unsigned long last_millis = 0;

static float max_v = 5.0F;
static float max_ma = 10.0F;
static float max_mw = 5.0F;

void setup() {
  TinyWireM.begin();
  oled.begin();
  power_mon.begin();

  oled.fill(0x00); // clear in black
  oled.set_pos(128 - 5, 0);
  oled.print("V");
  oled.set_pos(128 - 10, 2);
  oled.print("mA");
  oled.set_pos(128 - 15, 4);
  oled.print("mWh");
  oled.set_pos(CHART_RIGHT, 6);
  oled.print("Time:");

/* figure labels, comment out to reduce distraction
  oled.set_pos(CHART_RIGHT, 0);
  oled.print("V:");
  oled.set_pos(CHART_RIGHT, 2);
  oled.print("mA:");
  oled.set_pos(CHART_RIGHT, 4);
  oled.print("Time:");
  oled.set_pos(CHART_RIGHT, 6);
  oled.print("mWh:");
*/
}

void loop() {
  // collect figures
  voltage = power_mon.read_bus_voltage();
  current = power_mon.read_current();
  power = power_mon.read_power();
  mWh += power / 1000 * (millis() - last_millis) / 3600;
  last_millis = millis();

  // print figures
  oled.set_pos(CHART_RIGHT, 0);
  oled.print_float(voltage, 8);
  oled.set_pos(CHART_RIGHT, 2);
  oled.print_float(current, 7);
  oled.set_pos(CHART_RIGHT, 4);
  oled.print_float(mWh, 6);
  oled.set_pos(CHART_RIGHT, 7);
  oled.print(last_millis / 1000);

  // auto adjust graph ceiling
  if (voltage >= max_v) max_v = voltage + CEILING_BUFFER;
  if (current >= max_ma) max_ma = current + CEILING_BUFFER;
  if (power > max_mw) max_mw = power + CEILING_BUFFER;

  // plot graph
  oled.plot_value(i, 0, 2, voltage, 0.0F, max_v);
  oled.plot_value(i, 2, 2, current, 0.0F, max_ma);
  oled.plot_area(i, 4, 4, power, 0.0F, max_mw);

  // loop graph position
  i++;
  if (i >= CHART_RIGHT) {
    i = CHART_LEFT;
  }

  // draw current position line
  oled.draw_pattern(i, 0, 1, 8, 0xFF);
}

