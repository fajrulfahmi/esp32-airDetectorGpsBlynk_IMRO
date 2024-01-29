void mqBegin() {
  MQ135.setRegressionMethod(1);  //_PPM =  a*ratio^b
  MQ135.setA(110.47);
  MQ135.setB(-2.862);  // Configure the equation to to calculate H2 concentration

  /*
    Exponential regression:
  GAS      | a      | b
  CO       | 605.18 | -3.937  
  Alcohol  | 77.255 | -3.18 
  CO2      | 110.47 | -2.862
  Toluen   | 44.947 | -3.445
  NH4      | 102.2  | -2.473
  Aceton   | 34.668 | -3.369
  */

  MQ7.setRegressionMethod(1);
  MQ7.setA(99.042);
  MQ7.setB(-1.518);

  /*
  Exponential regression MQ7:
  GAS     |          a         |    b
  H2      | 69.014             | -1.374
  LPG     | 700000000          | -7.703
  CH4     | 60000000000000     | -10.54
  CO      | 99.042             | -1.518
  Alcohol | 40000000000000000  | -12.35
  */

  MQ5.setRegressionMethod(1);
  MQ5.setA(1163.8);
  MQ5.setB(-3.874);

  /*
  Exponential regression:
  Gas    | a      | b
  H2     | 1163.8 | -3.874
  LPG    | 80.897 | -2.431
  CH4    | 177.65 | -2.56
  CO     | 491204 | -5.826
  Alcohol| 97124  | -4.918
  */

  //Kalibrasi MQ135
  MQ135.init();
  Serial.print("Calibrating MQ-135 please wait.");
  float calcR0 = 0;
  for (int i = 1; i <= 10; i++) {
    MQ135.update();
    calcR0 += MQ135.calibrate(RatioMQ135CleanAir);
    Serial.print(".");
  }
  MQ135.setR0(calcR0 / 10);
  Serial.println("Calibrating MQ-135 done!.");

  //Kalibrasi MQ7
  MQ7.init();
  Serial.print("Calibrating MQ-7 please wait.");
  float calcR01 = 0;
  for (int i = 1; i <= 10; i++) {
    MQ7.update();
    calcR01 += MQ7.calibrate(RatioMQ7CleanAir);
    Serial.print(".");
  }
  MQ7.setR0(calcR01 / 10);
  Serial.println("Calibrating MQ-7 done!.");

  //Kalibrasi MQ5
  MQ5.init();
  Serial.print("Calibrating MQ-5 please wait.");
  float calcR02 = 0;
  for (int i = 1; i <= 10; i++) {
    MQ5.update();
    calcR02 += MQ5.calibrate(RatioMQ5CleanAir);
    Serial.print(".");
  }
  MQ5.setR0(calcR02 / 10);
  Serial.println("Calibrating MQ-5 done!.");

  Serial.println();
}