import pymodbus.client as ModbusClient
from time import sleep
from Common import *

if __name__ == '__main__':

    # Create Modbus client
    client = ModbusClient.ModbusSerialClient(port="COM3", stopbits=1, bytesize=8, parity='N', baudrate=115200)
    if not client.connect():
        exit('Connection failed')

    # Write programmable wave register (voltage, time)
    result_ = client.write_registers(address=100, values=[100, 500], slave=modbus_slave_addr)
    print(f'Write Holding Regs: {result_}')

    result_ = client.write_registers(address=102, values=[100, 500], slave=modbus_slave_addr)
    print(f'Write Holding Regs: {result_}')

    result_ = client.write_registers(address=104, values=[100, 500], slave=modbus_slave_addr)
    print(f'Write Holding Regs: {result_}')

    result_ = client.write_registers(address=106, values=[100, 500], slave=modbus_slave_addr)
    print(f'Write Holding Regs: {result_}')

    result_ = client.write_registers(address=108, values=[100, 500], slave=modbus_slave_addr)
    print(f'Write Holding Regs: {result_}')

    result_ = client.write_registers(address=110, values=[100, 500], slave=modbus_slave_addr)
    print(f'Write Holding Regs: {result_}')

    result_ = client.write_registers(address=112, values=[100, 500], slave=modbus_slave_addr)
    print(f'Write Holding Regs: {result_}')

    result_ = client.write_registers(address=114, values=[100, 500], slave=modbus_slave_addr)
    print(f'Write Holding Regs: {result_}')
