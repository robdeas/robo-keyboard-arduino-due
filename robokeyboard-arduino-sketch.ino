/*
 * This file is part of Robokeyboard.
 *
 * Copyright (C) 2024 Rob Deas
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */
#include <Keyboard.h>
#include <HID.h>

// Define a struct to represent key mappings
struct KeyMapping {
  const char* text;
  uint8_t keyCode;
};

enum CommandType {
  HELP,
  TYPE_HELP,
  PING,
  STATUS,
  LOREM,
  LOREM_LINES,
  TEXT,
  LINE,
  KEY,
  COMBO,
  EDIT,
  HOLD,
  CMD_OUTPUT_OFF,
  CMD_OUTPUT_ON,
  CMD_SET_DELAY,
  CMD_SET_PRESS_LENGTH,
  CMD_RESET,
  CMD_ECHO_ON,
  CMD_ECHO_OFF,
  CMD_DEBUG_ON,
  CMD_DEBUG_OFF,
  CMD_JITTER_ON,
  CMD_JITTER_OFF,
  CMD_KEY_JITTER_ON,
  CMD_KEY_JITTER_OFF,
  CMD_DELAY_JITTER_ON,
  CMD_DELAY_JITTER_OFF,
  CMD_SET_KEY_JITTER_VALUE,
  CMD_SET_DELAY_JITTER_VALUE,
  CMD_CONNECT,
  CMD_DISCONNECT,
  CMD_RECONNECT,
  RELEASE_KEY,
  RELEASE_ALL_KEYS,
  INVALID
};

// Define an array of key mappings
KeyMapping keyMappings[] = {
  { "@C", KEY_LEFT_CTRL },
  { "@S", KEY_LEFT_SHIFT },
  { "@A", KEY_LEFT_ALT },
  { "\\R", KEY_RETURN },
  { "\\N", KEY_RETURN },
  { "\\B", KEY_BACKSPACE },
  { "\\T", KEY_TAB },
  { "UP", KEY_UP_ARROW },
  { "DOWN", KEY_DOWN_ARROW },
  { "LEFT", KEY_LEFT_ARROW },
  { "RIGHT", KEY_RIGHT_ARROW },
  { "CTRL", KEY_LEFT_CTRL },
  { "SHIFT", KEY_LEFT_SHIFT },
  { "ALT", KEY_LEFT_ALT },
  { "GUI", KEY_LEFT_GUI },
  { "ALT_GR", KEY_RIGHT_ALT },
  { "DEL", KEY_DELETE },
  { "BS", KEY_BACKSPACE },
  { "BKSP", KEY_BACKSPACE },
  { "LEFT_CTRL", KEY_LEFT_CTRL },
  { "LEFT_SHIFT", KEY_LEFT_SHIFT },
  { "LEFT_ALT", KEY_LEFT_ALT },
  { "LEFT_GUI", KEY_LEFT_GUI },
  { "RIGHT_CTRL", KEY_RIGHT_CTRL },
  { "RIGHT_SHIFT", KEY_RIGHT_SHIFT },
  { "RIGHT_ALT", KEY_RIGHT_ALT },
  { "RIGHT_GUI", KEY_RIGHT_GUI },
  { "UP_ARROW", KEY_UP_ARROW },
  { "DOWN_ARROW", KEY_DOWN_ARROW },
  { "LEFT_ARROW", KEY_LEFT_ARROW },
  { "RIGHT_ARROW", KEY_RIGHT_ARROW },
  { "BACKSPACE", KEY_BACKSPACE },
  { "TAB", KEY_TAB },
  { "RETURN", KEY_RETURN },
  { "MENU", KEY_MENU },
  { "ESC", KEY_ESC },
  { "INSERT", KEY_INSERT },
  { "DELETE", KEY_DELETE },
  { "PAGE_UP", KEY_PAGE_UP },
  { "PAGE_DOWN", KEY_PAGE_DOWN },
  { "HOME", KEY_HOME },
  { "END", KEY_END },
  { "CAPS_LOCK", KEY_CAPS_LOCK },
  { "PRINT_SCREEN", KEY_PRINT_SCREEN },
  { "SCROLL_LOCK", KEY_SCROLL_LOCK },
  { "PAUSE", KEY_PAUSE },
  { "NUM_LOCK", KEY_NUM_LOCK },
  { "KP_SLASH", KEY_KP_SLASH },
  { "KP_ASTERISK", KEY_KP_ASTERISK },
  { "KP_MINUS", KEY_KP_MINUS },
  { "KP_PLUS", KEY_KP_PLUS },
  { "KP_ENTER", KEY_KP_ENTER },
  { "KP_1", KEY_KP_1 },
  { "KP_2", KEY_KP_2 },
  { "KP_3", KEY_KP_3 },
  { "KP_4", KEY_KP_4 },
  { "KP_5", KEY_KP_5 },
  { "KP_6", KEY_KP_6 },
  { "KP_7", KEY_KP_7 },
  { "KP_8", KEY_KP_8 },
  { "KP_9", KEY_KP_9 },
  { "KP_0", KEY_KP_0 },
  { "KP_DOT", KEY_KP_DOT },
  { "F1", KEY_F1 },
  { "F2", KEY_F2 },
  { "F3", KEY_F3 },
  { "F4", KEY_F4 },
  { "F5", KEY_F5 },
  { "F6", KEY_F6 },
  { "F7", KEY_F7 },
  { "F8", KEY_F8 },
  { "F9", KEY_F9 },
  { "F10", KEY_F10 },
  { "F11", KEY_F11 },
  { "F12", KEY_F12 },
  { "F13", KEY_F13 },
  { "F14", KEY_F14 },
  { "F15", KEY_F15 },
  { "F16", KEY_F16 },
  { "F17", KEY_F17 },
  { "F18", KEY_F18 },
  { "F19", KEY_F19 },
  { "F20", KEY_F20 },
  { "F21", KEY_F21 },
  { "F22", KEY_F22 },
  { "F23", KEY_F23 },
  { "F24", KEY_F24 },
};

