//  #include <Arduino.h>
//  #include <WiFi.h>
//  #include <WebServer.h>
//  #include <HTTPClient.h>

//  // Konfiguracja punktu dostępowego
//  const char* ap_ssid = "MGD_Base";  // Nazwa SSID punktu dostępowego
//  const char* ap_password = "12345678";  // Hasło do punktu dostępowego
//  WebServer server(80);  // Tworzy obiekt serwera HTTP na porcie 80

//  // Zmienne globalne i flagi
//  String message = "No data received";  // Zmienna globalna przechowująca wiadomość
//  int httpCode = 0;
//  int httpStartCode = 0; //!! powinno być 0
//  int numStations = 0; // ilość podlączonych urządzeń

//  const unsigned long delayConnectTime = 5000; //- dla podłączonych urządzeń
//  int flagDelayConnect  = 0; //- dla podłączonych urządzeń
//  int flagStartGame = 1; //!! powinno być 0  flag ze gra jest rozpoczęta

//  // Zmienne do Szpital type1
//    int numHealthTeam1 = 5; //!! powinno być 0
//    int numHealthTeam2 = 5;  //!! powinno być 0
//    //unsigned long  timeGame = 0;
//    //unsigned long  respDelayGame = 0;


//  void getFinish()
//  {
//    message = "getFinish - jest zapytanie";  // Aktualizuje zmienną message
//    if(numHealthTeam1 <= 0 || numHealthTeam2 <= 0)
//      {
//        server.send(11005);
//        flagStartGame = 0;
//          message = "getFinish - wysłany kod";  // Aktualizuje zmienną message
//      }

//    Serial.println(message);
//  }
//  // Funkcja obsługująca żądania HTTP na głównym URI ("/")
//  void handleRoot() 
//  {
//    server.send(200, "text/plain", message);  // Wysyła odpowiedź HTTP 200 z treścią message
//  }

//  // Funkcja obsługująca żądania HTTP na URI "/button1"
//  void handleButton1() 
//  {
//    message = "Button 1 Pressed!";  // Aktualizuje zmienną message
//    if(numHealthTeam1 > 0)
//    {
//      numHealthTeam1--;
//    server.send(1501, "text/plain", String(numHealthTeam1));  // Wysyła odpowiedź HTTP 200 z treścią "OK"
//    } else
//    {
//      getFinish();
//    }
//  Serial.println("Button 1 Pressed!");
//  Serial.println("Health updated: Team1 = " + String(numHealthTeam1) + ", Team2 = " + String(numHealthTeam2) + "Game = " + String(flagStartGame));

//  }

//  // Funkcja obsługująca żądania HTTP na URI "/button2"
//  void handleButton2() 
//  {
//    message = "Button 2 Pressed!";  // Aktualizuje zmienną message
//    if(numHealthTeam2 > 0)
//    {
//      numHealthTeam1--;
//    server.send(2501, "text/plain", String(numHealthTeam2));  // Wysyła odpowiedź HTTP 200 z treścią "OK"
//    } else
//    {
//      getFinish();
//    }
//    Serial.println("Button 2 Pressed!");
//    Serial.println("Health updated: Team1 = " + String(numHealthTeam1) + ", Team2 = " + String(numHealthTeam2) + "Game = " + String(flagStartGame));
//  }


//  // Pokazuję i wysyła dane o ilości urządzeń na ekran
//  void showConnectedDevices() 
//  {
//  static unsigned long previousMillis = 0; // Zmienna do przechowywania poprzedniego czasu
//  unsigned long currentMillis = millis(); // Pobranie bieżącego czasu
//      if (currentMillis - previousMillis >= delayConnectTime) 
//      {
//      previousMillis = currentMillis; 
//      numStations = WiFi.softAPgetStationNum();
//      Serial.print("Number of connected devices: ");
//      Serial.println(numStations);
//      } 
//  }
//  // odczytuje znaczenie kodu gry i zapisuje go do httpStartCode
//  void httpCodeStartRead( )
//  {
//  if (WiFi.status() == WL_CONNECTED) 
//      {
//        HTTPClient http;
//        httpStartCode = http.GET();
//        if (httpStartCode > 0) 
//        {
//          flagStartGame = 1;
//          Serial.println("Start Game ");
//        }
//        http.end();
//      }
//  }

//  // Funkcja obsługująca żądania HTTP na URI "/setHealth"
//  void startGameOnBase() 
//  {
//    if (server.hasArg("team1") && server.hasArg("team2") && server.hasArg("startGameFlag")) 
//    {
//      numHealthTeam1 = server.arg("team1").toInt();
//      numHealthTeam2 = server.arg("team2").toInt();
//      flagStartGame = server.arg("startGameFlag").toInt();
//      message = "Start updated";
//      server.send(200, "text/plain", "Health updated");
//      Serial.println("Health updated: Team1 = " + String(numHealthTeam1) + ", Team2 = " + String(numHealthTeam2) + "Game = " + String(flagStartGame));
//    } else 
//    {
//      server.send(400, "text/plain", "Bad Request");
//    }
//  }

//  void GameSzpitalType1()
//  {
//  startGameOnBase();
//  Serial.println("Game ////");
//  Serial.println(String(numHealthTeam1));
//  Serial.println(String(numHealthTeam2));
//  Serial.println("Health updated: Team1 = " + String(numHealthTeam1) + ", Team2 = " + String(numHealthTeam2) + "Game = " + String(flagStartGame));


//  }


//  // Funkcja setup, wykonywana raz na początku
//  void setup() 
//  {
//    Serial.begin(115200);  // Inicjalizuje komunikację szeregową z prędkością 115200 bps
//      // Ustawienia punktu dostępowego
//    WiFi.softAP(ap_ssid, ap_password);  // Tworzy punkt dostępowy z podanym SSID i hasłem
//      IPAddress IP = WiFi.softAPIP();  // Pobiera adres IP punktu dostępowego
//    Serial.print("AP IP address: ");
//    Serial.println(IP);  // Wyświetla adres IP punktu dostępowego

//    // Rejestruje funkcje obsługi żądań HTTP dla określonych URI
//    server.on("/", handleRoot);  // Dla głównego URI ("/")
//    server.on("/btn1Clk", handleButton1);  // Dla URI "/button1"
//    server.on("/btn2Clk", handleButton2);  // Dla URI "/button2"
//    //server.on("/2001", handleButton2);  // Dla URI "/button2"
//    server.on("/Game01/start", HTTP_POST, startGameOnBase);  // Dla URI "/setHealth"
//    server.on("/startGame", GameSzpitalType1);  // Dla URI "/setHealth"
//    server.on("/getFinish", getFinish); // pytanie czy koniec gry.
//    server.begin();  // Uruchamia serwer HTTP
//    Serial.println("HTTP server started");  // Wypisuje "HTTP server started" po uruchomieniu serwera
//  }

//  // Funkcja loop, wykonywana wielokrotnie w nieskończonej pętli
//  void loop() 
//  {
//    server.handleClient();  // Obsługuje żądania klientów HTTP

//    showConnectedDevices();// Czeka i wyświetla liste podłączonych urządzeń

//   if ( numStations > 0 && flagStartGame == 0)
//   {
//    httpCodeStartRead( );
//   }
//   if ( numStations > 0 && httpStartCode == 12001 && flagStartGame == 1)
//    {
//      GameSzpitalType1();
//    }
//  }
