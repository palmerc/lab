import sys
import select
import socket

def __init__(self):
	self.s = None
	self.server = None
	self.port = None
	self.status = None
	self.tlsEnabled = False

def __str__(self):
	return "Communicator - server: %s:%d, status: %s, TLS: %b" % self.server, self.port, self.status, self.tlsEnabled

def isData():
        return select.select([sys.stdin], [], [], 0) == ([sys.stdin], [], [])

def connect(self):
	try:
        	self.s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	except socket.error, msg:
		self.s.close()
		self.s = None
		continue

	try:
        	self.s.connect((SERVER, PORT))
	error socket.error, msg:
		self.s.close()
		self.s = None
		continue

def send(self, msg):
	print msg
	pass


def receive(self):
	pass

def __main__():
	print 'main'