const char LF = '\n';
const char CR = '\r';
// jitter is intended for testing fragile apis, the jitter value for keys will need to be kept short unless testing debouncing of keypresses for example
// as otherwise it will give repeated letters sometimes
const int KEY_JITTER_MAX_VALUE = 50;
// jitter interval between keys will default to larger that for keypress
const int DELAY_JITTER_MAX_VALUE = 100;
int keystrokeDelay = 0;  // Initial delay between keystrokes is 0 millisecs but each delay will have a minimum overriding it
int keyPressLength = 0;  // the time a key will be held down again there will be a minimum
int keyJitterMaxValue = KEY_JITTER_MAX_VALUE;
int delayJitterMaxValue = DELAY_JITTER_MAX_VALUE;
bool isDebugEnabled = true;
bool isConnected = false;
bool isEchoEnabled = true;
bool isStopRequested = false;
bool addKeyJitter = false;
bool addDelayJitter = false;

void setup() {
  delay(150);
  // Start the serial communication
  Serial.begin(9600);
  Keyboard.begin();
  isConnected = true;
  // Wait for a stable connection (optional)
  delay(1000);
  Serial.println("");
  infoMessage("Keyboard emulation started");
  infoMessage("HELP command will print help.");
}


void printHelp(bool sendToKeyboard = false) {
  // Define the help message
  const char* helpMessage[] = {
    "",
    "ROBO-KEYBOARD",
    "-------------",
    "This uses an Arduino Due to act as a virtual keyboard, that can be used for keyboard macros or testing."
    "",
    "Usage:",
    "  HELP - Send this help text to the serial port.",
    "  TYPE_HELP - Send this help text to the keyboard.",
    "  PING - Check if the device is responsive",
    "  STATUS - Get current status of the device",
    "  LOREM - Type the Lorem Ipsum text on the keyboard",
    "  LOREM_LINES - Type the Lorem Ipsum text on the keyboard, as 4 lines of",
    "  TEXT:<message> - Send a message as keystrokes",
    "  LINE:<message> - Send a message followed by a newline",
    "  KEY:<key> - Send a single key press",
    "  COMBO:<keys> - Send a combination of keys (e.g., CTRL-ALT-DEL)",
    "  EDIT:<action> - Perform an edit action (cut, copy, paste, selectall)",
    "  HOLD:<key> - Hold a key down",
    "  RELEASE_KEY:<key> - Release a specific key",
    "  RELEASE:ALL - Release all held keys",
    "  CMD:OUTPUT:OFF - Stop sending keys to the keyboard",
    "  CMD:OUTPUT:ON - Resume sending keys to the keyboard",
    "  CMD:SET:DELAY:<value> - Set delay between keystrokes",
    "  CMD:SET:PRESS-LENGTH:<value> - Set the length of key presses",
    "  CMD:RESET - Reset the device",
    "  CMD:ECHO:ON - Enable echoing of serial input",
    "  CMD:ECHO:OFF - Disable echoing of serial input",
    "  CMD:DEBUG:ON - Enable debug messages",
    "  CMD:DEBUG:OFF - Disable debug messages",
    "  CMD:JITTER:ON - Enable jitter for both key press and release timings",
    "  CMD:JITTER:OFF - Disable jitter for both key press and release timings",
    "  CMD:KEY_JITTER:ON - Enable jitter for key press timings only",
    "  CMD:KEY_JITTER:OFF - Disable jitter for key press timings only",
    "  CMD:DELAY_JITTER:ON - Enable jitter for key interval timings only",
    "  CMD:DELAY_JITTER:OFF - Disable jitter for key interval timings only",
    "  CMD:SET:KEY_JITTER_MAX:<value> - Set maximum key press jitter value",
    "  CMD:SET:DELAY_JITTER_MAX:<value> - Set maximum delay after keypress jitter value",
    "  CMD:CONNECT - Connect the keyboard",
    "  CMD:DISCONNECT - Disconnect the keyboard",
    "  CMD:RECONNECT - Reconnect the keyboard",
    "",
    "NOTE: - can be substituted for _ in commands.",
    "There are some shortnames for keys: @C is CTRL, @S is SHIFT, @A is ALT ",
    "These @ modifiers will work in combo along wth the f-keys everything else appart from the seperator - there is assumed to be normal keys.",
    "in the key command you can also use \\R or \\N as RETURN, \\B as BACKSPACE, \\T as TAB.",
    ""

  };


  // Send the help message either to the Serial or to the Keyboard
  for (const char* line : helpMessage) {
    if (sendToKeyboard) {
      // Send each line of the help message to the keyboard
      for (const char* p = line; *p != '\0'; p++) {
        Keyboard.press(*p);
        delay(10);
        Keyboard.release(*p);
        delay(10);
      }
      // Send a newline character after each line
      Keyboard.press(KEY_RETURN);
      delay(10);
      Keyboard.release(KEY_RETURN);
      delay(10);
    } else {
      // Send each line of the help message to the Serial
      Serial.println(F(line));
    }
  }
}


