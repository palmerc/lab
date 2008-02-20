import sys
from lexer import lex
from grammar import Rule
from grammar import Production
from grammar import Grammar
from grammar import PrettyPrint

source = None
token = None
grammar = None
rule = None
rules = []
pductions = []
terminals = []
start = ''

def yaccgrammar():
	tokens()
	start()
	productions()

def tokens():
	global terminals
	
	while token[0] == 'TOKEN':
		match('TOKEN')
		while token[0] == 'TERM':
			terminals.append(token[1])
			match('TERM')
	
def start():
	global start
	
	match('START')
	start = token[1]
	match('NONTERM')
	
def productions():
	global grammar, start, terminals
	
	match('BLOCK')
	while token[0] != 'BLOCK':
		left_side()
	grammar = Grammar(pductions)
	grammar.start = start
	grammar.tokens = terminals
	
def left_side():
	global rules
	
	rules = []
	production = token[1]
	match('NONTERM')
	match('COLON')
	right_side()
	rules.append(Rule(rule))
	pductions.append(Production(production, rules))
	match('SEMI')

def right_side():
	global rule
	rule = ''
	
	while token[0] != 'SEMI':
		if token[0] == 'TERM':
			rule += token[1]
			match('TERM')
		elif token[0] == 'NONTERM':
			rule += token[1]
			match('NONTERM')
		elif token[0] == 'CHAR':
			rule += token[1]
			match('CHAR')
		elif token[0] == 'PIPE':
			rules.append(Rule(rule))
			match('PIPE')
			right_side()
		else:
			print 'right_side:', token
		rule += ' '
	
def match(m):
	global token, source
	
	if m == token[0]:
		token = lex(source)
	else:
		print 'match error', m


def parser():
	global token, source
	
	token = lex(source)
	yaccgrammar()
        
def main(argv):
	global source, grammar
	
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
      
	source = iter(G)
    
	parser()
	
	PrettyPrint(grammar).printer()

if __name__ == "__main__":
    main(sys.argv[1:])
