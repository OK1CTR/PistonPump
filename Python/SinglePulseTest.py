import pymodbus.client as ModbusClient
from time import sleep
from Common import *

if __name__ == '__main__':

    # Create Modbus client
    client = ModbusClient.ModbusSerialClient(port="COM3", stopbits=1, bytesize=8, parity='N', baudrate=115200)
    if not client.connect():
        exit('Connection failed')

    # Write fwd_time, fwd_speed, rew_time, rew_speed
    result_ = client.write_registers(address=1, values=[1000, 500, 1000, 500], slave=modbus_slave_addr)
    print(f'Write Holding Regs: {result_}')

    # Send forward
    result_ = client.write_registers(address=0, values=[CMD_STEP_FORWARD], slave=modbus_slave_addr)
    print(f'Write Holding Regs: {result_}')

    sleep(5)

    # Send rewind
    result_ = client.write_registers(address=0, values=[CMD_STEP_REWIND], slave=modbus_slave_addr)
    print(f'Write Holding Regs: {result_}')
