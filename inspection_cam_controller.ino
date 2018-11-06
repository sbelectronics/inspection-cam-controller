#include <Mouse.h>

#define LIVE

#define TRUE 1
#define FALSE 0

#define MODE_UNKNOWN 0
#define MODE_AF 1
#define MODE_MF 2

#define PIN_LED1C 2
#define PIN_LED2C 3
#define PIN_LED3C 4
#define PIN_LED4C 5
#define PIN_LED5C 6
#define PIN_LED6C 7
#define PIN_C1 8
#define PIN_C2 9
#define PIN_R1 A0
#define PIN_R2 A1
#define PIN_R3 A2
#define PIN_EXT A3

#define ROW_PIN_COUNT 3
#define COLUMN_PIN_COUNT 2
#define KEYPAD_PIN_COUNT (ROW_PIN_COUNT + COLUMN_PIN_COUNT)

#define LED_PIN_COUNT 6

#define KEY(r, c) (1<< (r*COLUMN_PIN_COUNT + c))

#define KEY_AF KEY(2,1)
#define KEY_CLICK_FOCUS KEY(0, 1)

#define KEY_UP(x) (((lastKeypadBitmap & x) != 0) && ((keypadBitmap & x) == 0))

int ledPins[LED_PIN_COUNT] = {PIN_LED1C, PIN_LED2C, PIN_LED3C, PIN_LED4C, PIN_LED5C, PIN_LED6C};
int keypadPins[KEYPAD_PIN_COUNT] = {PIN_C1, PIN_C2, PIN_R1, PIN_R2, PIN_R3};
int rowPins[ROW_PIN_COUNT] = {PIN_R1, PIN_R2, PIN_R3};
int columnPins[COLUMN_PIN_COUNT] = {PIN_C1, PIN_C2};

int lastKeypadBitmap = 0;

int mouse_initialized = FALSE;
int global_mode = MODE_UNKNOWN;
int cur_mode = MODE_UNKNOWN;

void setup() {
    for (int i=0; i<KEYPAD_PIN_COUNT; i++) {
        pinMode(keypadPins[i], INPUT_PULLUP);
        //digitalWrite(keypadPis[i], 1);
    }

    for (int i=0; i<LED_PIN_COUNT; i++) {
        pinMode(ledPins[i], OUTPUT);
        digitalWrite(ledPins[i], 1);
    }

    Mouse.begin();
}

int pollKeypad()
{
    int keypadBitmap = 0;
  
    // invariant: all pints are inputs with pullups
  
    for (int row=0; row<ROW_PIN_COUNT; row++) {
        // make the row low
        pinMode(rowPins[row], OUTPUT);
        digitalWrite(rowPins[row], 0);
        for (int col=0; col<COLUMN_PIN_COUNT; col++) {
            if (!digitalRead(columnPins[col])) {
                keypadBitmap |= KEY(row, col);
            }
        }
        // return the row back to high
        pinMode(rowPins[row], INPUT);
        digitalWrite(rowPins[row], 1);
    }

    return keypadBitmap;
}

void mouse_to_menu() {
  int i;
  for (i=0; i<15; i++) {
      Mouse.move(-100, -100);
      delay(5);
  }
}

void set_af()
{
#ifdef LIVE
    mouse_to_menu();
    delay(50);
    Mouse.move(15,27);
    delay(10);
    Mouse.click();
    delay(50);
    Mouse.move(100,-50);
#endif
}

void set_mf()
{
#ifdef LIVE
    mouse_to_menu();
    delay(50);
    Mouse.move(70,27);
    delay(10);
    Mouse.click();
    delay(50);
    Mouse.move(100,-50);
#endif
}

void set_mode(int mode)
{
    if (cur_mode == mode) {
        return;
    }

    switch(mode) {
        case MODE_MF:
            set_mf();
            break;

        case MODE_AF:
            set_af();
            break;
    }

    cur_mode = mode;
}

void loop() {
    int keypadBitmap = pollKeypad();

    if (KEY_UP(KEY_AF)) {
        if (global_mode == MODE_AF) {
            global_mode = MODE_MF;
        } else {
            global_mode = MODE_AF;
        }
        delay(2); // debounce
    }

    if (keypadBitmap  & KEY_CLICK_FOCUS) {
        set_mode(MODE_AF);
    } else {
        set_mode(global_mode);
    }

    if (global_mode == MODE_AF) {
        digitalWrite(PIN_LED6C, 0);
    } else {
        digitalWrite(PIN_LED6C, 1);
    }

    /*
    if (cur_mode == MODE_AF) {
        digitalWrite(PIN_LED2C, 0);
    } else {
        digitalWrite(PIN_LED2C, 1);
    }
    */

    lastKeypadBitmap = keypadBitmap;
}
