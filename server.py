# In every route which requires login , just put if logged_in:
# Logout button that send request to /logout


from flask import Flask, render_template, url_for
from flask import jsonify, request
from flask import flash, redirect,abort
import paho.mqtt.client as mqttClient
import time
import random

import datetime as d
app = Flask(__name__)
lastOffTime = d.datetime.now()
netlitres = 0
prevtank1 = 10
diff = 0
x = 0
logged_in = False

def on_connect(client, userdata, flags, rc):
    if rc == 0:
        global Connected                #Use global variable
        Connected = True                #Signal connection 
        client.subscribe("WaterManagement/LevelData",0)  
    else:
        print("Connection failed")

def on_message(client, userdata, message):
   requestTopic = message.topic
   payload=message.payload.decode()
   print("message received")
   global tank5data
   tank5data = payload
#  print(payload)
   requestID = requestTopic.split('/')[3]       # obtain requestID as last field from the topic
   lTime = strftime('%H:%M:%S', localtime())
   client.publish((responseTopic + requestID), payload=lTime, qos=0, retain=False)

# MQTT connection
Connected = False #global variable for the state of the connection
broker_address= "m15.cloudmqtt.com"
port = 15400
user = "bhjdgsiu"
password = "rPoyUPthnjdI"

client = mqttClient.Client("Python")
client.username_pw_set(user, password=password)
client.on_connect = on_connect
# client.on_m6wessage = on_message
client.connect(broker_address, port=port)
client.loop_start()
while Connected != True:    #Wait for connection
    time.sleep(0.1)

# try:
#     while True:
#         value = input('Enter the message:')
#         client.publish("WaterManagement/Control",value)
 
# except KeyboardInterrupt:
#     client.disconnect()
#     client.loop_stop()

power =0
tank1data = 50
tank5data = 0
tank2data = random.randint(0,40)
current =  "OFF"
option = ""
ontime = 0
stat1 = ""
stat2 = ""
stat3 = ""

'''@app.route('/',  methods=['GET'])
def index():

    global tank1_level
    return render_template('index_gauge.html')  '''


@app.route('/')
def home():
    if logged_in:
        return render_template('home.html')
    else:
        return redirect('/login')
    lastOffTime = d.datetime.now()

@app.route('/login', methods=['GET'])
def login_page():
    return render_template('login.html')
@app.route('/logout', methods=['GET'])
def logout():
    global logged_in
    logged_in = False
    return render_template('login.html')
@app.route('/login', methods=['POST'])
def check_login():
    global logged_in
    if request.form['password'] == 'password' and request.form['username'] == 'admin':
        logged_in = True
        return redirect('/')
    else:
        return render_template('login.html')


@app.route('/water', methods=['GET'])
def water():
    if logged_in:
        return render_template('index_gauge.html')
    else:
        return redirect('/login')


@app.route('/energy', methods=['GET'])
def energy():
    if logged_in:
        return render_template('energy.html')
    else:
        return redirect('/login')

@app.route('/viz', methods=['GET'])
def viz():
    if logged_in:
        return render_template('viz.html')
    else:
        return redirect('/login')

'''@app.route('/power/<int:p>', methods=['GET'])
def power():
    global power
    power = p
    return 'ok' '''

@app.route('/deptho/<int:depth_cm1>', methods=['GET'])
def show_post1(depth_cm1):
    global tank1data
    global netlitres
    global prevtank1
    global diff
    if depth_cm1 < prevtank1:
        diff = (prevtank1 - depth_cm1)
    tank1data = depth_cm1
    prevtank1 = depth_cm1
    netlitres = netlitres + diff
    return 'ok'

@app.route('/power/<int:p>', methods=['GET'])
def power(p):
    global power
    power = p
    return 'ok'



@app.route('/change/<string:switch>', methods=['GET','POST'])
def change(switch):
    global option
    option = switch
    print(switch)
    client.publish("WaterManagement/remote",option)
    return option


@app.route('/stat/<int:p>', methods=['GET'])
def status(p):
    global ontime
    global tank1data
    global tank2data
    global current
    global option
    global stat1,stat2,stat3
    global power
    power = p
    s1 =""
    s2 =""
    s3 = ""

    if tank1data<20 and tank2data>20 and option == "auto":
        s1 = "xyz"
        stat1="a"
        current = "On"
    elif option == "on":
        s2 = "abc"
        stat2 = "a"
    elif option == "off":
        s2 = ""
        stat2 = ""
    elif option == "a2on":
        s3 = "pqr"
        stat3 = "a"
    elif option == "a2off":
        s3 = ""
        stat3 = ""
    elif option == "a1on":
        s1 = "xyz"
        stat1 = "a"
        current = "On"
    elif option == "a1off":
        s1 = ""
        stat1  = ""
        current = "Off"
    pfinal = s1 + s2 + s3
    if stat1 =="a":
        pfinal = pfinal + "xyz"
    if stat2 == "a":
        pfinal = pfinal + "abc"
    if stat3 == "a":
        pfinal = pfinal + "pqr"
    final = pfinal + "$"
    return final



@app.route('/depths/<int:depth_cm2>', methods=['GET'])
def show_post2(depth_cm2):
    global tank2data
    tank2data = depth_cm2
    return 'ok'

@app.route('/return_global', methods=['GET'])
def return_global():
    client.on_message = on_message
    # print(tank5data)
    global tank5data
    global tank2data
    global current
    global ontime
    global lastOffTime
    global netlitres
    global x
    global power

    x = x+ 1
    if current == "OFF":
        lastOffTime = d.datetime.now()
    elif current == "On":
        if  0==0:
            f= 0
            diff = d.datetime.now() - lastOffTime
            lastOffTime = d.datetime.now()
            f = diff.microseconds
            ontime = ontime + f
    print(tank1data)
    return jsonify(tank1 = tank5data , tank2 = tank2data, stat = current, time = ontime/1000000, net= netlitres)


if __name__ == "__main__":

    app.run(host='0.0.0.0', port=8080, debug=True)
