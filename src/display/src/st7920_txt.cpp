#include "st7920_txt.h" // Assumes the header file is now named "ST7920_txt.h" or similar
#include <mraa/spi.hpp>   // Include MRAA SPI C++ API
#include <mraa/gpio.hpp>  // Include MRAA GPIO C++ API
#include <stdint.h> 
#include <stdbool.h>
#include <chrono>
#include <thread>
#include <iostream>     // For error management

#define LCD_CS      10
#define LCD_COMMAND true
#define LCD_DATA    false
#define CLR_SCREEN  0x01
#define LCD_REVERSE 0x04
#define ENTRY_MODE  0x06
#define DISPLAY_ON  0x0C
#define BASIC_SET   0x30
#define EXTEND_SET  0x34
#define HOME_SCREEN 0x80

/**
 * @brief Construct a new ST7920_txt object.
 * 
 * Initializes GPIO pins for Reset and Chip Select using MRAA, 
 * and initializes the SPI bus 0 context.
 * 
 * @param Reset The GPIO pin number used for the physical reset line (RST).
 */
ST7920_txt::ST7920_txt(int Reset) {
    LCD_RST = Reset;

    // -- MRAA GPIO Setup for Reset Pin --
    mraa::Gpio* reset_pin = new mraa::Gpio(LCD_RST);
    reset_pin->dir(mraa::DIR_OUT);
    reset_pin->write(1);

    // --- MRAA GPIO setup for Chip Select Pin ---
    // digitalWrite(LCD_RST, HIGH); // This line was misplaced in the original code
    cs_pin = new mraa::Gpio(LCD_CS);
    cs_pin->dir(mraa::DIR_OUT);
    cs_pin->write(1); // Default HIGH (inactive)

    // --- MRAA SPI Initialization ---
    spi_context = new mraa::Spi(0);                       // Initialize SPI bus 0
}

/**
 * @brief Initializes the ST7920 display controller.
 * 
 * Sends the required sequence of commands to set the display to basic 
 * instruction mode, clear the screen, set entry mode, and turn the display on.
 */
void ST7920_txt::begin() {
    cs_pin->write(0);                             // Physical reset LOW
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    cs_pin->write(1);

    putByte(LCD_COMMAND, BASIC_SET);
    putByte(LCD_COMMAND, CLR_SCREEN);
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    putByte(LCD_COMMAND, ENTRY_MODE);
    putByte(LCD_COMMAND, DISPLAY_ON);
}

/**
 * @brief Clears the display screen and homes the cursor.
 * 
 * Sends the CLR_SCREEN command and waits for the operation to complete.
 */
void ST7920_txt::clear() {                        // Clears the display and sets
                                                  //   the cursor to (0, 0)
    putByte(LCD_COMMAND, CLR_SCREEN);
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
}

/**
 * @brief Sets the cursor position to the top-left corner (0, 0).
 * 
 * Sends the HOME_SCREEN command.
 */
void ST7920_txt::home() {                         // Sets the cursor position to (0, 0)
    putByte(LCD_COMMAND, HOME_SCREEN);
}

/**
 * @brief Writes a null-terminated string at the current cursor position.
 * 
 * @param text The string to write to the display.
 */
void ST7920_txt::print(const char* text) {
                                                  // Writes a text string
    int i = 0;
    while (text[i] !=0)
        putByte(LCD_DATA, text[i ++]);
}

/**
 * @brief Writes a null-terminated string at a specific position (x, y).
 * 
 * Validates coordinates before setting cursor position and writing text.
 * 
 * @param x The row index (0-3).
 * @param y The column index (0-15).
 * @param text The string to write.
 */
void ST7920_txt::print(int x, int y, const char* text) {
                                                  // Writes a text string at (x, y)
    uint8_t rowDir[4] = {0x80, 0x90, 0x88, 0x98};
    if ((x > -1) && (x < 4) && (y > -1) && (y < 16)) {
        putByte(LCD_COMMAND, rowDir[x] + y);
        int i = 0;
        while (text[i] !=0)
            putByte(LCD_DATA, text[i ++]);
    }
}

/**
 * @brief Enables reverse mode (negative display) for a specific row.
 * 
 * @param y The row index (0-3) to set in reverse mode.
 */
void ST7920_txt::reverse(int y) {                 // Sets the row 'y' to reverse mode
    if ((y > -1) && (y < 4)) {
        putByte(LCD_COMMAND, EXTEND_SET);
        putByte(LCD_COMMAND, LCD_REVERSE + y); 
        putByte(LCD_COMMAND, BASIC_SET);
    }
}

/**
 * @brief Sets the cursor position for subsequent print operations.
 * 
 * Validates coordinates before setting cursor position.
 * 
 * @param x The row index (0-3).
 * @param y The column index (0-15).
 */
void ST7920_txt::setPos(int x, int y) {           // Positions the cursor at (x, y) to write
    uint8_t rowDir[4] = {0x80, 0x90, 0x88, 0x98};
    if ((x > -1) && (x < 4) && (y > -1) && (y < 16))
        putByte(LCD_COMMAND, rowDir[x] + y);
}

/**
 * @brief Helper function to send a single byte (command or data) via SPI.
 * 
 * Manually toggles the Chip Select line to frame the communication burst.
 * 
 * @param d_c Data/Command selector (true for command, false for data).
 * @param data The 8-bit data/command byte to send.
 */
void ST7920_txt::putByte(bool d_c, uint8_t data) {
                                                // Sends a data/command byte to the display
    spi_context->frequency(200000);                     // frecuenciy in Hz
    spi_context->mode(mraa::SPI_MODE3);                 // SPI Mode 3 (0-3)
    spi_context->lsbmode(false);                        // MSBFIRST (false = big endian)
    cs_pin->write(1); // HIGH (Ensure inactive before starting transaction)
    {
	uint8_t tx_data[3];
	tx_data[0] = (d_c ? 0xF8 : 0xFA);
	tx_data[1] = (data & 0xF0);
	tx_data[2] = ((data << 4) & 0xF0);
	size_t length = 3;
        spi_context->write(tx_data, length);
    }
    cs_pin->write(0);
}
