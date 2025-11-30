#include "st7920_txt.h"
#include <iostream>

#define LCD_RESET_PIN 8

int main ()
{
    std::cout << "Starting LCD test application..." << std::endl;

    ST7920_txt lcd(LCD_RESET_PIN);
    lcd.begin();
    lcd.clear();
    lcd.print(0,0, (char*)"Hello Galileo!");

    std::cout << "Test Complete." << std::endl;
    return 0;
}
