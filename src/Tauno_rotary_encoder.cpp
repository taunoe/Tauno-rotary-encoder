/*
 * File         Tauno_rotary_encoder.cpp
 * Last edited  03.04.2021
 * 
 * Copyright 2021 Tauno Erik
 * https://taunoerik.art/
 */

#include <Arduino.h>

#include <Tauno_rotary_encoder.h>

// Constructor
Tauno_Rotary_Encoder::Tauno_Rotary_Encoder(
    uint8_t SW_PIN,
    uint8_t CLK_PIN,
    uint8_t DT_PIN)
    : new_SW_PIN(SW_PIN),
      new_CLK_PIN(CLK_PIN),
      new_DT_PIN(DT_PIN),
      _counter(0),    // Running count of encoder clicks
      _last_time(0),  // Last time read click
      _period(600),   // Velocity calculation period
      _last_speed(0)  // Last calculated speed (no sign) in clicks/second
    {}

// Destructor
Tauno_Rotary_Encoder::~Tauno_Rotary_Encoder() {
  // Cleanup
}

/*
 *  Call it on setup()
 */
void Tauno_Rotary_Encoder::begin() {
  pinMode(new_CLK_PIN, INPUT_PULLUP);
  pinMode(new_DT_PIN, INPUT_PULLUP);
  pinMode(new_SW_PIN, INPUT_PULLUP);
}


void Tauno_Rotary_Encoder::say_pins() {
  Serial.print("SW_PIN: ");
  Serial.print(new_SW_PIN);
  Serial.print(" CLK_PIN: ");
  Serial.print(new_CLK_PIN);
  Serial.print(" DT_PIN: ");
  Serial.println(new_DT_PIN);
}


/**
 * Reads rotary encoder turn direction
 * Returns:
 * 255 == DIR_CCW - Counterclockwise
 *   1 == DIR_CW  - Clockwise
 *   0 == DIR_NONE
 **/
uint8_t Tauno_Rotary_Encoder::read() {
  /**
   * The static keyword is used to create variables that are visible to only one function. 
   * However unlike local variables that get created and destroyed every time a function is called, 
   * static variables persist beyond the function call, preserving their data between function calls.
   * Variables declared as static will only be created and initialized the first time a function is called. 
   **/
/*
  Gray code state machine:
  CW:
  Position  Bit-A Bit-B
         0  0     0
       1/4  1     0
       1/2  1     1
       3/4  0     1
         1  0     0
  CCW:
  Position  Bit-A Bit-B
         0  0     0
       1/4  0     1
       1/2  1     1
       3/4  1     0
         1  0     0
*/

  uint8_t return_value = 0;
  static uint8_t old_a = 0;
  static uint8_t old_b = 0;

  // Lets store pin states to check  rotate direction:
  //  CW A=1,1,0 B=0,1,1
  // CCW A=0,1,1 B=1,1,0
  static uint8_t a_states = 0b00000000;
  static uint8_t b_states = 0b00000000;

  // Read pins
  uint8_t new_a = digitalRead(new_CLK_PIN);
  uint8_t new_b = digitalRead(new_DT_PIN);

  uint8_t i = 0;
  // If the value of CLK pin or the DT pin has changed
  if (new_a != old_a || new_b != old_b) {
    // shift values left
    a_states = a_states << 1;
    b_states = b_states << 1;
    // add new value at end
    a_states |= new_a;
    b_states |= new_b;

    // Serial.println(a_states, BIN);
    // Serial.println(b_states, BIN);
    // compare

    // CW 1 half stepp
    if ((a_states & 0b00000011) == 0b00000011
     && (b_states & 0b00000001) == 0b00000001) {
       return_value = 1;
    }
    // CW 2 half stepp
    if ((a_states & 0b00000010) == 0b00000010
     && (b_states & 0b00000011) == 0b00000011) {
       return_value = 1;
    }
    // CCW 1 half stepp
    if ((b_states & 0b00000011) == 0b00000011
     && (a_states & 0b00000001) == 0b00000001) {
       return_value = 255;
     }
    // CCW 2 half stepp
    if ((b_states & 0b00000010) == 0b00000010
     && (a_states & 0b00000011) == 0b00000011) {
       return_value = 255;
     }
  }

  old_a = new_a;
  old_b = new_b;

  count_clicks(return_value);
  count_speed();

  return return_value;
}


/*
 *  Counts rotary encoder clicks.
 *  Helps calculate speed.
 */
void Tauno_Rotary_Encoder::count_clicks(uint8_t status) {
  if (status != 0) {
    _counter++;
  }
}


/*
 *  Calculates speed
 */
void Tauno_Rotary_Encoder::count_speed() {
  if (millis() - _last_time >= _period) {
    // speed = ClickCount * (1000 / period)
    _last_speed = _counter * (1000 / _period);
    _last_time = millis();
    _counter = 0;
  }
}


/*
 *  Retuns speed
 */
uint16_t Tauno_Rotary_Encoder::speed() {
  return _last_speed;
}


bool Tauno_Rotary_Encoder::button() {
  /**
   * The static keyword is used to create variables that are visible to only one function. 
   * However unlike local variables that get created and destroyed every time a function is called, 
   * static variables persist beyond the function call, preserving their data between function calls.
   * Variables declared as static will only be created and initialized the first time a function is called. 
   **/
  // static uint8_t old_a = 0;

  uint8_t read = digitalRead(new_SW_PIN);
  boolean status = false;

  if (read == 0) {
    status = true;
  }

  return status;
}
