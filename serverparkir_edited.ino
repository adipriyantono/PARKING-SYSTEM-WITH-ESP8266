//PROGRAM SERVER
#include <LiquidCrystal_I2C.h>          // Import library lcd i2c
#include <ESP8266WiFi.h>                // import library esp8266 wifi

#define led D8                          // inisialisasi pin D8 untuk led
#define led1 D5                         // inisialisasi pin D5 untuk led1

LiquidCrystal_I2C lcd(0x3F, 16, 2);     // 

char ssid[] = "12345";                  // SSID WIFI
char pass[] = "12345678";               // password WIFI
WiFiServer server(80);                  //          

IPAddress ip(192, 168, 131, 22);        // IP address Server
IPAddress gateway(192,168,131,26);      // gateway jaringan
IPAddress subnet(255,255,255,0);        // subnet mask jaringan

void setup() {                          // perulangan awal

  pinMode(led, OUTPUT);                 // inisialisasi pin led sebagai output
  pinMode(led1, OUTPUT);                // inisialisasi pin led1 sebagai output

  lcd.begin();                          // Inisialisasi untuk memulai LCD
  lcd.backlight();                      // Menyalakan backlight pada LCD
  lcd.setCursor(0, 0);                  // menaruh karakter pada baris ke 1 kolom 1
  lcd.print("IP:");                     // menampilkan tulisan  
  lcd.setCursor(0, 1);                  // menaruh karakter pada baris ke 1 kolom 2
  lcd.print("P:");                      // menampilkan tulisan

  Serial.begin(115200);                 // setting baudrate menjadi 115200 untuk serial monitor
  WiFi.config(ip, gateway, subnet);     // setting ip, gateway dan subnet yang dipakai
  WiFi.begin(ssid, pass);               // Menghubungkan ke router / hotspot
  
  //Menampilkan status jika wifi blm terhubung
  while (WiFi.status() != WL_CONNECTED)  {
    Serial.print(".");                       // menampilkan tulisan .
    delay(500);                              // membuat delay selama 500 ms
  }
  
  // Memulai server
  server.begin();                                                
  Serial.println("Connected to wifi");                                // menampilkan tulisan 
  Serial.print("Status: ");   Serial.println(WiFi.status());          // menampilkan status wifi      
  Serial.print("IP: ");       Serial.println(WiFi.localIP());         // menampilkan ip lokal yang terhubung
  Serial.print("Subnet: ");   Serial.println(WiFi.subnetMask());      // menampilkan subnet 
  Serial.print("Gateway: ");  Serial.println(WiFi.gatewayIP());       // menampilkan gateway jaringan
  Serial.print("SSID: ");     Serial.println(WiFi.SSID());            // menampilkan nama ssid yang terhubung
  Serial.print("Signal: ");   Serial.println(WiFi.RSSI());            // menampilkan indikator sinyal
  Serial.print("Networks: "); Serial.println(WiFi.scanNetworks());    // menampilkan network / jaringan
}

void loop () {                                         // membuat fungsi perulangan

  WiFiClient client = server.available(); 
  if (client) {
    if (client.connected()) {
      String request = client.readStringUntil('\r');   // Menerima pesan dari Client
      int test = request.toInt();
      Serial.print("From client: ");                   // menampilkan tulisan from client : pada serial monitor
      Serial.println(request);                         // menampilkan isi dari request ke serial monitor 
      client.println("Halo \r");                       // Mengirim respon ke client


      if (test == 0) {                // jika nilai test sama dengan 0, maka :
          digitalWrite(led, LOW);     // led akan mati
          digitalWrite(led1, HIGH);   // led1 akan menyala
      }
      else{                           // jika tidak maka :  
          digitalWrite(led1, LOW);    // led1 akan mati
          digitalWrite(led, HIGH);    // led akan nyala
      }


      // Menampilkan LCD
      lcd.setCursor(3, 0);            // menempatkan karakter mulai dari baris ke 0 kolom ke 3   
      lcd.print(WiFi.localIP());      // Menampilkan IP Address dari Server
      lcd.setCursor(2, 1);            // menempatkan karakter mulai dari baris ke 1 kolom ke 2
      lcd.print(request);             // Menampilkan pesan yang ada pada request dari client
    }

    client.stop();                    // tarminates the connection with the client
  }
}


