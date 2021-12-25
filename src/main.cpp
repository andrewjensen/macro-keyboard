#include "Arduino.h"
#include <Bounce2.h>

#define PIN_BUTTON_LEFT 4
#define PIN_BUTTON_RIGHT 6
#define PIN_LED 13

const int DEBOUNCE_TIME   = 10; // ms
const int FLASH_TIME      = 50; // ms

Bounce btnLeft = Bounce(PIN_BUTTON_LEFT, DEBOUNCE_TIME);
Bounce btnRight = Bounce(PIN_BUTTON_RIGHT, DEBOUNCE_TIME);

// List of available key codes:
// https://www.pjrc.com/teensy/td_keyboard.html

// NOTE: I had to order functions based on the dependency graph

// IO HELPERS -----------------------------------------------------------------

void lightOn() {
  digitalWrite(PIN_LED, HIGH);
}

void lightOff() {
  digitalWrite(PIN_LED, LOW);
}

void flash() {
  lightOn();
  delay(FLASH_TIME);
  lightOff();
}

void sendKeyboardCommand(int modifier, int mainKey) {
  Keyboard.set_modifier(modifier);
  Keyboard.send_now();

  Keyboard.set_key1(mainKey);
  Keyboard.send_now();

  Keyboard.set_modifier(0);
  Keyboard.set_key1(0);
  Keyboard.send_now();
}

// COMMANDS -------------------------------------------------------------------

void toggleZoomMute() {
  sendKeyboardCommand(MODIFIERKEY_GUI | MODIFIERKEY_SHIFT, KEY_A);
}

void playPause() {
  Keyboard.press(KEY_MEDIA_PLAY_PAUSE);
  delay(300);  // Hold the key down so the OS recognizes it
  Keyboard.release(KEY_MEDIA_PLAY_PAUSE);
}

void nextTrack() {
  Keyboard.press(KEY_MEDIA_NEXT_TRACK);
  delay(300);  // Hold the key down so the OS recognizes it
  Keyboard.release(KEY_MEDIA_NEXT_TRACK);
}

// BUTTONS --------------------------------------------------------------------

void onBtnLeft() {
  playPause();
}

void onBtnRight() {
  nextTrack();

  // toggleZoomMute();
}

// LIFECYCLE ------------------------------------------------------------------

void setup() {
  pinMode(PIN_BUTTON_LEFT, INPUT_PULLUP);
  pinMode(PIN_BUTTON_RIGHT, INPUT_PULLUP);
  pinMode(PIN_LED, OUTPUT);
}

void loop() {
  btnLeft.update();
  btnRight.update();

  if (btnLeft.fell()) {
    flash();
    onBtnLeft();
  }

  if (btnRight.fell()) {
    flash();
    onBtnRight();
  }
}
