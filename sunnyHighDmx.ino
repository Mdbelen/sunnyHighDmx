#include "Conceptinetics.h"
#include <SoftwareSerial.h>
//#include "FastLED.h"

#define DMX_MASTER_CHANNELS 100
#define RXEN_PIN 2
DMX_Master dmx_master(DMX_MASTER_CHANNELS, RXEN_PIN);

#define CTS_PIN 9  // Connect this Trinket pin to BLE 'CTS' pin
#define RX_PIN 10   // Connect this Trinket pin to BLE 'RX' pin
#define TXO_PIN 11  // Connect this Trinket pin to BLE 'TXO' pin

#define FPS 60 // Animation frames/second (ish)

unsigned long mainLightData[100][4];

SoftwareSerial ser(RX_PIN, TXO_PIN);

//////////////////////FUNCTION FOR DELAYLESS TIMING
int brightness = 10;
int dim = 10;
int fadeAmount = 5;
uint8_t g_fast = 10;
unsigned step = 5;  // interval at which to blink (milliseconds)

void setup() {
  pinMode(CTS_PIN, OUTPUT);
  digitalWrite(CTS_PIN, HIGH);
  ser.begin(9600);
  dmx_master.enable();
  dmx_master.setChannelRange(1, 100, 0);

}

//ledhinten addr 68


// FILE ledPar.cpp

#define NUM_ROWS 5
#define NUM_COLS 2

void setRoofSpot(uint8_t col, uint8_t row, uint8_t red, uint8_t grn, uint8_t blu)
{
    uint8_t rowAdd = col==0 ? row * 6 : (3-row) * 6;
    uint8_t addr = col * 24 + rowAdd;
    if(row==4) addr = 71;
    dmx_master.setChannelValue(addr + 1, red);
    dmx_master.setChannelValue(addr + 2, grn);
    dmx_master.setChannelValue(addr + 3, blu);
}
void setRoofSpot(uint8_t col, uint8_t row, uint8_t red, uint8_t grn, uint8_t blu, uint8_t a1, uint8_t a2, uint8_t a3)
{
    uint8_t rowAdd = col==0 ? row * 6 : (3-row) * 6;
    uint8_t addr = col * 24 + rowAdd;
    if(row==4) addr = 71;
    dmx_master.setChannelValue(addr + 1, red);
    dmx_master.setChannelValue(addr + 2, grn);
    dmx_master.setChannelValue(addr + 3, blu);
    dmx_master.setChannelValue(addr + 4, a1);
    dmx_master.setChannelValue(addr + 5, a2);
    dmx_master.setChannelValue(addr + 6, a3);
}

// col = left right, 0 = window, 1 = bar
void setRoofCol(uint8_t col, uint8_t red, uint8_t grn, uint8_t blu) {
  for (uint8_t row = 0; row < NUM_ROWS; row++)
  {
    setRoofSpot(col, row, red, grn, blu);
  }
}
void setRoofCol(uint8_t col, uint8_t red, uint8_t grn, uint8_t blu, uint8_t a1, uint8_t a2, uint8_t a3) {
  for (uint8_t row = 0; row < NUM_ROWS; row++)
  {
    setRoofSpot(col, row, red, grn, blu, a1, a2, a3);
  }
}

// row = dj to back, 0 = dj, 3 = back
void setRoofRow(uint8_t row, uint8_t red, uint8_t grn, uint8_t blu) {
  for (uint8_t col = 0; col < NUM_COLS; col++)
  {
    setRoofSpot(col, row, red, grn, blu);
  }
}
void setRoofRow(uint8_t row, uint8_t red, uint8_t grn, uint8_t blu, uint8_t a1, uint8_t a2, uint8_t a3) {
  for (uint8_t col = 0; col < NUM_COLS; col++)
  {
    setRoofSpot(col, row, red, grn, blu, a1, a2, a3);
  }
}

