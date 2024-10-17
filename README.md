# robo-keyboard-arduino-due

This repository contains the Arduino Due sketch for the Robot Keyboard project. This program allows an Arduino Due to act as a virtual keyboard, enabling you to perform keyboard macros or conduct testing.

## ROBO-KEYBOARD User Guide

**General Commands**

* `HELP`: Sends this help text to the serial port.
* `TYPE_HELP`: Sends this help text to the keyboard.
* `PING`: Checks if the device is responsive.
* `STATUS`: Retrieves the current status of the device.
* `LOREM`: Types the Lorem Ipsum text on the keyboard.
* `LOREM_LINES`: Types the Lorem Ipsum text on the keyboard as 4 lines.

**Text Commands**

* `TEXT:<message>`: Sends a message as keystrokes.  
   Example: `TEXT:Hello, World!`
* `LINE:<message>`: Sends a message followed by a newline.  
   Example: `LINE:This is a new line`

**Key Commands**

* `KEY:<key>`: Sends a single key press.  
   Example: `KEY:A` (types 'A')
* `COMBO:<keys>`: Sends a combination of keys (e.g., CTRL-ALT-DEL).  
   Example: `COMBO:CTRL-ALT-DEL`
* `EDIT:<action>`: Performs an edit action (cut, copy, paste, select all).  
   Examples: `EDIT:CUT`, `EDIT:COPY`, `EDIT:PASTE`, `EDIT:SELECTALL`
* `HOLD:<key>`: Holds a key down.  
   Example: `HOLD:SHIFT`
* `RELEASE_KEY:<key>`: Releases a specific key.  
   Example: `RELEASE_KEY:SHIFT`
* `RELEASE`: Releases all held keys.

**Command Mode Controls**

* `CMD:OUTPUT ON`: Stops sending keys to the keyboard.
* `CMD:OUTPUT OFF`: Resumes sending keys to the keyboard.
* `CMD:SET:DELAY:<value>`: Sets the delay between keystrokes.  
   Example: `CMD:SET:DELAY:100` (sets delay to 100 ms)
* `CMD:SET:PRESS-LENGTH:<value>`: Sets the length of key presses.  
   Example: `CMD:SET:PRESS-LENGTH:50` (sets press length to 50 ms)
* `CMD:RESET`: Resets the device.
* `CMD:ECHO ON`: Enables echoing of serial input.
* `CMD:ECHO OFF`: Disables echoing of serial input.
* `CMD:DEBUG ON`: Enables debug messages.
* `CMD:DEBUG OFF`: Disables debug messages.
* `CMD:JITTER ON`: Enables jitter for both key press and release timings.
* `CMD:JITTER OFF`: Disables jitter for both key press and release timings.
* `CMD:KEY_JITTER ON`: Enables jitter for key press timings only.
* `CMD:KEY_JITTER OFF`: Disables jitter for key press timings only.
* `CMD:DELAY_JITTER ON`: Enables jitter for key interval timings only.
* `CMD:DELAY_JITTER OFF`: Disables jitter for key interval timings only.
* `CMD:SET:KEY_JITTER_MAX:<value>`: Sets maximum key press jitter value.  
   Example: `CMD:SET:KEY_JITTER_MAX:20` (sets max key press jitter to 20 ms)
* `CMD:SET:DELAY_JITTER_MAX:<value>`: Sets maximum delay after keypress jitter value.  
   Example: `CMD:SET:DELAY_JITTER_MAX:30` (sets max delay jitter to 30 ms)
* `CMD:CONNECT`: Connects the keyboard.
* `CMD:DISCONNECT`: Disconnects the keyboard.
* `CMD:RECONNECT`: Reconnects the keyboard.

**Special Notes**

* Hyphens (-) can be substituted for underscores (_) in commands.
* Shortnames for keys:
    * `@C` is CTRL
    * `@S` is SHIFT
    * `@A` is ALT
    These `@` modifiers work in combo along with the F-keys. Everything else apart from the separator `-` is assumed to be normal keys.
* Special keys in the `KEY` command:
    * `\R` or `\N` for RETURN
    * `\B` for BACKSPACE
    * `\T` for TAB

