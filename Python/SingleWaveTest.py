import pymodbus.client as ModbusClient
from time import sleep
from Common import *

if __name__ == '__main__':

    # Create Modbus client
    client = ModbusClient.ModbusSerialClient(port="COM3", stopbits=1, bytesize=8, parity='N', baudrate=115200)
    if not client.connect():
        exit('Connection failed')

    # Send wave
    result_ = client.write_registers(address=0, values=[CMD_WAVE], slave=modbus_slave_addr)
    print(f'Write Holding Regs: {result_}')
