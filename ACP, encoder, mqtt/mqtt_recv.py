import paho.mqtt.client as mqtt
from serial import Serial
import time
import random

s = Serial('COM6', baudrate = 9600)

def on_connect(client, userdata, flags, rc):
    print("Connected: " + str(rc))
    client.subscribe("pinboards/ledline/percent")

def on_message(client, userdata, msg):
    print(msg.topic, msg.payload.decode())
    s.write(bytes(msg.payload.decode() + '\n', "utf-8"))

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message
client.connect("simurg.iszf.irk.ru", 1883, 60)
client.loop_forever()

'''while True:
    #value = int(random.random() * 100)
    #client.publish("pinboards/ledline/percent", payload=value)
    s.write(msg.payload.decode())
    #time.sleep(1)'''
