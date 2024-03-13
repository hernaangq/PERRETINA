# PERRETINA - Hernán García Quijano & Ángel Rodrigo Pérez Iglesias
 
Este es nuestro proyecto **PERRETINA** del curso 2022-2023 de la asignatura de Sistemas Digitales II. 

![foto1](https://lh3.googleusercontent.com/pw/AJFCJaUbV2pstc6I94YPashC6WMACJv6riVmZPGBhgOmsZkg-Q3ZTyrHig7mV8MMm2iA64zkd7gukXwXcRu61Zlos5oNgVyw5kYUJIXUIgpTTvmKvhpFYWw=w2400)

![foto2](https://lh3.googleusercontent.com/om7DVhZdZ-7e99xcDsF9r3ANLxoo6NIUjGKY47pwj1_91O13wr4oSGkRlyJsz0HMTB35HjiEwBCth1SthKdRYjdKkr4E0eStNAxGgt4_Z5w6gPsqM7RRsMwpTHlJyOMEeCjt883WbQ=w2400)

Este proyecto consigue transmitir y recibir tramas NEC con distintos comandos para cumplir diversas funcionalidades. 
El objetivo principal del proyecto es la transmisión y recepción de tramas NEC que muestren distintos colores RGB. En nuestro caso, mediante los conocimientos adquiridos a lo largo de la asignatura, hemos añadido las siguientes mejoras:
- Implementación de un buzzer que funciona mediante ondas PWM, al igual que el transmisor.
- Instalación de un fotorresistor para utilizarlo como un detector de luz. Cuando detecta que hay poca luz entra en el estado de emergencia.
- El núcleo va alimentado por una batería portátil de 5V y 2A.

Estas mejoras han sido añadidas con el objetivo de convertir el proyecto en algo más funcional que una placa a la que se le manden comandos. Hemos querido darle un sentido y hemos construido un arnés de perro que se controla mediante un mando de comandos infrarrojos. Las características del sistema han hecho que nos centremos exclusivamente en el modo de recepción de tramas NEC:
- Al igual que en el proyecto inicial, el LED RGB del sistema es capaz de emitir 8 colores RGB distintos en función del comando que se le mande (rojo, verde, azul, cyan, magenta, amarillo, blanco y negro/apagado).
- Cuando se pulsa el botón FADE del mando, el buzzer emite una melodía de duración aproximada de 1 minuto.
- Si se detecta que hay poca luz el dispositivo entrará en estado de emergencia, es decir, el buzzer emitirá un "DO" y el LED se quedará en el color blanco mientras dure la emergencia. En el momento que se vuelva a detectar una luz normal, el buzzer emitirá un "RE" y el LED una luz verde, indicando que ya está todo bien. Acto seguido se podrá volver a cambiar de color.
- Todo ello montado sobre un arnés con una batería externa que convierte nuestro proyecto en un sistema digital inalámbrico, que hace cobrar sentido práctico a los modos de bajo consumo añadimos en la última versión.

Para añadir estas mejoras, hemos tenido que modificar la máquina de estados principal: fsm_retina, y añadir una nueva máquina de estados para el fotorresistor: 

**Nuevo diagrama de estados del sistema RETINA:**
![fsm_retina](https://lh3.googleusercontent.com/Udi3_ZkyR1Do30hfkRrFtgCCvuD7rsJQtpDQaoGvN5AN8MvmftYa4FdO4cskREJ11nS1wv4IoJsxC2JnR6CjBWy483swtLkRj_DAgUl2QtJeagxNfd5O1-qoHiDPlNuMID26EpKy8w=w2400)

**Diagrama de estados del sensor de luz:**
![fsm_sensor](https://lh3.googleusercontent.com/yADnEf5m1xdEqufH7lk8JDDNgubMZ1FMDYopBoOQzBhpLYbEQaMIRCb9F3_qxf0K3s1OYS9aDWtHqzv5eG7tM0KUZrLGMs5p1nMh-D8NgLnkFWe8ET00m6Gs0_BMtG4BCoGML0_FRA=w2400)



Puedes acceder al vídeo del demostrador del proyecto pinchando en la imagen:


[![Demostrador del proyecto Retina](https://lh3.googleusercontent.com/UJ2BvT0R7w5Xqt0GvD2Z_x5curXcVZWYF7HjGHMAfaNtyc13NI0AmlLntinOBD3QDuwAGtM5R9_eIPlE8auZQCsKyoC4AW7WBYbXS1g_OmgJzTPchmwzVB1Bi0L1Rx1fw7MrdlhOWw=w2400)](https://youtu.be/M0baAnunEMg "Demostrador del proyecto Retina.")


Se adjunta también una captura en el osciloscopio del laboratorio de las tramas NEC transmitidas y detectadas por el sistema RETINA:
![Captura en el osciloscopio de las tramas NEC transmitidas y detectadas](https://lh3.googleusercontent.com/1y_RJ5e2mZAhHGmtKUDfxnjf7rosQYfcqTePLckxYoT-EMMoUiwKPe_UEizy6JbRhT16V4paQ9FMTEfEqcksWH4PLYCwA0J5xHdzh2J9J1hXSa4Iv5CHAqZ9VsRynUydppO1DYEw3w=w2400)


**Lista de materiales:**
- Nucleo-STM32F446RE - 1 BASE L1, L2, L3
- Cable USB. A Macho-B Mini Importante: cable de alimentación y datos
- Latiguillos macho-macho (Cables de conexión para protoboard)
- Latiguillos macho-hembra (Cables de conexión para protoboard)
- Protoboard Tamaño mínimo: 80x60 mm 
- Diodo emisor infrarrojos De inserción. (Encapsulado 5 mm. Longitud de onda
central: ∼ 940 − 050 nm. Potencia max: 40 mW e.g.:
17◦ visión)
- MOSFET puerta tipo N (Empaquetado: TO-92. Tensión max D-S: 40 − 60 V .
Corriente max: ∼ 0,5 A)
- Resistencia 33 Ω (De inserción. Tolerancia: ≤ 10 %. Potencia: 1/4 − 1/2 W)
- LED RGB cátodo común (De inserción. e.g 60º visión)
- Receptor de infrarrojos (frecuencia portadora 38kHz. Longitud de onda central: 950nm)
- Resistencia 150 Ω (De inserción. Tolerancia: ≤ 10 %. Potencia: 1/4 − 1/2 W)
- Resistencia 91 Ω (De inserción. Tolerancia: ≤ 10 %. Potencia: 1/4 − 1/2 W) x2
- Dispositivo Buzzer
- Fotorresistor
- Batería portátil (5V, 2A)
- Resistencia 10 kΩ (De inserción. Tolerancia: ≤ 10 %. Potencia: 1/4 − 1/2 W)
- Carcasa de plástico para el montaje.
- Arnés de perro pequeño


_Ningún animal ha sido herido en la realización de este proyecto._
![PERRETINA](https://lh3.googleusercontent.com/I0k8DUN_ZrI97RdYnjAMDkVELGWVhXaVzxooNWD94KIW9IT8RN0AWRB-J3dDQY4qkLverdWfDn0u_MMyH-d8g9w6XUJjTdA1W-Wm7f-agYLux1-iDf7UV_VWKLB9gEVWWH6S-fLOpQ=w2400)

**Información de contacto:**

**Hernán García Quijano:** <hernan.garcia.quijano@alumnos.upm.es>

**Ángel Rodrigo Pérez Iglesias:** <angelrodrigo.perez@alumnos.upm.es>



