#include "display_class.h"

// Konstruktor klasy
Display::Display() : tft(TFT_CS, TFT_DC, TFT_RST)
{

    // inicjalizacja zmiennych pinów przycisków
    btPin1 = 22;
    btPin2 = 17;
    btPin3 = 16;
}

// Inicjalizacja wyswietlacza (patcz ILI9488::begin)
void Display::begin()
{
    tft.begin();
}

// Inicjalizacja przycisków
void Display::buttonsInit()
{
    pinMode(btPin1, INPUT_PULLUP);
    pinMode(btPin2, INPUT_PULLUP);
    pinMode(btPin3, INPUT_PULLUP);
}

// Odczytanie kliknięć z debouncingiem
int Display::clickButton()
{
    static bool lastButtonState1 = HIGH;
    static bool lastButtonState2 = HIGH;
    static bool lastButtonState3 = HIGH;
    static unsigned long lastDebounceTime = 0;
    const unsigned long debounceDelay = 200;

    bool buttonUP = digitalRead(btPin1);
    bool buttonDown = digitalRead(btPin2);
    bool buttonSelect = digitalRead(btPin3);

    if ((millis() - lastDebounceTime) > debounceDelay)
    {
        if (buttonUP == LOW && lastButtonState1 == HIGH)
        {
            Serial.println("Button - 1#");
            lastButtonState1 = LOW;
            lastDebounceTime = millis();
            return 1; // Zwraca numer przycisku 1
        }
        if (buttonDown == LOW && lastButtonState2 == HIGH)
        {
            Serial.println("Button - 2#");
            lastButtonState2 = LOW;
            lastDebounceTime = millis();
            return 2; // Zwraca numer przycisku 2
        }
        if (buttonSelect == LOW && lastButtonState3 == HIGH)
        {
            Serial.println("Button - 3#");
            lastButtonState3 = LOW;
            lastDebounceTime = millis();
            return 3; // Zwraca numer przycisku 3
        }
    }

    lastButtonState1 = buttonUP;
    lastButtonState2 = buttonDown;
    lastButtonState3 = buttonSelect;

    return 0; // Zwraca 0 gdy żaden przycisk nie został kliknięty
}

// Ekran początkowy
void Display::onDisplay(bool &flag_onDisplay, bool &flag_mainMenu)
{
    if (flag_onDisplay)
    {
        tft.fillScreen(ILI9488_BLACK);
        tft.setCursor(190, 110);
        tft.setTextColor(ILI9488_WHITE);
        tft.setTextSize(6);
        tft.setRotation(1);
        delay(500);
        tft.println("ASG");
        tft.setCursor(40, 190);
        tft.setTextColor(ILI9488_RED);
        tft.setTextSize(4);
        tft.println("Multi Game Device");
        delay(2000);
        tft.fillScreen(ILI9488_BLACK);
        flag_onDisplay = false;
        flag_mainMenu = true;
    }
}

// menu główne
void Display::mainMenu(bool &flag_mainMenu, bool &flag_spitalMenu, bool &flag_dominacjaMenu)
{

    if (flag_mainMenu)
    {
        bool start_stop = true;
        int stateMenu = 1;

        if (start_stop)
        {
            tft.fillRoundRect(150, 0, 153, 30, 5, ILI9488_DARKGREEN);
            tft.setFont(&FreeMono12pt7b);
            tft.setCursor(155, 20);
            tft.setTextColor(ILI9488_LIGHTGREY);
            tft.setTextSize(1);
            tft.println("Game Setup");
            tft.setFont(&FreeSans12pt7b);
            tft.fillRect(1, 75, 10, 35, ILI9488_GREEN);
            tft.setCursor(20, 100);
            tft.setTextColor(ILI9488_GREEN);
            tft.print(" Szpital");
            tft.setCursor(20, 140);
            tft.setTextColor(ILI9488_WHITE);
            tft.print(" Domination");
            tft.drawRect(0, 0, 480, 320, ILI9488_GREEN);
            // tft.drawRect(225, 290, 280, 30, ILI9488_GREEN);
            start_stop = false;
        }

        while (flag_mainMenu)
        {
            int click = clickButton();
            switch (click)
            {
            case 1:
            case 2:
            {
                stateMenu = (stateMenu == 1) ? 2 : 1;
                select_mainMenu(stateMenu);
                break;
            }
            case 3:
            {
                if (stateMenu == 1)
                {
                    flag_spitalMenu = true;
                    flag_mainMenu = false;
                }
                else if (stateMenu == 2)
                {
                    flag_dominacjaMenu = true;
                    flag_mainMenu = false;
                }
                tft.fillScreen(ILI9488_BLACK);
                break;
            }
            }
        }
    }
}

