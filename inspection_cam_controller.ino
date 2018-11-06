#include <Mouse.h>

#define PIN_AF 2
#define PIN_MF 3
#define PIN_AF_MOMENTARY 4

#define TRUE 1
#define FALSE 0

#define MODE_UNKNONW 0
#define MODE_AF 1
#define MODE_MF 2

int mouse_initialized = FALSE;
int mode = MODE_UNKNONW;

void setup() {
    pinMode(PIN_AF, INPUT_PULLUP);
    pinMode(PIN_MF, INPUT_PULLUP);
    pinMode(PIN_AF_MOMENTARY, INPUT_PULLUP);
    Mouse.begin();
}

void mouse_init() {
    if (!mouse_initialized) {
        Mouse.begin();
        mouse_initialized = TRUE;
    }
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
    mouse_to_menu();
    delay(50);
    Mouse.move(15,27);
    delay(10);
    Mouse.click();
    delay(50);
    Mouse.move(100,-50);
    mode = MODE_AF;
}

void set_mf()
{
    mouse_to_menu();
    delay(50);
    Mouse.move(70,27);
    delay(10);
    Mouse.click();
    delay(50);
    Mouse.move(100,-50);
    mode = MODE_MF;
}

void loop() {
     if (!digitalRead(PIN_AF)) {
         if (mode != MODE_AF) {
             set_af();
         }
     } else if (!digitalRead(PIN_MF)) {
         if (mode != MODE_MF) {
             set_mf();
         }
     } else if (!digitalRead(PIN_AF_MOMENTARY)) {
     }

}
