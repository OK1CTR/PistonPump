#!/usr/bin/env python
# -*- coding: utf8 -*-

"""
Test of programmable wave
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
    pp.set(REG_REPEAT, [3, 2000])  # count, period

    # pp.command(CMD_WAVE)
    pp.command(CMD_REPEAT)

    sys.exit(0)
