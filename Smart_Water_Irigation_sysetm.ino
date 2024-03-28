#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

#define DHT_SENSOR_PIN 4
#define DHT_SENSOR_TYPE DHT11

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);
DHT dht_sensor(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);

#define moisturepin A0
#define pump 7

float humi = 0;
float tempC = 0;

// Variables to store current and previous humidity values
int moisturevalue = 0;

void setup()
{
  // Initialize the serial monitor
  Serial.begin(9600);
  // Configure pump and humidity sensor pins
  pinMode(pump, OUTPUT);
  pinMode(moisturepin, INPUT);
  dht_sensor.begin();
  // initialize the LCD
  lcd.init();

  // Turn on the blacklight and print a message.
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Soil Moisture");
  lcd.clear();
}

void loop()
{
  humi = dht_sensor.readHumidity();
  tempC = dht_sensor.readTemperature();
  Serial.println("Temperature: " + String(tempC));
  Serial.println("Humidity: " + String(humi));

  lcd.clear();
  lcd.setCursor(0, 0); 
  lcd.print("Temp: " + String(tempC));

  lcd.setCursor(0, 1); 
  lcd.print("Humidity: " + String(humi));
  delay(2000);
  // Read the current value from the humidity sensor
  moisturevalue = analogRead(moisturepin);
  // moisturevalue = map(moisturevalue , 0, 1023, 0, 100);
  moisturevalue = ( 100 - ( (moisturevalue / 1023.00) * 100 ) );
  // Check if humidity is less than or equal to 30 (dry soil)
  if (moisturevalue <= 30) {
    // Turn on the pump
    digitalWrite(pump, HIGH);
    lcd.clear();
    lcd.setCursor(0, 0);
    // tell the screen to write “hello, from” on the
    lcd.print("Moisture: ");
    lcd.print(moisturevalue);
    lcd.setCursor(0, 1);
    lcd.print("Pump: ON");
    // Print the current humidity value to the serial monitor
    Serial.println("Soil Humidity = " + String(moisturevalue) + "% - Pump ON");
  } else {
    // Turn off the pump
    digitalWrite(pump, LOW);
    lcd.clear();
    lcd.setCursor(0, 0);
    // tell the screen to write “hello, from” on the
    lcd.print("Moisture: ");
    lcd.print(moisturevalue);
    lcd.setCursor(0, 1);
    lcd.print("Pump: OFF");
    // Print the current humidity value to the serial monitor
    Serial.println("Soil Humidity = " + String(moisturevalue) + "% - Pump OFF");
  }

  delay(2000);
}
