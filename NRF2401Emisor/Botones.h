/*
  Boton.h -
  Created by
  Released into the public domain.
*/
#ifndef Boton_h
#define Boton_h

#include "Arduino.h"

class Boton
{
public:
  Boton();
  bool buttonPush();
  void set(int);

private:
  int _pin;
  int _pin_estado;
  int _pin_estado_instante_anterior;
  unsigned long _pin_timer;
  unsigned long _TIEMPO_BASE;
};

#endif
