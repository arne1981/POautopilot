#include <math.h>
void motor_set(int, int);
#include "bluetooth.h"
#include <analogWrite.h>

#define RPWM 33
#define LPWM 32

long prevMillis = 0;
int startSpeed = 255;
float current = 0;

//for pulsing motor
bool motor_running = false;
long motor_state_millis = 0;  //to check how long the motor is in the current state, on or off

//loop times
static uint32_t prev_pid_ms = 0;

void setup() {
  Serial.begin(115200);
  bluetooth_setup();
}

void loop() {
  if (millis() - prev_pid_ms > 100) {
    bluetooth_loop();
  }
}

void motor_set(int motor_speed, int motor_direction) {
  Serial.print("motor_set ");
  Serial.println(motor_speed);
  if (motor_speed == 0) {
    motor_stop();
  }

  if (motor_speed > 255) motor_speed = 255;

  if (motor_direction == 1) {
    analogWrite(LPWM, 0);
    analogWrite(RPWM, motor_speed);
  } else {  // motor direct = 0
    analogWrite(RPWM, 0);
    analogWrite(LPWM, motor_speed);
  }

  if (!motor_running) {
    motor_running = true;
    motor_state_millis = millis();
  }
}

void motor_stop() {
  analogWrite(LPWM, 0);
  analogWrite(RPWM, 0);

  if (motor_running) {
    motor_running = false;
    motor_state_millis = millis();
  }
}