**Example Usage**

* **Typing a Message:**  
   Command: `TEXT:Hello`  
   This will type "Hello" on the connected keyboard.

* **Sending a Combination of Keys:**  
   Command: `COMBO:CTRL-ALT-DEL`  
   This will send the Ctrl+Alt+Del key combination.

* **Setting a Delay:**  
   Command: `CMD:SET:DELAY:200`  
   This sets a 200 ms delay between keystrokes.


## Supported Keys

**Modifiers and Control Keys:**

* `@C`: Left Control (CTRL)
* `@S`: Left Shift (SHIFT)
* `@A`: Left Alt (ALT)
* `CTRL`: Left Control (CTRL)
* `SHIFT`: Left Shift (SHIFT)
* `ALT`: Left Alt (ALT)
* `GUI`: Left GUI (Windows key)
* `ALT_GR`: Right Alt (ALT GR)
* `LEFT_CTRL`: Left Control (CTRL)
* `LEFT_SHIFT`: Left Shift (SHIFT)
* `LEFT_ALT`: Left Alt (ALT)
* `LEFT_GUI`: Left GUI (Windows key)
* `RIGHT_CTRL`: Right Control (CTRL)
* `RIGHT_SHIFT`: Right Shift (SHIFT)
* `RIGHT_ALT`: Right Alt (ALT)
* `RIGHT_GUI`: Right GUI (Windows key)

**Navigation and Editing Keys:**

* `UP`: Up Arrow
* `DOWN`: Down Arrow
* `LEFT`: Left Arrow
* `RIGHT`: Right Arrow
* `DEL`: Delete
* `BS`: Backspace
* `BKSP`: Backspace
* `UP_ARROW`: Up Arrow
* `DOWN_ARROW`: Down Arrow
* `LEFT_ARROW`: Left Arrow
* `RIGHT_ARROW`: Right Arrow
* `BACKSPACE`: Backspace
* `TAB`: Tab
* `RETURN`: Return (Enter)
* `MENU`: Menu
* `ESC`: Escape
* `INSERT`: Insert
* `DELETE`: Delete
* `PAGE_UP`: Page Up
* `PAGE_DOWN`: Page Down
* `HOME`: Home
* `END`: End

**Lock Keys:**

* `CAPS_LOCK`: Caps Lock
* `NUM_LOCK`: Num Lock
* `SCROLL_LOCK`: Scroll Lock

**System Keys:**

* `PRINT_SCREEN`: Print Screen
* `PAUSE`: Pause
* `KP_SLASH`: Keypad Slash
* `KP_ASTERISK`: Keypad Asterisk
* `KP_MINUS`: Keypad Minus
* `KP_PLUS`: Keypad Plus
* `KP_ENTER`: Keypad Enter
* `KP_DOT`: Keypad Dot
* `KP_1`: Keypad 1
* `KP_2`: Keypad 2
* `KP_3`: Keypad 3
* `KP_4`: Keypad 4
* `KP_5`: Keypad 5
* `KP_6`: Keypad 6
* `KP_7`: Keypad 7
* `KP_8`: Keypad 8
* `KP_9`: Keypad 9   

* `KP_0`: Keypad 0

**Function Keys:**

* `F1`: F1
* `F2`: F2
* `F3`: F3
* `F4`: F4
* `F5`: F5
* `F6`: F6
* `F7`: F7
* `F8`: F8
* `F9`: F9
* `F10`: F10
* `F11`: F11
* `F12`: F12
* `F13`: F13
* `F14`: F14
* `F15`: F15
* `F16`: F16
* `F17`: F17
* `F18`: F18
* `F19`: F19
* `F20`: F20
* `F21`: F21
* `F22`: F22
* `F23`: F23
* `F24`: F24   


**Special Characters:**

* `\\R` or `\\N`: Return (Enter)
* `\\B`: Backspace
* `\\T`: Tab

**Explanation**

These keys and their aliases allow you to send various types of key presses and combinations to the Arduino Due, which will then emulate these as keyboard inputs on the connected machine. The commands can be used in the `TEXT`, `KEY`, `COMBO`, `HOLD`, and other relevant command types to perform actions as if you were physically pressing the keys