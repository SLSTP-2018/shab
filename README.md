# shab

This repo contains all the hardware specifications, software, manuals, etc. for
the SLSTP 2018 High Altitude Balloon (SHAB) group project.

## Directory Structure

This list briefly summarizes the purpose of each folder in this repo to ease
navigation:

* Flight Software: Code related to performing the flight, e.g. Arduino code
* Hardware: Hardware designs, e.g. CAD files, PCB schematics, etc.
* Manuals: Operating and software manuals for all parts
* Reference Code: Any software from previous projects to reference in our work

## Flight Software

The code for our Arduino is completely contained within the [SHAB](flight_software)
folder. Using an Arduino IDE or Visual Studio, you can compile the folder into
a single file and upload it to an Arduino in a single step.

Our software performs the following protocol:

1. Initialize hardware:
    * Altimeter
    * Linear Actuators
    * SD card interface
    * Real-Time Clock (RTC)
    * Error LEDs
2. Perform an initial self-test:
    1. Turn on all LEDs for three seconds to ensure function
    2. Test for RTC response
        * Light <span style="color:blue">**blue**</span> LED on failure
    3. Test for SD card availability
        * Light <span style="color:green">**green**</span> LED on failure
    4. Write column headers for "datalog.txt" on SD card
    5. Test for altimeter connectivity
        * Light <span style="color:red">**red**</span> LED on failure
    6. Perform altimeter Cyclic Redundancy Check (CRC)
        * Light <span style="color:yellow">**yellow**</span> LED on failure
    7. Test troposphere linear actuator
        1. Flash all LEDs *three* times to indicate start of test
        2. Attempt to retract actuator for 30 seconds
        3. Extend actuator for 23 seconds
        4. Pause for one second
        5. Retract actuator for 30 seconds
    8. Test stratosphere linear actuator
        1. Flash all LEDs *four* times to indicate start of test
        2. Attempt to retract actuator for 30 seconds
        3. Extend actuator for 23 seconds
        4. Pause for one second
        5. Retract actuator for 30 seconds
    9. Turn off all LEDs
    10. Flash all LEDs 30 times to indicate completion of test sequence
3. Perform main loop of program indefinitely
    1. Obtain current Unix Time from RTC
    2. Calculate current altitude
        1. Read pressure and temperature from altitmeter
        2. Correct pressure for temperature
        3. Extrapolate altitude from known altitude-pressure table
    3. Perform altimeter CRC test to verify altimeter function
    4. Log the following to "datalog.txt":
        * Unix Time
        * Altitude (meters)
        * Pressure (Pascals)
        * Temperature (Celsius)
        * Troposphere arm currently extended (1/0 for True/False)
        * Stratosphere arm currently extended (1/0 for True/False)
        * Altimeter CRC test result (should be 11)
    5. If within troposphere sample range (3000-4000 meters),
       extend troposphere arm. Otherwise, retract it.
    6. If within stratosphere sample range (15000-16000 metres),
       extend stratosphere arm. Otherwise, retract it.

## Important Caveats

The L12-100-210-6-S Actuonix linear actuators we used do not report their
current position to the Arduino, nor do they report any data whatsoever.
These actuator simply extend upon receiving a 5V current and retract upon
receiving a -5V current. We would recommend using a different actuator
controller (the "-S" in the product code), but should you choose to use this
same model, be aware of some oddities in the code. The Arduio only estimates
whether or not the actuator is extended based on how long the actuators take
to extend and retract (~26 seconds under no load). Our actuator extends for
23 seconds to prevent it from falling off the rails of our specific setup.
You may need to adjust this number for your needs.

Furthermore, and *very importantly*, the Arduino will write that the
actuators were extended during the sampling altitudes regardless of actual
deployment since it simply cannot know if deployment was successful. Thus,
other means--such as a camera--should be used to confirm linear actuator
deployment.

## Licensing

All our own code and CAD designs are licensed as per the GPL V3 License provided
herein. All manuals, reference_code, and hardware are subject to the copyrights
of their owners and are provided in this repository for your convenience.
Additionally, the following libraries are provided under their original
license: `RTClib`, `MS%xxx`, and `MS5607Altimeter`.
