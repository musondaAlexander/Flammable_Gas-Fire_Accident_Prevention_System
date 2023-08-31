#include<SoftwareSerial.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x20,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display


// Define the pins for the GSM module
#define GSM_TX_PIN 10
#define GSM_RX_PIN 11
//Define the pins for the led,buzzer,
int redLed = 11;
int greenLed = 12;
int buzzer = 13;

// Define the analog sensor pin
#define ANALOG_SENSOR_PIN A0


// Set the baud rate for the GSM module
#define GSM_BAUD_RATE 9600

// Set the threshold value for data transmission
#define THRESHOLD 50
//int sensorThres = 1;

// Initialize the GSM module
SoftwareSerial gsmSerial(GSM_TX_PIN, GSM_RX_PIN);

void setup() {
   lcd.init();                      // initialize the lcd 
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("FIRE/GAS DETECTED");
  // Start serial communication for debugging
lcd.begin(16, 2);
Serial.begin(9600);
pinMode(redLed, OUTPUT);
pinMode(greenLed,OUTPUT);
pinMode(buzzer, OUTPUT);
  
  // Start GSM serial communication
  gsmSerial.begin(GSM_BAUD_RATE);
  
  // Set the GSM module to text mode
  gsmSerial.write("AT+CMGF=1\r\n");
  delay(200);
  
  // Print a message to indicate the setup is complete
  Serial.println("System Ready!");
}

void loop() {
  // Read the analog sensor value
  int sensorValue = analogRead(ANALOG_SENSOR_PIN);
  Serial.print("STATUS: ");
  lcd.setCursor(3,0);
  lcd.print("STATUS: ");
  //lcd.print(sensorValue);
  
  // Convert the analog value to a string
 String dataToSend1 = String("FIRE/GAS DET");
 String dataToSend = String(sensorValue);
  
  
  
  // Check if the value exceeds the threshold for transmission
  if (sensorValue > THRESHOLD) {
    // Send the data via GSM module
    sendSMS(dataToSend1);
    sendSMS(dataToSend);
    digitalWrite(redLed, HIGH);
    lcd.setCursor(0,1);
    lcd.print("FIRE/GAS DETECTED");
    digitalWrite(greenLed, LOW);
    tone(buzzer, 1000, 200);
    
    
     // Print the data to the serial monitor
    
    Serial.println( dataToSend1);
    Serial.println("Sensor Value: " + dataToSend);
    
  }
  else
{
digitalWrite(redLed, LOW);
digitalWrite(greenLed, HIGH);
Serial.println("NO FIREE/GAS DETECTED!");
lcd.setCursor(0,1);
lcd.print("NO FIRE/GAS DETECTED");
noTone(buzzer);

}  

  // Wait for a moment before reading the sensor again
  delay(200);
}

void sendSMS(String message) {
  // Command to set the recipient phone number
  gsmSerial.write("AT+CMGS=\"+1234567890\"\r\n");
  delay(200);
  
  // Send the message
  gsmSerial.print(message);
  delay(200);
  
  // Send the CTRL+Z escape character to indicate the end of the message
  gsmSerial.write((char)26);
  delay(100);
  
  // Print a message to indicate successful data transmission
  Serial.println("Data sent: " + message);
}
