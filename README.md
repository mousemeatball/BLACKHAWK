# Thrust-Vector-Control
This repository contains the flight software and supporting systems developed by our high school rocketry team for competition use. Our project focuses on designing, building, and testing an actively controlled model rocket with advanced avionics and control systems.

The rocket integrates multiple subsystems, including custom-designed printed circuit boards (PCBs), thrust vector control (TVC), airbrake deployment, and real-time flight control algorithms. These systems work together to improve flight stability, trajectory accuracy, and overall performance during powered ascent and descent.

At the core of the software is a state machine that manages each phase of flight, from ignition to apogee and recovery. Control is achieved through PID loops that dynamically adjust the rocket’s orientation using the TVC system, while additional logic governs airbrake actuation to regulate altitude and velocity.

This project reflects a combination of mechanical design, embedded systems, and software engineering, developed collaboratively in preparation for competitive rocketry events.


#PID Control Loop
The PID control loop is responsible for adjusting the orientation of the motor mount based on the current orientation of the rocket and the desired orientation. The TVC system uses two separate PID control loops, one for the orientation in the x-direction and one for the orientation in the y-direction.

Each PID control loop has three components:

Proportional: The proportional component adjusts the orientation of the rocket based on the difference between the current orientation and the desired orientation.
Integral: The integral component adjusts the orientation of the rocket based on the cumulative error between the current orientation and the desired orientation.
Derivative: The derivative component adjusts the orientation of the rocket based on the rate of change of the error between the current orientation and the desired orientation.
By using two separate PID control loops, the TVC system is able to independently adjust the orientation of the rocket in both the x-direction and the y-direction, by using two servos which gimbal the motor mount.
