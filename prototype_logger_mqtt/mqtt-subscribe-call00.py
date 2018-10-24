import paho.mqtt.client as paho
import time
import logging
import csv

with open('data.csv', 'w') as csvFile:
	fieldnames = ['topic-00',
				'topic-01',
				'topic-02',
				'topic-03',
				'topic-04']
	writer = csv.DictWriter(csvFile, fieldnames=fieldnames, delimiter='\t', lineterminator='\n')
	
	writer.writeheader()

	topics = [("topic-00",0)]

	#frequency 2.53 for period 400 ms

	frequency = 1 #harus diatur sendiri
	period = 1.0/frequency
	j = 0
	msg00 = 0
	msg01 = 0
	msg02 = 0
	msg03 = 0
	msg04 = 0
	
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
		global msg00
		time_begin = time.time()
		print('printing message ke-' +  str(j))
		j += 1
		print(str(msg.payload.decode("utf-8")))
		msg00 = msg.payload.decode("utf-8")
		client.unsubscribe(msg.topic)
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
		# time.sleep(1)
		# client.loop()
		# i += 1
		writer.writerow({'topic-00': msg00,
							'topic-01': msg01,
							'topic-02': msg02,
							'topic-03': msg03,
							'topic-04': msg04})
		
		while (time.time() - time_begin_2) < period:
			time.sleep(0.001)
			
	time.sleep(1) # wait
	client.loop_stop() #stop the loop

csvFile.close()