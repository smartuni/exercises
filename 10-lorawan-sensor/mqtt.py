import paho.mqtt.client as mqtt

# Condifugure as needed
USERNAME="your-application-id@ttn"
API_KEY="your-application-API-key"

# Subscribe to the uplink topic of all devices
TOPIC = f"v3/{USERNAME}/devices/+/up"

# Address of TTN MQTT server
TTN_MQTT_SERVER = "eu1.cloud.thethings.network"

# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))

    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    client.subscribe("#")

# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    print(msg.topic+" "+str(msg.payload))

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.username_pw_set(USERNAME, API_KEY)
client.connect(TTN_MQTT_SERVER, 1883, 60)

# Blocking call that processes network traffic, dispatches callbacks and
# handles reconnecting.
# Other loop*() functions are available that give a threaded interface and a
# manual interface.
client.loop_forever()
