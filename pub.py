#!/usr/bin/python3

import time
import sys
import zmq
import json
from collections import OrderedDict

context = zmq.Context()
sock = context.socket(zmq.PUB)
sock.bind("tcp://*:3333")
i = 0
msg = OrderedDict()

while True:
        msg["data"] = []
        msg["data"].append( {"version":i} )
        msg["name"]="ywkim"
        i=i+1
        prn = json.dumps(msg)
        print(prn)
        sock.send_string(prn)
        time.sleep(1)

