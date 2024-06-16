#include <Adafruit_NeoPixel.h>
#include <WiFi.h>
#include <HTTPClient.h>

// Definicja pinów
const int buttonPin = 2; // Pin, do którego podłączony jest przycisk
const int np_pin = 4;    // Pin, do którego podłączony jest NeoPixel

// Zmienne globalne
const char *ssid = "MGD_Base";
const char *password = "12345678";
const char *serverClickBotton1 = "http://192.168.4.1/btn1Clk";
int         httpCodeIn = 0;
const int   count_pixels = 12;

// flags
bool flagConnectingDevice = false;
bool flagStartGame = true;
bool flagGetMessageToServer = false;
bool lastButtonState = HIGH; // Poprzedni stan przycisku


// Tworzenie obiektów klas
Adafruit_NeoPixel pixels(count_pixels, np_pin, NEO_GRB + NEO_KHZ800);

// deklaracja funkcji
void connectedDevice(int user_delay, bool* flag1);
void trueConnect(bool* flag1);
void clickButton(bool* flag1, bool* flag2);
void send_acceptMessageServer(bool* flag1, bool* flag2, bool* flag3);

void setup()
{
    // Ustawienie pinu przycisku jako wejście z wewnętrznym rezystorem pull-up
    pinMode(buttonPin, INPUT_PULLUP);
    // Podłączenie do serwera
    WiFi.begin(ssid, password);
    // Tworzenie instancji NeoPixel
    pixels.begin();
    // Inicjalizacja komunikacji szeregowej i WiFi
    Serial.begin(115200); 
}

void loop()
{
    // sprawdzanie połączenia
    trueConnect(&flagConnectingDevice);  
    // wyświetlane statusu (lub podtwierdzenia) połączenia na neo_pixel
    connectedDevice(100, &flagConnectingDevice);
    //obsługa kliknięcia przycisku
    clickButton(&flagStartGame, &flagGetMessageToServer);
    //osługa wysłania i przyjęcia danych z servera
    send_acceptMessageServer(&flagGetMessageToServer, &flagConnectingDevice, &flagStartGame);
    
}

//init: tmr1, tmr3
void connectedDevice(int user_delay, bool *flag1)
{
    static bool wasConnected = false; // Flaga do śledzenia poprzedniego stanu połączenia

    if ((WiFi.status() != WL_CONNECTED) && (*flag1 == false))
    { 
        static unsigned long tmr1 = 0; // Zmienna do przechowywania poprzedniego czasu
        static int currentPixel = 0;   // Aktualnie obsługiwany indeks diody
        
        // Sprawdzenie, czy upłynął określony czas
        if (millis() - tmr1 >= user_delay)
        {
            // Zaktualizowanie poprzedniego czasu
            tmr1 = millis(); 
            // Wyczyść wszystkie diody
            pixels.clear();
            // Włącz diodę o aktualnym indeksie
            pixels.setPixelColor(currentPixel, pixels.Color(255, 255, 255));
            // Wyświetl ustawione kolory
            pixels.show();
            Serial.println(".");
            // Przełącz do następnej diody
            currentPixel++;
            if (currentPixel >= count_pixels)
            {
                currentPixel = 0; // Wróć do pierwszej diody po osiągnięciu ostatniej
            }
        }
        wasConnected = false; // Ustawienie flagi na false, ponieważ połączenie jest utracone
                              // lub nawiązywane po raz pierwszy(dla funkcji bieżącej).
    } 
    else 
    {
        *flag1 = true; //zmiana flagi(połączenie nawiązane) dla funkcji zewnętrznych         
        if (wasConnected == false)
        {
            wasConnected = true; // Ustawienie flagi, że połączenie zostało nawiązane(dla funkcji bieżącej)

            unsigned long tmr3 = millis(); // Zmienna do przechowywania poprzedniego czasu startowego
            // Ustawienie diod na zielono i wyświetlenie ich
            pixels.fill(pixels.Color(0, 255, 0));
            pixels.show();
            // Sprawdzenie, czy upłynęła 1 sekunda
            while (millis() - tmr3 < 1000)
            {
                // Poczekaj 1 sekundę
            }
            pixels.clear(); // Wyczyść wszystkie diody po 1 sekundzie
            pixels.show();  // Pokaż zmiany
            Serial.println("    Connected"); //Wyprowadzenie statusu na serial_port
        }
    }
}

//init: tmr1,tmr3,(new)tmr2
void trueConnect(bool* flag1)
{
    static unsigned long tmr2 = 0;
    if(millis() - tmr2 >= 1000)
    {
        tmr2 = millis();
        if(WiFi.status() != WL_CONNECTED)
        {
            Serial.println("try connect");
            *flag1 = false;
        }
    }
}

//init: tmr1, tmr2,tmr3, (new)tmr4
void clickButton(bool* flag1, bool* flag2)
{
    if(*flag1)
    {
        static unsigned long tmr4 = 0;
        bool buttonState = digitalRead(buttonPin);

        if (millis() - tmr4 > 50)
        {
            tmr4 = millis();
            if (!buttonState && lastButtonState)
            {
                Serial.println("Pressed button");
                *flag2 = true;
                // Zmienna do przechowywania poprzedniego czasu startowego
                unsigned long tmr5 = millis();
                // Ustawienie diod na czrwono i wyświetlenie ich
                if (WiFi.status() == WL_CONNECTED)//
                {
                    pixels.fill(pixels.Color(255, 0, 0));
                    pixels.show();
                    // Sprawdzenie, czy upłynęła 1 sekunda
                    while (millis() - tmr5 < 1000)
                    {
                        // Poczekaj 1 sekundę
                    }
                    pixels.clear();                    // Wyczyść wszystkie diody po 1 sekundzie
                    pixels.show();                     // Pokaż zmiany
                    Serial.println("neo_pixel light"); // Wyprowadzenie podtwierdzenia zaświecenia neo_pixel na serial_port
                }
            }
            lastButtonState = buttonState;
        }
    }
}

void send_acceptMessageServer(bool* flag1, bool* flag2, bool* flag3)
{
    if(*flag1 && *flag2 && flag3 ) //jeżeli przycisk został wciśnięty(*flag1(flagGetMessageToServer))
    {
        Serial.println("Wysłanie wiadomości na serwer");
        Serial.println();
        if (WiFi.status() == WL_CONNECTED)
            {
                HTTPClient http;
                http.begin(serverClickBotton1);
                httpCodeIn = http.GET();
                if (httpCodeIn == 1501)
                {
                    Serial.println("Button 1 pressed, sent to server");
                }
                http.end();
            }
            *flag1 = false;
    }
    /*
    *
    *
    *Kod do interakcji z serverem
    *
    * 
    */
}