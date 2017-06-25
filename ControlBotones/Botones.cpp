/*
Boton.cpp - Library for flashing Morse code.
Created by
Released into the public domain.
*/

#include "Arduino.h"
#include "Botones.h"

Boton::Boton()
{
  _TIEMPO_BASE = 100; //us

}
//
void Boton::set(int pin){
  _pin = pin;
  _pin_estado=LOW;
  _pin_estado_instante_anterior=LOW;
  _pin_timer=micros();

}
//
bool Boton::buttonPush()
{
  int estado_instante = digitalRead(_pin);
  if (_pin_estado_instante_anterior != estado_instante)
  {
    _pin_timer=micros();
    _pin_estado_instante_anterior = estado_instante;
  }
  if (micros() - _pin_timer > _TIEMPO_BASE)
  {
    _pin_estado = estado_instante;
  }

  return _pin_estado==HIGH;
}

