#define BLYNK_TEMPLATE_ID "TMPL6fj8YxMH3"
#define BLYNK_TEMPLATE_NAME "Fire"
#define BLYNK_AUTH_TOKEN "hE98_6y7M1Fd0QNViGmp2hgRVH7fAZkr"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "Hello!";      // type your wifi name
char pass[] = "0987654321";   // type your wifi password

int flame_sensor = D1;         // Connected to D1 pin of NodeMCU for flame sensor
int smoke_sensor = A0;         // Connected to A0 pin of NodeMCU for smoke sensor
int buzzer_pin = D3;           // Connected to D3 pin of NodeMCU for buzzer
int led_pin = D4;              // Connected to D4 pin of NodeMCU for LED

BlynkTimer timer;

void sendFlameSensorData() {
  int isButtonPressed = digitalRead(flame_sensor);
  if (isButtonPressed == LOW) {
    Serial.println("Fire in the House");
    Blynk.logEvent("fire_alert", "Fire Detected");  
    digitalWrite(buzzer_pin, LOW); // Turn on the buzzer
    digitalWrite(led_pin, LOW);    // Turn on the LED
    timer.setTimeout(3000L, []() {
      digitalWrite(buzzer_pin, HIGH); // Turn off the buzzer after 3 seconds
      digitalWrite(led_pin, HIGH);    // Turn off the LED after 3 seconds
    });
  }
}

void sendSmokeSensorData() {
  int smokeLevel = analogRead(smoke_sensor);
  Blynk.virtualWrite(V0, smokeLevel);
  Serial.print("Smoke Level: ");
  Serial.println(smokeLevel); // Print the smoke level value in the serial monitor

  if (smokeLevel > 125) {
    digitalWrite(led_pin, HIGH); // Turn on the LED
  } else {
    digitalWrite(led_pin, LOW); // Turn off the LED
  }

  if (smokeLevel > 200) {
    Blynk.logEvent("gas_alert", "Gas Leakage Detected");
  }

  if (smokeLevel > 200) {
    digitalWrite(buzzer_pin, LOW); // Turn on the buzzer
  } else {
    digitalWrite(buzzer_pin, HIGH); // Turn off the buzzer
  }
}

void setup() {
  pinMode(flame_sensor, INPUT);
  pinMode(smoke_sensor, INPUT);
  pinMode(buzzer_pin, OUTPUT);
  pinMode(led_pin, OUTPUT);
  digitalWrite(buzzer_pin, HIGH); // Set buzzer pin to default HIGH (LED off)
  digitalWrite(led_pin, HIGH);    // Set LED pin to default HIGH (LED off)

  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(5000L, sendFlameSensorData);
  timer.setInterval(2500L, sendSmokeSensorData);
}

void loop() {
  Blynk.run();
  timer.run();
}
