# shab

This repo contains all the hardware specifications, software, manuals, etc. for
the SLSTP 2018 High Altitude Balloon group project.

## Directory Structure

This list briefly summarizes the purpose of each folder in this repo to ease
navigation:

* 16S: Code and workflows for analyzing 16S data
* Black Carbon Telelmetry: Code relating to combining our various data streams
                           and analyzing ABCD data
* Flight: Code related to performing the flight, e.g. Arduino code
* Hardware: Hardware designs, e.g. CAD files, PCB schematics, etc.
* Manuals: Operating and software manuals for all parts
* Reference Code: Any software from previous projects to reference in our work

## Flight Software

The code for our Arduino is completely contained within the [SHAB](flight/SHAB)
folder. The [build](flight/build) folder contains a pre-compiled build of
[SHAB](flight/SHAB) for reducing build verification time by Visual Studio Code.

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
