/*
   -------------------------------------------------------------------------------------
   Leer celda de carga
   Sketch para ilustrar el uso de la librería HX711_ADC.

   Este sketch lee constantemente el valor entregado por el CI HX711 y lo comparte por puerto serie.
   Se asume que el valor de calibración está almacenado en la EEPROM, así que asegúrate de calibrarlo antes.
   Hay un pin (tare_switch) asignado para solicitar manualmente la tara (puesta a cero) del peso.

   Leandro Bertoluzzi 2024
   -------------------------------------------------------------------------------------
   Read load cell
   Sketch to illustrate the usage of the library HX711_ADC.

   This sketch constantly reads the value given by the HX711 IC and shares it via serial port.
   It assumes the calibration value is stored in the EEPROM, so be sure to calibrate it previously.
   It adds a pin (tare_switch) to manually request to tare (set to zero) the weight.

   Leandro Bertoluzzi 2024
   -------------------------------------------------------------------------------------
*/

#include <HX711_ADC.h>
#include <EEPROM.h>

// Pins
const int HX711_dout = 4; //mcu > HX711 dout pin
const int HX711_sck = 5; //mcu > HX711 sck pin
const int tare_switch = 2; //mcu < Tare button pin

// HX711 constructor
HX711_ADC LoadCell(HX711_dout, HX711_sck);

// Global variables
const int calVal_eepromAdress = 0;
unsigned long t = 0;

void setup() {
  Serial.begin(57600); delay(10);
  Serial.println();
  Serial.println("Starting...");

  LoadCell.begin();

  // Fetch calibration value from EEPROM
  float calibrationValue;
  EEPROM.get(calVal_eepromAdress, calibrationValue);

  // Initialize with:
  // - Stabilizing time (ms) to improve the precision
  // - Tare, to set the measured value to zero
  unsigned long stabilizingtime = 2000;
  boolean _tare = true; //set this to false if you don't want tare to be performed in the next step
  LoadCell.start(stabilizingtime, _tare);
  if (LoadCell.getTareTimeoutFlag()) {
    Serial.println("Timeout, check MCU>HX711 wiring and pin designations");
    while (1);
  }
  else {
    LoadCell.setCalFactor(calibrationValue); // set calibration value (float)
    Serial.println("Startup is complete");
  }

  // Initialize the digital pin tare_switch as an input
  pinMode(tare_switch, INPUT_PULLUP);
  // Add interruption to request tare manually
  attachInterrupt(digitalPinToInterrupt(tare_switch), tare, RISING);
}

void loop() {
  static boolean newDataReady = 0;
  const int serialPrintInterval = 0; //increase value to slow down serial print activity

  // Check if there is new data, or start a new conversion
  if (LoadCell.update()) newDataReady = true;

  // Get smoothed value from the dataset
  if (newDataReady && millis() > t + serialPrintInterval) {
    float i = LoadCell.getData();
    Serial.print("Load_cell output val: ");
    Serial.println(i);
    newDataReady = 0;
    t = millis();
  }

  // Receive command via serial port:
  // - Send 't' to request tare
  if (Serial.available() > 0) {
    char inByte = Serial.read();
    if (inByte == 't') tare();
  }

  // Check if the last tare operation is complete
  if (LoadCell.getTareStatus() == true) {
    Serial.println("Tare complete");
  }

}

void tare() {
  LoadCell.tareNoDelay();
}
