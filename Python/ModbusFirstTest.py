#from pymodbus.client import AsyncModbusSerialClient
import pymodbus.client as ModbusClient

if __name__ == '__main__':
    client = ModbusClient.ModbusSerialClient(port="COM3", stopbits=1, bytesize=8, parity='N', baudrate=115200)
    if not client.connect():
        exit('Connection failed')
    #for i in range(4):
    if True:
        i = 3
        result_ = client.read_input_registers(address=i, count=1, slave=22)
        print(f'Input {i}: {result_}')

    """
    result_ = client.read_holding_registers(address=0, count=1, slave=22)
    print(f'Holdig before": {result_.registers}')
    client.write_register(address=0, value=15, slave=22)
    result_ = client.read_holding_registers(address=0, count=1, slave=22)
    print(f'Holding after": {result_.registers}')
    client.close()
    """