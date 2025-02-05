# PistonPump

Simple and quick-built electronic control unit for a screw actuator moving a piston to pump a liquid in a physical experimental setup. The piston movement speed is roughly proportional to the DC  voltage applied to the motor, thus to the duty cycle of the motor driving signal. The speed, as a function of time is defined by table in microcontroller memory and interpolated using a digital filter. A default table is stored in ROM, and user defined one can be set over the USB interface, where a standard Modbus protocol is simulated. The unit itself is controlled by commands over the Modbus. An user or a superior system can use commands to "play" the defined function or move the piston any direction or speed, as well as brake it if needed. The STM32F103 microcontroller is used on cheap widely available board called ,,Blue Pill''.

**Version history**

| Version | Comment | Date |
| --- | --- | --- |
| VER-1.0.3 | Improved folder structure and documentation | 05.02.25 |
| VER-1.0.2 | Hardware setup for Visitors day | 26.01.25 |
| VER-1.0.1 | First full hardware test | 24.01.25 |

## Hardware

- STM32F103C8T6 board ,,Blue Pill''
- Full bridge motor driver HW-039 with two BTS7960 and level shifter

![HW-039 schematics](Documents/hw_039_schematics.png "The schematic diagram of the motor driver")

**The motor driver to MCU connection**

| Motor driver pin | MCU pin | MCU function | Comment |
| --- | --- | --- | --- |
| 1 | PA8 | TIM1 OC1 | Forward PWM |
| 2 | PA9 | TIM1 OC2 | Reverse PWM |
| 3 | PB4 |  | Forward Enable |
| 4 | PB5 |  | Reverse Enable |
| 5 | reserved | reserved | Forward Current Alarm |
| 6 | reserved | reserved | Reverse Current Alarm |
| 7 |  |  | Vcc +3.3 V |
| 8 |  |  | GND |

**Peripheral HW usage**

| Block | Usage |
| --- | --- |
| USB | Communication with PC, simulated Modbus |
| SYSTICK | Communication timeouts, indication |
| TIM1 | PWM generation for DC motor control, motor control timing |

**Interrupt usage and priorities**

| Interrupt | Priority |
| --- | --- |
| USB | 0 |
| Systick | 0 |
| TIM1 | 0 |

**Miscellaneous parameters**

- Core clock = 48 MHz


## Modbus communication

- The device is set to constant address **22** (0x16).
- Supported operations are MB_READ_HOLDING_REGS (3), MB_READ_INPUT_REGS (4) and MB_WRITE_MULTIPLE_REGS (16)

**Input registers**

| Address | Function |
| --- | --- |
| 0 | System status flag field (reserved) |
| 1 | System error flag field (reserved) |

**Holding registers**

| Address | Min | Max | Default | Function |
| --- | --- | --- | --- | --- |
| 0 |   |       |      | Command |
| 1 | 0 | 65535 | 1000 | Forward maximal time (number of PWM cycles) if started by command |
| 2 | 0 |  1000 | 1000 | Forward speed (PWM duty cycle) if started by command |
| 3 | 0 | 65535 | 1000 | Rewind maximal time (number of PWM cycles) if started by command |
| 4 | 0 |  1000 | 1000 | Rewind speed (PWM duty cycle) if started by command |
| 5 | 0 |    16 |   13 | Programmable wave repeat count, for command CMD_REPEAT |
| 6 | 0 | 65535 | 1100 | Programmable wave repeat period in milliseconds, for command CMD_REPEAT |

**Holding registers for programmable wave data**

Programmable wave table has 8 positions to define steps by motor speed and time of run. It occupied 16 registers from base address 100. Example of first step is shown here.

| Address | Min | Max | Function |
| --- | --- | --- | --- |
| 100 |  0    | 65535 | 1st step time |
| 101 | -1000 |  1000 | 1st step speed |

Step time means the number of PWM cycles. Speed is the PWM duty cycle, while the sign determines the direction, positive is forward, negative is rewind. Zero step time is interpreted as end of wave data. Changing direction for a short time after motor run can be used as a brake.

The default programmable wave function is this.

| Speed | Time |
| --- | --- |
| -1000 | 150 |
|   100 | 150 |
|     0 | 200 |
|  -300 | 150 |
|     0 | 150 |

**Commands**

| Command | Value | Comment |
| --- | --- | --- |
| CMD_STEP_FORWARD | 1 | Do forward step                                            |
| CMD_STEP_REWIND  | 2 | Do rewind step                                             |
| CMD_WAVE         | 3 | Do programmed wave                                         |
| CMD_CFG_SAVE     | 4 | Save the configuration to backup and reinitialize system   |
| CMD_CFG_LOAD     | 5 | Load the configuration from backup and reinitialize system |
| CMD_CFG_DEFAULT  | 6 | Restore configuration to default and reinitialize system   |
| CMD_STOP         | 7 | Emergency motor stop                                       |
| CMD_REPEAT       | 8 | Repeat programmable way by defined period and count        |

## DC Motor control

- PWM resolution is 12 bit (4096 steps from 0 to maximum voltage).
- PWM frequency is 1 kHz

## Additional SW

The project folder Python contains various scripts for testing firmware functionalities or control the machine directly, as well as a module for easy building new scripts.

## ToDo + Wishlist

- Modbus - test special conditions
- Utilize status and error registers
- Change keyboard event handling library to avoid need of superuser rights in Linux
- Support of hardware user interface controller

## References

- [Brief Modbus protocol description](https://www.modbustools.com/modbus.html)
- [Python Modbus module documentation](https://pymodbus.readthedocs.io)
- [Simple but useful Modbus master - terminal application](https://qmodbus.sourceforge.net)