// Function to find the key code based on text
uint8_t findKeyCode(const String& text) {
  for (size_t i = 0; i < sizeof(keyMappings) / sizeof(keyMappings[0]); i++) {
    if (text.equals(keyMappings[i].text)) {
      debugMessage("Key found: " + text + ", keyCode: " + String(keyMappings[i].keyCode));
      return keyMappings[i].keyCode;
    }
  }
  debugMessage("Key not found: " + text);
  // Return 0 if key not found
  return 0;
}

void sendKeyboardMessage(const String& message, bool newLine = false) {
  debugMessage("Processing sendMessage to Keyboard: " + message);
  for (int i = 0; i < message.length(); i++) {
    char currentChar = message[i];
    if (isStopRequested) {
      Keyboard.releaseAll();
      delay(10);
      return;
    }
    Keyboard.press(currentChar);
    delay(10);
    keyPressWaitWithMinimum(10);
    Keyboard.releaseAll();
    keyIntervalWaitWithMinimum(10);
  }
  if (newLine) sendReturn();
}


void typeLoremIpsum(bool newLine = false) {
  const char* loremIpsum1 = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. ";
  const char* loremIpsum2 = "Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. ";
  const char* loremIpsum3 = "Duis aute irure dolor in reprehender it in voluptate velit esse cillum dolore eu fugiat nulla pariatur. ";
  const char* loremIpsum4 = "Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";

  sendKeyboardMessage(loremIpsum1, newLine);
  sendKeyboardMessage(loremIpsum2, newLine);
  sendKeyboardMessage(loremIpsum3, newLine);
  sendKeyboardMessage(loremIpsum4, newLine);
}

