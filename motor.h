#include <analogWrite.h>

#define RPWM 33
#define LPWM 32

void motor_stop() {
  analogWrite(LPWM, 0);
  analogWrite(RPWM, 0);
}

void motor_set(int motor_speed, int motor_direction) {
  if (motor_speed == 0) {
    motor_stop();
    return;
  }

  if (motor_speed > 255) motor_speed = 255;

  if (motor_direction == 1) {
    analogWrite(LPWM, 0);
    analogWrite(RPWM, motor_speed);
  } else {  // motor direct = 0
    analogWrite(RPWM, 0);
    analogWrite(LPWM, motor_speed);
  }
}