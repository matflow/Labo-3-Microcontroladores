import serial

arduino = serial.Serial(port='/tmp/ttyS1')


import csv
header = ['Set Point', 'Humedad', 'Temp sensada', 'Salida']
data = ['', '', '', '']
arduino = serial.Serial(port='/tmp/ttyS1')

# abrir o crear archivo en modo escritura
f = open('csvpy.csv', 'w', encoding='UTF8')
# create the csv writer
writer = csv.writer(f)
print(header)
writer.writerow(header)  # escribe encabezado
while True:
    	# write a row to the csv file
    	i = 0
    	search = arduino.readline().decode('utf-8').replace('\r\n', "") 
    	if search == 'loop':       # revisa si inició ciclo de datos
    		for i in range(4):
	    		data[i] = arduino.readline().decode('utf-8')
	    		data[i] = data[i].replace('\r\n', "") 
	    		
	    	print(data)
	    	writer.writerow(data)

    	
f.close()

