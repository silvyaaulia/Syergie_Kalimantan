import paho.mqtt.client as paho
import time
import logging

topics = [("topic-00",0)]

#frequency 2.53 for period 400 ms

frequency = 1 #harus diatur sendiri
period = 1.0/frequency
j = 0

broker="localhost"
port=1883

logging.basicConfig(filename='test.log', level=logging.INFO)

# def on_message(client, userdata, message):
    # time.sleep(1)
    # print("received message =",str(message.payload.decode("utf-8")))

### TOPIC-00
def on_message_topic_00(mosq, obj, msg):
	global period
	global j
	time_begin = time.time()
	print('printing message ke-' +  str(j))
	j += 1
	print(str(msg.payload.decode("utf-8")))
	# client.unsubscribe("topic-00")
	# logging.info(str(msg.payload))
	# time.sleep(1)
	# while (time.time() - time_begin) < period:
		# time.sleep(0.001)

client= paho.Client("client-sub-00", True)                           #create client object
client.connect(broker,port)                                 #establish connection

client.message_callback_add('topic-00', on_message_topic_00)

client.loop_start() #start the loop

# i = 0
time_init = time.time();
while time.time() - time_init < 10:
	time_begin_2 = time.time()
	# print('loop ke-' +  str(i))
	client.subscribe(topics)
	time.sleep(1)
	# client.loop()
	# i += 1
	# while (time.time() - time_begin_2) < period:
		# time.sleep(0.001)

time.sleep(1) # wait
client.loop_stop() #stop the loop