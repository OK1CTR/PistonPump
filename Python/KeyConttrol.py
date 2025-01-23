import sys
import pymodbus.client as ModbusClient
from Common import *

import keyboard

if __name__ == '__main__':

    # Create Modbus client
    client = ModbusClient.ModbusSerialClient(port="COM3", stopbits=1, bytesize=8, parity='N', baudrate=115200)
    if not client.connect():
        exit('Connection failed')

    while True:
        a = keyboard.read_key()
        if a == '\\':  # Send wave
            result_ = client.write_registers(address=0, values=[CMD_WAVE], slave=modbus_slave_addr)
            print(f'Write Holding Regs: {result_}')
            continue

        elif a == 'c':  # pulse forward
            result_ = client.write_registers(address=0, values=[CMD_STEP_FORWARD], slave=modbus_slave_addr)
            print(f'Write Holding Regs: {result_}')
            continue

        elif a =='x':  # pulse back
            result_ = client.write_registers(address=0, values=[CMD_STEP_REWIND], slave=modbus_slave_addr)
            print(f'Write Holding Regs: {result_}')
            continue

        elif a =='0':  # emergency stop
            result_ = client.write_registers(address=0, values=[CMD_STOP], slave=modbus_slave_addr)
            print(f'Write Holding Regs: {result_}')
            continue

        elif a =='1':  # emergency stop
            result_ = client.write_registers(address=1, values=[1000, 250, 1000, 250], slave=modbus_slave_addr)
            print(f'Write Holding Regs: {result_}')
            continue

        elif a =='2':  # emergency stop
            result_ = client.write_registers(address=1, values=[1000, 500, 1000, 500], slave=modbus_slave_addr)
            print(f'Write Holding Regs: {result_}')
            continue

        elif a =='3':  # emergency stop
            result_ = client.write_registers(address=1, values=[1000, 1000, 1000, 1000], slave=modbus_slave_addr)
            print(f'Write Holding Regs: {result_}')
            continue

        elif a == 'q':  # Quit
            break

    print('Finished')
    sys.exit(0)
