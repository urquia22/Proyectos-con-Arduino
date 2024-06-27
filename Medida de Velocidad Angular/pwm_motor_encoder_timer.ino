/* 
+++++ Práctica de Medición de Velocidad Angular Con Encoder Optico e Interrupciones de TIMER1 Arduino ++++++
++++++++++++++++++++++++++++++++++++++++PWM por medio de Timer 2++++++++++++++++++++++++++++++++++++++++++++

                                     Materia Sistemas Digitales

                     Participantes:  
                                     Diana Carpio
                                     Angel Urquia

                     Fecha:          21-Marzo-2024              
                                    
                     Correo:         urquia22@gmail.com

+++++++++++++++++++++++++                 Datos  Generales                      +++++++++++++++++++++++++++

  Tarjeta de Desarrollo :         Arduino Nano 328P       ||  PinPot       :        A0
  Temporizador          :         Timer 1                 ||  PWM_PIN = 11 :        11
  PWM                   :         Timer 2                 ||  Interrupción :        INT0 (Pin 2)
  Prescaler    Timer 1  :         1024                    ||  

  Prescaler    Timer 2  :         128 - Variable          ||  
  Encoder               :         36    Orificios         ||  
  Optoacoplador         :         Ranurado                || 
  
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
//Timer 0: Los pines asociados para las salidas PWM son el pin 5 (OC0B) y el pin 6 (OC0A).
//Timer 1: Los pines asociados para las salidas PWM son el pin 9 (OC1A) y el pin 10 (OC1B).
//Timer 2: Los pines asociados para las salidas PWM son el pin 11 (OC2A) y el pin 3 (OC2B).

#include <Wire.h>                                                         
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);

float contador = 0;                                                 // Variable para contar las interrupciones
volatile float fq1 = 0.00;
volatile float fq  = 0.00;

const byte POT = A0;
const byte PWM_PIN = 11;                                             // Pin PWM de salida

int frequency = 500;                                                 // Frecuencia inicial en Hz
int prescalerSetting = 0;                                            // Valor inicial del preescaler

void setup() {
Serial.begin(9600);                                                  // Inicia la comunicación serial a 9600 baudios
attachInterrupt(0, interrupcion0, RISING);                           // Adjunta una interrupción al pin 2 que llama a la función 'interrupcion0' en flanco de subida
lcd.init();                                                          // Inicializa el objeto lcd

lcd.backlight();                                                     // Enciende la retroiluminación del LCD
lcd.setCursor(1, 0);                                                 // Establece el cursor en la posición (1, 0) del LCD
lcd.print("UNIVERSIDAD CENTRAL");                                    // Imprime el texto "UNIVERSIDAD CENTRAL"
lcd.setCursor(6, 1);                                                 // Establece el cursor en la posición (6, 1) del LCD
lcd.print("VENEZUELA");                                              // Imprime el texto "VENEZUELA"
delay(3000);                                                         // Espera 3 segundos

lcd.setCursor(0, 0);                                                 // Establece el cursor en la posición (0, 0) del LCD
lcd.print("                    ");                                   // Borra la línea 0 del LCD
lcd.setCursor(0, 1);                                                 // Establece el cursor en la posición (0, 1) del LCD
lcd.print("                   ");                                    // Borra la línea 1 del LCD

lcd.setCursor(3, 1);                                                 // Establece el cursor en la posición (3, 1) del LCD
lcd.print("Facultad de Csc");                                        // Imprime el texto "Facultad de Csc"
lcd.setCursor(0, 2);                                                 // Establece el cursor en la posición (0, 2) del LCD
lcd.print("     Postgrado");                                         // Imprime el texto "     Postgrado"
lcd.setCursor(0, 3);                                                 // Establece el cursor en la posición (0, 3) del LCD
lcd.print("  Instrumentacion");                                      // Imprime el texto "  Instrumentacion"
delay(3000);                                                         // Espera 3 segundos

lcd.clear();                                                         // Limpia toda la pantalla del LCD

lcd.setCursor(1, 0);                                                 // Establece el cursor en la posición (1, 0) del LCD
lcd.print("Por: ");                                                  // Imprime el texto "Por: "
lcd.setCursor(6, 1);                                                 // Establece el cursor en la posición (6, 1) del LCD
lcd.print("Angel Urquia");                                           // Imprime el texto "Angel Urquia"
lcd.setCursor(6, 2);                                                 // Establece el cursor en la posición (6, 2) del LCD
lcd.print("Diana Carpio");                                           // Imprime el texto "Diana Carpio"
delay(6000);                                                         // Espera 6 segundos
lcd.clear();                                                         // Limpia toda la pantalla del LCD

                                                                     // timer1 
  TCCR1A = 0;                                                        // El registro de control A queda todo en 0, pines OC1A y OC1B deshabilitados
  TCCR1B = 0;                                                        // limpia el registrador
  TCCR1B |= (1<<CS10)|(1 << CS12);                                   // configura prescaler para 1024: CS12 = 1 e CS10 = 1

  TCNT1 = 49912;                                                     // inicia timer para desbordamiento 1 segundo
                                                                     // 65536-(16MHz/1024/1Hz - 1) = 49912 = 0xC2F8
  TIMSK1 |= (1 << TOIE1);                                            // habilita la interrupcion del TIMER1
//#################################################################################################################

  pinMode(PWM_PIN, OUTPUT);                                          // Configurar el pin PWM como salida
  
  // Configurar Timer 2 para PWM rápido, sin preescaler
  TCCR2A = _BV(COM2A1) | _BV(WGM20) | _BV(WGM21);                    // Modo Fast PWM
  TCCR2B = prescalerSetting;                                         // Establecer el preescaler inicial
}

void loop() {
  int potValue = analogRead(POT);                                    // Leer el valor del potenciómetro (0-1023)
  int dutyCycle = map(potValue, 0, 1023, 0, 255);                    // Mapear el valor del potenciómetro al ciclo de trabajo (0-255)

// Ajustar la frecuencia cambiando el preescaler del Timer 2
// frequency = map(analogRead(A1), 0, 1023, 1, 1000);                // Leer la frecuencia deseada desde otro potenciómetro
  
  if (frequency >= 31250) {
    prescalerSetting = _BV(CS20);                                    // Sin preescaler
  } else if (frequency >= 3906) { 
    prescalerSetting = _BV(CS21);                                    // Preescaler de 8
  } else if (frequency >= 488) {
    prescalerSetting = _BV(CS21) | _BV(CS20);                        // Preescaler de 32
  } else if (frequency >= 122) {
    prescalerSetting = _BV(CS22);                                    // Preescaler de 64
  } else {
    prescalerSetting = _BV(CS22) | _BV(CS20);                        // Preescaler de 128
  }
  
  TCCR2B = prescalerSetting;                                         // Establecer el preescaler para la frecuencia deseada
  
  OCR2A = dutyCycle;                                                 // Establecer el ciclo de trabajo para el Timer 2 (OC2A)
  
  delay(10);                                                         // Pequeña pausa para estabilizar la lectura del potenciómetro

  lcd.backlight();                                                   // Enciende la retroiluminación del LCD
  lcd.setCursor(0, 0);                                               // Establece el cursor en la posición (0, 0) del LCD
  lcd.print("Medida Vel-Ang (W)");                                   // Imprime el texto "Medida Vel-Ang (W)" en la línea 0
  lcd.setCursor(0, 1);                                               // Establece el cursor en la posición (0, 1) del LCD
  lcd.print("RPS: ");                                                // Imprime el texto "RPS: " en la línea 1
  lcd.setCursor(5, 1);                                               // Establece el cursor en la posición (5, 1) del LCD
  lcd.print(fq1);                                                    // Imprime el valor de la variable 'fq1' en la línea 1
  lcd.print("    ");                                                 // Imprime espacios adicionales en la línea 1 para borrar cualquier contenido previo

  lcd.setCursor(0, 2);                                               // Establece el cursor en la posición (0, 2) del LCD
  lcd.print("RPM: ");                                                // Imprime el texto "RPM: " en la línea 2
  lcd.setCursor(5, 2);                                               // Establece el cursor en la posición (5, 2) del LCD
  lcd.print(fq);                                                     // Imprime el valor de la variable 'fq' en la línea 2
  lcd.print("    ");                                                 // Imprime espacios adicionales en la línea 2 para borrar cualquier contenido previo

  lcd.setCursor(0, 3);                                               // Establece el cursor en la posición (0, 3) del LCD
  lcd.print("PWM: ");                                                // Imprime el texto "PWM: " en la línea 3
  lcd.setCursor(5, 3);                                               // Establece el cursor en la posición (5, 3) del LCD
  lcd.print(OCR2A);                                                  // Imprime el valor de la variable 'OCR2A' en la línea 3
  lcd.print("    ");                                                 // Imprime espacios adicionales en la línea 3 para borrar cualquier contenido previo

  lcd.setCursor(10, 3);                                              // Establece el cursor en la posición (10, 3) del LCD
  lcd.print("POT: ");                                                // Imprime el texto "POT: " en la línea 3 (continuación)
  lcd.setCursor(15, 3);                                              // Establece el cursor en la posición (15, 3) del LCD
  lcd.print(potValue);                                               // Imprime el valor de la variable 'potValue' en la línea 3 (continuación)
  lcd.print("    ");                                                 // Imprime espacios adicionales en la línea 3 para borrar cualquier contenido previo

}


ISR(TIMER1_OVF_vect)                                                  //interrupcion del TIMER1 
{
  TCNT1 = 49912;                                                      // Renicia TIMER
  fq1 = contador * 1/36;                                              // Calcular la frecuencia en revoluciones por minuto
  fq = fq1*60;                                                        // Calcular la frecuencia en revoluciones por minuto
  Serial.print("Frecuencia:    ");
  Serial.print(fq1);                                                  // Imprimir la frecuencia
  Serial.print(" RPS           ");                                    // Imprimir la unidad de medida
  Serial.print(" RPS x (60 s / 1 min)    =    ");                     // Imprimir la unidad de medida
  Serial.print(fq);                                                   // Imprimir la frecuencia
  Serial.print(" RPM");                                               // Imprimir la unidad de medida
  Serial.print("      PWM  =   ");                                    // Imprimir la unidad de medida
  Serial.println(OCR2A);                                              // Imprimir la unidad de medida
 
  contador = 0;                                                       // Reiniciar el contador
}

void interrupcion0()                                                  // Funcion que se ejecuta durante cada interrupion
{
  contador++;                                                         // Se incrementa en uno el contador
}
