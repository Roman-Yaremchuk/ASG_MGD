
#include <Adafruit_NeoPixel.h>
#include <WiFi.h>
#include <HTTPClient.h>

// Definicja pinów
const int buttonPin = 2; // Pin, do którego podłączony jest przycisk
const int np_pin = 4; // Pin, do którego podłączony jest NeoPixel

// Zmienne globalne
const char* ssid = "MGD_Base";
const char* password = "12345678";
const char* serverClickBotton1 = "http://192.168.4.1/1001";
int httpCodeIn = 0;

// Zmienne do funkcji
bool buttonState = 0; // Zmienna do przechowywania stanu przycisku
const int count_pixels = 12; // Zmienna do przechowywania liczby diod LED
unsigned long previousMillis = 0; // Zmienna do przechowywania poprzedniego czasu
bool ledsOn = LOW; // Zmienna do przechowywania stanu diod LED

// Tworzenie instancji NeoPixel
Adafruit_NeoPixel pixels(count_pixels, np_pin, NEO_GRB + NEO_KHZ800);

void connect_wait_led(int user_delay) {
    static unsigned long previousMillis = 0; // Zmienna do przechowywania poprzedniego czasu
    static int currentPixel = 0; // Aktualnie obsługiwany indeks diody
    unsigned long currentMillis = millis(); // Pobranie bieżącego czasu

    // Sprawdzenie, czy upłynął określony czas
    if (currentMillis - previousMillis >= user_delay) {
        previousMillis = currentMillis; // Zaktualizowanie poprzedniego czasu

        pixels.clear(); // Wyczyść wszystkie diody
        pixels.setPixelColor(currentPixel, pixels.Color(255, 255, 255)); // Włącz diodę o aktualnym indeksie
        pixels.show(); // Wyświetl ustawione kolory
        Serial.println(".");

        // Przełącz do następnej diody
        currentPixel++;
        if (currentPixel >= count_pixels) {
            currentPixel = 0; // Wróć do pierwszej diody po osiągnięciu ostatniej
        }
    }
}

void light_led(int user_delay) {
    static unsigned long previousMillis = 0; // Zmienna do przechowywania czasu startu
    unsigned long currentMillis = millis(); // Pobranie bieżącego czasu
if (currentMillis - previousMillis >= user_delay) {
    previousMillis = currentMillis; // Zapisanie bieżącego czasu
    if (ledsOn == LOW) {
    for (int i = 0; i < count_pixels; i++) {
            pixels.setPixelColor(i, pixels.Color(0, 255, 0)); // Zapal wszystkie diody na zielono
        }
    ledsOn = HIGH;
    }
 else {
        for (int i = 0; i < count_pixels; i++) {
            pixels.setPixelColor(i, pixels.Color(0, 0, 0)); // Zapal wszystkie diody na zielono
        }
        }
        pixels.show();
    }
}

void click_button() {
    if (digitalRead(buttonPin) == LOW) {
        if (WiFi.status() == WL_CONNECTED) {
            HTTPClient http;
            http.begin(serverClickBotton1);
            httpCodeIn = http.GET();
            if (httpCodeIn == 1001) {
                Serial.println("Button 1 pressed, sent to server");
            }
            http.end();
        }
    }
}

void setup() {
    // Ustawienie pinu przycisku jako wejście z wewnętrznym rezystorem pull-up
    pinMode(buttonPin, INPUT_PULLUP);

    // Podłączenie do serwera
    WiFi.begin(ssid, password);

    // Tworzenie instancji NeoPixel
    pixels.begin();

// Inicjalizacja komunikacji szeregowej i WiFi
    Serial.begin(115200);
    while (WiFi.status() != WL_CONNECTED) {
        connect_wait_led(100);
    }
    
    //previousMillis = millis(); // Resetowanie poprzedniego czasu po połączeniu z WiFi
    Serial.println("WiFi connected");
}

void loop() {
    // Zapalanie diod na określony czas po połączeniu z Wi-Fi
    light_led(1000); // Zapal wszystkie diody na 1000 ms
    click_button(); // Sprawdź stan przycisku i wyślij żądanie HTTP, jeśli naciśnięty
}