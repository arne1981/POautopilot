#include "motor.h"
void motor_set(int, int);
#include "bluetooth.h"

void setup() {
  bluetooth_setup();
}

void loop() {
    bluetooth_update();
}