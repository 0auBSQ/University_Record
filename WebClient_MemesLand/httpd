#!/usr/bin/python3

import os
from http.server import HTTPServer
from http.server import CGIHTTPRequestHandler
from random import randint

os.chdir("www")

port = randint(8000,8999)

print(("Le port du serveur est %d" % (port) ))

serv = HTTPServer(("", port), CGIHTTPRequestHandler)
serv.serve_forever()

