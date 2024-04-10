# clone_codding_task

ENV CONFIG:

DATABASE_FILE
- default: "database.db"
- desc: Path of the database file
    
SIMULATION
- default: "0"
- desc: Will run app in simulation mode when != 0.

SERIAL_PORT
- default: "/dev/ttyUSB0"
- desc: Device port

SIM_SERIAL_PORT_1
- default: "/home/s0mas/sim1"
- desc: Sim Device port (sim dev writes/reads to/from this port)

SIM_SERIAL_PORT_2
- default: "/home/s0mas/sim2"
- desc: Sim Device port (read from write to sim device)

SERVER_PORT
- default: "7100"
- desc: Http server port

SERVER_ADDRESS
- default: "localhost"
- desc: Http server address

SERIAL_BAUDRATE
- default: "115000"
- desc: Baudrate of serial ports

NO_GUI
- default: "0"
- desc: Will run app without gui when != 0
