/*
Macro Keyboard
by Andrew Jensen, 2020

To configure before compiling:

- Board: Teensy LC
- USB Type: Keyboard
- Port: (whatever is available)

*/

#include <Bounce2.h>

#define PIN_BUTTON_TOGGLE 4
#define PIN_BUTTON_MOMENTARY 6
#define PIN_LED 13

const int DEBOUNCE_TIME   = 10; // ms
const int FLASH_TIME      = 50; // ms

Bounce btnToggle = Bounce(PIN_BUTTON_TOGGLE, DEBOUNCE_TIME);
Bounce btnMomentary = Bounce(PIN_BUTTON_MOMENTARY, DEBOUNCE_TIME);

void setup() {
  pinMode(PIN_BUTTON_TOGGLE, INPUT_PULLUP);
  pinMode(PIN_BUTTON_MOMENTARY, INPUT_PULLUP);
  pinMode(PIN_LED, OUTPUT);
}

void loop() {
  btnToggle.update();
  btnMomentary.update();

  if (btnToggle.fell()) {
    toggleZoomMute();
    flash();
  }

  if (btnMomentary.fell()) {
    toggleZoomMute();
    flash();
  }

  if (btnMomentary.rose()) {
    toggleZoomMute();
    flash();
  }
}

// COMMANDS --------------------------------------------------------------------

void toggleZoomMute() {
  sendKeyboardCommand(MODIFIERKEY_GUI | MODIFIERKEY_SHIFT, KEY_A);
}

// void stepOver() {
//   sendKeyboardCommand(MODIFIERKEY_GUI, KEY_QUOTE);
// }

// void stepInto() {
//   sendKeyboardCommand(MODIFIERKEY_GUI, KEY_SEMICOLON);
// }

// void stepOut() {
//   sendKeyboardCommand(MODIFIERKEY_GUI | MODIFIERKEY_SHIFT, KEY_SEMICOLON);
// }

// void toggleBreakpoints() {
//   sendKeyboardCommand(MODIFIERKEY_GUI, KEY_F8);
// }

// void toggleCurrentLine() {
//   sendKeyboardCommand(MODIFIERKEY_GUI, KEY_B);
// }

// IO HELPERS ------------------------------------------------------------------

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
  delay(FLASH_TIME);
  lightOff();
}

void lightOn() {
  digitalWrite(PIN_LED, HIGH);
}

void lightOff() {
  digitalWrite(PIN_LED, LOW);
}
