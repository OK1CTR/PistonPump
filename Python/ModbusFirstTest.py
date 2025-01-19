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
