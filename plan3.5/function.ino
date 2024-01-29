double lat;
double lng;
double temp;
double hum;
double co, co2, h2;
int indexMap = 0;

double lat1[100];
double lng1[100];

void updateMyLoc() {
  while (ss.available() > 0) {
    gps.encode(ss.read());
    if (gps.location.isUpdated()) {
      Serial.print("Latitude= ");
      Serial.print(gps.location.lat(), 6);
      Serial.print(" Longitude= ");
      Serial.println(gps.location.lng(), 6);
      // Blynk.virtualWrite(V7, 1 , gps.location.lat(), gps.location.lng(), );
      // Blynk.virtualWrite(V7, gps.location.lng(), gps.location.lat());
      // Blynk.virtualWrite(V0, hum);

      lat = gps.location.lat();
      lng = gps.location.lng();

      Blynk.virtualWrite(V5, gps.location.lat());
      Blynk.virtualWrite(V6, gps.location.lng());

      Blynk.setProperty(V2, "label", "H2");
      Blynk.setProperty(V1, "label", "CO");
      Blynk.setProperty(V0, "label", "CO2");
    }
  }
  actionButtonLoc();
}

void actionButtonLoc() {
  currentState = digitalRead(BUTTON_PIN);
  if (currentState != lastFlickerableState) {
    lastDebounceTime = millis();
    lastFlickerableState = currentState;
  }

  if ((millis() - lastDebounceTime) > DEBOUNCE_TIME) {
    if (lastSteadyState == HIGH && currentState == LOW) {
      Serial.println("The button is pressed");
      
    } else if (lastSteadyState == LOW && currentState == HIGH) {
      Serial.println("The button is released");
      
      lat1[indexMap] = lat;
      lng1[indexMap] = lng;

      Blynk.virtualWrite(V7, indexMap, lat1[indexMap], lng1[indexMap], "CO : " + (String)co + " ppm\n" + "CO2 : " + (String)co2 + " ppm\n" +"H2 : "+ (String)h2 + " ppm\n" + "Temp : " + (String)temp + " C\n" + "Hum : " + (String)hum + " %", 111111);
      indexMap += 1;
    }
    lastSteadyState = currentState;
  }
}


void updateGas() {
  MQ135.update();
  MQ7.update();
  MQ5.update();
  Serial.print("MQ-135 (CO2): ");
  Serial.print(MQ135.readSensor());
  Serial.println(" PPM");
  Serial.print("MQ-7    (CO): ");
  Serial.print(MQ7.readSensor());
  Serial.println(" PPM");
  Serial.print("MQ-5    (H2): ");
  Serial.print(MQ5.readSensor());
  Serial.println(" PPM");

  co = MQ7.readSensor();
  co2 = MQ135.readSensor();
  h2 = MQ5.readSensor();

  Blynk.virtualWrite(V0, MQ135.readSensor());
  Blynk.virtualWrite(V1, MQ7.readSensor());
  Blynk.virtualWrite(V2, MQ5.readSensor());
}

void updateTemp() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print("%  Temperature: ");
  Serial.print(t);
  Serial.println("°C ");
  Serial.println();

  temp = t;
  hum = h;

  Blynk.virtualWrite(V3, t);
  Blynk.virtualWrite(V4, h);
}

BLYNK_WRITE(V8) {
  int btn = param.asInt();
  Serial.print("Add Location");
  Serial.println(btn);
  if (btn == 1){
    Blynk.virtualWrite(V7, indexMap, lat1[indexMap], lng1[indexMap], "CO : " + (String)co + "\n" + "CO2 : " + (String)co2 + "\n" +"H2 : "+ (String)h2 + "\n" + "Temp : " + (String)temp + "\n" + "Hum : " + (String)hum);
    indexMap += 1;
  }
}

BLYNK_WRITE(V9) {
  if (param.asInt()) {
    myMap.clear(); // erase any marker(s)
    indexMap = 0;
    Serial.print("Map GPS markers erased!");
  }
}
