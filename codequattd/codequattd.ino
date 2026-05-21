  #include <LiquidCrystal_I2C.h>
  #include <Servo.h>
  #include <Wire.h>
  #include <DHT.h>

  #define DHTPIN A0
  #define DHTTYPE DHT11
  DHT dht(DHTPIN, DHTTYPE);
  LiquidCrystal_I2C lcd(0x27, 16, 2);
  Servo swingServo; 

  const int IN1 = 8;
  const int IN2 = A2;
  const int EN1 = 11;

  const int servoPin = 10;

  const int btnOnOff = A1;
  const int btnMode  = 9;
  const int btnSpeed = 12;
  const int btnSwing = 13;

  bool systemOn = false;
  bool autoMode = true;
  bool swingOn = false;

  int manualLevel = 0;
  int fanPWM = 0;

  int servoAngle = 90;
  int servoStep = 1;

  unsigned long lastServoTime = 0;
  unsigned long lastLCDTime = 0;

  bool oldOnOff = HIGH;
  bool oldMode = HIGH;
  bool oldSpeed = HIGH;
  bool oldSwing = HIGH;

  void setup() {
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(EN1, OUTPUT);

    pinMode(btnOnOff, INPUT_PULLUP);
    pinMode(btnMode, INPUT_PULLUP);
    pinMode(btnSpeed, INPUT_PULLUP);
    pinMode(btnSwing, INPUT_PULLUP);

    lcd.init();
    lcd.backlight();
    dht.begin();
    swingServo.attach(servoPin);
    servoAngle = 90;
    servoStep = 1;
    swingServo.write(90);

    runFan(0);


    lcd.setCursor(0, 0);
    lcd.print("HE THONG QUAT TD");
    lcd.setCursor(0, 1);  
    lcd.print("SAN SANG");
    delay(1200);
    lcd.clear();
  }

  void loop() {
    readButtons();

    if (!systemOn) {
      fanPWM = 0;
    } else {
      if (autoMode) {
        fanPWM = autoFanSpeed(readTemperature());
      } else {
        fanPWM = manualFanSpeed();
      }
    }

    runFan(fanPWM);


    if (systemOn && swingOn) {
      runSwing();
    }

    if (millis() - lastLCDTime >= 300) {
      displayLCD(readTemperature(), fanPWM);
      lastLCDTime = millis();
    }
  }

  float readTemperature() {
    return dht.readTemperature();
  }

  void runFan(int pwm) {
    if (pwm <= 0) {
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      analogWrite(EN1, 0);
    } else {
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      analogWrite(EN1, pwm);
    }
  }

  void readButtons() {
    bool newOnOff = digitalRead(btnOnOff);
    bool newMode  = digitalRead(btnMode);
    bool newSpeed = digitalRead(btnSpeed);
    bool newSwing = digitalRead(btnSwing);

    if (oldOnOff == HIGH && newOnOff == LOW) {
      delay(50);

      if (digitalRead(btnOnOff) == LOW) {
        systemOn = !systemOn;

        if (systemOn) {
          autoMode = true;
          manualLevel = 0;
          swingOn = false;
          servoAngle = 90;
          servoStep = 1;
          swingServo.write(90);
        } else {
          fanPWM = 0;
          manualLevel = 0;
          swingOn = false;
          runFan(0);
          stopServo();
        }

        while (digitalRead(btnOnOff) == LOW);
      }
    }

    if (oldMode == HIGH && newMode == LOW) {
      delay(50);

      if (digitalRead(btnMode) == LOW && systemOn) {
        autoMode = !autoMode;

        if (autoMode) {
          manualLevel = 0;
        }

        while (digitalRead(btnMode) == LOW);
      }
    }

    if (oldSpeed == HIGH && newSpeed == LOW) {
      delay(50);

      if (digitalRead(btnSpeed) == LOW && systemOn && !autoMode) {
        manualLevel++;

        if (manualLevel > 3) {
          manualLevel = 0;
        }

        while (digitalRead(btnSpeed) == LOW);
      }
    }

    if (oldSwing == HIGH && newSwing == LOW) {
      delay(50);

      if (digitalRead(btnSwing) == LOW && systemOn) {
        swingOn = !swingOn;

        if (swingOn) {
          servoAngle = 0;
          servoStep = 1;
          swingServo.write(0);
          lastServoTime = millis();
        } else {
          stopServo();
        }

        while (digitalRead(btnSwing) == LOW);
      }
    }

    oldOnOff = digitalRead(btnOnOff);
    oldMode  = digitalRead(btnMode);
    oldSpeed = digitalRead(btnSpeed);
    oldSwing = digitalRead(btnSwing);
  }

  int autoFanSpeed(float temp) {
    if (temp < 25) return 0;
    else if (temp >= 25 && temp <= 30) return 100;
    else if (temp > 30 && temp < 33) return 200;
    else return 255;
  }

  int manualFanSpeed() {
    if (manualLevel == 1) return 100;
    if (manualLevel == 2) return 200;
    if (manualLevel == 3) return 255;
    return 0;
  }

  void runSwing() {
    if (millis() - lastServoTime >= 60) {
      servoAngle += servoStep;

      if (servoAngle >= 180) {
        servoAngle = 180;
        servoStep = -1;
      }

      if (servoAngle <= 0) {
        servoAngle = 0;
        servoStep = 1;
      }

      swingServo.write(servoAngle);
      lastServoTime = millis();
    }
  }

  void stopServo() {
    swingOn = false;

    while (servoAngle != 90) {
      if (servoAngle > 90) servoAngle--;
      else if (servoAngle < 90) servoAngle++;

      swingServo.write(servoAngle);
      delay(15);
    }

    servoStep = 1;
    swingServo.write(90);
  }

  void displayLCD(float temp, int speedPWM) {
    lcd.setCursor(0, 0);
    lcd.print("T:");
    lcd.print(temp, 1);
    lcd.print((char)223);
    lcd.print("C ");

    if (!systemOn) lcd.print("OFF  ");
    else if (autoMode) lcd.print("AUTO  ");
    else lcd.print("MANUAL ");

    lcd.setCursor(0, 1);

    if (speedPWM == 0) lcd.print("SPD:0    ");
    else if (speedPWM <= 100) lcd.print("SPD:LOW  ");
    else if (speedPWM <= 200) lcd.print("SPD:MED  ");
    else lcd.print("SPD:MAX  ");

    if (systemOn && swingOn) lcd.print("SW:ON ");
    else lcd.print("SW:OFF ");
  }