import time
from serial import Serial
from serial.serialutil import SerialException
while True:
    try:
        with Serial('/dev/tty.usbmodem143401') as s:
            f = s.readline()
            while f:
                print(f)
                f = s.readline()
    except SerialException as e:
        print(e)
        time.sleep(1)
        continue
