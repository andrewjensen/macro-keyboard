/*

# Macro Keyboard

by Andrew Jensen, 2020

## To configure before compiling:

- Board: Teensy LC
- USB Type: Keyboard
- Port: (whatever is available)

## Physical circuit:

- Switch needs a 10K pull-down resistor that connects PIN_SWITCH to ground.

## Resources

See the Teensy website for a reference on the `Keyboard` interface, including
convenience functions and constants for each key code.

https://www.pjrc.com/teensy/td_keyboard.html

*/

#include <Bounce2.h>

#define PIN_BUTTON_LEFT 4
#define PIN_BUTTON_RIGHT 6
#define PIN_SWITCH 10
#define PIN_LED 13

const int DEBOUNCE_TIME_MS = 10;
const int FLASH_TIME_MS    = 50;

Bounce debounceButtonLeft = Bounce(PIN_BUTTON_LEFT, DEBOUNCE_TIME_MS);
Bounce debounceButtonRight = Bounce(PIN_BUTTON_RIGHT, DEBOUNCE_TIME_MS);
Bounce debounceSwitch = Bounce(PIN_SWITCH, DEBOUNCE_TIME_MS);

// You can use the switch as a modifier by checking this variable as a button is pressed!
bool switchEnabled = false;

void setup() {
  pinMode(PIN_BUTTON_LEFT, INPUT_PULLUP);
  pinMode(PIN_BUTTON_RIGHT, INPUT_PULLUP);
  pinMode(PIN_SWITCH, INPUT);
  pinMode(PIN_LED, OUTPUT);

  Serial.begin(9600); // USB baudrate

  delay(1000);

  Serial.println("Started up!");

  switchEnabled = digitalRead(PIN_SWITCH);

  if (switchEnabled) {
    Serial.println("Switch starting in ON position");
  } else {
    Serial.println("Switch starting in OFF position");
  }
}

void loop() {
  debounceButtonLeft.update();
  debounceButtonRight.update();
  debounceSwitch.update();

  if (debounceButtonLeft.fell()) {
    leftButtonOn();
  } else if (debounceButtonLeft.rose()) {
    leftButtonOff();
  }

  if (debounceButtonRight.fell()) {
    rightButtonOn();
  } else if (debounceButtonRight.rose()) {
    rightButtonOff();
  }

  if (debounceSwitch.rose()) {
    switchEnabled = true;
    switchOn();
  } else if (debounceSwitch.fell()) {
    switchEnabled = false;
    switchOff();
  }
}

// EVENT HANDLERS --------------------------------------------------------------

void leftButtonOn() {
  Serial.println("Left: ON");
  flash();

  if (switchEnabled) {
    Keyboard.print("SUPER Hello World!");
  } else {
    Keyboard.print("Hello World!");
  }
}

void leftButtonOff() {}

void rightButtonOn() {
  Serial.println("Right: ON");
  flash();

  if (switchEnabled) {
    Keyboard.print("SUPER Another saying!");
  } else {
    Keyboard.print("Another saying!");
  }
}

void rightButtonOff() {}

void switchOn() {
  Serial.println("Switch: ON");
  flash();
}

void switchOff() {
  Serial.println("Switch: OFF");
  flash();
}

// IO HELPERS ------------------------------------------------------------------

/**
 * Send a key combination, including modifier keys.
 *
 * Use the bitwise OR operator `|` to combine modifiers.
 *
 * ### Examples:
 *
 * Send CTRL+S to save a file:
 *
 * ```
 * sendKeyboardCommand(MODIFIERKEY_CTRL, KEY_S)`
 * ```
 *
 * Send CTRL+ALT+DEL to rescue Windows:
 *
 * ```
 * sendKeyboardCommand(MODIFIERKEY_CTRL | MODIFIERKEY_ALT, KEY_DELETE)`
 * ```
 */
void sendKeyboardCommand(int modifier, int mainKey) {
  Keyboard.set_modifier(modifier);
  Keyboard.send_now();

  Keyboard.set_key1(mainKey);
  Keyboard.send_now();

  Keyboard.set_modifier(0);
  Keyboard.set_key1(0);
  Keyboard.send_now();
}

void flash() {
  lightOn();
  delay(FLASH_TIME_MS);
  lightOff();
}

void lightOn() {
  digitalWrite(PIN_LED, HIGH);
}

void lightOff() {
  digitalWrite(PIN_LED, LOW);
}
