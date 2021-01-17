#!/usr/bin/python3
# -*- coding: utf-8 -*-

import cgi
import cgitb
import os
import sys
import string
import pickle
from datetime import datetime
try:
    import json
except ImportError:
	sys.path.append(os.getcwd() + '/htbin/simplejson')
	import simplejson as json 

cgitb.enable()

print('Cache-Control: no-cache')
print('Content-type: application/json')
print('')


data_file = open('../data/chat.dat','r')

msglist = list()
for line in data_file:
	msglist.append(json.loads(line))

data_file.close()

print((json.dumps(msglist)))
