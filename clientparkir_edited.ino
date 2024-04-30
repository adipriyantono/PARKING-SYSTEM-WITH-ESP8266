//CLIENT
//#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>

// Define the component pins
#define trig D7
#define echo D8

int kirim;

LiquidCrystal_I2C lcd(0x27, 16, 2);

char ssid[] = "12345";           // SSID of your home WiFi
char pass[] = "12345678";            // password of your home WiFi

unsigned long askTimer = 0;

IPAddress server(192,168,131,22);       // the fix IP address of the server
WiFiClient client;

void setup() {  
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);

  lcd.begin(); 
  lcd.backlight();
  lcd.setCursor(0, 0);         
  lcd.print("IP:");        
  lcd.setCursor(0, 1);         
  lcd.print("FS:");

  Serial.begin(115200);               // set baudrate
  WiFi.begin(ssid, pass);             // connects to the WiFi router
 
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Connecting to...");
    delay(500);
  }

  Serial.println("Connected to wifi");
  Serial.print("Status: "); Serial.println(WiFi.status());    // Network parameters
  Serial.print("IP: ");     Serial.println(WiFi.localIP());
  Serial.print("Subnet: "); Serial.println(WiFi.subnetMask());
  Serial.print("Gateway: "); Serial.println(WiFi.gatewayIP());
  Serial.print("SSID: "); Serial.println(WiFi.SSID());
  Serial.print("Signal: "); Serial.println(WiFi.RSSI());  
}

void loop () {
  client.connect(server, 80);                   // Connection to the server
  digitalWrite(LED_BUILTIN, LOW);               // to show the communication only (inverted logic)


//Get the ultrasonic sensor values
  digitalWrite(trig, LOW);
  delayMicroseconds(4);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  long t = pulseIn(echo, HIGH);
  int distance = t / 29 / 2;

  if (distance <= 10) {
    kirim = 0;
  }
  else{
    kirim = 1;
  }

  Serial.print("distance : ");
  Serial.println(distance);
  client.println(kirim);


  String answer = client.readStringUntil('\r'); // respon dari server
  Serial.println("From server: " + answer);       // menampilkan respon dari server ke serial monitor
  client.flush();
  digitalWrite(LED_BUILTIN, HIGH);

  // Menampilkan LCD
  lcd.setCursor(3, 0);         
  lcd.print(WiFi.localIP());
  //delay(1000);
  lcd.setCursor(3,1);
  lcd.print(answer);
  //delay(2000);               // client will trigger the communication after two seconds

}