void sendReturn() {
  Keyboard.press('\n');
  keyPressWaitWithMinimum(50);  // Delay before newline (optional, adjust as needed)
  Keyboard.releaseAll();
  keyIntervalWaitWithMinimum(50);
}

void sendPingResponse() {
  Serial.println("RESPONSE:OK");  // Indicate that the command was received and processed successfully
}

void sendStatusResponse() {
  String status = isConnected ? "connected" : "disconnected";
  Serial.print(F("RESPONSE:{\"app\": \"DueKeys\", \"status\": \""));
  Serial.print(status);
  Serial.print(F("\", \"debug\": \""));
  Serial.print(isDebugEnabled ? "on" : "off");
  Serial.print(F("\", \"echo\": \""));
  Serial.print(isEchoEnabled ? "on" : "off");
  Serial.print(F("\", \"output\": \""));
  Serial.print(isStopRequested ? "off" : "on");
  Serial.print(F("\", \"keyJitter\": \""));
  Serial.print(addKeyJitter ? "on" : "off");
  Serial.print(F("\", \"delayJitter\": \""));
  Serial.print(addDelayJitter ? "on" : "off");
  if (keystrokeDelay != 0) {
    Serial.print(F("\", \"KeyStrokeDelay\": \""));
    Serial.print(String(keystrokeDelay));
  }
  if (keyPressLength != 0) {
    Serial.print(F("\", \"keyPressLength\": \""));
    Serial.print(String(keyPressLength));
  }
  if (addKeyJitter && keyJitterMaxValue != 0) {
    Serial.print(F("\", \"keyJitterMaxValue\": \""));
    Serial.print(String(keyJitterMaxValue));
  }
  if (addDelayJitter && delayJitterMaxValue != 0) {
    Serial.print(F("\", \"delayJitterMaxValue\": \""));
    Serial.print(String(delayJitterMaxValue));
  }
  Serial.println(F("\", \"version\": \"0.4.1\"}"));
}


