import sys
import select
import socket
import re
from collections import deque
from Login import Login
from StaticData import StaticData
from QFields import QFields

PLATFORM = 'pyTrader'
VERSION = '1.0'
PROTOCOL = '2.0'
SERVER = '10.0.0.4'
PORT = 7780
USERNAME = 'cameron'
PASSWORD = 'sierra'
stocks = []

def connect():
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	s.connect((SERVER, PORT))
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

def parseSecurities(string):
	securities = string.split(':')
	for security in securities:
		print security
		elements = security.split(';')
		feedTicker = elements[0]
		twoHalves = feedTicker.split('/')
		feed = twoHalves[0]
		ticker = twoHalves[1]
		stocks.append((feed, ticker))
		#ticker = elements[1]
		name = elements[2]
		exchange = elements[3]
		twoHalves = exchange.split('[')
		exchangeName = twoHalves[0].strip()
		exchangeCode = twoHalves[1].strip(']')

		number = elements[4]
		type = elements[5]
		isin = elements[6]
		extendedIsin = elements[7]

def processLine(line):
	securities = re.compile(r'^Securities: (.+)$')
	
	result = securities.match(line)
	if result != None:
		string = result.group(1)
		print string
		parseSecurities(string)

def receiveLoop(s):
	buf = deque()
	line = ''
	while 1:
		data = s.recv(1024)
		print data
		chars = list(data)
		buf.extend(chars)

		try:
			char = buf.popleft()
			while char != '\n':
				line += char
				char = buf.popleft()
			if char == '\n':
				processLine(line)
				line = ''
				
				for stock in stocks:
					staticData = StaticData()
					staticData.username = USERNAME
					staticData.feedNumber = stock[0]
					staticData.tickerSymbol = stock[1]
					print staticData
					s.send(str(staticData))
		except IndexError:
			pass
				
def main():
	try:
		s = connect()
		login(s)
		streamFields(s)
		receiveLoop(s)
	except KeyboardInterrupt:
		print 'Exiting the application...'

		#s.close()
		sys.exit()

if __name__ == "__main__":
	main()
