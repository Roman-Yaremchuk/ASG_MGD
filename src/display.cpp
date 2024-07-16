#include "display_class.h"

//incjalizacja obiektu klasy "Display"
Display display;

//flagi do zarządzania metodami w "loop"
bool flag_onDisplay = true;
bool flag_mainMenu  = false;

bool flag_spitalMenu = false;
bool flag_dominacjaMenu = false;
bool flag_startGame = false;

//zminne do przechowywania informacji o parametrach gry i póżniejszej wysyłki/odbierania  na/z serwera
int  countOfLives = 5;
int  timeOfGame = 0;
bool groupOuting = false;

int gameMode = 0; 

//Peryferia
bool lastButtonState1 = HIGH;
bool lastButtonState2 = HIGH;
bool lastButtonState3 = HIGH;


void setup() {
    Serial.begin(115200);
    Serial.println("ILI9488 Test!");
    display.begin();
    display.buttonsInit();
}

void loop() {
    
    //ekran powitalny
    display.onDisplay(flag_onDisplay, flag_mainMenu);

    //mainMenu
    display.mainMenu(flag_mainMenu, flag_spitalMenu, flag_dominacjaMenu);

    //spital menu
    display.spitalMenu(flag_mainMenu, flag_spitalMenu, countOfLives, timeOfGame, groupOuting, flag_startGame, gameMode);

    //domination menu
    display.dominationMenu(flag_dominacjaMenu, flag_mainMenu, timeOfGame, flag_startGame, gameMode);

    //start of game
    display.startGame(gameMode,flag_mainMenu, flag_spitalMenu, flag_dominacjaMenu, flag_startGame, countOfLives, timeOfGame, groupOuting);
}