void Display::select_mainMenu(int select_OPtion)
{
    if (select_OPtion == 1)
    {
        // tft.drawRoundRect(2, 75, 476, 35, 5, ILI9488_DARKCYAN);
        // tft.drawRoundRect(2, 115, 476, 35, 5, ILI9488_BLACK);
        tft.setFont(&FreeSans12pt7b);
        tft.fillRect(1, 75, 10, 35, ILI9488_GREEN);
        tft.fillRect(1, 115, 10, 35, ILI9488_BLACK);
        tft.setCursor(20, 100);
        tft.setTextColor(ILI9488_GREEN);
        tft.print(" Szpital");
        tft.setCursor(20, 140);
        tft.setTextColor(ILI9488_WHITE);
        tft.print(" Domination");
    }
    else if (select_OPtion == 2)
    {
        // tft.drawRoundRect(2, 75, 476, 35, 5, ILI9488_BLACK);
        // tft.drawRoundRect(2, 115, 476, 35, 5, ILI9488_DARKCYAN);
        tft.setFont(&FreeSans12pt7b);
        tft.fillRect(1, 75, 10, 35, ILI9488_BLACK);
        tft.fillRect(1, 115, 10, 35, ILI9488_GREEN);
        tft.setCursor(20, 100);
        tft.setTextColor(ILI9488_WHITE);
        tft.print(" Szpital");
        tft.setCursor(20, 140);
        tft.setTextColor(ILI9488_GREEN);
        tft.print(" Domination");
    }
}

