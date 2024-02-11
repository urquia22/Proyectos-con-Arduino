/* 
++++++++++++ Practica de Control de Control de Fase Triac con Timer1 Por Desbordamiento ++++++++++++++++++

                                     Materia Sistemas Digitales

                     Participantes:  
                                     Diana Carpio
                                     Angel Urquia

                     Fecha:          11-febrero-2024              
                                    
                     Correo:         urquia22@gmail.com

+++++++++++++++++++++++++                 Datos  Generales                      +++++++++++++++++++++++++++

  Tarjeta de Desarrollo :         Arduino Nano 328P       ||  PinPot       :        A1
  Temporizador          :         Timer 1                 ||  PinDisparo   :        9
  Modo de Temporizador  :         Por Desbordamiento      ||  Interrupción :        INT0 (Pin 2)
  Prescaler             :         8                       ||  

  Señal AC              :         110 V - 60 Hz           ||  Triac        :        BTA08 y BTA16
  Cruce Por Cero        :         Onda Completa y         ||  Bufer        :        74ls04
                                  Media Onda              ||  OptoTriac    :        MOC 3011
  Optoacoplador         :         PC817                   ||  Diodo        :        1N4007
  Periodo               :         16,66 ms
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

int pinPot = A1;                                      // Pin del potenciómetro
int pinDisparo = 9;                                   // Pin de salida que activaria la fase del Triac
unsigned long contador;                               // Variable para almacenar el valor del temporizador

void setup() {
  pinMode(pinDisparo, OUTPUT);                        // Configurar el pin 9 como salida
  attachInterrupt(0, Cx0, RISING);                    // Configurar la interrupción en el pin 2 (INT0) 
                                                      // por  flanco de subida (Cx0)

  TCCR1A = 0;                                         // Configurar el Timer1 en modo normal (sin PWM)-
                                                      // controla el modo de operación del Timer1

  TCCR1B = 0;                                         // Detener el temporizador, esto detiene el conteo del temporizador
                                                      // y se desactiva su funcionamiento

  TCCR1B |= (1 << CS11);                              // Seleccionamos el Prescaler 8, se establece el bit CS11 en 1
}

void loop() {                                         // ciclo repetitivo del codigo
  int ValorPot = analogRead(pinPot);                  // Leer el valor del potenciómetro y lo almacena en una variable int
  contador = map(ValorPot, 0, 1023, 64200, 49500);    // Mapear el valor del potenciómetro a un rango de temporización
}

void Cx0() {                                          // Funcion del cruce por cero (Cx0)
 
  TCNT1 = contador;                                   // Calcular el valor inicial del temporizador
  TIFR1 |= (1 << TOV1);                               // Borrar el flag de desbordamiento
  TIMSK1 |= (1 << TOIE1);                             // Habilitar la interrupción por desbordamiento del Timer1
  TCCR1B |=  (1<<CS11);                               // Seleccionamos el Prescaler 8, se establece el bit CS11 en 1 
}

ISR(TIMER1_OVF_vect) {                                // Rutina de servicio de interrupción (ISR)
  digitalWrite(pinDisparo, HIGH);                     // Escribe un 1 logico
  delayMicroseconds(50);                              // Delay 50 microsegundos
  digitalWrite(pinDisparo, LOW);                      // Escribe un o logico
  TCCR1B &= ~(1 << CS11);                             // Detener el temporizador, esto desactiva el prescaler del Timer1
                                                      // Hace una operacion AND a nivel de bits con el complemento de CS11 
                                                      // para borrar el bit correspondiente
}
