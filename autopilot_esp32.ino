#include <math.h>
#include "motor.h"
void motor_set(int, int);
#include "bluetooth.h"

//loop times
static uint32_t prev_pid_ms = 0;

void setup() {
  bluetooth_setup();
}

void loop() {
  if (millis() - prev_pid_ms > 100) {
    bluetooth_loop();
  }
}