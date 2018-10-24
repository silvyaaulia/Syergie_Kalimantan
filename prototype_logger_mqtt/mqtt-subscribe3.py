

import paho.mqtt.subscribe as subscribe
import time
import csv

with open('person.csv', 'w') as csvFile:
	fieldnames = ['topic-00',
				'topic-01',
				'topic-02',
				'topic-03',
				'topic-04']
	writer = csv.DictWriter(csvFile, fieldnames=fieldnames, delimiter='\t')
	# writer = csv.DictWriter(csvFile, fieldnames=fieldnames)
	
	writer.writeheader()
	
	frequency = 2 #harus diatur sendiri
	period = 1.0/frequency
	
	i = 0
	
	time_init = time.time()
	while time.time() - time_init < 10:
		time_begin_2 = time.time()
		msg00 = subscribe.simple("topic-00", hostname="localhost")
		msg01 = subscribe.simple("topic-00", hostname="localhost")
		msg02 = subscribe.simple("topic-00", hostname="localhost")
		msg03 = subscribe.simple("topic-00", hostname="localhost")
		msg04 = subscribe.simple("topic-00", hostname="localhost")
		
		# csvData = [[msg00.topic], [msg00.payload]]#,
				# # [msg01.topic, msg01.payload],
				# # [msg02.topic, msg02.payload],
				# # [msg03.topic, msg03.payload]]
		writer.writerow({'topic-00': msg00.payload.decode("utf-8"),
						'topic-01': msg01.payload.decode("utf-8"),
						'topic-02': msg02.payload.decode("utf-8"),
						'topic-03': msg03.payload.decode("utf-8"),
						'topic-04': msg04.payload.decode("utf-8")})
		# writer.writerows(csvData)
		# print("%s %s" % (msg.topic, msg.payload))
		
		print("write line-", str(i))
		i += 1
		
		while (time.time() - time_begin_2) < period:
			time.sleep(0.001)

csvFile.close()