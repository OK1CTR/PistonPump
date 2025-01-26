#!/usr/bin/env python
# -*- coding: utf8 -*-

"""
Common project definition of functions, variables and constants.
"""

__author__ = "Richard Linhart"
__copyright__ = ""
__credits__ = [""]
__license__ = "GPL"
__version__ = "1.0.0"
__maintainer__ = "Richard Linhart"
__email__ = "OK1CTR@gmail.com"
__status__ = "Development"


# Pump control Modbus address
MODBUS_SLAVE_ADDR = 22

# Commands definition
CMD_STEP_FORWARD = 1           # do forward step
CMD_STEP_REWIND = 2            # do rewind step
CMD_WAVE = 3                   # do programmed wave
CMD_CFG_SAVE = 4               # save the configuration to backup and reinitialize system
CMD_CFG_LOAD = 5               # load the configuration from backup and reinitialize system
CMD_CFG_DEFAULT = 6            # restore configuration to default and reinitialize system
CMD_STOP = 7                   # emergency motor stop
CMD_REPEAT = 8                 # repeat the programmable wave

# Holding registers addresses
REG_COMMAND = 0                # operation command
REG_FORWARD_TIME = 1           # maximal time of motor run forward
REG_FORWARD_SPEED = 2          # motor speed (driver duty factor) for run forward
REG_REWIND_TIME = 3            # maximal time of motor run back
REG_REWIND_SPEED = 4           # motor speed (driver duty factor) for run back
REG_FILTER_LENGTH = 5          # filter motor length, configure to update
REG_REPEAT_COUNT = 6           # repeat count of programmable wave
REG_REPEAT_PERIOD = 7          # programmable wave repeat period in milliseconds

# Holding register array addresses
REG_FORWARD = 1                # forward time and speed
REG_REWIND = 3                 # rewind time and speed
REG_REPEAT = 6                 # repeat count and period

# Input registers addresses
REG_STATE = 0                  # system global status flag register
REG_ERROR = 1                  # system global error flag register
