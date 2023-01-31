Cellulo V2

The actual version of Cellulo (V2) is born from two core needs: Adapt the previous version to correct flaws and errors discovered over its use and seek to expand the functionalities to adapt to the necessities of the users.

## Table of Content

* [Motivation and goals](#motivation-and-goals)
* [Lessons learned from the previous version](#lessons-learned-from-the-previous-version)
* [Mechanics](#mechanics)
* [Electronics](#electronics)
* [Firmware](#firmware)
* [Next steps](#nextsteps)

## Motivation and goals
Cellulo is a project born in 2013 as an educational tool and the digital and autonomous pen of the classrooms. After constant use by the researchers and users, it was possible to point out weaknesses in the design. Additionally, the advance in technology has opened new capabilities for the robot.
The goals of the new redesign are to solve the flaws spotted, improve the functionalities of the robot and increase its capacities.
The strategy to achieve those goals is based on the following factors: Usability, maintainability, cost, and scalability.

## Lessons learned from the previous version.

The old Cellulo had two core flaws spotted; battery capacities, mainly caused for the lack of on/off buttons, and locomotion maintenance. Additionally, it was learned that the users would appreciate having versatility in the way to interact with the robots, whether it is from a mechanical perspective or from the electronic one, which led us to the introduction of modularity. These flaws, as well as other lessons learned, are dissected in the following sections.

To iterate in the design, the following factors were considered:

* Easiness to produce
* Easiness to mount
* Easiness to repair
* Scalability
* Costs
* Usability
* Modularity
* Aesthetics

A schematic of the V2 is displayed below.

<p align="center">
  <a href="url"><img src="https://github.com/chili-epfl/CelluloV2/blob/614d37fcd87cc5d7110fd84945640ce23725eaef/Media/Cellulo%20V2%20Exploited%20view.png" align="center" height="700" width="400" ></a>

## Mechanics

The design of the new mechanics was mainly driven by locomotion and modularity. The core changes are the following
* Introduction of 3 modules; Main module with locomotion and core electronics, battery module, and User interface module. 
* The connections between the modules are done mechanically through a rail and assisted by magnets to provide an additional feeling of connection plus robustness.
* 2 Versions of locomotion are designed. Magnetic drive and Omniwheels.
* Omniwheels are in-house designed and tested.

Details about the mechanics can be found in the [Wiki](https://github.com/chili-epfl/cellulo/wiki/Cellulo-V2-Mechanics) and the files in the [Git](https://github.com/chili-epfl/CelluloV2/tree/main/Mechanics) 

## Electronics

The change in electronics was driven by the new technologies and parts accessible in the market, as well as to adequate to the new features that wanted to be implemented. However, during the development of the current version, a big shortage of electronic parts reduced the possibilities of development as well as delayed and rerouted the initial design. The core changes are the following:

* Introduction of ESP32 as a communication module and an auxiliary MCU to control peripheries.
* Introduction of modularity in the PCBs, Core PCB with MCUs camera and big components, driver PCB, battery PCB and finally when necessary, the UI electronics.
* Use of Pogo connectors to connect the different modules.
* Changes in components, especially in the battery management module.
* New Led system and design, as well as touch sensors.

More details on the electronics in the [Wiki](https://github.com/chili-epfl/cellulo/wiki/Cellulo-V2-Electronics) and access to the files in the [Git](https://github.com/chili-epfl/CelluloV2/tree/main/Electronics). The wiki describes the lessons learned in the current development and a suggestion for future steps.

## Firmware

Due to the limitations in electronic availability, the core MCU used was the same as the previous version, and hence, the firmware is really similar with some changes. The description of it is detailed in the [Wiki](https://github.com/chili-epfl/cellulo/wiki/Cellulo-V2-Firmware) and access to the code is in the [Git](https://github.com/chili-epfl/CelluloV2/tree/main/Firmware) repository.


## Next Steps





