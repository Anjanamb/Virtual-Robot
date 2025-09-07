# Webots Virtual Robot – EN2532 Robot Design Competition

>A **Webots-based virtual robot** designed for the **EN2532 Robot Design Competition**.  
>The robot is capable of **line following, wall following, gate navigation, box detection & catching, and color recognition** using simulated sensors and cameras.

---

## Features

- **Line Following** – IR sensors track and follow predefined lines on the arena floor.  
- **Wall Following** – Distance sensors help maintain a safe distance from arena boundaries.  
- **Gate Navigation** – The robot can detect and interact with gates controlled by a secondary controller.  
- **Box Catching with Arm Control** – Robotic arm motors pick and place boxes detected on the arena.  
- **Color Detection** – Dual cameras (front & bottom) identify arena markings and colored boxes.  

---

## Tech Stack

- **Simulation**: [Webots](https://cyberbotics.com/)  
- **Languages**: C++ (controllers), Webots world configuration  
- **Hardware (simulated)**:
  - Distance sensors & IR sensors  
  - Encoders & position sensors  
  - DC motors & robotic arm  
  - Dual cameras (front + bottom)  

---

## Repository Structure
```
Virtual-Robot/
├── controllers/
│ ├── Final/ # Main robot controller (C++ logic)
│ │ └── Final.cpp
│ └── gate_controller/ # Secondary controller for gate logic
│ └── gate_controller.cpp
│
├── worlds/ # Webots simulation worlds & objects
│ ├── arena.wbt
│ ├── gate.wbo
│ ├── robot.wbo
│ └── *.stl
│
├── Video/ # Demonstration videos
│ └── demo.mp4
│
└── README.md
```

---

## Getting Started

### 1. Clone the repository
```bash
git clone https://github.com/Anjanamb/Virtual-Robot.git
cd Virtual-Robot
```

### 2. Install Webots

Download and install the [latest Webots release](https://cyberbotics.com).
Ensure you can run Webots and compile controllers (C++ toolchain required).

### 3. Open the simulation

- Launch Webots.
- Open `worlds/arena.wbt`.

### 4. Run the robot

- Press Play in Webots.
- The robot will start executing the behavior defined in `controllers/Final/Final.cpp`.

---

## Usage

- Switching arenas – Open any `.wbt` file in the `worlds/` directory.
- Customizing behavior – Modify `Final.cpp` to adjust line-following thresholds, wall distances, or arm control.
- Gate logic – Controlled by `controllers/gate_controller/`.
- Debugging – Use Webots simulation console + 3D visualization for sensor feedback.

---

## Demo

A sample run of the robot can be found in the [Video/](./Video/) folder.
(You may also embed a GIF or YouTube link here for better visibility.)

---

## License

MIT License – feel free to use, modify, and share.

---

## Authors & Credits

- Anjana Bandara ([Anjanamb](https://github.com/Anjanamb/))
- Ayesh Rajakaruna ([Ayesh-Rajakaruna](https://github.com/Ayesh-Rajakaruna))

Special thanks to the EN2532 Robot Design Competition organizers and teammates.

---
