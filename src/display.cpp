#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "MGD_Base";
const char* password = "12345678";
const char* serverName = "http://192.168.4.1/12001"; // Zmodyfikuj adres IP serwera
const char* game01Start = "http://192.168.4.1/Game01/start";  // Zmień na adres IP punktu dostępowego ESP32
const int buttonPin = 15; 

// flags
bool flagConnectingDevice = false;

//Game01 - szpital tylko zdrowie ważne
int gameType = 1; // typ gry
// Zmienne zdrowia drużyn
int team1Health = 20;
int team2Health = 20;


// Deklaracji funkcji
void trueConnect(bool* flag1);
void sendToBaseStartGame01(int team1, int team2);


void setup() 
{
  // Inicjalizacja komunikacji szeregowej
  Serial.begin(115200);
  // Podłączenie do serwera
  WiFi.begin(ssid, password);
  // Ustwienie pina przycisku
  pinMode(buttonPin, INPUT_PULLUP);

}

void loop() 
{
  // sprawdzanie połączenia
  if (!&flagConnectingDevice)
  {
  trueConnect(&flagConnectingDevice);  
  }

  if (digitalRead(buttonPin) == LOW && gameType == 1) 
  {
    sendToBaseStartGame01(team1Health, team2Health);
    delay(1000); // Debounce
  }
 
}


//wyświetla w port try connect kiedy probuję podłączyć sie do wifi
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

// Funkcja do wysyłania danych zdrowia i kodu startu gry
void sendToBaseStartGame01(int team1, int team2) 
{
  if (WiFi.status() == WL_CONNECTED) 
    {
    HTTPClient http;
    http.begin(game01Start);
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