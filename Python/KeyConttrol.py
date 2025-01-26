#!/usr/bin/env python
# -*- coding: utf8 -*-

"""
User friendly control by pressing keys.
"""

__author__ = "Richard Linhart"
__copyright__ = ""
__credits__ = [""]
__license__ = "GPL"
__version__ = "1.0.0"
__maintainer__ = "Richard Linhart"
__email__ = "OK1CTR@gmail.com"
__status__ = "Development"

from Common import *
from MbPistonPump import *
import sys
import keyboard


if __name__ == '__main__':
    pp = MbPistonPump()
    pp.open()
    pulse_speed = 500

    while True:
        a = keyboard.read_key()
        if a == 't':  # Send wave
            pp.command(CMD_WAVE)
            continue

        elif a =='r':  # repeated wave
            pp.command(CMD_REPEAT)
            continue

        elif a == 'p':  # pulse forward
            pp.set(REG_FORWARD, [500, pulse_speed])  # time, speed
            pp.command(CMD_STEP_FORWARD)
            continue

        elif a =='o':  # pulse back
            pp.set(REG_REWIND, [500, pulse_speed])  # time, speed
            pp.command(CMD_STEP_REWIND)
            continue

        elif a =='u':  # return after repeat
            pp.set(REG_FORWARD, [2500, 1000])  # time, speed
            pp.command(CMD_STEP_FORWARD)
            continue

        elif a =='x':  # emergency stop
            pp.command(CMD_STOP)
            continue

        elif a =='b':  # set slow
            pulse_speed = 300
            continue

        elif a =='n':  # set medium
            pulse_speed = 500
            continue

        elif a =='m':  # set fast
            pulse_speed = 1000
            continue

        elif a == 'q':  # Quit
            break

    print('Finished')
    sys.exit(0)
