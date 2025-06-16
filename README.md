# 🐶 PERRETINA — Hernán García Quijano & Ángel Rodrigo Pérez Iglesias

This is our **PERRETINA** project developed for the **Digital Systems II** course during the 2022–2023 academic year.

<div align="center">
  <img src="https://lh3.googleusercontent.com/pw/AJFCJaUbV2pstc6I94YPashC6WMACJv6riVmZPGBhgOmsZkg-Q3ZTyrHig7mV8MMm2iA64zkd7gukXwXcRu61Zlos5oNgVyw5kYUJIXUIgpTTvmKvhpFYWw=w2400" alt="photo1" width="35%" />
  <img src="https://lh3.googleusercontent.com/om7DVhZdZ-7e99xcDsF9r3ANLxoo6NIUjGKY47pwj1_91O13wr4oSGkRlyJsz0HMTB35HjiEwBCth1SthKdRYjdKkr4E0eStNAxGgt4_Z5w6gPsqM7RRsMwpTHlJyOMEeCjt883WbQ=w2400" alt="photo2" width="35%" />
</div>


## 🧠 Project Summary

PERRETINA is an embedded system designed to **transmit and receive NEC frames** with different infrared commands to control system behavior. The main goal is to use NEC communication to control RGB lighting and implement smart features for real-world use.

We enhanced the base project with the following improvements:
- A **buzzer** using **PWM waves** (like the NEC transmitter).
- A **photoresistor** that detects ambient light. If it senses darkness, the system enters an **emergency state**.
- Powered by a **portable battery** (5V, 2A), making the system fully mobile.

These features were integrated into a **dog harness** controlled via an infrared remote, turning the project into a wireless, digital system with practical functionality.

---

## 🧩 Features

- The RGB LED can display **8 different colors** depending on the command received (red, green, blue, cyan, magenta, yellow, white, and off).
- Pressing the **FADE** button on the remote plays a **1-minute melody** through the buzzer.
- If low ambient light is detected:
  - The buzzer emits a “DO” sound.
  - The RGB LED stays white (emergency mode).
  - Once light is detected again, the buzzer emits a “RE” sound and the LED turns green.
- The system’s behavior is modeled using **state machines**.

---

## 🧮 FSM Diagrams

**Main RETINA state machine:**


<p align="center">
  <img src="https://lh3.googleusercontent.com/Udi3_ZkyR1Do30hfkRrFtgCCvuD7rsJQtpDQaoGvN5AN8MvmftYa4FdO4cskREJ11nS1wv4IoJsxC2JnR6CjBWy483swtLkRj_DAgUl2QtJeagxNfd5O1-qoHiDPlNuMID26EpKy8w=w2400" alt="fsm_retina" width="70%" />
</p>

**Light sensor state machine:**

<p align="center">
  <img src="https://lh3.googleusercontent.com/yADnEf5m1xdEqufH7lk8JDDNgubMZ1FMDYopBoOQzBhpLYbEQaMIRCb9F3_qxf0K3s1OYS9aDWtHqzv5eG7tM0KUZrLGMs5p1nMh-D8NgLnkFWe8ET00m6Gs0_BMtG4BCoGML0_FRA=w2400" alt="fsm_sensor" width="60%" />
</p>

---

## 🎥 Project Demonstration

Click the image to watch the video demo of the project:

<p align="center">
  <a href="https://youtu.be/M0baAnunEMg" title="PERRETINA demo">
    <img src="https://lh3.googleusercontent.com/UJ2BvT0R7w5Xqt0GvD2Z_x5curXcVZWYF7HjGHMAfaNtyc13NI0AmlLntinOBD3QDuwAGtM5R9_eIPlE8auZQCsKyoC4AW7WBYbXS1g_OmgJzTPchmwzVB1Bi0L1Rx1fw7MrdlhOWw=w2400" alt="PERRETINA demo video" width="50%" />
  </a>
</p>

---

## 📷 Oscilloscope Capture

Here is an oscilloscope capture of the NEC frames being transmitted and detected:

![oscilloscope](https://lh3.googleusercontent.com/1y_RJ5e2mZAhHGmtKUDfxnjf7rosQYfcqTePLckxYoT-EMMoUiwKPe_UEizy6JbRhT16V4paQ9FMTEfEqcksWH4PLYCwA0J5xHdzh2J9J1hXSa4Iv5CHAqZ9VsRynUydppO1DYEw3w=w2400)

---

## 🧰 Components Used

- Nucleo-STM32F446RE (BASE L1, L2, L3)
- USB cable (A Male to B Mini, with power & data)
- Jumper wires (male-male and male-female)
- Breadboard (minimum size: 80x60 mm)
- IR emitter diode (5mm, ~940–1050nm, 40mW, 17° beam)
- N-type MOSFET (TO-92, 40–60V, ~0.5A)
- 33 Ω resistor
- RGB common cathode LED
- IR receiver (38kHz, 950nm)
- 150 Ω resistor
- 91 Ω resistor ×2
- Buzzer
- Photoresistor
- Portable battery (5V, 2A)
- 10 kΩ resistor
- Plastic casing
- Small dog harness

---

**No animals were harmed during the making of this project.**  
<p align="center">
  <img src="https://lh3.googleusercontent.com/I0k8DUN_ZrI97RdYnjAMDkVELGWVhXaVzxooNWD94KIW9IT8RN0AWRB-J3dDQY4qkLverdWfDn0u_MMyH-d8g9w6XUJjTdA1W-Wm7f-agYLux1-iDf7UV_VWKLB9gEVWWH6S-fLOpQ=w2400" alt="PERRETINA" width="40%" />
</p>


---

## 📬 Contact

**Hernán García Quijano** — <hernan.garcia.quijano@alumnos.upm.es>  
**Ángel Rodrigo Pérez Iglesias** — <angelrodrigo.perez@alumnos.upm.es>


