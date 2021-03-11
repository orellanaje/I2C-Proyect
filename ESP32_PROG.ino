// Adafruit IO Publish Example
//JOS OREREELLA

// edit the config.h tab and enter your Adafruit IO credentials
// and any additional configuration needed for WiFi, cellular,
// or ethernet clients.
#include "config.h" //Crendeciales para conectar el WIFI y IOT


int DATA2 = 0;
int incomingByte = 0;
String led_state;    

AdafruitIO_Feed *GRAPH = io.feed("ESP32");
AdafruitIO_Feed *BTN_1 = io.feed("BTN_1");
AdafruitIO_Feed *BTN_2 = io.feed("BTN_2"); 
//Nombres de los FEEDS a utilizar

void handleMessage(AdafruitIO_Data *data) { //RECIBIR DATOS Y RETENERNLOS
     
    led_state = data->value();
    
}


void setup() {
  
  pinMode(2, OUTPUT); 
  Serial.begin(9600); 

  
  io.connect();       // conexion a IOT 

  BTN_1->onMessage(handleMessage);
  BTN_2->onMessage(handleMessage);
  
  // wait for a connection
  while(io.status() < AIO_CONNECTED) { 
    digitalWrite(2,HIGH);             
    delay(500);
  }
  digitalWrite(2,LOW);               

  BTN_1->get();
  BTN_2->get();                  
}

void loop() {

  io.run();

  if (Serial.available() > 0) {       // UART
  
    incomingByte = Serial.read();     
    
    GRAPH->save((incomingByte, DEC)); 
   
  }


  switch (led_state.toInt()) { //CONFIG BOTOES IOT
  case 00:
    
    Serial.write(0X0A);       
    break;
  case 01:
    
    Serial.write(0X0B);
    break;
    case 10:
    
    Serial.write(0X0C);
    break;
    case 11:
    
    Serial.write(0X0D);
    break;
  default:
    break;
}

  delay(3000);

}

// Written by Todd Treece for Adafruit Industries
// Copyright (c) 2016 Adafruit Industries
// Licensed under the MIT license.
