# Pump control Modbus address
modbus_slave_addr = 22

# Commands definition
CMD_STEP_FORWARD = 1           # do forward step
CMD_STEP_REWIND = 2            # do rewind step
CMD_WAVE = 3                   # do programmed wave
CMD_CFG_SAVE = 4               # save the configuration to backup and reinitialize system
CMD_CFG_LOAD = 5               # load the configuration from backup and reinitialize system
CMD_CFG_DEFAULT = 6            # restore configuration to default and reinitialize system
CMD_STOP = 7                   # emergency motor stop
CMD_REPEAT = 8                 # repeat the programmable wave
