SEDS Audition — Avionics

Name: Dhanyatha
ID: 2026AAPS1125H

Task 1 — Finding the Sea Floor

I used Python with Pandas and Matplotlib to process and visualize the ship's depth sensor data.

The depth data was first read from the CSV file and converted into numerical values. Invalid readings such as #VALUE! were identified. Suspicious corrupted readings were inspected and replaced with missing values.

The missing values were then estimated using interpolation. A 5-point moving average was applied to reduce random sensor noise while preserving the overall shape of the depth data.

Finally, I created a depth-versus-time graph and animated it by adding one new data point every 1 second.

Original Sensor Data

Raw sensor data before cleaning.

![Original sensor data](Task1/screenshots/original_graph.png)

Cleaned and Smoothed Data

Corrupted readings were corrected using interpolation and a 5-point moving average was used to reduce noise.

![Cleaned and smoothed data](Task1/screenshots/cleaned_graph.png)

Final Animated Graph

![Animated graph](Task1/screenshots/animated_graph.png)

The cleaned and smoothed data was displayed progressively, with one new point added every second.




Task-2 Keeping Watch Over Odysseus

Building the Circuit

I built the required system in Tinkercad using an Arduino Uno, LDR, ultrasonic sensor, LCD, push button, LED, and buzzer. The sensors were used as inputs, while the LCD, LED, and buzzer provided the outputs. I tested the components individually before combining them.

Programming Approach

I first separated the system into inputs, outputs, states, and state transitions. This helped me define the behavior of each state before implementing the code and made it easier to handle the different sensor conditions without conflicting transitions.

I implemented a state machine with five states: OPEN SEA, ANCHOR DROPPED, STORM, CHARYBDIS, and WRECKED. The sensors are continuously checked and the state changes according to the required conditions.

I used millis() for the 5-second danger timers and LED blinking so the system could continue checking its sensors without long delays.

The push button toggles the anchor. While the anchor is dropped, the ship is protected from both dangers and the danger timer is reset. WRECKED remains permanent until the simulation is restarted.

Problems

The main issue was getting the push button to change the anchor state reliably. The state could change too quickly when the button was pressed and released, so I used a previous-button-state check to detect only a new press.

Testing

I tested the system step-by-step: Storm detection and LED, Charybdis detection and buzzer, both 5-second wrecking conditions, anchor protection from both dangers, and simultaneous Storm and Charybdis conditions. All major behaviors were successfully tested.

