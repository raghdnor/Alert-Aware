#include <BluetoothSerial.h>
#include <DHT.h>

#define DHTPIN 12              // Pin connected to DHT22
#define DHTTYPE DHT22         // DHT22 sensor type
#define MQ3PIN 27             // Analog pin for MQ-3 alcohol sensor
#define LDRPIN 26             // Analog pin for LDR (light sensor)

#define MQindicator 23                // red
#define Tempindicator 19             // yellow
#define Humindicator 18             // blue

#define buzzerPin 5             // buzzer


BluetoothSerial BTSerial;

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();
  
  pinMode(MQ3PIN, INPUT);
  pinMode(LDRPIN, INPUT);

  pinMode(MQindicator, OUTPUT);
  pinMode(Tempindicator, OUTPUT);
  pinMode(Humindicator, OUTPUT);

  pinMode(buzzerPin, OUTPUT);

  // Initialize Bluetooth Serial
  if (BTSerial.begin("ESP32_Monitor")) {

    Serial.println("Bluetooth initialized, discoverable as ESP32_Monitor");

  }else{

    Serial.println("Bluetooth initialization failed");
    return;

  }
  
}

void loop() {
  // Gather data from sensors
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  float alcoholRate = analogRead(MQ3PIN);   // Raw reading from MQ-3 sensor
  int lightIntensity = analogRead(LDRPIN); // Raw reading from LDR sensor


  lightIntensity = map(lightIntensity, 4095, 0, 0 , 100);

  alcoholRate = map(alcoholRate, 10, 3480, 0 , 100);

  if (temperature > 25 || temperature < 20) {
    digitalWrite(buzzerPin, HIGH);
    delay(500);
    digitalWrite(buzzerPin, LOW);

    digitalWrite(Tempindicator, HIGH);
    delay(500);
    digitalWrite(Tempindicator, LOW);
  }

  if (humidity > 60 || humidity < 30) {
    digitalWrite(buzzerPin, HIGH);
    delay(500);
    digitalWrite(buzzerPin, LOW);

    digitalWrite(Humindicator, HIGH);
    delay(500);
    digitalWrite(Humindicator, LOW);


  }
  
  if (alcoholRate > 50 || alcoholRate < 20) {
    digitalWrite(buzzerPin, HIGH);
    delay(500);
    digitalWrite(buzzerPin, LOW);

    digitalWrite(MQindicator, HIGH);
    delay(500);
    digitalWrite(MQindicator, LOW);
  }
  
  // Format data as CSV and send over Bluetooth

  String dataString = String(temperature) + ", " + String(humidity) + ", " + String(alcoholRate) + ", " + String(lightIntensity) + "\n"; //
  
  BTSerial.print(dataString); // Send data via Bluetooth

  // Optional: Print data to Serial Monitor for debugging
  Serial.print("Sent data: ");
  Serial.print(dataString);


  delay(5000); // Transmit every 5 seconds
}
 