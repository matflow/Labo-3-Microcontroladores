import serial

arduino = serial.Serial(port='/tmp/ttyS1')
while True:
	print(arduino.readline())
