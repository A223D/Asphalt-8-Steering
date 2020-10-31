/*
    Arduino and ADXL345 Accelerometer Tutorial
     by Dejan, https://howtomechatronics.com
*/
#include <Wire.h>  // Wire library - used for I2C communication
#include <VirtualWire.h>

// nitro, brake, powerup1, powerup2, left, right, 0, 0
byte message = 0b00000000;

const int whiteButton = 12;
const int blackButton = 8;
const int redButton1 = 5;
const int redButton2 = 2;
const int transmit_pin = 6;
const int transmit_en_pin = 3;

const int led = 13;
double tanValue = 0;
int ADXL345 = 0x53; // The ADXL345 sensor I2C address
double X_out, Y_out, Z_out, X_outN = 0, Y_outN = 0, Z_outN = 0; // Outputs
void setup() {
  Serial.begin(9600); // Initiate serial communication for printing the results on the Serial monitor
  vw_set_tx_pin(transmit_pin);
  vw_set_ptt_pin(transmit_en_pin);
  vw_set_ptt_inverted(true); // Required for DR3100
  vw_setup(2000);   // Bits per sec
  pinMode(redButton1, INPUT);
  pinMode(redButton2, INPUT);
  pinMode(whiteButton, INPUT);
  pinMode(blackButton, INPUT);
  pinMode(led, OUTPUT);
  Wire.begin(); // Initiate the Wire library
  // Set ADXL345 in measuring mode
  Wire.beginTransmission(ADXL345); // Start communicating with the device
  Wire.write(0x2D); // Access/ talk to POWER_CTL Register - 0x2D
  // Enable measurement
  Wire.write(8); // (8dec -> 0000 1000 binary) Bit D3 High for measuring enable
  Wire.endTransmission();

  delay(10);

  Wire.beginTransmission(ADXL345);
  Wire.write(0x1E); // Access/ talk to POWER_CTL Register - 0x2D
  // Enable measurement
  Wire.write(0xFFFFFFFE); // (8dec -> 0000 1000 binary) Bit D3 High for measuring enable
  Wire.endTransmission();


  Wire.beginTransmission(ADXL345);
  Wire.write(0x1F); // Access/ talk to POWER_CTL Register - 0x2D
  // Enable measurement
  Wire.write(0x2); // (8dec -> 0000 1000 binary) Bit D3 High for measuring enable
  Wire.endTransmission();

  //  Wire.beginTransmission(ADXL345);
  //  Wire.write(0x20); // Access/ talk to POWER_CTL Register - 0x2D
  //  // Enable measurement
  //  Wire.write(0x3); // (8dec -> 0000 1000 binary) Bit D3 High for measuring enable
  //  Wire.endTransmission();
  Serial.println("Everything Done. Let's begin in 5 seconds. Jai Shri Ram...");
  digitalWrite(led, HIGH);
  delay(5000);
  digitalWrite(led, LOW);
  
}
void loop() {
  message = 0b00000000;
  // === Read acceleromter data === //
  Wire.beginTransmission(ADXL345);
  Wire.write(0x32); // Start with register 0x32 (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(ADXL345, 6, true); // Read 6 registers total, each axis value is stored in 2 registers
  X_outN = ( Wire.read() | Wire.read() << 8); // X-axis value
  X_outN = X_outN / 256; //For a range of +-2g, we need to divide the raw values by 256, according to the datasheet
  Y_outN = ( Wire.read() | Wire.read() << 8); // Y-axis value
  Y_outN = Y_outN / 256;
  Z_outN = ( Wire.read() | Wire.read() << 8); // Z-axis value
  Z_outN = Z_outN / 256;

  X_out = (0.5 * X_out) + (0.5 * X_outN);
  Y_out = (0.5 * Y_out) + (0.5 * Y_outN);
  Z_out = (0.5 * Z_out) + (0.5 * Z_outN);
  //  Serial.print("Xa= ");
  //  Serial.print(X_out);
  //  Serial.print("   Ya= ");
  //  Serial.print(Y_out);
  //  Serial.print("   Za= ");
  //  Serial.println(Z_out);

  tanValue = Y_out / X_out;
  tanValue = atan(tanValue) * 57.295;

  if (tanValue < 0 ) {
    tanValue = map(tanValue, -90.00, -1.00, 90.00, 179.00);
  } else {
    tanValue = tanValue + 0.5;
    tanValue = (int)tanValue;
  }
  //Serial.print("Angle: ");
  //Serial.println(tanValue);

  if (tanValue < 70) {
    message = message | 0b00001000;
  }

  if (tanValue > 110) {
    message = message | 0b00000100;
  }

  if (digitalRead(whiteButton)) {
    message = message | 0b10000000;
  }
  if (digitalRead(blackButton)) {
    message = message | 0b01000000;
  }
  if (digitalRead(redButton1)) {
    message = message | 0b00100000;
  }
  if (digitalRead(redButton2)) {
    message = message | 0b00010000;
  }

  //  if (digitalRead(whiteButton) || digitalRead(blackButton) || digitalRead(redButton1) || digitalRead(redButton2)) {
  //    digitalWrite(led, HIGH);
  //    //Serial.println("LED On");
  //  } else {
  //    digitalWrite(led, LOW);
  //    //Serial.println("LED Off");
  //  }

  digitalWrite(led, HIGH);
  vw_send(&message, 1);
  vw_wait_tx();
  digitalWrite(led, LOW);


  Serial.println(message, BIN);
  
}