CommandType parseCommandType(const String& command) {
  debugMessage("<" + command + ">");
  if (command == "HELP") {
    return HELP;
  } else if (command == "TYPE_HELP" || command == "TYPE-HELP") {
    return TYPE_HELP;
  } else if (command == "PING") {
    return PING;
  } else if (command == "STATUS") {
    return STATUS;
  } else if (command == "LOREM") {
    return LOREM;
  } else if (command == "LOREM_LINES" || command == "LOREM-LINES") {
    return LOREM_LINES;
  } else if (command.startsWith("TEXT:")) {
    // Text is a string that doesnt have a new line
    return TEXT;
  } else if (command.startsWith("LINE:")) {
    // Line is alike text but types a new line
    return LINE;
  } else if (command.startsWith("KEY:")) {
    return KEY;
    // press and release a key
  } else if (command.startsWith("COMBO:")) {
    // combo is a combination of keys eg ctrl-alt-x
    return COMBO;
  } else if (command.startsWith("EDIT:")) {
    // cut copy paste
    return EDIT;
  } else if (command.startsWith("HOLD:")) {
    // holds down a key and doesnt release it
    return HOLD;
  } else if (command.startsWith("CMD:SET:DELAY:")) {
    // an additional delay adter each keypress, delays will be needed anyway, but could still sometimes be too fast
    return CMD_SET_DELAY;
  } else if (command.startsWith("CMD:SET:DELAY-JITTER-MAX:") || command.startsWith("CMD:SET:DELAY-JITTER_MAX:")) {
    // sets the actual max value of jitter but doesnt turn it on or off
    return CMD_SET_DELAY_JITTER_VALUE;
  } else if (command.startsWith("CMD:SET:KEY-JITTER-MAX:") || command.startsWith("CMD:SET:KEY_JITTER_MAX:")) {
    // sets the actual max value of jitter but doesnt turn it on or off
    return CMD_SET_KEY_JITTER_VALUE;
  } else if (command.startsWith("CMD:SET:PRESS-LENGTH:") || command.startsWith("CMD:SET:PRESS_LENGTH:")) {
    // the time a keypress will be held down, delays will be needed anyway, but could still sometimes be too fast
    return CMD_SET_PRESS_LENGTH;
  } else if (command.startsWith("RELEASE_KEY:") || command.startsWith("RELEASE-KEY:")) {
    // release a specific held key
    return RELEASE_KEY;
  } else if (command == "RELEASE:ALL") {
    // release all held keys, note currently using text or line will release automatically
    return RELEASE_ALL_KEYS;
  } else if (command == "CMD:OUTPUT:OFF") {
    // output off just means keys wont be sent to the keyboard until resume, any keypresses will be lost
    return CMD_OUTPUT_OFF;
  } else if (command == "CMD:OUTPUT:ON") {
    // resume sending keys to the keyboard
    return CMD_OUTPUT_ON;
  } else if (command == "CMD:RESET") {
    // reset arduino
    return CMD_RESET;
  } else if (command == "CMD:ECHO:ON") {
    return CMD_ECHO_ON;
  } else if (command == "CMD:ECHO:OFF") {
    return CMD_ECHO_OFF;
  } else if (command == "CMD:DEBUG:ON") {
    return CMD_DEBUG_ON;
  } else if (command == "CMD:DEBUG:OFF") {
    return CMD_DEBUG_OFF;
  } else if (command == "CMD:JITTER:ON") {
    // Jitter is intended for testing fragile APIS that might duplicate keys, or have some odd effects
    return CMD_JITTER_ON;
  } else if (command == "CMD:JITTER:OFF") {
    return CMD_JITTER_OFF;
  } else if (command == "CMD:KEY_JITTER:ON") {
    // Jitter is intended for testing fragile APIS that might duplicate keys, or have some odd effects
    return CMD_KEY_JITTER_ON;
  } else if (command == "CMD:KEY_JITTER:OFF") {
    return CMD_DELAY_JITTER_OFF;
  } else if (command == "CMD:DELAY_JITTER:ON") {
    // Jitter is intended for testing fragile APIS that might duplicate keys, or have some odd effects
    return CMD_DELAY_JITTER_ON;
  } else if (command == "CMD:DELAY_JITTER:OFF") {
    return CMD_DELAY_JITTER_OFF;
  } else if (command == "CMD:CONNECT") {
    return CMD_CONNECT;
  } else if (command == "CMD:DISCONNECT") {
    return CMD_DISCONNECT;
  } else if (command == "CMD:RECONNECT") {
    return CMD_RECONNECT;
  } else {
    return INVALID;
  }
}


void keyPressWaitWithMinimum(int minimum) {
  waitWithMinimumValues(minimum, keyPressLength, keyJitterMaxValue, addKeyJitter);
}

void keyIntervalWaitWithMinimum(int minimum) {
  waitWithMinimumValues(minimum, keystrokeDelay, delayJitterMaxValue, addDelayJitter);
}

/**
* there is both a key and a delay jitter they take default values
*/
void waitWithMinimumValues(int minimum, int delayMillisSetting, int jitterMaxValue, bool addJitter) {
  int delayMillis = delayMillisSetting;
  if (addJitter && jitterMaxValue > 0) {
    // could mean if we fiddle with these values jitter will be tiny or dissapear as there is a minimum
    if (delayMillisSetting == 0) {
      delayMillis = jitterMaxValue + minimum;
    }
    int jitter = random(0, jitterMaxValue + 1);
    if (random(0, 2) == 0) {  // Randomly decide to add or subtract
      delayMillis += jitter;
    } else {
      delayMillis -= jitter;
    }
    if (delayMillis < 0) {
      delayMillis = 0;  // Ensure delayMillis is not negative
    }
  }
  if (delayMillis < minimum) delayMillis = minimum;
  if (addJitter) debugMessage("Delay applied: " + String(delayMillis));
  delay(delayMillis);
}

