#!/usr/bin/python3

import time
import sys
import zmq
import json
import numpy as np
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
    datasize = dic['reply_params']['validdatasize']
    captime = dic['reply_params']['time']
    chno = dic['reply_params']['ch_name']
    data = dic['reply_params']['data']
    a = np.array(data)
    su = np.sum(a)
    avg = np.mean(a)
    mi = np.min(a)
    ma = np.max(a)
    std = np.std(a)
    p2p = ma - mi
    s = datetime.datetime.fromtimestamp(captime/1000)

    print(chno, s, 
          format(datasize, "5d"), format(mi,"8.2f"), 
          format(ma,"10.2f"), format( su, "10.2f"), format(p2p,"10.2f"), 
          format(avg,"10.4f"), format(std,"10.4f"))
