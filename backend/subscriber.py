import json



import paho.mqtt.client as mqtt

from pymongo import MongoClient



# ---------------- MongoDB ----------------


mongo = MongoClient("mongodb://localhost:27017/")


db = mongo["esp32_database"]


collection = db["sensor_data"]


# ---------------- MQTT ----------------


BROKER = "localhost"

PORT = 1883

TOPIC = "sensor/environment"

def on_connect(client, userdata, flags, reason_code, properties):

print("Connected to MQTT")

client.subscribe(TOPIC)

def on_message(client, userdata, msg):

payload = msg.payload.decode()

print("Received:", payload)

data = json.loads(payload)

collection.insert_one(data)

print("Stored in MongoDB\n")

client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)

client.on_connect = on_connect

client.on_message = on_message

client.connect(BROKER, PORT)



client.loop_forever()