void sendKey(const String& text, boolean hold) {
  String upperCaseText = text;
  upperCaseText.toUpperCase();
  Serial.println("Keyboard key send requested : " + text);
  uint8_t keyCode = findKeyCode(upperCaseText);
  if (keyCode != 0) {
    debugMessage("Key code found : " + upperCaseText);
    if (isStopRequested) {
      Keyboard.releaseAll();
      delay(10);
      return;
    }
    Keyboard.press(keyCode);  // Press the key
    keyPressWaitWithMinimum(50);
    if (!hold) {                  // Optional delay to ensure key press is registered
      Keyboard.release(keyCode);  // Release the key
      keyIntervalWaitWithMinimum(50);
      debugMessage("key released");
    }
  }
}

void sendKeyCombination(String keys) {
  String lowerCaseModifiers = keys;
  lowerCaseModifiers.toLowerCase();

  // Split the keys string into individual key components
  int indexOfCtrl = lowerCaseModifiers.indexOf("ctrl");
  if (indexOfCtrl == -1) indexOfCtrl = lowerCaseModifiers.indexOf("@c");
  int indexOfAlt = lowerCaseModifiers.indexOf("alt");
  if (indexOfAlt == -1) indexOfAlt = lowerCaseModifiers.indexOf("@a");
  int indexOfAltGr = lowerCaseModifiers.indexOf("altgr");
  int indexOfShift = lowerCaseModifiers.indexOf("shift");
  if (indexOfShift == -1) indexOfShift = lowerCaseModifiers.indexOf("@s");
  int indexOfGui = lowerCaseModifiers.indexOf("gui");
  int indexOfKey = lowerCaseModifiers.indexOf("-");

  if (indexOfCtrl == -1 && indexOfAlt == -1 && indexOfShift == -1 && indexOfGui == -1 && indexOfKey == -1 && indexOfAltGr == -1) {
    debugMessage("No modifier key");
  }

  // Check if each modifier is present in the keys string and press accordingly
  if (indexOfCtrl != -1) {
    Keyboard.press(KEY_LEFT_CTRL);
    keyPressWaitWithMinimum(10);
    debugMessage("CTRL held down");
  }
  if (indexOfAlt != -1) {
    Keyboard.press(KEY_LEFT_ALT);
    keyPressWaitWithMinimum(10);
    debugMessage("ALT held down");
  }
  if (indexOfShift != -1) {
    Keyboard.press(KEY_LEFT_SHIFT);
    keyPressWaitWithMinimum(10);
    debugMessage("SHIFT held down");
  }
  if (indexOfGui != -1) {
    Keyboard.press(KEY_LEFT_GUI);
    keyPressWaitWithMinimum(10);
    debugMessage("GUI held down");
  }
  if (indexOfAltGr != -1) {
    Keyboard.press(KEY_RIGHT_ALT);
    keyPressWaitWithMinimum(10);
    debugMessage("ALT-GR held down");
  }

  // Press the specified key
  String mainKey = keys.substring(indexOfKey + 1);
  if (mainKey.startsWith("f")) {
    // If the key is a function key (F1-F24)
    int fKeyNumber = mainKey.substring(1).toInt();
    if (fKeyNumber >= 1 && fKeyNumber <= 24) {
      Keyboard.press(KEY_F1 + fKeyNumber - 1);  // Convert F1-F24 to corresponding key code
      debugMessage("Function key pressed.");
    }
  } else {
    // If the key is not a function key, assume it's a normal key
    //Keyboard.press(mainKey.toInt());  // Convert to key code (assuming it's a key code)
    for (int i = 0; i < mainKey.length(); i++) {
      char currentChar = mainKey[i];
      if (isStopRequested) {
        Keyboard.releaseAll();
        delay(10);
        return;
      }
      Keyboard.press(currentChar);
      delay(10);
    }
    debugMessage("Normal key press.");
  }
  keyPressWaitWithMinimum(10);
  // Release all keys
  Keyboard.releaseAll();
  keyIntervalWaitWithMinimum(10);
}

