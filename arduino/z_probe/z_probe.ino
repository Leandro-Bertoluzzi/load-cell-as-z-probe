/*
   -------------------------------------------------------------------------------------
   Sonda Z con celda de carga
   Sketch para usar una celda de carga como sonda Z para el CNC, mediante la librería HX711_ADC.

   Este sketch lee constantemente el valor entregado por el CI HX711 y lo compara con un valor preestablecido.
   Si se supera dicho valor, se activa la salida del sensor. Si baja de dicho valor, se desactiva la salida.
   Se asume que el valor de calibración está almacenado en la EEPROM, así que asegúrate de calibrarlo antes.
   Hay un pin (tare_switch) asignado para solicitar manualmente la tara (puesta a cero) del sensor.

   Leandro Bertoluzzi 2024
   -------------------------------------------------------------------------------------
   Z-probe with load cell
   Sketch to use a load cell as a Z-probe for a CNC machine, via the library HX711_ADC.

   This sketch constantly reads the value given by the HX711 IC and compares it with a fixed value.
   If the value is exceeded, the sensor output is activated. If it falls below this value, the output is deactivated.
   It assumes the calibration value is stored in the EEPROM, so be sure to calibrate it previously.
   It adds a pin (tare_switch) to manually request to tare (set to zero) the sensor.

   Leandro Bertoluzzi 2024
   -------------------------------------------------------------------------------------
*/

#include <HX711_ADC.h>
#include <EEPROM.h>

// Pins
const int HX711_dout = 4; //mcu > HX711 dout
const int HX711_sck = 5; //mcu > HX711 sck
const int sensor_output = LED_BUILTIN; //mcu > Sensor output
const int tare_switch = 2; //mcu < Tare button

// Constants
#define THRESHOLD_WEIGHT 50.0
#define HYSTERESIS 5.0
#define THRESHOLD_WEIGHT_HIGH (THRESHOLD_WEIGHT + HYSTERESIS)
#define THRESHOLD_WEIGHT_LOW (THRESHOLD_WEIGHT - HYSTERESIS)

// HX711 constructor
HX711_ADC LoadCell(HX711_dout, HX711_sck);

// Global variables
const int calVal_eepromAdress = 0;

void setup() {
  Serial.begin(57600); delay(10);
  Serial.println(); Serial.println("Starting...");

  LoadCell.begin();

  // Fetch calibration value from EEPROM
  float calibrationValue;
  EEPROM.get(calVal_eepromAdress, calibrationValue);

  // Initialize with:
  // - Stabilizing time (ms) to improve the precision
  // - Tare, to set the measured value to zero
  unsigned long stabilizingtime = 2000;
  boolean _tare = true;
  LoadCell.start(stabilizingtime, _tare);
  if (LoadCell.getTareTimeoutFlag()) {
    Serial.println("Timeout, chequear la conexión MCU>HX711");
    while (1);
  }
  else {
    LoadCell.setCalFactor(calibrationValue);
    Serial.println("¡Listo!");
  }

  // Initialize the digital pin sensor_output as an output
  pinMode(sensor_output, OUTPUT);
  // Initialize the digital pin tare_switch as an input
  pinMode(tare_switch, INPUT_PULLUP);
  // Add interruption to request tare manually
  attachInterrupt(digitalPinToInterrupt(tare_switch), tare, RISING);
}

void loop() {
  static boolean newDataReady = 0;
  static boolean sensor_high = 0;

  // Check if there is new data, or start a new conversion
  if (LoadCell.update()) newDataReady = true;

  // Get smoothed value from the dataset
  if (newDataReady) {
    float i = LoadCell.getData();
    newDataReady = 0;

    // Validate if the sensor output must be updated
    if (i > THRESHOLD_WEIGHT_HIGH && !sensor_high){
      digitalWrite(sensor_output, HIGH);
      Serial.println("Sensor HIGH!");
      sensor_high = 1;
    }

    if (i < THRESHOLD_WEIGHT_LOW && sensor_high){
      digitalWrite(sensor_output, LOW);
      Serial.println("Sensor LOW!");
      sensor_high = 0;
    }
  }

  // Receive command via serial port:
  // - Send 't' to request tare
  // - Send 'p' to see the current measured value
  if (Serial.available() > 0) {
    char inByte = Serial.read();
    if (inByte == 't') tare();
    if (inByte == 'p') show_value();
  }

  // Check if the last tare operation is complete
  if (LoadCell.getTareStatus() == true) {
    Serial.println("Tare complete");
  }

}

void tare() {
  LoadCell.tareNoDelay();
}

void show_value() {
  float i = LoadCell.getData();
  Serial.print("Load_cell output val: ");
  Serial.println(i);
}
