import sys

from PyQt5.QtCore import QUrl, QObject, pyqtSignal, pyqtSlot, QTimer, pyqtProperty
from PyQt5.QtGui import QGuiApplication
from PyQt5.QtQuick import QQuickView
from PyQt5.QtQml import QQmlApplicationEngine
from PyQt5.QtWidgets import QApplication

import time
import paho.mqtt.client as paho
broker="localhost"
port = 1883

pubdelay = 5 #delay publish to all wind and engine box
counter = 0

directProp1 = 0
directProp2 = 0
directProp3 = 0
directProp4 = 0

speedEngine1 = 0
speedEngine2 = 0
speedEngine3 = 0
speedEngine4 = 0

speedPropeler1 = 0
speedPropeler2 = 0
speedPropeler3 = 0
speedPropeler4 = 0

speedPump1 = 0
speedPump2 = 0
speedPump3 = 0
speedPump4 = 0

lfuel1 = 0
lfuel2 = 0
lfuel3 = 0
lfuel4 = 0

ltemp1 = 0
ltemp2 = 0
ltemp3 = 0
ltemp4 = 0

depth1 = 0
depth2 = 0
depth3 = 0
depth4 = 0

indicator1 = 0
indicator2 = 0
indicator3 = 0
indicator4 = 0
indicator5 = 0
indicator6 = 0

ind1 = 0
ind2 = 0
ind3 = 0
ind4 = 0
windInd = 0
ind6 = 0

directWind = 0
speedWind = 0

val_speed1 = 0
val_speed2 = 0
val_steer1 = 0
val_steer2 = 0

def reMap(value, maxInput, minInput, maxOutput, minOutput):

	value = maxInput if value > maxInput else value
	value = minInput if value < minInput else value

	inputSpan = maxInput - minInput
	outputSpan = maxOutput - minOutput

	scaledThrust = float(value - minInput) / float(inputSpan)

	return minOutput + (scaledThrust * outputSpan)

class MQTTValue(QObject):      
	
	def __init__(self):
		super(MQTTValue,self).__init__()
		
	# if a slot returns a value the return value type must be explicitly
	# defined in the decorator
	
	####################################################
	@pyqtSlot(result=int)
	def azimut1(self):  return directProp1
	
	@pyqtSlot(result=int)
	def enginespeed1(self):  return speedEngine1
	
	@pyqtSlot(result=int)
	def propelerspeed1(self):  return speedPropeler1
	
	@pyqtSlot(result=int)
	def pumpspeed1(self):  return speedPump1
	
	@pyqtSlot(result=int)
	def fuel1(self):  return 20-lfuel1
	
	@pyqtSlot(result=int)
	def temp1(self):  return ltemp1
	
	@pyqtSlot(result=int)
	def vdepth1(self):  return depth1
	
	@pyqtSlot(result=int)
	def engineconect1(self): return indicator1

	
	####################################################
	@pyqtSlot(result=int)
	def azimut2(self):  return directProp2
	
	@pyqtSlot(result=int)
	def enginespeed2(self):  return speedEngine2
	
	@pyqtSlot(result=int)
	def propelerspeed2(self):  return speedPropeler2
	
	@pyqtSlot(result=int)
	def pumpspeed2(self):  return speedPump2
	
	@pyqtSlot(result=int)
	def fuel2(self):  return 20-lfuel2
	
	@pyqtSlot(result=int)
	def temp2(self):  return ltemp2
	
	@pyqtSlot(result=int)
	def vdepth2(self):  return depth2
	
	@pyqtSlot(result=int)
	def engineconect2(self): return indicator2

	
	####################################################
	@pyqtSlot(result=int)
	def azimut3(self):  return directProp3
	
	@pyqtSlot(result=int)
	def enginespeed3(self):  return speedEngine3
	
	@pyqtSlot(result=int)
	def propelerspeed3(self):  return speedPropeler3
	
	@pyqtSlot(result=int)
	def pumpspeed3(self):  return speedPump3
	
	@pyqtSlot(result=int)
	def fuel3(self):  return 20-lfuel3
	
	@pyqtSlot(result=int)
	def temp3(self):  return ltemp3
	
	@pyqtSlot(result=int)
	def vdepth3(self):  return depth3
	
	@pyqtSlot(result=int)
	def engineconect3(self):  return indicator3

	
	####################################################
	@pyqtSlot(result=int)
	def azimut4(self):  return directProp4

	@pyqtSlot(result=int)
	def enginespeed4(self):  return speedEngine4
	
	@pyqtSlot(result=int)
	def propelerspeed4(self):  return speedPropeler4
	
	@pyqtSlot(result=int)
	def pumpspeed4(self):  return speedPump4
	
	@pyqtSlot(result=int)
	def fuel4(self):  return 20-lfuel4
	
	@pyqtSlot(result=int)
	def temp4(self):  return ltemp4
	
	@pyqtSlot(result=int)
	def vdepth4(self):  return depth4
	
	@pyqtSlot(result=int)
	def engineconect4(self): return indicator4
		
	####################################################
	@pyqtSlot(result=int)
	def directWind(self):  return directWind

	@pyqtSlot(result=int)
	def speedWind(self):  return speedWind
	
	@pyqtSlot(result=int)
	def windconect(self): return indicator5
	
	####################################################
	@pyqtSlot(result=int)
	def speed1(self):  return (val_speed1/100)-15

	@pyqtSlot(result=int)
	def speed2(self):  return (val_speed2/100)-15
	
	@pyqtSlot(result=int)
	def steer1(self): 
		if (val_steer1 == 1200): return 225
		elif (val_steer1 == 1300): return 270
		elif (val_steer1 == 1400): return 315
		elif (val_steer1 == 1500): return 0
		elif (val_steer1 == 1600): return 45
		elif (val_steer1 == 1700): return 90
		elif (val_steer1 == 1800): return 135
		else: return 180
	
	@pyqtSlot(result=int)
	def steer2(self): 
		if (val_steer2 == 1200): return 225
		elif (val_steer2 == 1300): return 270
		elif (val_steer2 == 1400): return 315
		elif (val_steer2 == 1500): return 0
		elif (val_steer2 == 1600): return 45
		elif (val_steer2 == 1700): return 90
		elif (val_steer2 == 1800): return 135
		else: return 180
	
	@pyqtSlot(result=int)
	def joyconect(self): return indicator6
	
	
	@pyqtSlot(result=str)
	def maincontrol(self):
		global counter,pubdelay,indicator1,indicator2,indicator3,indicator4,indicator5,indicator6
		global ind1,ind2,ind3,ind4,windInd,ind6
		counter=counter+1
		if (counter >= pubdelay):
			client.publish("MainControl", "active")#publish
			if (ind1): ind1 = 0 
			else: indicator1 = 0
			if (ind2): ind2 = 0 
			else: indicator2 = 0
			if (ind3): ind3 = 0 
			else: indicator3 = 0
			if (ind4): ind4 = 0 
			else: indicator4 = 0
			if (windInd): windInd = 0 
			else: indicator5 = 0
			if (ind6): ind6 = 0 
			else: indicator6 = 0
			counter = 0;
		return ""
	