void releaseKey(const String& text) {
  String upperCaseText = text;
  upperCaseText.toUpperCase();
  debugMessage("Keyboard key send requested : " + upperCaseText);
  uint8_t keyCode = findKeyCode(upperCaseText);
  // Optional delay to ensure key press is registered
  Keyboard.release(keyCode);  // Release the key
  keyIntervalWaitWithMinimum(50);
  debugMessage("key released");
}

void resetArduino() {
  // Reset the Arduino Due
  NVIC_SystemReset();
}

void sendEditAction(char actionKey) {
  Keyboard.press(KEY_LEFT_CTRL);
  keyPressWaitWithMinimum(10);
  Keyboard.press(actionKey);
  keyPressWaitWithMinimum(50);
  Keyboard.releaseAll();
  keyIntervalWaitWithMinimum(50);
}

// Function to perform the cut action (Ctrl + X)
void cut() {
  sendEditAction('x');
  debugMessage("cut sent");
}

// Function to perform the copy action (Ctrl + C)
void copy() {
  sendEditAction('c');
  debugMessage("copy sent");
}

// Function to perform the paste action (Ctrl + V)
void paste() {
  sendEditAction('v');
  debugMessage("paste sent");
}

// Function to perform the select all action (Ctrl + A)
void selectAll() {
  sendEditAction('a');
  debugMessage("Select all sent");
}


void handleEditCommand(const String& text) {
  debugMessage("Handling edit command");
  String command = text;
  command.toLowerCase();
  if (command == "cut") {
    cut();
  } else if (command == "copy") {
    copy();
  } else if (command == "paste") {
    paste();
  } else if (command == "selectall" || command == "select:all" || command == "select-all") {
    selectAll();
  } else {
    debugMessage("A bad edit command was given");
    // Handle invalid command
  }
}

