#!/usr/bin/python3
# -*- coding: utf-8 -*-

import cgi
import cgitb
import os
import string
import hashlib
import pickle


cgitb.enable()

print('Cache-Control: no-cache')
print('Content-type: application/xhtml+xml; charset=utf-8')
print('')


data = dict()
data['username'] = 'Obligatoire'
data['useremail'] = 'Obligatoire'
data['userpwd'] = 'Obligatoire'
data['firstname'] = 'Facultatif'
data['lastname'] = 'Facultatif'
data['birthdate'] = 'Facultatif'

form = cgi.FieldStorage()
for name in list(form.keys()):
	if name.lower() == 'userpwd':
		data[name] = hashlib.sha512(form.getfirst(name).encode('utf-8')).hexdigest()
	else:
		data[name] = form.getfirst(name)


data_file = open('../data/user.dat','wb')
data_file.write(pickle.dumps(data))
data_file.close()

template_file = open('../templates/register.xhtml','r')
template = string.Template(template_file.read())
template_file.close()

print((template.safe_substitute(data)))

