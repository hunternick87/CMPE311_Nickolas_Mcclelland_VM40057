

const int LED1 = 2;
const int LED2 = 3;

unsigned long previousMillis1 = 0;
unsigned long previousMillis2 = 0;
int interval1 = 0; // default for LED1 (off)
int interval2 = 0; // default for LED2 (off)
bool ledState1 = false;
bool ledState2 = false;

void setup() {
  Serial.begin(9600);
  
  // set LED pins as outputs
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  
  promptUser();
}

// On startup the code sets pin 2 and pin 3 as outputs then prompts the user to select an LED and set its blinking interval.
// once the user selects an LED, the code waits for the user to input a blinking interval.
// it also swiches the internal 'state; to 1, which means the code is waiting for the interval input.
// once the user inputs the interval, the code sets the interval var for the selected LED and switches the state back to 0. 
// The code then checks if the intervals was 0 or less, if it was then it turns off the LED.
// if the interval was greater than 0, it sets the LED to blink at the specified interval.

void loop() {
  // get the serial input
  checkSerialInput();
  
  unsigned long currentMillis = millis();
  
  // toggle LED1
  if (interval1 > 0) {
    if (currentMillis - previousMillis1 >= interval1) {
      previousMillis1 = currentMillis;
      ledState1 = !ledState1;
      digitalWrite(LED1, ledState1);
    }
  } else {
    // interval is 0, turn LED off
    digitalWrite(LED1, LOW);
  }
  
  // toggle LED2
  if (interval2 > 0) {
    if (currentMillis - previousMillis2 >= interval2) {
      previousMillis2 = currentMillis;
      ledState2 = !ledState2;
      digitalWrite(LED2, ledState2);
    }
  } else {
    // interval is 0, turn LED off
    digitalWrite(LED2, LOW);
  }
}

void checkSerialInput() {
  static int state = 0; // 0 = waiting for LED selection, 1 = waiting for interval input
  static int selectedLED = 0;
  
  if (Serial.available() > 0) {
    if (state == 0) {
      selectedLED = Serial.parseInt();
      Serial.read();
      
      if (selectedLED == 1 || selectedLED == 2) {
        Serial.print("Arduino: What interval (in msec)? \n");
        state = 1; // move to next input state
      } else {
        Serial.println("Invalid LED number. Please enter 1 or 2. \n");
        promptUser();
      }
    } 
    else if (state == 1) {
      int newInterval = Serial.parseInt();
      Serial.read();
      
      // set the interval for the selected LED
      if (selectedLED == 1) {
        interval1 = newInterval;
        Serial.println(newInterval);
        if (newInterval <= 0) {
          Serial.println("[LED1 turned OFF] \n");
          digitalWrite(LED1, LOW);
          ledState1 = false;
        } else {
          Serial.println("[LED1 now blinking at interval of " + String(newInterval) + "ms] \n");
        }
      } else {
        interval2 = newInterval;
        Serial.println(newInterval);
        if (newInterval <= 0) {
          Serial.println("[LED2 turned OFF] \n");
          digitalWrite(LED2, LOW);
          ledState2 = false;
        } else {
          Serial.println("[LED2 now blinking at interval of " + String(newInterval) + "ms] \n");
        }
      }
      
      // reset state and send new prompt
      state = 0;
      promptUser();
    }
  }
}

void promptUser() {
  Serial.print("Arduino: What LED? (1 or 2) \n");
}
