#!/usr/bin/env python
# -*- coding: utf8 -*-

"""
Module to control the Piston Pump via Modbus interface.
"""

__author__ = "Richard Linhart"
__copyright__ = ""
__credits__ = [""]
__license__ = "GPL"
__version__ = "1.0.0"
__maintainer__ = "Richard Linhart"
__email__ = "OK1CTR@gmail.com"
__status__ = "Development"
__all__ = ["MbPistonPump"]

import Common
import pymodbus.client as mbc

# print messages about transmitted and received frames
verbose = False


class MbPistonPump:
    def __init__(self, port, address):
        self.port = port
        self.client = None
        self.address = address

    def open(self):
        """
        Open the communication
        :return: True if successful
        """
        self.client = mbc.ModbusSerialClient(port=self.port, stopbits=1, bytesize=8, parity='N', baudrate=115200)
        if not self.client.connect():
            return False
        else:
            return True

    def send_command(self, command):
        """
        Send command to the Piston Pump
        :param command: Command code
        :return: True if successful
        """
        result = self.client.write_registers(address=ADR_COMMAND, values=command, slave=self.address)