// menu spital
void Display::spitalMenu(bool &flag_mainMenu, bool &flag_spitalMenu, int &countOfLives, int &timeOfGame, bool &groupOuting, bool& flag_startGame, int& gameMode)
{
    if (flag_spitalMenu)
    {
        bool start_stop = true;
        int stateMenu = 1;

        if (start_stop)
        {
            tft.drawRect(0, 0, 480, 320, ILI9488_GREEN);
            tft.fillRoundRect(130, 0, 193, 25, 5, ILI9488_DARKGREEN);
            tft.setFont(&FreeMono12pt7b);
            tft.setCursor(135, 17);
            tft.setTextColor(ILI9488_LIGHTGREY);
            tft.setTextSize(1);
            tft.println("Szpital Setup");
            tft.setFont(&FreeSans12pt7b);
            tft.fillRect(1, 55, 10, 35, ILI9488_GREEN);
            tft.setCursor(15, 80);
            tft.setTextColor(ILI9488_GREEN);
            tft.print(" Count of Lives:");
            tft.drawRoundRect(240, 53, 85, 40, 4, ILI9488_PINK);
            tft.setCursor(328, 88);
            tft.setTextColor(ILI9488_DARKGREY);
            tft.print("amt.");
            tft.setCursor(274, 80);
            tft.setTextColor(ILI9488_WHITE);
            if (countOfLives < 10)
            {
                tft.setCursor(274, 80);
            }
            else
            {
                tft.setCursor(267, 80);
            }
            tft.print(countOfLives);
            // tft.drawLine(20, 130, 450, 130, ILI9488_DARKGREY);
            tft.setCursor(15, 160);
            tft.setTextColor(ILI9488_WHITE);
            tft.print(" Time:");
            tft.drawRoundRect(240, 133, 85, 40, 4, ILI9488_PINK);
            tft.setCursor(328, 168);
            tft.setTextColor(ILI9488_DARKGREY);
            tft.print("min.");
            tft.setTextColor(ILI9488_WHITE);
            if (timeOfGame > 0 && timeOfGame < 9)
            {
                tft.setCursor(274, 160);
                tft.print(timeOfGame);
            }
            else if (timeOfGame > 9)
            {
                tft.setCursor(267, 160);
                tft.print(timeOfGame);
            }
            else
            {
                tft.setCursor(245, 160);
                tft.setFont(&FreeSans9pt7b);
                tft.setTextColor(ILI9488_DARKGREEN);
                tft.print("Unlimited");
                tft.setFont(&FreeSans12pt7b);
            }
            // tft.drawLine(20, 210, 450, 210, ILI9488_DARKGREY);
            tft.setCursor(15, 240);
            tft.setTextColor(ILI9488_WHITE);
            tft.print(" Group outing: ");
            tft.drawRoundRect(240, 213, 85, 40, 4, ILI9488_PINK);
            tft.setCursor(328, 248);
            tft.setTextColor(ILI9488_DARKGREY);
            tft.print("o/o.");
            tft.setCursor(258, 240);
            if (groupOuting == false)
            {
                tft.setTextColor(ILI9488_RED);
                tft.print("OFF");
            }
            else if (groupOuting == true)
            {
                tft.setTextColor(ILI9488_GREEN);
                tft.print(" ON");
            }
            // tft.drawLine(200, 60, 200, 290, ILI9488_DARKGREY);
            tft.drawRoundRect(0, 290, 150, 200, 5, ILI9488_GREEN);
            tft.setCursor(10, 310);
            tft.setTextColor(ILI9488_DARKGREEN);
            tft.setFont(&FreeMono12pt7b);
            tft.print("<< return");
            tft.drawRoundRect(330, 290, 150, 200, 5, ILI9488_GREEN);
            tft.setCursor(350, 310);
            tft.print("start >>");
            tft.setFont(&FreeSans12pt7b);

            start_stop = false;
        }

        while (flag_spitalMenu)
        {
            int click = clickButton();
            switch (click)
            {
            case 1:
            {
                if (stateMenu > 1)
                {
                    stateMenu--;
                    Serial.print("stateMenu - ");
                    Serial.println(stateMenu);
                }
                else
                {
                    stateMenu = 5;
                    Serial.print("stateMenu - ");
                    Serial.println(stateMenu);
                }
                select_spitalMenu(stateMenu, groupOuting);
                break;
            }
            case 2:
            {
                if (stateMenu < 5)
                {
                    stateMenu++;
                    Serial.print("stateMenu - ");
                    Serial.println(stateMenu);
                }
                else
                {
                    stateMenu = 1;
                    Serial.print("stateMenu - ");
                    Serial.println(stateMenu);
                }
                select_spitalMenu(stateMenu, groupOuting);
                break;
            }
            case 3:
            {
                static unsigned long tmr1 = 0;
                bool state = false;
                if (stateMenu == 1)
                {
                    bool flagLoop = true;
                    while (flagLoop)
                    {
                        int clickLoop = clickButton();

                        if (millis() - tmr1 >= 250)
                        {
                            tmr1 = millis();
                            if (countOfLives < 10)
                            {
                                tft.setCursor(274, 80);
                            }
                            else
                            {
                                tft.setCursor(267, 80);
                            }

                            tft.setTextColor(state ? ILI9488_GREEN : ILI9488_BLACK);
                            tft.print(countOfLives);
                            state = !state;
                        }

                        if (clickLoop)
                        {
                            switch (clickLoop)
                            {
                            case 1:
                            {
                                tft.fillRect(250, 60, 60, 28, ILI9488_BLACK);
                                if (countOfLives < 50)
                                    countOfLives += 5;
                                else
                                    countOfLives = 5;

                                break;
                            }
                            case 2:
                            {
                                tft.fillRect(250, 60, 60, 28, ILI9488_BLACK);
                                if (countOfLives > 5)
                                    countOfLives -= 5;
                                else
                                    countOfLives = 50;
                                break;
                            }
                            case 3:
                            {
                                tft.fillRect(250, 60, 60, 28, ILI9488_BLACK);
                                if (countOfLives < 10)
                                {
                                    tft.setCursor(274, 80);
                                }
                                else
                                {
                                    tft.setCursor(267, 80);
                                }
                                tft.setTextColor(ILI9488_WHITE);
                                tft.print(countOfLives);
                                flagLoop = false;
                            }
                            }
                        }
                    }
                }
                else if (stateMenu == 2)
                {
                    bool flagLoop = true;
                    while (flagLoop)
                    {
                        int clickLoop = clickButton();

                        if (millis() - tmr1 >= 250)
                        {
                            tmr1 = millis();
                            if (timeOfGame > 0 && timeOfGame < 9)
                            {
                                tft.setCursor(274, 160);
                                tft.print(timeOfGame);
                            }
                            else if (timeOfGame > 9 && timeOfGame <= 60)
                            {
                                tft.setCursor(267, 160);
                                tft.print(timeOfGame);
                            }
                            else if (timeOfGame == 0)
                            {
                                tft.setCursor(245, 160);
                                tft.setFont(&FreeSans9pt7b);
                                tft.print("Unlimited");
                                tft.setFont(&FreeSans12pt7b);
                            }

                            tft.setTextColor(state ? ILI9488_GREEN : ILI9488_BLACK);
                            state = !state;
                        }

                        if (clickLoop)
                        {
                            switch (clickLoop)
                            {
                            case 1:
                            {
                                tft.fillRect(245, 140, 75, 28, ILI9488_BLACK);
                                tft.setTextColor(ILI9488_GREEN);
                                if (timeOfGame < 60)
                                    timeOfGame += 5;
                                else
                                    timeOfGame = 0;
                                break;
                            }
                            case 2:
                            {
                                tft.fillRect(245, 140, 75, 28, ILI9488_BLACK);
                                tft.setTextColor(ILI9488_GREEN);
                                if (timeOfGame > 0)
                                    timeOfGame -= 5;
                                else
                                    timeOfGame = 60;
                                break;
                            }
                            case 3:
                            {
                                tft.setTextColor(ILI9488_WHITE);
                                tft.fillRect(245, 140, 75, 28, ILI9488_BLACK);
                                if (timeOfGame > 0 && timeOfGame < 9)
                                {
                                    tft.setCursor(274, 160);
                                    tft.setTextColor(ILI9488_WHITE);
                                    tft.print(timeOfGame);
                                }
                                else if (timeOfGame > 9 && timeOfGame < 61)
                                {
                                    tft.setCursor(267, 160);
                                    tft.setTextColor(ILI9488_WHITE);
                                    tft.print(timeOfGame);
                                }
                                else if (timeOfGame == 0)
                                {
                                    tft.setCursor(245, 160);
                                    tft.setFont(&FreeSans9pt7b);
                                    tft.setTextColor(ILI9488_DARKGREEN);
                                    tft.print("Unlimited");
                                    tft.setFont(&FreeSans12pt7b);
                                }
                                flagLoop = false;
                            }
                            }
                        }
                    }
                }
                else if (stateMenu == 3)
                {
                    bool flagLoop = true;
                    while (flagLoop)
                    {
                        int clickLoop = clickButton();

                        if (millis() - tmr1 >= 250)
                        {
                            tmr1 = millis();
                            state = !state;
                            if (groupOuting == true)
                            {
                                tft.setCursor(258, 241);
                                tft.setTextColor(state ? ILI9488_DARKGREEN : ILI9488_BLACK);
                                tft.print(" ON");
                            }
                            else if (groupOuting == false)
                            {
                                tft.setCursor(258, 241);
                                tft.setTextColor(state ? ILI9488_RED : ILI9488_BLACK);
                                tft.print("OFF");
                            }

                            if (groupOuting == true)
                            {
                                tft.setTextColor(ILI9488_BLACK);
                            }
                        }

                        if (clickLoop)
                        {
                            switch (clickLoop)
                            {
                            case 1:
                            case 2:
                            {
                                if (groupOuting == true)
                                {
                                    tft.fillRect(245, 218, 72, 28, ILI9488_BLACK);
                                    groupOuting = false;
                                    tft.setCursor(258, 241);
                                    tft.print("OFF");
                                }
                                else if (groupOuting == false)
                                {
                                    tft.fillRect(245, 218, 72, 28, ILI9488_BLACK);
                                    groupOuting = true;
                                    tft.setCursor(258, 241);
                                    tft.print(" ON");
                                }
                                break;
                            }
                            case 3:
                            {
                                if (groupOuting == true)
                                {
                                    tft.setTextColor(ILI9488_DARKGREEN);
                                }
                                else if (groupOuting == false)
                                {
                                    tft.setTextColor(ILI9488_RED);
                                }

                                tft.fillRect(245, 218, 72, 28, ILI9488_BLACK);
                                if (groupOuting == true)
                                {
                                    tft.setCursor(258, 241);
                                    tft.print(" ON");
                                }
                                else if (groupOuting == false)
                                {
                                    tft.setCursor(258, 241);
                                    tft.print("OFF");
                                }
                                flagLoop = false;
                            }
                            }
                        }
                    }
                }
                else if(stateMenu == 4)
                {
                    tft.fillScreen(ILI9488_BLACK);
                    flag_spitalMenu = false;
                    flag_startGame = true;
                    gameMode = 1;
                }
                else if (stateMenu == 5)
                {
                    tft.fillScreen(ILI9488_BLACK);
                    flag_spitalMenu = false;
                    flag_mainMenu = true;
                }

                break;
            }
            }
        }
    }
}

