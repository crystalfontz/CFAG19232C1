#define __AVR_ATmega32U4__
#include <avr/io.h> 
//==============================================================================
//
//  CRYSTALFONTZ
//
//  This code drives the CFAG19232C and CFAG19232C1 family of displays
//    https://www.crystalfontz.com/product/cfag19232c1yyhtt
//    https://www.crystalfontz.com/product/cfag19232cyyhtt
//    https://www.crystalfontz.com/product/cfag19232ctmitt
//
//  The controller is a Sitronix ST7920
//    https://www.crystalfontz.com/controllers/uploaded/Sitronix_ST7920_v4.0.pdf
//
//  Seeeduino v4.2, an open-source 3.3v capable Arduino clone.
//    https://www.crystalfontz.com/product/cfapn15062-seeeduino-arduino-clone-microprocessor
//    https://github.com/SeeedDocument/SeeeduinoV4/raw/master/resources/Seeeduino_v4.2_sch.pdf
//
//==============================================================================
//This is free and unencumbered software released into the public domain.
//
//Anyone is free to copy, modify, publish, use, compile, sell, or
//distribute this software, either in source code form or as a compiled
//binary, for any purpose, commercial or non-commercial, and by any
//means.
//
//In jurisdictions that recognize copyright laws, the author or authors
//of this software dedicate any and all copyright interest in the
//software to the public domain. We make this dedication for the benefit
//of the public at large and to the detriment of our heirs and
//successors. We intend this dedication to be an overt act of
//relinquishment in perpetuity of all present and future rights to this
//software under copyright law.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
//OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
//ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
//OTHER DEALINGS IN THE SOFTWARE.
//
//For more information, please refer to <http://unlicense.org/>
//==============================================================================
// LCD control lines
// | DISPLAY | ARDUINO | Port  | Function - 6800 Parallel |
// | :-----: | :-----: | :---: | :----------------------- |
// | 1       | GND     | ANY   | GROUND                   |
// | 2       | 5V      | ANY   | POWER 5V                 |
// | ------- | ------- | ----- | ------------------------ |
// | 3       | N/A     | N/A   | NC                       |
// | ------- | ------- | ----- | ------------------------ |
// | 4       |  D8     | PORTB | Register Select     (RS) |
// | 5       |  D9     | PORTB | Read/Write         (R/W) |
// | 6       |  D10    | PORTB | Chip Enable          (E) |
// | ------- | ------- | ----- | ------------------------ |
// | 7       |  D0     | PORTD | (DB0)                    |
// | 8       |  D1     | PORTD | (DB1)                    |
// | 9       |  D2     | PORTD | (DB2)                    |
// | 10      |  D3     | PORTD | (DB3)                    |
// | 11      |  D4     | PORTD | (DB4)                    |
// | 12      |  D5     | PORTD | (DB5)                    |
// | 13      |  D6     | PORTD | (DB6)                    |
// | 14      |  D7     | PORTD | (DB7)                    |
// | ------- | ------- | ----- | ------------------------ |
// | 15      | N/A     | N/A   | NC                       |
// | 16      | N/A     | N/A   | NC                       |
//
//==============================================================================
//
//  This code demonstrates using 6800 Parallel
//
//==============================================================================
//This creates a software serial port since the default serial port is used by the display
#include <SoftwareSerial.h>
SoftwareSerial mySerial(12, 13); // RX, TX
//==============================================================================
#define PIN_RS 0x01
#define PIN_RW 0x02
#define PIN_E  0x04
#define HRES 192
#define VRES 32
#define pixelCount_X (HRES/8)
#define pixelCount_Y VRES
//==============================================================================
#define CLR_RS (PORTB &= ~(PIN_RS))
#define SET_RS (PORTB |=  (PIN_RS))
#define CLR_RW (PORTB &= ~(PIN_RW))
#define SET_RW (PORTB |=  (PIN_RW))
#define CLR_E  (PORTB &= ~(PIN_E))
#define SET_E  (PORTB |=  (PIN_E))
//==============================================================================
#include "CFAG19232C1_Splash.h"
//==============================================================================
//Function Prototypes
void showImage(const uint8_t (&image)[pixelCount_Y][pixelCount_X]);
void waitBusy();
void writeCommand(uint8_t command);
void writeData(uint8_t data);
void setGraphicsModeAddress(uint8_t y, uint8_t x_word);
void st7920_init();
void fillScreen(uint8_t value);
void showImage(const uint8_t (&image)[pixelCount_Y][pixelCount_X]);
void showStripes(bool invert);
//================================================================================
void setup()
{
  //Set up port directions
  DDRB = 0xFF;
  DDRD = 0xFF;

  //Idle the lines in a reasonable state
  PORTD = 0x00;
  CLR_E;

  //Initialize Software Serial
  mySerial.begin(9600);
  mySerial.println("Serial init");

  //Initialize the display
  st7920_init();

}
//================================================================================
void loop()
{
  uint8_t x, y;

  mySerial.println("Top of the loop");

  writeCommand(0x36);
  delay(250);
  uint8_t dTime = 10;

  // Empty screen
  mySerial.println("All off");
  fillScreen(0x00);
  delay(2000);

  //Write stripes
  mySerial.println("Stripes 1");
  showStripes(false);
  delay(2000);

  mySerial.println("Stripes 2");
  showStripes(true);
  delay(2000);

  // Full screen
  mySerial.println("All on");
  fillScreen(0xFF);
  delay(2000);

  // Splash screen
  mySerial.println("Splash screen");
  showImage(cfag19232c);
  
  // Leave the splash screen on the display
  while(1);
}
//================================================================================
void waitBusy()
{
  bool busy;

  //Clear the buffer
  PORTD = 0x00;

  //Clear RS to put the display into Command Mode
  CLR_RS;
  //delayMicroseconds(100);

  //Set RW to read from the display
  SET_RW;
  //delayMicroseconds(100);

  do
  {
    //select the LCD controller
    SET_E;
    delayMicroseconds(1);

    //Read the value off the port. Busy flag it PORTD.7
    uint8_t response = PORTD;
    busy = (bool)(response>>7);
    delayMicroseconds(1);

    //deselect the LCD controller
    CLR_E;
    delayMicroseconds(10);

    if(busy)
    {
      mySerial.println("Busy");
    }
  } while (busy);
}
//================================================================================
void writeCommand(uint8_t command)
{
  //Clear RS to put the display into Command Mode
  CLR_RS;
  delayMicroseconds(100);
  CLR_RW;
  delayMicroseconds(100);

  //Put the values on the port
  PORTD = command;
  delayMicroseconds(100);

  //select the LCD controller
  SET_E;
  delayMicroseconds(100);

  //deselect the LCD controller
  CLR_E;
  delayMicroseconds(100);

  //Check to see if the controller is busy
  waitBusy();
}
//================================================================================
void writeData(uint8_t data)
{
  //Set RS to put the display into Data Mode
  SET_RS;
  delayMicroseconds(1);
  CLR_RW;
  delayMicroseconds(1);

  //Put the values on the port
  PORTD = data;
  delayMicroseconds(100);

  //select the LCD controller
  SET_E;
  delayMicroseconds(100);

  //deselect the LCD controller
  CLR_E;
  delayMicroseconds(100);

  //Check to see if the controller is busy
  waitBusy();
}
//================================================================================
//X is only addressable to the word, a grouping of 16 horizontal pixels:
//  0-17 => 18 * 8 = 144
void setGraphicsModeAddress(uint8_t y, uint8_t x_word)
{
  //Set Y pixel address
  writeCommand(0x80 | y);
  //Set X byte address
  writeCommand(0x80 | x_word);
}
//================================================================================
void st7920_init()
{
  //Function set            | 0 | 0 | 1 | 1 | X | 0 |X   | X |
  writeCommand(0x30);            // 0x30
  //Function set            | 0 | 0 | 1 | 1 | X | 0 |X   | X |
  writeCommand(0x30);            // 0x30
  //Display ON/OFF Control  | 0 | 0 | 0 | 0 | 1 | D |C   | B |
  writeCommand(0x0C);  // 0x0C
  //Entry mode set          | 0 | 0 | 0 | 0 | 0 | 1 |I/D | S |
  writeCommand(0x06);              // 0x06
  //Home
  writeCommand(0x02);                        // 0x02
  //Display Clear           | 0 | 0 | 0 | 0 | 0 | 0 |0   | 1 |
  writeCommand(0x01);                // 0x01
}
//================================================================================
void fillScreen(uint8_t value)
{
  uint8_t x;
  uint8_t y;

  for(y = 0; y < pixelCount_Y; y++)
  {
    //Set Y address to this line, reset X address
    setGraphicsModeAddress(y, 0);
    for(x = 0; x < pixelCount_X; x++)
    {
      writeData(value);
    }
  }
}
//================================================================================
void showImage(const uint8_t (&image)[pixelCount_Y][pixelCount_X])
{
  uint8_t x;
  uint8_t y;
  //Get the image from the flash, stuff it into the RAM frame buffer.
  for(y = 0; y < pixelCount_Y; y++)
  {
    //Set Y address to this line, reset X address
    setGraphicsModeAddress(y, 0);
    for(x = 0; x < pixelCount_X; x++)
    {
      //Normal
      writeData(pgm_read_byte(&image[y][x]));
    }
  }
}
//================================================================================
void showStripes(bool invert)
{
  uint8_t x;
  uint8_t y;
  //Send the FrameBuffer RAM data to the LCD.
  for(y = 0; y < pixelCount_Y; y++)
  {
    //Set Y address to this line, reset X address
    setGraphicsModeAddress(y, 0);
    for(x = 0; x < pixelCount_X; x++)
    {
      if (x%2==invert)
      {
        writeData(0x00);
      }
      else
      {
        writeData(0xff);
      }
    }
  }
}
//================================================================================
