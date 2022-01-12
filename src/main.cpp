#include <Arduino.h>
#include "SPIFFS.h"

void setup() {
/*******************************************************************
 * Iniciando Comunicação Serial
 *******************************************************************/
  Serial.begin(9600);
  Serial.println("-----------------------------------------------------------------------------------------------------------------");
  Serial.println("Serial Iniciado");
/*******************************************************************
 * Iniciando Spiffs (Envio de arquivo)
 *******************************************************************/
  if(!SPIFFS.begin(true)){
    Serial.println("Ocorreu um erro iniciando o SPIFFS");
    return;
  }
  Serial.println("Spiffs Iniciado");
}

void loop() {
  // put your main code here, to run repeatedly:
}