void Display::select_spitalMenu(int selectOption, bool &groupOuting)
{
    if (selectOption == 1)
    {
        tft.setFont(&FreeSans12pt7b);
        tft.fillRect(1, 55, 10, 35, ILI9488_GREEN);
        tft.setCursor(15, 80);
        tft.setTextColor(ILI9488_GREEN);
        tft.print(" Count of Lives:");
        tft.fillRect(1, 135, 10, 35, ILI9488_BLACK);
        tft.setCursor(15, 160);
        tft.setTextColor(ILI9488_WHITE);
        tft.print(" Time:");
        tft.fillRect(1, 215, 10, 35, ILI9488_BLACK);
        tft.setCursor(15, 240);
        tft.setTextColor(ILI9488_WHITE);
        tft.print(" Group outing:");
        tft.setCursor(258, 241);
        if (groupOuting == true)
        {
            tft.setTextColor(ILI9488_DARKGREEN);
            tft.print(" ON");
        }
        else if (groupOuting == false)
        {
            tft.setTextColor(ILI9488_RED);
            tft.print("OFF");
        }
        tft.fillRoundRect(0, 290, 150, 30, 5, ILI9488_BLACK);
        tft.drawRoundRect(0, 290, 150, 30, 5, ILI9488_GREEN);
        tft.setFont(&FreeMono12pt7b);
        tft.setCursor(10, 310);
        tft.setTextColor(ILI9488_DARKGREEN);
        tft.print("<< return");
        tft.fillRoundRect(330, 290, 150, 30, 5, ILI9488_BLACK);
        tft.drawRoundRect(330, 290, 150, 30, 5, ILI9488_GREEN);
        tft.setCursor(350, 310);
        tft.setTextColor(ILI9488_DARKGREEN);
        tft.print("start >>");
        tft.setFont(&FreeSans12pt7b);
    }
    else if (selectOption == 2)
    {
        tft.setFont(&FreeSans12pt7b);
        tft.fillRect(1, 55, 10, 35, ILI9488_BLACK);
        tft.setCursor(15, 80);
        tft.setTextColor(ILI9488_WHITE);
        tft.print(" Count of Lives:");
        tft.fillRect(1, 135, 10, 35, ILI9488_GREEN);
        tft.setCursor(15, 160);
        tft.setTextColor(ILI9488_GREEN);
        tft.print(" Time:");
        tft.fillRect(1, 215, 10, 35, ILI9488_BLACK);
        tft.setCursor(15, 240);
        tft.fillRect(243, 217, 80, 30, ILI9488_BLACK);
        tft.setTextColor(ILI9488_WHITE);
        tft.print(" Group outing:");
        tft.setCursor(258, 241);
        if (groupOuting == true)
        {
            tft.setTextColor(ILI9488_DARKGREEN);
            tft.print(" ON");
        }
        else if (groupOuting == false)
        {
            tft.setTextColor(ILI9488_RED);
            tft.print("OFF");
        }
    }
    else if (selectOption == 3)
    {
        tft.setFont(&FreeSans12pt7b);
        tft.fillRect(1, 55, 10, 35, ILI9488_BLACK);
        tft.setCursor(15, 80);
        tft.setTextColor(ILI9488_WHITE);
        tft.print(" Count of Lives:");
        tft.fillRect(1, 135, 10, 35, ILI9488_BLACK);
        tft.setCursor(15, 160);
        tft.setTextColor(ILI9488_WHITE);
        tft.print(" Time:");
        tft.fillRect(1, 215, 10, 35, ILI9488_GREEN);
        tft.setCursor(15, 240);
        tft.setTextColor(ILI9488_GREEN);
        tft.print(" Group outing:");
        tft.setCursor(258, 241);
        if (groupOuting == true)
        {
            tft.setTextColor(ILI9488_DARKGREEN);
            tft.print(" ON");
        }
        else if (groupOuting == false)
        {
            tft.setTextColor(ILI9488_RED);
            tft.print("OFF");
        }
        tft.fillRoundRect(0, 290, 150, 30, 5, ILI9488_BLACK);
        tft.drawRoundRect(0, 290, 150, 30, 5, ILI9488_GREEN);
        tft.setFont(&FreeMono12pt7b);
        tft.setCursor(10, 310);
        tft.setTextColor(ILI9488_DARKGREEN);
        tft.print("<< return");
        tft.fillRoundRect(330, 290, 150, 30, 5, ILI9488_BLACK);
        tft.drawRoundRect(330, 290, 150, 30, 5, ILI9488_GREEN);
        tft.setCursor(350, 310);
        tft.setTextColor(ILI9488_DARKGREEN);
        tft.print("start >>");
        tft.setFont(&FreeSans12pt7b);
    }
    else if (selectOption == 4)
    {
        tft.setFont(&FreeSans12pt7b);
        tft.fillRect(1, 55, 10, 35, ILI9488_BLACK);
        tft.setCursor(15, 80);
        tft.setTextColor(ILI9488_WHITE);
        tft.print(" Count of Lives:");
        tft.fillRect(1, 135, 10, 35, ILI9488_BLACK);
        tft.setCursor(15, 160);
        tft.setTextColor(ILI9488_WHITE);
        tft.print(" Time:");
        tft.fillRect(1, 215, 10, 35, ILI9488_BLACK);
        tft.setCursor(15, 240);
        tft.setTextColor(ILI9488_WHITE);
        tft.print(" Group outing:");
        tft.setCursor(258, 241);
        if (groupOuting == true)
        {
            tft.setTextColor(ILI9488_DARKGREEN);
            tft.print(" ON");
        }
        else if (groupOuting == false)
        {
            tft.setTextColor(ILI9488_RED);
            tft.print("OFF");
        }
        tft.fillRoundRect(0, 290, 150, 30, 5, ILI9488_BLACK);
        tft.drawRoundRect(0, 290, 150, 30, 5, ILI9488_GREEN);
        tft.setFont(&FreeMono12pt7b);
        tft.setCursor(10, 310);
        tft.setTextColor(ILI9488_DARKGREEN);
        tft.print("<< return");
        tft.fillRoundRect(330, 290, 150, 30, 5, ILI9488_GREEN);
        tft.setCursor(350, 310);
        tft.setTextColor(ILI9488_BLACK);
        tft.print("start >>");
        tft.setFont(&FreeSans12pt7b);
    }
    else if (selectOption == 5)
    {
        tft.setFont(&FreeSans12pt7b);
        tft.fillRect(1, 55, 10, 35, ILI9488_BLACK);
        tft.setCursor(15, 80);
        tft.setTextColor(ILI9488_WHITE);
        tft.print(" Count of Lives:");
        tft.fillRect(1, 135, 10, 35, ILI9488_BLACK);
        tft.setCursor(15, 160);
        tft.setTextColor(ILI9488_WHITE);
        tft.print(" Time:");
        tft.fillRect(1, 215, 10, 35, ILI9488_BLACK);
        tft.setCursor(15, 240);
        tft.setTextColor(ILI9488_WHITE);
        tft.print(" Group outing:");
        tft.setCursor(258, 241);
        if (groupOuting == true)
        {
            tft.setTextColor(ILI9488_DARKGREEN);
            tft.print(" ON");
        }
        else if (groupOuting == false)
        {
            tft.setTextColor(ILI9488_RED);
            tft.print("OFF");
        }
        tft.fillRoundRect(0, 290, 150, 30, 5, ILI9488_GREEN);
        tft.setFont(&FreeMono12pt7b);
        tft.setCursor(10, 310);
        tft.setTextColor(ILI9488_BLACK);
        tft.print("<< return");
        tft.fillRoundRect(330, 290, 150, 30, 5, ILI9488_BLACK);
        tft.drawRoundRect(330, 290, 150, 30, 5, ILI9488_GREEN);
        tft.setCursor(350, 310);
        tft.setTextColor(ILI9488_DARKGREEN);
        tft.print("start >>");
        tft.setFont(&FreeSans12pt7b);
    }
}

