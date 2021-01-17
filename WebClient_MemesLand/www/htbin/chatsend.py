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

data_file = open('../data/user.dat','rb')
data = pickle.loads(data_file.read())
data_file.close()


form_data = dict()
form = cgi.FieldStorage()
for name in list(form.keys()):
	form_data[name] = form.getfirst(name)


if not 'msg' in form_data:
	print((json.dumps(dict({'num': 1,'msg':'Erreur, le message est absent.'}))))
elif not 'username' in data:
	print((json.dumps(dict({'num': -1,'msg':'Erreur de lecture du nom d\'utilisateur'}))))
else:
	timestamp = datetime.now()
	msgjson = dict()
	msgjson['date'] = timestamp.strftime('%d/%m/%y')
	msgjson['time'] = timestamp.strftime('%H:%M')
	msgjson['user'] = data['username']
	msgjson['msg'] = form_data['msg']
	data_file = open('../data/chat.dat','a')
	data_file.write(json.dumps(msgjson))
	data_file.write('\n')
	data_file.close()
	print((json.dumps(dict({'num': 0,'msg':'Message envoyé.'}))))

