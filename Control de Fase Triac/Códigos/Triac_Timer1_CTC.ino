/* 
+++++++++++ Practica de Control de Control de Fase Triac con Timer1 en Modo CTC en Comparación ++++++++++++

                                     Materia Sistemas Digitales

                     Participantes:  
                                     Diana Carpio
                                     Angel Urquia

                     Fecha:          11-febrero-2024              
                                    
                     Correo:         urquia22@gmail.com

+++++++++++++++++++++++++                 Datos  Generales                      +++++++++++++++++++++++++++

  Tarjeta de Desarrollo :         Arduino Nano 328P       ||  PinPot       :        A1
  Temporizador          :         Timer 1                 ||  PinDisparo   :        9
  Modo de Temporizador  :         CTC  Comparación        ||  Interrupción :        INT0 (Pin 2)
  Prescaler             :         8                       ||  

  Señal AC              :         110 V - 60 Hz           ||  Triac        :        BTA08 y BTA16
  Cruce Por Cero        :         Onda Completa y         ||  Bufer        :        74ls04
                                  Media Onda              ||  OptoTriac    :        MOC 3011
  Optoacoplador         :         PC817                   ||  Diodo        :        1N4007
  Periodo               :         16,66 ms
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

const int triacPin = 9;                                     // Pin para control del triac
const int potentiometerPin = A1;                            // Pin para el potenciómetro
volatile int OCR1A_Valor = 0;                               // Valor de OCR1A para la temporización

void setup() {                                              // Conmfiguración

  attachInterrupt(0, Cx0, RISING);                          // Configurar la interrupción por flanco de subida
  pinMode(triacPin, OUTPUT);                                // Pin de Disparo
  Configuro_Timer1();                                       // Funcion de configuracion del Timer1
}

void loop() {
  int pot_Valor = analogRead(potentiometerPin);             // Leemos el valor del potenciómetro
  OCR1A_Valor = map(pot_Valor, 0, 1023, 500, 15000);        // Mapear el valor del potenciómetro al rango de temporización 
}

void Cx0() {
  OCR1A = OCR1A_Valor;                                      // Asignamos Valor a OCR1A para la temporización
  TIFR1 = _BV(OCF1A);                                       // Borrar la bandera de comparación del temporizador
  TCCR1B |= (1 << WGM12);                                   // Activar el modo CTC
  TCCR1B |= (1 << CS11);                                    // Configurar el prescaler en 8
  TIMSK1 |= (1 << OCIE1A);                                  // Habilitar la interrupción por comparación
}
void Configuro_Timer1() {                                   // Funcion para configural el Timer1
  cli();                                                    // Deshabilitar interrupciones                     
  TCCR1A = 0;                                               // Configurar el temporizador en modo CTC
  TCCR1B = 0;                                               // Resetea el temporizador
  TCNT1 = 0;                                                // Reiniciar el contador del temporizador
  TCCR1B |= (1 << WGM12);                                   // Activar el modo CTC
  TCCR1B |= (1 << CS11);                                    // Configurar el prescaler en 8
  TIMSK1 |= (1 << OCIE1A);                                  // Habilitar la interrupción por comparación
  sei();                                                    // Habilitar interrupciones
}

ISR(TIMER1_COMPA_vect) {
  digitalWrite(triacPin, HIGH);                             // Activa el pin del triac
  delayMicroseconds(20);                                    // Esperar 20 us
  digitalWrite(triacPin, LOW);                              // Desactiva el pin del triac
  TCCR1B &= ~(1 << CS11);                                   // Detener el temporizador
}
