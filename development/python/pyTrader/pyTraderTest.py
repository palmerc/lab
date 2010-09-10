import sys
import select
import socket
from Login import Login
from StaticData import StaticData
from QFields import QFields

PLATFORM = 'pyTrader'
VERSION = '1.0'
PROTOCOL = '2.0'
SERVER = 'wireless.theonlinetrader.com'
PORT = 7780
USERNAME = 'cameron'
PASSWORD = 'sierra'

def isData():
	return select.select([sys.stdin], [], [], 0) == ([sys.stdin], [], [])

def connect():
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	s.connect((SERVER, PORT))
	s.setblocking(0)
	return s

def login(s):
	l = Login()
	l.username = USERNAME
	l.password = PASSWORD
	l.platform = PLATFORM
	l.version = VERSION
	l.protocol = PROTOCOL
	s.send(str(l))

def streamFields(s):
	q = QFields()
	q.username = USERNAME
	s.send(str(q))

def receiveLoop(s):
	buf = [] 
	while 1:
		data = s.recv(1024)
		chars = data.split('')
		buf.extend(chars)
		print buf

		if isData():
			c = sys.stdin.read(1)
			if c == '\x1b':
				break

def main():
	s = connect()
	login(s)
	streamFields(s)
	receiveLoop(s)
	s.close()

if __name__ == "__main__":
	main()
