#define CHECK_INTERVAL   5   // Delay inbetween reading gpio values
#define PRESS_INTERVAL   10  // Minimum time before a press is registered
#define RELEASE_INTERVAL 100 // Minimum time before a press can be released

const int num_cols = 3;
const int num_rows = 4;

int val = LOW;
const uint8_t input_cols[] = {D5, D4, D6};
const uint8_t input_rows[] = {D1, D3, D8, D7};

uint8_t keymap[num_rows][num_cols] = {{0, 1, 2}, {3, 4, 5}, {6, 7, 8}, {9, 10, 11}}; 
bool_t debounced_key[num_rows] = {{false, false, false, false}}

void debounce(int j) {
  static uint8_t count = RELEASE_INTERVAL / CHECK_INTERVAL;
  bool_t raw_state;
  raw_state = digitalRead(input_rows[j]);
  if (raw_state != debounced_key[j]) {
    if (debounced_key[j]) {
      Count = RELEASE_MSEC / CHECK_MSEC;
    }
    else {
      Count = PRESS_MSEC / CHECK_MSEC;
    }
  }
  else {
    if (--count == 0) {
      debounced_key[j] = raw_state;
      if (raw_state) {
        count = RELEASE_INTERVAL / CHECK_INTERVAL;
      }
      else {
        count = PRESS_INTERVAL / CHECK_INTERVAL;
      }

    }
  }

}

void setup() {
  // initialize column pins to a low state
  for (int i = 0; i < num_cols; ++i) {
    pinMode(input_cols[i], OUTPUT);
    digitalWrite(input_cols[i], LOW);
  }

  // intialize row pins as inputs
  for (int j = 0; j < num_rows; ++j) {
    pinMode(input_rows[j], INPUT_PULLDOWN);
  }

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  // Serial.println("y");
  delay(5);
  for (int i = 0; i < num_cols; ++i) {
    // Select column and set to low
    digitalWrite(input_cols[i], HIGH);
    for (int j = 0; j < num_rows; ++j) {
      if (digitalRead(input_rows[j]) == HIGH) {
        Serial.write(keymap[j][i]);
        //delay(50);

        while(digitalRead(input_rows[j]) == HIGH);

        //delay(50);
      }
    }
    digitalWrite(input_cols[i], LOW);
    //delay(100);
  }
}