// dominacja menu
void Display::dominationMenu(bool &flag_dominacjaMenu, bool &flag_mainMenu, int &timeOfGame, bool& flag_startGame, int& gameMode)
{
    bool start_stop = true;
    int stateMenu = 1;

    if (flag_dominacjaMenu)
    {
        if (timeOfGame == 0)
        {
            timeOfGame = 5;
        }

        if (start_stop)
        {
            tft.drawRect(0, 0, 480, 320, ILI9488_GREEN);
            tft.fillRoundRect(110, 0, 233, 25, 5, ILI9488_DARKGREEN);
            tft.setFont(&FreeMono12pt7b);
            tft.setCursor(115, 17);
            tft.setTextColor(ILI9488_LIGHTGREY);
            tft.setTextSize(1);
            tft.println("Domination Setup");
            tft.setFont(&FreeSans12pt7b);
            tft.setCursor(15, 160);
            tft.fillRect(1, 135, 10, 35, ILI9488_GREEN);
            tft.setCursor(15, 160);
            tft.setTextColor(ILI9488_GREEN);
            tft.print(" Time:");
            tft.drawRoundRect(240, 133, 85, 40, 4, ILI9488_PINK);
            tft.setCursor(328, 168);
            tft.setTextColor(ILI9488_DARKGREY);
            tft.print("min.");
            tft.setTextColor(ILI9488_WHITE);
            if (timeOfGame > 0 && timeOfGame < 9)
            {
                tft.setCursor(274, 160);
                tft.print(timeOfGame);
            }
            else if(timeOfGame > 9)
            {
                tft.setCursor(267, 160);
                tft.print(timeOfGame);
            }
            tft.drawRoundRect(0, 290, 150, 200, 5, ILI9488_GREEN);
            tft.setCursor(10, 310);
            tft.setTextColor(ILI9488_DARKGREEN);
            tft.setFont(&FreeMono12pt7b);
            tft.print("<< return");
            tft.drawRoundRect(330, 290, 150, 200, 5, ILI9488_GREEN);
            tft.setCursor(350, 310);
            tft.print("start >>");
            tft.setFont(&FreeSans12pt7b);

            start_stop = false;
        }

        while (flag_dominacjaMenu)
        {
            int click = clickButton();

            switch (click)
            {
            case 1:
            {
                if (stateMenu > 1)
                {
                    stateMenu--;
                    Serial.print("stateMenuDomination - ");
                    Serial.println(stateMenu);
                }
                else
                {
                    stateMenu = 3;
                    Serial.print("stateMenuDomination - ");
                    Serial.println(stateMenu);
                }
                select_dominationMenu(stateMenu);
                break;
            }
            case 2:
            {
                if (stateMenu < 3)
                {
                    stateMenu++;
                    Serial.print("stateMenuDomination - ");
                    Serial.println(stateMenu);
                }
                else
                {
                    stateMenu = 1;
                    Serial.print("stateMenuDomination - ");
                    Serial.println(stateMenu);
                }
                select_dominationMenu(stateMenu);
                break;
            }
            case 3:
            {
                static unsigned long tmr1 = 0;
                bool state = false;

                if (stateMenu == 1)
                {
                    bool flagLoop = true;
                    
                    while (flagLoop)
                    {
                        int clickLoop = clickButton();

                        if (millis() - tmr1 >= 250)
                        {
                            tmr1 = millis();
                            if (timeOfGame > 0 && timeOfGame < 9)
                            {
                                tft.setCursor(274, 160);
                                tft.print(timeOfGame);
                            }
                            else if (timeOfGame > 9)
                            {
                                tft.setCursor(267, 160);
                                tft.print(timeOfGame);
                            }
                            

                            tft.setTextColor(state ? ILI9488_GREEN : ILI9488_BLACK);
                            state = !state;
                        }

                        if (clickLoop)
                        {
                            switch (clickLoop)
                            {
                            case 1:
                            {
                                tft.fillRect(245, 140, 75, 28, ILI9488_BLACK);
                                tft.setTextColor(ILI9488_GREEN);
                                if (timeOfGame < 60)
                                    timeOfGame += 5;
                                else
                                    timeOfGame = 5;
                                break;
                            }
                            case 2:
                            {
                                tft.fillRect(245, 140, 75, 28, ILI9488_BLACK);
                                tft.setTextColor(ILI9488_GREEN);
                                if (timeOfGame > 5)
                                    timeOfGame -= 5;
                                else
                                    timeOfGame = 60;
                                break;
                            }
                            case 3:
                            {
                                tft.setTextColor(ILI9488_WHITE);
                                tft.fillRect(245, 140, 75, 28, ILI9488_BLACK);
                                if (timeOfGame >0 && timeOfGame < 9)
                                {
                                    tft.setCursor(274, 160);
                                    tft.setTextColor(ILI9488_WHITE);
                                    tft.print(timeOfGame);
                                }
                                else if (timeOfGame > 9)
                                {
                                    tft.setCursor(267, 160);
                                    tft.setTextColor(ILI9488_WHITE);
                                    tft.print(timeOfGame);
                                }
                               
                                flagLoop = false;
                            }
                            }
                        }
                    }
                }
                else if(stateMenu == 2)
                {
                    tft.fillScreen(ILI9488_BLACK);
                    flag_dominacjaMenu = false;
                    flag_startGame = true;
                    gameMode = 2;
                }
                else if(stateMenu == 3)
                {
                    tft.fillScreen(ILI9488_BLACK);
                    flag_dominacjaMenu = false;
                    flag_mainMenu = true;
                }
            }
            }
        }
    }
}

