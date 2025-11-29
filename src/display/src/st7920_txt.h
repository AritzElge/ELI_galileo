#ifndef ST7920_TXT_H
#define ST7920_TXT_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
*                                                              *
*  Library defined to handle the ST7920 display in text mode   *
*                                                              *
*  The connection is made via SPI:                             *
*     13(SCK)  <-> E(Enable)                                   *
*     11(MOSI) <-> R/W                                         *
*     10(CS)   <-> RS                                         *
*               -> RST                                         *
*                                                              * 
*                                                              *
*  ST7920_txt(int pinReset) (Constructor)                      *
*                                                              *
*  void begin()                                                *
*  void clear()                                                *
*  void home()                                                 *
*  void print(int row, int column, *char text)                 *
*  void reverse(int row)                                       *
*  void setPos(int row, int column)                            *
*  void putByte(byte data))                                    *
*                                                              *
*                                                              *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <mraa/spi.hpp>
#include <mraa/gpio.hpp>
#include <stdint.h>

 class ST7920_txt {
    // Attributes
    private:
        int LCD_RST;

        mraa::Spi* spi_context;
        mraa::Gpio* cs_pin;
    // Methods
    public:
        ST7920_txt(int Reset);             // Constructor

        void begin();                           // Initializes display
        void clear();                           // Clears display
        void home();                            // Position (0, 0)
        void print(int x, int y, const char* text);
        void print(const char* text);                 // Writes text at (x, y)
        void reverse(int x);                    // Sets the row to reverse mode
        void setPos(int x, int y);              // Sets the cursor position at (x, y)
        void putByte(bool d_c, uint8_t data);   // Sends a byte via SPI

    // private:
};

#endif
