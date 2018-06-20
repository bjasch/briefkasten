#include <SoftwareSerial.h>
#include "DumbServer.h"

/* The WiFi shield is connected to
 * the Arduino pins 3 and 2, as the
 * Arduino has only one hardware serial
 * port (pins 0 and 1) we are using a
 * serial port emulated in software. */
SoftwareSerial esp_serial(3, 2);
EspServer esp_server;

int led = 13;                // LED Pin
int sensor = 2;              // Sensor Pin
int Zustand = LOW;             // Keine Bewegung erfasst
int Wert = 0;

int licht = A0;
int lichtwert;
boolean einmal = true;
int a = 0;

void setup()
{
  Serial.begin(9600);
  esp_serial.begin(9600);

  /* Connect to the wireless network with the name "GDI"
   * and password "password", change these to match
   * your wifi settings.
   *
   * If anything fails begin() will not return.
   *
   * To debug possible problems you can flash a second
   * Arduino with the "Bare Minimum" example,
   * connect the GNDs of the two Arduinos,
   * connect pin 3 or 2 of the Arduino with the Wifi-shield
   * to pin 1(TX) of the other Arduino and use the Serial monitor
   * to see the Wifi commands and error-messages. */
  Serial.println("Starting server...");
  esp_server.begin(&esp_serial, "o2-WLAN56", "02230607jessica", 30303);
  Serial.println("...server is running");


  /* Get and print the IP-Address the python program
   * should connect to */
  char ip[16];
  esp_server.my_ip(ip, 16);

  Serial.print("My ip: ");
  Serial.println(ip);

  pinMode(led, OUTPUT);      // LED als Output
  pinMode(sensor, INPUT);    // Sensor als Input
}

void loop()
{
  Wert = digitalRead(sensor);   // Sensor Wert ablesen
  if (Wert == HIGH) {           // Checken ob der Sensor Wert Hoch ist
    digitalWrite(led, HIGH);   // LED anschalten
    delay(2000);                // 2 Sekunden verzögern 

    if (Zustand == LOW) {
      Serial.println("0"); 
      Zustand = HIGH;       // Den Status auf HIGH setzen
    }
  } 
  else {
      digitalWrite(led, LOW); // LED aus
      delay(3000);             // 3 Sekunden verzögern 
      
      if (Zustand == HIGH){
        Serial.println("1");
        Zustand = LOW;       // Den Variablen Status auf LOW setzen
    }
   }

    lichtwert = analogRead(licht);

    if(lichtwert <= 500){
      esp_server.println(a);
      einmal = true;
    }
    else if (lichtwert > 500&& einmal==true){
      a++;
      esp_server.println(a);
      einmal = false;
    }
    Serial.println(lichtwert);
}