void handleCommand(const String& commandString) {
  // might get extra blank lines , if terminator is CR/LF but can safely ignore them
  if (commandString.length() > 0) {
    String upperCaseCommandString = commandString;
    upperCaseCommandString.toUpperCase();
    CommandType type = parseCommandType(upperCaseCommandString);
    String parameter = commandString.substring(commandString.indexOf(':') + 1);

    // Declare variable outside of the switch statement
    int delayValue = 0;

    switch (type) {
      case HELP:
        printHelp();
        break;
      case TYPE_HELP:
        printHelp(true);
        break;
      case PING:
        sendPingResponse();
        break;
      case STATUS:
        sendStatusResponse();
        break;
      case LOREM:
        typeLoremIpsum();
        break;
      case LOREM_LINES:
        typeLoremIpsum(true);
        break;
      case TEXT:
        sendKeyboardMessage(parameter);
        break;
      case LINE:
        sendKeyboardMessage(parameter, true);
        break;
      case KEY:
        sendKey(parameter, false);
        break;
      case COMBO:
        sendKeyCombination(parameter);
        break;
      case EDIT:
        handleEditCommand(parameter);
        break;
      case HOLD:
        sendKey(parameter, true);
        break;
      case RELEASE_KEY:
        releaseKey(parameter);
        break;
      case RELEASE_ALL_KEYS:
        Keyboard.releaseAll();  // Release the key
        delay(50);
        break;
      case CMD_OUTPUT_OFF:
        isStopRequested = true;
        break;
      case CMD_OUTPUT_ON:
        isStopRequested = false;
        break;
      case CMD_SET_KEY_JITTER_VALUE:
        // Move the variable declaration here
        keyJitterMaxValue = commandString.substring(23).toInt();  // Extract delay value from command
        debugMessage("Maximum key jitter value set to +/- " + String(keyJitterMaxValue) + " milliseconds");
        break;
      case CMD_SET_DELAY_JITTER_VALUE:
        // Move the variable declaration here
        delayJitterMaxValue = commandString.substring(25).toInt();  // Extract delay value from command
        debugMessage("Maximum delay jitter value set to +/- " + String(delayJitterMaxValue) + " milliseconds");
        break;
      case CMD_SET_DELAY:
        // Move the variable declaration here
        delayValue = commandString.substring(14).toInt();  // Extract delay value from command
        keystrokeDelay = delayValue;
        debugMessage("Additional Keystroke delay set to " + String(keystrokeDelay) + " milliseconds");
        break;
      case CMD_SET_PRESS_LENGTH:
        keyPressLength = commandString.substring(21).toInt();  // Extract delay value from command
        debugMessage("Additional keypress time set to " + String(keystrokeDelay) + " milliseconds");
        break;
      case CMD_RESET:
        // Handle reset command
        delay(5000);
        resetArduino();
        break;
      case CMD_ECHO_ON:
        isEchoEnabled = true;
        break;
      case CMD_ECHO_OFF:
        isEchoEnabled = false;
        break;
      case CMD_DEBUG_ON:
        isDebugEnabled = true;
        break;
      case CMD_DEBUG_OFF:
        isDebugEnabled = false;
        break;
      case CMD_JITTER_ON:
        addKeyJitter = true;
        addDelayJitter = true;
        break;
      case CMD_JITTER_OFF:
        addKeyJitter = false;
        addDelayJitter = false;
        break;
      case CMD_KEY_JITTER_ON:
        addKeyJitter = true;
        break;
      case CMD_KEY_JITTER_OFF:
        addKeyJitter = false;
        break;
      case CMD_DELAY_JITTER_ON:
        addDelayJitter = true;
        break;
      case CMD_DELAY_JITTER_OFF:
        addDelayJitter = false;
        break;
      case CMD_CONNECT:
        // Handle connect command
        if (!isConnected) {
          Keyboard.begin();
          debugMessage("Keyboard is now connected");
        } else {
          isConnected = true;  // Simulate keyboard connection
          debugMessage("Keyboard already connected");
        }
        break;
      case CMD_DISCONNECT:
        // Handle disconnect command
        Keyboard.end();
        isConnected = false;  // Simulate keyboard disconnection
        debugMessage("Keyboard is now disconnected");
        break;
      case CMD_RECONNECT:
        // Handle reconnect command
        Keyboard.end();
        delay(500);
        Keyboard.begin();
        delay(500);
        isConnected = true;  // Simulate keyboard connection
        debugMessage("Keyboard reconnected");
        break;
      case INVALID:
        // Handle invalid command
        Serial.println("Invalid command.");
        break;
    }
  }
}

void debugMessage(const String& message) {
  if (isDebugEnabled) {
    Serial.print("DEBUG:");
    Serial.println(message);
  }
}

void infoMessage(const String& message) {
  Serial.print("INFO:");
  Serial.println(message);
}

void errorMessage(const String& message) {
  Serial.print("ERROR:");
  Serial.println(message);
}

String readSerialUntil(int maxLength) {

  String inputString = "";
  while (true) {
    if (Serial.available() > 0) {
      char incomingChar = Serial.read();

      if (incomingChar == LF || incomingChar == CR) {
        if (isEchoEnabled) {
          Serial.println("");
        }
        // If the current character is CR, check if the next character is LF
        if (incomingChar == CR && Serial.peek() == LF) {
          // Dont add the next character (LF) to the command string and break out of the loop
          // it only doesnt get disguarded if it needs to be echoed back to the input device
          char disposedChar = Serial.read();
        }
        break;  // Exit the loop after processing LF or CR
      }
      if (isEchoEnabled) {
        Serial.write(incomingChar);
      }
      inputString += incomingChar;  // Add the character to the input string
      if (inputString.length() >= maxLength) {
        break;  // Exit the loop if the maximum length is reached
      }
    }
  }
  return inputString;
}

void loop() {
  String command = readSerialUntil(1024);  // Read until newline character or max chars
  if (command.length() > 0) {
    infoMessage("Sending To Keyboard: " + command);
  }
  handleCommand(command);
}