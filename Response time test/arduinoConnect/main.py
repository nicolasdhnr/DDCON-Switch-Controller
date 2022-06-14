import serial
import time
import csv


ArduinoLeonardoSerial = serial.Serial('/dev/ttyACM0', 9600)
time.sleep(2)

print(ArduinoLeonardoSerial.readline())

response_time = []

while True:

    if ArduinoLeonardoSerial.in_waiting > 0:
        if int(ArduinoLeonardoSerial.readline()) > 5: # This is to remove outliers - reponse time of 5ms is
            # considered impossible in out
            response_time.append(int(ArduinoLeonardoSerial.readline()))
            print(ArduinoLeonardoSerial.readline())

        else:
            continue

    if len(response_time) >= 500:
        break

with open("response_time_data.csv", "w") as f:
    write = csv.writer(f)
    for time in response_time:
        write.writerow([time])


