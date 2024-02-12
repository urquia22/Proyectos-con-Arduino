//+++++++++++++++++++++++ Practica de Control de Motor Paso a Paso +++++++++++++++++++++++++++
//                        
//                             Materia Sistemas Digitales
//
//                      Participantes:
//                                      Diana Carpio
//                                      Angel Urquia
// 
//+++++++++++++++++++++ Modelo: Motor PM55L-048-HP07_02277-60100 NMB +++++++++++++++++++++++++
//   Datos del Fabricante:                        Tarjeta de Desarrollo: ESP32
//     7.5 grados por paso                        Driver : Modulo ULN2003
//     Voltaje nominal      :24 V
//     Resistencia (Ohmios) :22 
//     Pasos / Revolución   :48
//     Peso en gramos       :275                                        
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

                              
const int retapas = 17;     // declaramos la variable "retapas" que sera el tiempo o periodo cada paso.
const int IN1_A   = 2;      // entrada "A" bobina 1, cable color verde.
const int IN2_B   = 4;      // entrada "B" bobina 2, cable color rojo.
const int IN3_C   = 16;     // entrada "C" bobina 1, cable color amarillo.
const int IN4_D   = 17;     // entrada "D" bobina 2, cable color marron.


void setup(){          // Configuraciones
  
pinMode(IN1_A,OUTPUT);     // configuramos el pin (2)  que va a la IN1_A como salida.

pinMode(IN2_B,OUTPUT);     // configuramos el pin (4)  que va a la IN2_B como salida.

pinMode(IN3_C,OUTPUT);     // configuramos el pin (16) que va a la IN2_C como salida.

pinMode(IN4_D,OUTPUT);     // configuramos el pin (17) que va a la IN4_D como salida.
 
}

void loop(){                        // ciclo repetitivo ( inicio del programa principal)
  for (int i=0; i<12;i++){          // clico for para repetir la función "antihorario()" 12 veces.
  antihorario();                    // llamada a la función "antihorario()"
 
  }                                 // fin del ciclo for para giro antihorario.
  
  delay(500);                       // luego de dar una revolución o vuelta completa espera un retardo de 500 ms para iniciar pasos en sentido contrario.
  
  for (int i=0; i<12;i++){          // clico for para repetir la función "horario()" 12 veces.
  horario();                        // llamada a la función "horario()"
  
  }                                 // fin del ciclo for para giro sentido horario.
  
  delay(500);                       // luego de dar una revolución o vuelta completa espera un retardo de 500 ms
 } 


void antihorario(){                // función para giro sentido antihorario.

  digitalWrite(IN1_A, HIGH);       // escribo el nivel logico alto en el pin(2) IN1_A que es la entrada al canal 1 driver uln2003
  delay(retapas);                  // retardo definido en la declaración de variables, es de 17 ms.
  digitalWrite(IN1_A, LOW);        // escribo el nivel logico bajo en el pin(2) IN1_A que es la entrada al canal 1 driver uln2003
  
  digitalWrite(IN2_B, HIGH);       // escribo el nivel logico alto en el pin(4) IN2_B que es la entrada al canal 2 driver uln2003
  delay(retapas);
  digitalWrite(IN2_B, LOW);        // escribo el nivel logico bajo en el pin(4) IN2_B que es la entrada canal 2 al driver uln2003

  digitalWrite(IN3_C, HIGH);       // escribo el nivel logico alto en el pin(16) IN3_C que es la entrada canal 3 al driver uln2003
  delay(retapas);
  digitalWrite(IN3_C, LOW);        // escribo el nivel logico bajo en el pin(16) IN3_C que es la entrada canal 3 al driver uln2003

  digitalWrite(IN4_D, HIGH);       // escribo el nivel logico alto en el pin(17) IN4_D que es la entrada canal 4 al driver uln2003
  delay(retapas);
  digitalWrite(IN4_D, LOW);        // escribo el nivel logico bajo en el pin(17) IN4_D que es la entrada canal 4 al driver uln2003

}


void horario(){                    // función para giro sentido horario

  digitalWrite(IN4_D, HIGH);       // escribo el nivel logico alto en el pin(17) IN4_D que es la entrada canal 4 al driver uln2003
  delay(retapas);                  // retardo definido en la declaración de variables, es de 17 ms.
  digitalWrite(IN4_D, LOW);        // escribo el nivel logico bajo en el pin(17) IN4_D que es la entrada canal 4 al driver uln2003

  digitalWrite(IN3_C, HIGH);       // escribo el nivel logico alto en el pin(16) IN3_C que es la entrada canal 3 al driver uln2003
  delay(retapas);
  digitalWrite(IN3_C, LOW);        // escribo el nivel logico bajo en el pin(16) IN3_C que es la entrada canal 3 al driver uln2003
  
  digitalWrite(IN2_B, HIGH);       // escribo el nivel logico alto en el pin(4) IN2_B que es la entrada al canal 2 driver uln2003
  delay(retapas);
  digitalWrite(IN2_B, LOW);        // escribo el nivel logico bajo en el pin(4) IN2_B que es la entrada canal 2 al driver uln2003


  digitalWrite(IN1_A, HIGH);       // escribo el nivel logico alto en el pin(2) IN1_A que es la entrada al canal 1 driver uln2003
  delay(retapas);
  digitalWrite(IN1_A, LOW);        // escribo el nivel logico bajo en el pin(2) IN1_A que es la entrada al canal 1 driver uln2003

}

 
