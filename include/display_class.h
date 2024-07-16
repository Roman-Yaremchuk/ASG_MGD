#pragma once

//biblioteki niezbędne do implementacji funkcji
#include "SPI.h"
#include <Adafruit_GFX.h>
#include <ILI9488.h>
#include <Adafruit_I2CDevice.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "FreeMono18pt7b.h"
#include "FreeSans12pt7b.h"
#include "FreeMono12pt7b.h"
#include "FreeSans9pt7b.h"

// definicja pinów wyświetlacza
#define TFT_CS         5
#define TFT_DC         2
#define TFT_RST        4

class Display {
//deklaracja funkcji z dostępem do człąków klasy "priwate"    
public:
    Display();
    void begin();
    void onDisplay(bool& flag_onDisplay, bool& flag_mainMenu);
    void buttonsInit();
    int clickButton();
    void select_mainMenu(int select_OPtion);
    void mainMenu (bool& flag_mainMenu, bool& flag_spitalMenu, bool& flag_dominacjaMenu);
    
    void spitalMenu(bool& flag_mainMenu, bool& flag_spitalMenu, int& countOfLives, int& timeOfGame, bool& groupOuting, bool& flag_startGame, int& gameMode);
    void select_spitalMenu(int selectOption, bool& groupOuting);
    
    void dominationMenu(bool& flag_dominacjaMenu, bool& flag_mainMenu, int& timeOfGame, bool& flag_startGame, int& gameMode);
    void select_dominationMenu(int selectOption);

    void startGame(int& gameMode, bool& flag_mainMenu, bool& flag_spitalMenu, bool& flag_dominacjaMenu, bool& flag_startGame, int& countOfLives, int& timeOfGame, bool& groupOuting);

private:
//zmienne potrzebne w "display_class.cpp"
    ILI9488 tft;
    
//zmienne do przechowywania pinów przycisków
    int btPin1;
    int btPin2;
    int btPin3;
};