void Display::select_dominationMenu(int selectOption)
{
    if(selectOption == 1)
    {
        tft.fillRect(1, 135, 10, 35, ILI9488_GREEN);
        tft.setCursor(15, 160);
        tft.setTextColor(ILI9488_GREEN);
        tft.print(" Time:");
        tft.fillRoundRect(0, 290, 150, 30, 5, ILI9488_BLACK);
        tft.drawRoundRect(0, 290, 150, 30, 5, ILI9488_GREEN);
        tft.setFont(&FreeMono12pt7b);
        tft.setCursor(10, 310);
        tft.setTextColor(ILI9488_DARKGREEN);
        tft.print("<< return");
        tft.fillRoundRect(330, 290, 150, 30, 5, ILI9488_BLACK);
        tft.drawRoundRect(330, 290, 150, 30, 5, ILI9488_GREEN);
        tft.setCursor(350, 310);
        tft.setTextColor(ILI9488_DARKGREEN);
        tft.print("start >>");
        tft.setFont(&FreeSans12pt7b);
    }else if(selectOption == 2)
    {
        tft.fillRect(1, 135, 10, 35, ILI9488_BLACK);
        tft.setCursor(15, 160);
        tft.setTextColor(ILI9488_WHITE);
        tft.print(" Time:");
        tft.fillRoundRect(0, 290, 150, 30, 5, ILI9488_BLACK);
        tft.drawRoundRect(0, 290, 150, 30, 5, ILI9488_GREEN);
        tft.setFont(&FreeMono12pt7b);
        tft.setCursor(10, 310);
        tft.setTextColor(ILI9488_DARKGREEN);
        tft.print("<< return");
        tft.fillRoundRect(330, 290, 150, 30, 5, ILI9488_GREEN);
        tft.setCursor(350, 310);
        tft.setTextColor(ILI9488_BLACK);
        tft.print("start >>");
        tft.setFont(&FreeSans12pt7b);
    }else if(selectOption == 3)
    {
        tft.fillRect(1, 135, 10, 35, ILI9488_BLACK);
        tft.setCursor(15, 160);
        tft.setTextColor(ILI9488_WHITE);
        tft.print(" Time:");
        tft.fillRoundRect(0, 290, 150, 30, 5, ILI9488_GREEN);
        tft.setFont(&FreeMono12pt7b);
        tft.setCursor(10, 310);
        tft.setTextColor(ILI9488_BLACK);
        tft.print("<< return");
        tft.fillRoundRect(330, 290, 150, 30, 5, ILI9488_BLACK);
        tft.drawRoundRect(330, 290, 150, 30, 5, ILI9488_GREEN);
        tft.setCursor(350, 310);
        tft.setTextColor(ILI9488_DARKGREEN);
        tft.print("start >>");
        tft.setFont(&FreeSans12pt7b);
    }
}

