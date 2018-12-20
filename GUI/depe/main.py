import sys

from PyQt5.QtCore import QUrl, QObject, pyqtSignal, pyqtSlot, QTimer, pyqtProperty
from PyQt5.QtGui import QGuiApplication
from PyQt5.QtQuick import QQuickView
from PyQt5.QtQml import QQmlApplicationEngine
from PyQt5.QtWidgets import QApplication

#import serial
import time
import paho.mqtt.client as paho

broker="localhost"
port = 1883	

pubdelay = 5 #delay publish to all wind and engine box
counter = 0

modeSystem = 0

val_heading = 0
val_velocity = 0
val_latitude = 0
val_longitude = 0

waypoint_lat = 0
waypoint_lon = 0

vali=5

##findchild
class MQTTValue (QObject):

	def __init__(self):
		super(MQTTValue,self).__init__()
		
	
	# if a slot returns a value the return value type must be explicitly
	# defined in the decorator
	
	@pyqtSlot(result=str)
	def mainControl (self): return modeSystem  
	
	@pyqtSlot(result=int)
	def headingShip (self): return val_heading
	
	@pyqtSlot(result=int)
	def velocityShip (self): return val_velocity
	
	@pyqtSlot(result=int)
	def latitudeShip (self): return val_latitude
	
	@pyqtSlot(result=int)
	def longitudeShip (self): return val_longitude	
	

#cek message dari topic	
def on_message(client, userdata, message):
	msg = str(message.payload.decode("utf-8"))
	t = str(message.topic)	
	
	#check message
	if (msg[0] == 'c'):
		val =1
	else:
		val = int(msg)
		//val = 3
	
	#check topic
	if (t[0] == "S"):
		print("GPS-Ship")
		if (t[4] == "H"): global val_heading; val_heading = val
		elif (t[4] == "V"): global val_velocity; val_velocity = val
		elif (t[4] == "L" and t[5] == "a"): global val_latitude; val_latitude = val
		else ((t[4] == "L") and (t[5] == "o")): global val_longitude; val_longitude = val
		

#masukin data ke waypoint
	
if __name__== "__main__":
	
	##Mosquitto Mqtt Configuration			
	client= paho.Client("GUI2")
	client.on_message=on_message

	print("connecting to broker ",broker)
	client.connect(broker,port)#connect
	print(broker,"GUI depe-connected")
	
	client.loop_start()
	print("Subscribing")
	
	
	#subcribe SMC(manual/auto)
	client.subscribe("mainControl")#subscribe (buat cek)
	print("Syergie Main Control subscribed ")
	
	#subcribe GPS (latitude, longitude, compass/heading, velocity)
	client.subscribe("ShipHeading")#subscribe
	client.subscribe("ShipVelocity")#subscribe
	client.subscribe("ShipLatitude")#subscribe
	client.subscribe("ShipLongitude")#subscribe
	print("GPS subscribed	")


	#publish training
	client.publish("Training", "on")#publish

	#publish mission (add waypoint, start mission)
	client.publish("LatitudeWay", "waypoint_lat")#publish
	client.publish("LongitudeWay", "waypoint_lon")#publish
	client.publish("StartMission", "on")#publish
	
	#publish tracking
	client.publish("clearTrack", "clear")#publish
	
	

	####### QT5 GUI
	print("Graphical User Interface - Dynamic Positioning ")
	app = QApplication(sys.argv)
		
	engine = QQmlApplicationEngine() 
	#ctx = engine.rootContext()
	engine.load('main.qml')
	win = engine.rootObjects()[0]
	engine.quit.connect(app.quit) ## Quit Button Respon

	mqttvalue = MQTTValue()  #qml ambil data di python

	timer = QTimer()
	timer.start(10) ##Update screen every 10 miliseconds

	context = engine.rootContext()
	context.setContextProperty("mqttvalue", mqttvalue)

	timer.timeout.connect(win.updateValue) ##Call function update in GUI QML
		
	win.show()

	sys.exit(app.exec_())




