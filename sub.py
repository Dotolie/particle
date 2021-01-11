#!/usr/bin/python3

import time
import sys
import zmq
import json
import datetime

# Socket to talk to server
context = zmq.Context()
socket = context.socket(zmq.SUB)
socket.connect ("tcp://localhost:3333")
socket.setsockopt_string(zmq.SUBSCRIBE, '')

# Process 5 updates
#for update_nbr in range (10):
while True:
    string = socket.recv_multipart()
    msg = string[0].decode()
    dic = json.loads(msg)
    sgp30 = dic['sgp30']
    ver=sgp30['version']
    type=sgp30['type']
    serial=sgp30['serial']
    ethanol=sgp30['ethanol']
    h2=sgp30['h2']
    tvoc=sgp30['tvoc']
    co2eq=sgp30['co2eq']
    s = datetime.datetime.fromtimestamp(time.time())

    print( "{},SGP30 ver={},type={},serial={},ethanol={},H2={},TVOC={}ppb,CO2eq={}ppm".
	format(s,ver,type,serial,ethanol,h2,tvoc,co2eq) ) 