#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);


#define VT_PIN A0
#define AT_PIN A1

float volts;
float amps;

void setup()
{
  Serial.begin(9600);
  lcd.init();
  
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Starting meter");
}

void loop() 
{
  readVoltageCurrent();
  
  displayResult();
  
  delay(10);
}

void readVoltageCurrent()
{

  int averageVoltageReading = 0;
  int averageCurrentReading = 0;

  int numReadings = 100;
  long totalVoltageReading = 0;
  long totalCurrentReading = 0;

  for (int i = 0; i < numReadings; i++)
  {
    totalVoltageReading += analogRead(VT_PIN);
    totalCurrentReading += analogRead(AT_PIN);

    delay(10);
  }

  averageVoltageReading = totalVoltageReading / numReadings;
  averageCurrentReading = totalCurrentReading / numReadings;

  float vcc = readVcc() / 1000.0;  // convert mV to V
  volts = ((averageVoltageReading * vcc) / 1023) * vcc ;
  amps = (averageCurrentReading * vcc) / 1023;
  
  Serial.print("V:");
  Serial.print(volts);
  Serial.print(";A:");
  Serial.print(amps);
  Serial.println(";");
}

void displayResult()
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(volts);
  lcd.print(" volts");
  lcd.setCursor(0,1);
  lcd.print(amps);
  lcd.print(" amps");
}

long readVcc() {
  long result;
  // Read 1.1V reference against AVcc
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Convert
  while (bit_is_set(ADCSRA,ADSC));
  result = ADCL;
  result |= ADCH<<8;
  result = 1125300L / result; // Back-calculate AVcc in mV
  return result;
}

