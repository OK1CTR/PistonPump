#!/usr/bin/env python
# -*- coding: utf8 -*-

"""
First testo of Modbus operations with new library version
"""

__author__ = "Richard Linhart"
__copyright__ = ""
__credits__ = [""]
__license__ = "GPL"
__version__ = "1.0.0"
__maintainer__ = "Richard Linhart"
__email__ = "OK1CTR@gmail.com"
__status__ = "Development"

import sys

import pymodbus.client as ModbusClient

if __name__ == '__main__':

    # Create Modbus client
    client = ModbusClient.ModbusSerialClient(port="COM3", stopbits=1, bytesize=8, parity='N', baudrate=115200)
    if not client.connect():
        exit('Connection failed')

    # Read Input registers
    result_ = client.read_input_registers(address=0, count=4, slave=22)
    print(f'Input Regs: {result_}')

    # Read Holding registers before write
    result_ = client.read_holding_registers(address=0, count=5, slave=22)
    print(f'Holding Regs before: {result_}')

    # Write Holding registers
    result_ = client.write_registers(address=0, values=[10, 20, 100, 2000, 50000], slave=22)
    print(f'Write Holding Regs: {result_}')

    # Read Holding registers after write
    result_ = client.read_holding_registers(address=0, count=5, slave=22)
    print(f'Holding Regs after: {result_}')

    # close client
    client.close()
    sys.exit(0)
