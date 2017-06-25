/*

*/
#include "Botones.h"

Boton boton;

void setup() {
    Serial.begin(9600);

    boton.set(8);
}

void loop() {
    if(boton.buttonPush()){
        Serial.println("Push");
    }
}