void Display::startGame(int& gameMode, bool& flag_mainMenu, bool& flag_spitalMenu, bool& flag_dominacjaMenu, bool& flag_startGame, int& countOfLives, int& timeOfGame, bool& groupOuting)
{
    if(flag_startGame)
    {
        if(gameMode = 1)
        {
            tft.setCursor(100, 160);
            tft.setTextColor(ILI9488_BLUE);
            tft.println("Game mode selected: ");
            tft.setTextColor(ILI9488_RED);
            tft.setCursor(100, 190);
            tft.println("SPITAL");
        }else
        {
            tft.setCursor(100, 160);
            tft.setTextColor(ILI9488_BLUE);
            tft.println("Game mode selected: ");
            tft.setTextColor(ILI9488_RED);
            tft.setCursor(100, 190);
            tft.println("DOMINATION");
        }
    }
}

/*
#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "MGD_Base";
const char* password = "12345678";
const char* serverName = "http://192.168.4.1/12001"; // Zmodyfikuj adres IP serwera
const char* serverIP = "192.168.4.1";  // Zmień na adres IP punktu dostępowego ESP32
const int buttonPin = 15; 

int gameType = 1; // typ gry
// Zmienne zdrowia drużyn
int team1Health = 20;
int team2Health = 20;

// Funkcja do wysyłania danych zdrowia
void sendToBaseStartGame01(int team1, int team2) 
{
  if (WiFi.status() == WL_CONNECTED) 
    {
    HTTPClient http;
    String url = String("http://") + serverIP + "/Game01/start";
    http.begin(url);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    String payload = "team1=" + String(team1) + "&team2=" + String(team2) + "&startGameFlag=1";
    int httpResponseCode = http.POST(payload);

    if (httpResponseCode > 0) 
      {
        String response = http.getString();
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        Serial.print("Response: ");
        Serial.println(response);
      } else 
      {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
    http.end();
    } else 
    {
      Serial.println("WiFi Disconnected");
    }
}


void setup() 
{
  Serial.begin(115200);
  pinMode(buttonPin, INPUT_PULLUP);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
}

void loop() {
  if (digitalRead(buttonPin) == LOW && gameType == 1) 
  {
    sendToBaseStartGame01(team1Health, team2Health);
    delay(1000); // Debounce
  }
 
}

*/
