#!/usr/bin/env python
# -*- coding: utf8 -*-

"""
Test of register settings and simple pulses forward and back
"""

__author__ = "Richard Linhart"
__copyright__ = ""
__credits__ = [""]
__license__ = "GPL"
__version__ = "1.0.0"
__maintainer__ = "Richard Linhart"
__email__ = "OK1CTR@gmail.com"
__status__ = "Development"

from time import sleep
from Common import *
from MbPistonPump import *
import sys


if __name__ == '__main__':
    pp = MbPistonPump()
    pp.open()
    pp.set(REG_FORWARD,[500, 500])  # time, speed
    pp.set(REG_REWIND, [500, 500])  # time, speed

    pp.command(CMD_STEP_FORWARD)

    sleep(5)

    pp.command(CMD_STEP_REWIND)

    sys.exit(0)