def on_message(client, userdata, message):
		msg = str(message.payload.decode("utf-8"))
		t = str(message.topic)

		if(msg[0] == 'c'):
			val =  1
		else:
			val = int(msg)

		if (t[0] == "1"):
			print("Engine 1")
			if (t[1] == "A"): global directProp1; directProp1 = val 
			elif (t[1] == "E"): global speedEngine1; speedEngine1 = val
			elif (t[1] == "P"): global speedPropeler1; speedPropeler1 = val 
			elif (t[1] == "F"): global lfuel1; lfuel1 = val
			elif (t[1] == "T"): global ltemp1; ltemp1 = val
			elif (t[1] == "D"): global depth1; depth1 = val       
			elif (t[1] == "B"): global ind1; ind1 = val; global indicator1; indicator1 = val
		elif (t[0] == "2"):
			print("Engine 2")
			if (t[1] == "A"): global directProp2; directProp2 = val
			elif (t[1] == "E"): global speedEngine2; speedEngine2 = val
			elif (t[1] == "P"): global speedPropeler2; speedPropeler2 = val 
			elif (t[1] == "F"): global lfuel2; lfuel2 = val
			elif (t[1] == "T"): global ltemp2; ltemp2 = val
			elif (t[1] == "D"): global depth2; depth2 = val
			elif (t[1] == "B"): global ind2; ind2 = val; global indicator2; indicator2 = val;
		elif (t[0] == "3"):
			print("Engine 3")
			if (t[1] == "A"): global directProp3; directProp3 = val
			elif (t[1] == "E"): global speedEngine3; speedEngine3 = val
			elif (t[1] == "P"): global speedPropeler3; speedPropeler3 = val 
			elif (t[1] == "F"): global lfuel3; lfuel3 = val
			elif (t[1] == "T"): global ltemp3; ltemp3 = val
			elif (t[1] == "D"): global depth3; depth3 = val
			elif (t[1] == "B"): global ind3; ind3 = val; global indicator3; indicator3 = val;
		elif (t[0] == "4"):
			print("Engine 4")
			if (t[1] == "A"): global directProp4; directProp4 = val
			elif (t[1] == "E"): global speedEngine4; speedEngine4 = val
			elif (t[1] == "P"): global speedPropeler4; speedPropeler4 = val 
			elif (t[1] == "F"): global lfuel4; lfuel4 = val
			elif (t[1] == "T"): global ltemp4; ltemp4 = val
			elif (t[1] == "D"): global depth4; depth4 = val
			elif (t[1] == "B"): global ind4; ind4 = val; global indicator4; indicator4 = val;
		elif (t[0] == "W"):
			print("Wind Condition")
			if (t[4] == "D"): global directWind; directWind = val; 
			elif (t[4] == "S"): global speedWind; speedWind = val; 
			else: global windInd ; windInd = val; global indicator5; indicator5 = val;
		elif (t[0] == "s"): 
			print("Joystick Input")
			if ((t[9] == "1") and (t[8] == "d")) : global val_speed1; val_speed1 = val
			elif ((t[9] == "3") and (t[8] == "d")): global val_speed2; val_speed2 = val
			elif ((t[9] == "1") and (t[8] == "r")): global val_steer1; val_steer1 = val 
			elif ((t[9] == "3") and (t[8] == "r")): global val_steer2; val_steer2 = val
		else: global ind6 ; ind6 = val; global indicator6; indicator6 = val;
