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

![Original sensor data](Task%201/screenshots/original_graph.png)

Cleaned and Smoothed Data

Corrupted readings were corrected using interpolation and a 5-point moving average was used to reduce noise.

![Cleaned and smoothed data](Task%201/screenshots/cleaned_graph.png)

Final Animated Graph

![Animated graph](Task%201/screenshots/animated_final_graph.png)

The cleaned and smoothed data was displayed progressively, with one new point added every second.


## Task 2 — Keeping Watch Over Odysseus

### Building the Circuit

I first built the circuit in Tinkercad using an Arduino Uno and the required components: an LDR, HC-SR04 ultrasonic sensor, 16×2 LCD, push button, LED, and buzzer.

The LDR was connected to an analog input so that the Arduino could measure the light level. The ultrasonic sensor used trigger and echo pins to measure the distance of nearby objects. The push button was connected using the Arduino's internal pull-up resistor, making it easier to detect when the button was pressed.

The LCD was used to display the current state of the ship. The LED acts as a visual warning during a storm, while the buzzer indicates that Charybdis has been detected. I tested the individual components first and then combined them into the complete circuit.

![Tinkercad wiring](Task%202/screenshots/wiring.png)

### Problem-Solving Approach

I approached the problem by first separating it into **inputs, outputs, states, and transitions**.

* **Inputs:** LDR, ultrasonic sensor, and push button
* **Outputs:** LCD, LED, and buzzer
* **States:** `OPEN SEA`, `ANCHOR DROPPED`, `STORM`, `CHARYBDIS`, and `WRECKED`
* **Transitions:** determined by sensor readings, the anchor button, and the 5-second timer

I then defined what should happen in each state before writing the code. This helped prevent different sensor conditions from interfering with each other.

### State Machine Logic

The system starts in `OPEN SEA`. While in this state, the Arduino continuously checks both sensors.

If the LDR reading falls below half of its range, the system enters `STORM`. The LED blinks while the storm continues. If the light level returns to normal before 5 seconds, the ship returns to `OPEN SEA`.

If the ultrasonic sensor detects an object closer than 100 cm, the system enters `CHARYBDIS`. The buzzer remains on while the ship is near the obstacle. If the ship moves away before 5 seconds, it returns to `OPEN SEA`.

If both dangers occur at the same time, whichever state is entered first remains active. The other sensor does not interrupt the current danger state.

### Anchor Logic

The push button toggles the anchor between dropped and raised. I used a previous-button-state check so that one press results in only one toggle.

When the anchor is dropped, the state becomes `ANCHOR DROPPED` and the danger timer is reset. This protects the ship from both Storm and Charybdis. Pressing the button again raises the anchor and returns the ship to `OPEN SEA`.

### Timing and Wrecked State

I used `millis()` instead of a long `delay()` for the 5-second danger timer. This allows the Arduino to continue running the rest of the system while timing how long the danger has lasted.

If either `STORM` or `CHARYBDIS` continues for 5 seconds without the anchor being dropped, the state changes to `WRECKED`. The `WRECKED` state is permanent and the program stops checking for further state changes until the simulation is restarted.

### Problems Encountered

The main problem during development was the push button changing states too quickly. A direct reading of the button could cause repeated changes while the button was being pressed. I solved this by comparing the current button reading with the previous reading and only detecting a new press when the button changes from unpressed to pressed.

I also had to ensure that the 5-second timer was reset whenever a danger ended or the anchor was dropped. This was important to prevent an old timer from causing the ship to become wrecked incorrectly.

### Testing

I tested the system progressively rather than testing everything at once:

1. Tested the LDR and adjusted the light level to trigger `STORM`.
2. Verified that the LED blinks during a storm.
3. Verified that the ship returns to `OPEN SEA` when the storm ends.
4. Tested the 5-second Storm → `WRECKED` transition.
5. Tested the ultrasonic sensor using different distances.
6. Verified that the buzzer activates during `CHARYBDIS`.
7. Tested the 5-second Charybdis → `WRECKED` transition.
8. Tested the anchor button and confirmed that pressing it toggles between `ANCHOR DROPPED` and `OPEN SEA`.
9. Tested dropping the anchor during both dangers to confirm that it protects the ship and resets the timer.
10. Tested simultaneous Storm and Charybdis conditions to confirm that the first state entered remains active.

The final circuit and program were tested successfully in Tinkercad, with the required state transitions, sensor responses, outputs, anchor protection, and wrecking conditions working as intended.