void setRoof(uint8_t red, uint8_t grn, uint8_t blu) {
  for (uint8_t col = 0; col < NUM_COLS; col++)
  {
    setRoofCol(col, red, grn, blu);
  }
}
void setRoof(uint8_t red, uint8_t grn, uint8_t blu, uint8_t a1, uint8_t a2, uint8_t a3) {
  for (uint8_t col = 0; col < NUM_COLS; col++)
  {
    setRoofCol(col, red, grn, blu, a1, a2, a3);
  }
}





















uint8_t buf[3],          // Enough for RGB parse; expand if using sensors
  animMode = 1;          // Current animation mode
uint32_t prevTime = 0L;  // For animation timing

uint8_t redd = 255, green = 0, blue = 0;

void loop(void) {
  int c;
  uint32_t t;

  digitalWrite(CTS_PIN, LOW);  // Signal to BLE, OK to send data!
  for (;;) {
    t = micros();                              // Current time
    if ((t - prevTime) >= (1000000L / FPS)) {  // 1/30 sec elapsed?
      prevTime = t;
      break;                        // Yes, go update LEDs
    }                               // otherwise...
    if ((c = ser.read()) == '!') {  // Received UART app input?
      while ((c = ser.read()) < 0)
        ;  // Yes, wait for command byte
      switch (c) {
        case 'B':  // Button (Control Pad)
          if (readAndCheckCRC(255 - '!' - 'B', buf, 2) && (buf[1] == '1')) {
            buttonPress(buf[0]);  // Handle button-press message
          }
          break;
        case 'C':  // Color Picker
          if (readAndCheckCRC(255 - '!' - 'C', buf, 3)) {
            redd = buf[0];
            green = buf[1];
            blue = buf[2];
          }
          break;
      }
    }
  }
  digitalWrite(CTS_PIN, HIGH);

  switch (animMode /*sketch*/) {
    case 0:
      fullon();
      break;
    case 1:
      pulse();
      break;
    case 2:
      randomizer();
      break;
    case 3:
      lauflicht();
      break;
    case 4:
      strobe();
      break;
    case 5:
      chaseCol();
      break;
    case 6:
      chaseRow();
      break;
    case 7:
      auto1();
      break;
    case 8:
      auto2();
      break;
    case 9:
      auto3();
      break;
  }
}

boolean readAndCheckCRC(uint8_t sum, uint8_t *buf, uint8_t n) {
  for (int c;;) {
    while ((c = ser.read()) < 0)
      ;                           // Wait for next byte
    if (!n--) return (c == sum);  // If CRC byte, we're done
    *buf++ = c;                   // Else store in buffer
    sum -= c;                     // and accumulate sum
  }
}

//CHANGED SERIAL TO SER ERROR GONE... DOES IT CHANGE STH ON THE RUNNING SCRIPT?!
void skipBytes(uint8_t n) {
  while (n--) {
    while (ser.read() < 0)
      ;
  }
}

void buttonPress(char c) {
  static uint8_t fogLvl = 0;
  switch (c) {
    case '1':
      fogLvl = fogLvl==192 ? 255 : (fogLvl==255 ? 0 : fogLvl+64);
      dmx_master.setChannelValue(64, fogLvl);
      //animMode = 4;
      //dim = 255;
      //step = 1;
      break;
    case '2':
      fogLvl = 0;
      dmx_master.setChannelValue(64, 0);
      break;
    case '3':
      if (dim > 5) {
        dim -= 5;
      }
      break;
    case '4':
      if (dim < 150) {
        dim += 5;
      }
      break;
    case '5': //arrowLTop
      if (step < 24) {
        step++;
      }
    if (g_fast < 24) {
        g_fast++;
      }
      break;
    case '6': //arrowDown
      if (step > 1) {
        step--;
      }
    if (g_fast > 1) {
        g_fast--;
      }
      break;
    case '8':  //arrowRight
      animModeUp();
      break;
    case '7':  //arrowLeft
      animModeDown();
      break;
  }
}

void animModeUp() {
  setRoof(0, 0, 0, 0, 0, 0);
  if (animMode < 9) {
    animMode++;
  } else {
    animMode = 0;
  }
}

void animModeDown() {
  setRoof(0, 0, 0, 0, 0, 0);
  if (animMode > 0) {
    animMode--;
  }
}
