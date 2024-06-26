#include "config.h"
#define CHECK_INTERVAL   1   // Delay inbetween reading gpio values
#define PRESS_INTERVAL   2  // Minimum time before a press is registered
#define RELEASE_INTERVAL 20 // Minimum time before a press can be released

// Assing pins to each row and column
// Make sure the order is from left to right and top to bottom
// from the perspective of looking at the keypad from above
const uint8_t input_cols[NUM_COLS] = {D5, D4, D6};
const uint8_t input_rows[NUM_ROWS] = {D1, D3, D8, D7};

// Maps each pair j, i (which is a key) to an unique integer:
uint8_t keymap[NUM_ROWS][NUM_COLS]; 
// The current debounced state of a key, which could be different than the instantaneous reading:
uint8_t debounced_key[NUM_ROWS][NUM_COLS];
// For a given key, this is the number of contiguous readings until a state change is accepted:
uint8_t counts[NUM_ROWS][NUM_COLS];
// Number of readings needed to change switch state to released (LOW)
uint8_t release_count  = RELEASE_INTERVAL / CHECK_INTERVAL;
// Number of readings needed to change swithc state to pressed (HIGH)
uint8_t press_count    = PRESS_INTERVAL   / CHECK_INTERVAL;

void debounce(int j, int i, bool *key_changed, uint8_t *key_pressed) {
  // Static var to count how many reads until the key state will
  // register as changed subject to the debouncing rules
  uint8_t count = counts[j][i];
  uint8_t debounced_key_latest = debounced_key[j][i];
  *key_changed = false;
  *key_pressed = debounced_key_latest;
  uint8_t raw_state = digitalRead(input_rows[j]);

  if (raw_state == debounced_key_latest) {
    if (debounced_key_latest == HIGH) {
      count = release_count;
    }
    else {
      count = press_count;
    }
  }

  else {
    if (--count == 0) {
      debounced_key[j][i] = raw_state;
      *key_changed = true;
      *key_pressed = raw_state;
      // Then resent the countdown
      if (raw_state == HIGH) {
        count = release_count;
      }
      else {
        count = press_count;
      }

    }
  }
  counts[j][i] = count;
}

void setup() {
  // initialize column pins to a low state
  for (int i = 0; i < NUM_COLS; ++i) {
    pinMode(input_cols[i], OUTPUT);
    digitalWrite(input_cols[i], LOW);
  }

  // intialize row pins as inputs
  for (int j = 0; j < NUM_ROWS; ++j) {
    pinMode(input_rows[j], INPUT_PULLDOWN);
  }

  uint8_t symbol = 0;
  for (int j = 0; j < NUM_ROWS; ++j) {
    for (int i = 0; i < NUM_COLS; ++i) {
      keymap[j][i] = symbol;        // Keys assigned as integers starting from 0 increasing left and then down
      debounced_key[j][i] = LOW;    // Assume every key starts of in a LOW state
      counts[j][i] = release_count; // Assume keys are currently being "pressed" in the low position
      ++symbol;
    }
  }
  Serial.begin(BAUDRATE);
}

void loop() {
  // Every "CHECK_INTERVAL" miliseconds supply power to the columns and then scan each row
  delay(CHECK_INTERVAL);
  for (int i = 0; i < NUM_COLS; ++i) {
    // Select column and set to low
    digitalWrite(input_cols[i], HIGH);
    for (int j = 0; j < NUM_ROWS; ++j) {
      bool key_changed = false;
      uint8_t key_pressed = LOW;
      debounce(j, i, &key_changed, &key_pressed);
      if (key_changed == true & key_pressed == HIGH) {
        Serial.write(keymap[j][i]);
      }
    }
    digitalWrite(input_cols[i], LOW);
  }
}
