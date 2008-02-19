import sys
from lexer import lex

token = None
grammar = None

def yaccgrammar():
	tokens()
	start()
	productions()

def tokens():
	while token[0] == 'TOKEN':
		match('TOKEN')
		while token[0] == 'TERM':
			match('TERM')
	
def start():
	match('START')
	match('NONTERM')
	
def productions():
	match('BLOCK')
	while token != 'BLOCK':
		left_side()
	match('BLOCK')
	
def left_side():
	match('NONTERM')
	match('COLON')
	right_side()

def right_side():
	if 
	match('TERM')
	expression()
	match('PIPE')
	if token == 'NONTERM':
		match('NONTERM')
	match('SEMI')
	
def match(m):
	global token, grammar
	
	print token
	if m == token[0]:
		print m
		token = lex(grammar)
	else:
		print 'error', m


def parser():
	global token, grammar
	
	token = lex(grammar)
	yaccgrammar()
        
def main(argv):
	global grammar
	
	try:
		filename = argv[0]
	except IndexError:
		print "Usage: %s grammar.yacc" % sys.argv[0]
		sys.exit(2)
    
	try:
		f = open(filename, "r")
	except IOError:
		print "Unable to open file", filename
		sys.exit(2)

	G = ""
	for line in f:
		G += line
	f.close()
      
	grammar = iter(G)
    
	parser()


if __name__ == "__main__":
    main(sys.argv[1:])
