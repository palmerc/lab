import sys
import re

def main(argv):
	usernameRE = re.compile(r'^\[(.+)\]$')
	clientRE = re.compile(r'^Client=(.+)$')
	platformRE = re.compile(r'^Platform=(.+)$')
	lastLoginRE = re.compile(r'^LastLogin=(.+)$')

	f = open(argv[0], 'r')
	username = ''
	users = {}

	for line in f:
		line = line.strip()
		if usernameRE.match(line):
			m = usernameRE.match(line)
			username = m.group(1)
			users[username]	= {}
		elif clientRE.match(line):
			m = clientRE.match(line)
			client = m.group(1)
			users[username]['client'] = client
		elif platformRE.match(line):
			m = platformRE.match(line)
			platform = m.group(1)
			users[username]['platform'] = platform
		elif lastLoginRE.match(line):
			m = lastLoginRE.match(line)
			lastLogin = m.group(1)
			users[username]['lastLogin'] = lastLogin

	y2008 = 0
	y2009 = 0
	y2010 = 0
	iPhones = 0
	iPods = 0
	iPads = 0
	blackberries = 0
	j2mes = 0
	for k in users.keys():
		if users[k].has_key('lastLogin'):
			date = users[k]['lastLogin']
			if re.search(r'2008', date) != None:
				y2008 += 1
			elif re.search(r'2009', date) != None:
				y2009 += 1
			elif re.search(r'2010', date) != None:
				y2010 += 1

		if (users[k].has_key('platform')) and (users[k].has_key('lastLogin')):
			date = users[k]['lastLogin']
			if re.search(r'2010', date) != None:
				platform = users[k]['platform']
				if re.search(r'iPhone', platform) != None:
					iPhones += 1
				elif re.search(r'iPod', platform) != None:
					iPods += 1
				elif re.search(r'iPad', platform) != None:
					iPads += 1
				elif re.search(r'RIM', platform) != None:
					blackberries += 1
				elif re.search(r'MIDP', platform) != None:
					j2mes += 1

	print '\tUsers in DB:', len(users)
	print '\tLast Login in 2008:', y2008
	print '\tLast Login in 2009:', y2009
	print '\tLast Login in 2010:', y2010

	print 'Totals for 2010:'
	print '\tiPhones:', iPhones
	print '\tiPods:', iPods
	print '\tiPads:', iPads
	print '\tBlackberries:', blackberries
	print '\tJ2ME:', j2mes

if __name__ == "__main__":
	main(sys.argv[1:])
