import paho.mqtt.client as paho
import time

#frequency 2.53 for period 400 ms

frequency = 10 #harus diatur sendiri
period = 1.0/frequency
runtime = 20

broker="localhost"
port=1883

def on_publish(client,userdata,result):             #create function for callback
	# print("data published \n")
	pass
client= paho.Client("client1")                           #create client object
client.on_publish = on_publish                          #assign function to callback
client.connect(broker,port)                                 #establish connection

client.loop_start() #start the loop

# for x in range(6):
	# ret= client.publish("house", x)                   #publish
	# print(ret)

time_init = time.time()
time_last = time_init
i = 0
while time.time() - time_init < runtime:
	time_begin = time.time()
	delta_time = time_last - time_begin
	time_last = time_begin
	# ret00= client.publish("topic-00", (delta_time) * 1000)
	# ret01= client.publish("topic-01", (delta_time) * 1000)
	# ret02= client.publish("topic-02", (delta_time) * 1000)
	# ret03= client.publish("topic-03", (delta_time) * 1000)
	# ret04= client.publish("topic-04", (delta_time) * 1000) #, qos=0, retain=True)                   #publish
	ret00= client.publish("Lat", (time_begin - time_init) * 1000)
	ret01= client.publish("Long", (time_begin - time_init) * 100)
	ret02= client.publish("RPM1", (time_begin - time_init) * 10)
	ret03= client.publish("RPM2", (time_begin - time_init) * 1)
	ret04= client.publish("RPM3", (time_begin - time_init) * -1)
	ret04= client.publish("RPM4", (time_begin - time_init) * -10)
	ret04= client.publish("az1", (time_begin - time_init) * -100)
	ret04= client.publish("az2", (time_begin - time_init) * -1000)
	ret04= client.publish("az3", (time_begin - time_init) * 0.1)
	ret04= client.publish("az4", (time_begin - time_init) * 0.01)
	ret04= client.publish("kompas", (time_begin - time_init) * -0.001)
	ret04= client.publish("dkompas_dt", (time_begin - time_init) * -0.01)
	ret04= client.publish("velocity", (time_begin - time_init) * -0.1)
	i += 1
	while (time.time() - time_begin) < period:
		time.sleep(0.001)

time.sleep(1) # wait
client.loop_stop() #stop the loop