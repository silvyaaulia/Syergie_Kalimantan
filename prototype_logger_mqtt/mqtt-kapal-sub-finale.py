import paho.mqtt.client as paho
import time
import logging
import csv

with open('data.csv', 'w') as csvFile:
	
	#Edit topics names here, 13 topics in total
	#to add topic, add also msgXX, on_meesage_topic_XX, callback_add
	#and input argument in writer.writerow
	topics = [("Lat",0), ("Long",0), ("RPM1",0), ("RPM2",0), ("RPM3",0),
			("RPM4",0), ("az1",0), ("az2",0), ("az3",0), ("az4",0),
			("kompas",0), ("dkompas_dt",0), ("velocity",0)]

	fieldnames = [topics[0][0], topics[1][0], topics[2][0], topics[3][0], topics[4][0],
				topics[5][0], topics[6][0], topics[7][0], topics[8][0], topics[9][0],
				topics[10][0], topics[11][0], topics[12][0]]
	writer = csv.DictWriter(csvFile, fieldnames=fieldnames, delimiter='\t', lineterminator='\n')
	writer.writeheader()

	#perioda
	period = 0.4 #dalam second
	#waktu running dalam detik
	runtime = 10
	
	msg00 = msg01 = msg02 = msg03 = msg04 = msg05 = msg06 = 0
	msg07 = msg08 = msg09 = msg10 = msg11 = msg12 = 0
	
	broker="localhost"
	port=1883

	### TOPIC CALLBACK
	def on_message_topic_00(mosq, obj, msg):
		global msg00
		msg00 = msg.payload.decode("utf-8")
		client.unsubscribe(msg.topic)
	def on_message_topic_01(mosq, obj, msg):
		global msg01
		msg01 = msg.payload.decode("utf-8")
		client.unsubscribe(msg.topic)
	def on_message_topic_02(mosq, obj, msg):
		global msg02
		msg02 = msg.payload.decode("utf-8")
		client.unsubscribe(msg.topic)
	def on_message_topic_03(mosq, obj, msg):
		global msg03
		msg03 = msg.payload.decode("utf-8")
		client.unsubscribe(msg.topic)
	def on_message_topic_04(mosq, obj, msg):
		global msg04
		msg04 = msg.payload.decode("utf-8")
		client.unsubscribe(msg.topic)
	def on_message_topic_05(mosq, obj, msg):
		global msg05
		msg05 = msg.payload.decode("utf-8")
		client.unsubscribe(msg.topic)
	def on_message_topic_06(mosq, obj, msg):
		global msg06
		msg06 = msg.payload.decode("utf-8")
		client.unsubscribe(msg.topic)
	def on_message_topic_07(mosq, obj, msg):
		global msg07
		msg07 = msg.payload.decode("utf-8")
		client.unsubscribe(msg.topic)
	def on_message_topic_08(mosq, obj, msg):
		global msg08
		msg08 = msg.payload.decode("utf-8")
		client.unsubscribe(msg.topic)
	def on_message_topic_09(mosq, obj, msg):
		global msg09
		msg09 = msg.payload.decode("utf-8")
		client.unsubscribe(msg.topic)
	def on_message_topic_10(mosq, obj, msg):
		global msg10
		msg10 = msg.payload.decode("utf-8")
		client.unsubscribe(msg.topic)
	def on_message_topic_11(mosq, obj, msg):
		global msg11
		msg11 = msg.payload.decode("utf-8")
		client.unsubscribe(msg.topic)
	def on_message_topic_12(mosq, obj, msg):
		global msg12
		msg12 = msg.payload.decode("utf-8")
		client.unsubscribe(msg.topic)

	client= paho.Client("client-sub-01")	#create client object
	client.connect(broker,port)	#establish connection

	client.message_callback_add(topics[0][0], on_message_topic_00)
	client.message_callback_add(topics[1][0], on_message_topic_01)
	client.message_callback_add(topics[2][0], on_message_topic_02)
	client.message_callback_add(topics[3][0], on_message_topic_03)
	client.message_callback_add(topics[4][0], on_message_topic_04)
	client.message_callback_add(topics[5][0], on_message_topic_05)
	client.message_callback_add(topics[6][0], on_message_topic_06)
	client.message_callback_add(topics[7][0], on_message_topic_07)
	client.message_callback_add(topics[8][0], on_message_topic_08)
	client.message_callback_add(topics[9][0], on_message_topic_09)
	client.message_callback_add(topics[10][0], on_message_topic_10)
	client.message_callback_add(topics[11][0], on_message_topic_11)
	client.message_callback_add(topics[12][0], on_message_topic_12)
	
	client.loop_start() #start the loop
	
	time_init = time.time();
	while time.time() - time_init < runtime:
		client.subscribe(topics)
		time.sleep(period)
		writer.writerow({topics[0][0]: msg00, topics[1][0]: msg01, topics[2][0]: msg02,
						topics[3][0]: msg03, topics[4][0]: msg04, topics[5][0]: msg05,
						topics[6][0]: msg06, topics[7][0]: msg07, topics[8][0]: msg08,
						topics[9][0]: msg09, topics[10][0]: msg10, topics[11][0]: msg11,
						topics[12][0]: msg12})
			
	time.sleep(1) # wait
	client.loop_stop() #stop the loop

csvFile.close()