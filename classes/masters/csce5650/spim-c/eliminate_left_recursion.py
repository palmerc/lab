import sys
import re

terminals = {}
start = ""

def kil_LeR():
	# double for loop Page 177 of 1988 edition of Dragon book
	pass

def token(line):
	# read in the tokens of the yacc grammar
	for word in line.split():
		# I should never see a terminal twice but
		# I shall do something useful with the value field
		if terminals.has_key(word):
			terminals[word] += 1
		else:
			terminals[word] = 1

def productions(grammar):
	# Break production into parts and process with algorithm
	# Format:
	# production : rule1 | rule2 | ... | ruleN ;
	# rules will be comprised of terminals in single quotes
	# or all capital letters and non-terminals in lowercase
	print grammar

def main(argv):
	# open file
	try:
		filename = argv[0]
	except IndexError:
		print "Usage: %s grammar.yacc" % sys.argv[0]
		sys.exit(2)
		
	f = open(filename, "r")

	startToken = re.compile('^%token (.+)');
	startStart = re.compile('^%start (.+)');
	startProductions = re.compile('^%%');
	for line in f:
		if startToken.search(line):
			# Add tokens to dictionary
			m = startToken.match(line)
			token(m.group(1))
		elif startStart.search(line):
			# starting production discovered
			m = startStart.match(line)
			start = m.group(1)
		elif startProductions.search(line): 
			# process productions
			grammar = "" 
			line = f.next();
			while not startProductions.search(line):
				grammar += line
				line = f.next()
			productions(grammar)

if __name__ == "__main__":
	main(sys.argv[1:])
