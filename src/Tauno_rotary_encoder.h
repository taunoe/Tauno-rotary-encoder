/*
 * File         Tauno_rotary_encoder.h
 * Last edited  04.04.2021
 * 
 * Copyright 2021 Tauno Erik
 * https://taunoerik.art/
 * 
 */

#ifndef LIB_TAUNO_ROTARY_ENCODER_TAUNO_ROTARY_ENCODER_H_
#define LIB_TAUNO_ROTARY_ENCODER_TAUNO_ROTARY_ENCODER_H_

#include <Arduino.h>

//  Direction values returned by read()
const uint8_t DIR_NONE = 0;
const uint8_t DIR_CW   = 1;
const uint8_t DIR_CCW  = 255;

class Tauno_Rotary_Encoder {
 private:
  // Pins
  uint8_t new_SW_PIN;
  uint8_t new_CLK_PIN;
  uint8_t new_DT_PIN;
  // Running count of encoder clicks:
  uint16_t _counter;
  // Last time read click:
  uint32_t _last_time;
  // Velocity calculation period:
  uint16_t _period;
  // Last calculated speed (no sign) in clicks/second:
  uint16_t _last_speed;

  void count_clicks(uint8_t status);

  void count_speed();

 public:
  Tauno_Rotary_Encoder(
    uint8_t SW_PIN,
    uint8_t CLK_PIN,
    uint8_t DT_PIN);

  // Destructor
  ~Tauno_Rotary_Encoder();

  void begin();  // input pullups

  void say_pins();  // only for testing

  uint8_t read();

  uint16_t speed();

  bool button();
};


#endif  // LIB_TAUNO_ROTARY_ENCODER_TAUNO_ROTARY_ENCODER_H_