if __name__ == "__main__":

	##Mosquitto Mqtt Configuration			
	client= paho.Client("GUI")
	client.on_message=on_message

	print("connecting to broker ",broker)
	client.connect(broker,port)#connect
	print(broker," connected")
	
	client.loop_start()
	print("Subscribing")
	client.subscribe("1Azimuth")#subscribe
	client.subscribe("1EngineSpeed")#subscribe
	client.subscribe("1PropellerSpeed")#subscribe
	client.subscribe("1Temperature")#subscribe
	client.subscribe("1Fuel")#subscribe
	client.subscribe("1Depth")#subscribe
	client.subscribe("1Box")#subscribe
	print("Box Engine 1 subscribed ")
	
	client.subscribe("2Azimuth")#subscribe
	client.subscribe("2EngineSpeed")#subscribe
	client.subscribe("2PropellerSpeed")#subscribe
	client.subscribe("2Temperature")#subscribe
	client.subscribe("2Fuel")#subscribe
	client.subscribe("2Depth")#subscribe
	client.subscribe("2Box")#subscribe
	print("Box Engine 2 subscribed ")
	
	client.subscribe("3Azimuth")#subscribe
	client.subscribe("3EngineSpeed")#subscribe
	client.subscribe("3PropellerSpeed")#subscribe
	client.subscribe("3Temperature")#subscribe
	client.subscribe("3Fuel")#subscribe
	client.subscribe("3Depth")#subscribe
	client.subscribe("3Box")#subscribe
	print("Box Engine 3 subscribed ")
	
	client.subscribe("4Azimuth")#subscribe
	client.subscribe("4EngineSpeed")#subscribe
	client.subscribe("4PropellerSpeed")#subscribe
	client.subscribe("4Temperature")#subscribe
	client.subscribe("4Fuel")#subscribe
	client.subscribe("4Depth")#subscribe
	client.subscribe("4Box")#subscribe
	print("Box Engine 4 subscribed ")
	
	client.subscribe("WindDirection")#subscribe
	client.subscribe("WindSpeed")#subscribe
	client.subscribe("WindBox")#subscribe
	print("Box Wind subscribed ")
	
	client.subscribe("spc_speed1")#subscribe
	client.subscribe("spc_speed3")#subscribe
	client.subscribe("spc_steer1")#subscribe
	client.subscribe("spc_steer3")#subscribe
	client.subscribe("ControlBox")#subscribe
	print("Box Controller subscribed ")
	
	client.publish("MainControl", "active")#publish

	## QT5 GUI
	print("Graphical User Interface ")
	app = QApplication(sys.argv)
		
	engine = QQmlApplicationEngine() 
	#ctx = engine.rootContext()
	engine.load('main.qml')
	win = engine.rootObjects()[0]
	engine.quit.connect(app.quit) ## Quit Button Respon

	mqttvalue = MQTTValue()

	timer = QTimer()
	timer.start(10) ##Update screen every 10 miliseconds

	context = engine.rootContext()
	context.setContextProperty("mqttvalue", mqttvalue)

	timer.timeout.connect(win.updateValue) ##Call function update in GUI QML
		
	win.show()

	sys.exit(app.exec_())
