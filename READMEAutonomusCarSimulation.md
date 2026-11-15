# Autonomous Car Simulation

This project demonstrates a simple autonomous car control system using a finite state machine (FSM) written in C. The simulation cycles through driving states and responds to randomly generated sensor and system conditions.

## Features

- State-based control using an enum and a function-pointer table
- Simulated system startup and sensor checks
- Autonomous navigation, obstacle avoidance, and reversing at dead ends
- Responses to critical battery faults and low battery conditions
- Calibration and manual override modes
- Traffic-light, parking, and night-mode behavior
- Randomized input values on each simulation step

## States

The FSM includes idle, system check, navigation, obstacle avoidance, reverse, emergency, charging, calibration, manual override, traffic light, parking, and night mode states.

## Requirements

- A C compiler, such as GCC
- A command-line terminal

## Build and run

From the repository directory, compile the source file and run the resulting program:

```bash
gcc "Autonomus Car Simulation.c" -o autonomus_car_simulation
./autonomus_car_simulation
```

The program runs for 100 steps. At each step, it generates random values for the simulated start signal, sensors, battery, and driving conditions, then calls the handler for the current state.

> This is an educational FSM simulation. Its randomly generated inputs and simplified state handlers are not suitable for controlling a real vehicle.
