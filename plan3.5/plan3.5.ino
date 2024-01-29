#include "DHT.h"
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <MQUnifiedsensor.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL6_86pzthq"
#define BLYNK_TEMPLATE_NAME "airDetector"
#define BLYNK_AUTH_TOKEN "UEjiCG_PcA1OH_l8g1U7zSbsMT5W-BdX"
#define DHTPIN 12  // pins 3, 4, 5, 12, 13 or 14 
#define DHTTYPE DHT11
#define SHORT_PRESS_TIME 200
#define LONG_PRESS_TIME  800
#define Board ("ESP-32")
#define PinMQ135 (35)
#define PinMQ7 (39)
#define PinMQ5 (36)
#define Type1 ("MQ-135")
#define Type2 ("MQ-7")
#define Type3 ("MQ-5")
#define Voltage_Resolution (3.3)
#define ADC_Bit_Resolution (12)
#define RatioMQ135CleanAir (3.6)
#define RatioMQ7CleanAir (27.5)
#define RatioMQ5CleanAir (6.5)
#define BUTTON_PIN     32
#define DEBOUNCE_TIME  50
#define led1 26
#define led2 27


static const int RXPin = 16, TXPin = 17;  //RX -> TX & TX -> RX
static const uint32_t GPSBaud = 9600;
int lastSteadyState = LOW;
int lastFlickerableState = LOW;
int currentState;   
unsigned long lastDebounceTime = 0;

MQUnifiedsensor MQ135(Board, Voltage_Resolution, ADC_Bit_Resolution, PinMQ135, Type1);
MQUnifiedsensor MQ7(Board, Voltage_Resolution, ADC_Bit_Resolution, PinMQ7, Type2);
MQUnifiedsensor MQ5(Board, Voltage_Resolution, ADC_Bit_Resolution, PinMQ5, Type3);
TinyGPSPlus gps;
WidgetMap myMap(V7);
SoftwareSerial ss(RXPin, TXPin);
BlynkTimer timer;
DHT dht(DHTPIN, DHTTYPE);

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "admin";
char pass[] = "12345678";

void setup() {
  Serial.begin(115200);
  digitalWrite(led1, LOW);
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  Serial.println(Blynk.connected());
  if(Blynk.connected() == 1){
    digitalWrite(led1, HIGH);
  }
  Serial.println("OK");
  ss.begin(GPSBaud);
  mqBegin();
  dht.begin();
  pinMode(BUTTON_PIN, INPUT_PULLDOWN);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);

  // timer.setInterval(100L, updateMyLoc);
  // timer.setInterval(100L, actionButtonLoc);
  timer.setInterval(500L, updateGas);
  timer.setInterval(500L, updateTemp);
}

void loop() {
  Blynk.run();
  updateMyLoc();
  timer.run();
  // actionButtonLoc();
}