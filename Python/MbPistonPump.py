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

from Common import *
import pymodbus.client as mbc
import json

# print messages about transmitted and received frames
verbose = False


class MbPistonPump:
    def __init__(self):
        self.client = None
        self.config = None
        self.config_default = {
            'port': 'COM1',
            'stopbits': '1',
            'bytesize': '8',
            'parity': 'N',
            'baudrate': '115200'
        }

    def open(self):
        """
        Open the communication
        :return: True if successful
        """
        try:
            with open('configuration.json', 'r') as f_cfg:
                self.config = json.load(f_cfg)
        except FileNotFoundError:
            self.config = self.config_default
            with open('configuration.json', 'w') as f_cfg:
                json_object = json.dumps(self.config, indent=4)
                with open('configuration.json', 'w') as f_cfg_new:
                    f_cfg_new.write(json_object)

        try:
            self.client = mbc.ModbusSerialClient(port=self.config['port'], stopbits=int(self.config['stopbits']),
                bytesize=int(self.config['bytesize']), parity=self.config['parity'],
                baudrate=int(self.config['baudrate']))
        except (AttributeError, ValueError):
            print('Wrong configuration, see the \'configuration.json\' file!')
            return False


        if not self.client.connect():
            print('Cannot connect to Modbus port!')
            return False
        else:
            return True

    def command(self, command):
        """
        Send command to the Piston Pump
        :param command: Command code
        :return: RESERVED, the complete result from the Modbus library
        """
        result = self.client.write_registers(address=REG_COMMAND, values=[command], slave=MODBUS_SLAVE_ADDR)
        return result

    def set(self, register, values):
        """
        Set the remote register to defined value
        :param register: First register Modbus address
        :param values: List of values to set
        :return: RESERVED, the complete result from the Modbus library
        """
        result = self.client.write_registers(address=register, values=values, slave=MODBUS_SLAVE_ADDR)
        return result
