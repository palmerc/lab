import sys
from lexer import lex
from grammar import Rule
from grammar import Production
from grammar import Grammar
from grammar import PrettyPrint
from grammar import Transform

class Parser:
	__token = ''
	__rule = []
	__rule_list = []
	__production_list = []
	
	source = None
	start = ''
	terminal_list = []
	grammar = None
	
	def __init__(self, source):
		
		self.source = iter(source)
		self.parser()
	
	def yygrammar(self):
		self.tokens()
		self.start()
		self.productions()
	
	def tokens(self):	
		while self.__token[0] == 'TOKEN':
			self.match('TOKEN')
			while self.__token[0] == 'TERM':
				self.terminal_list.append(self.__token[1])
				self.match('TERM')
		
	def start(self):	
		self.match('START')
		self.start = self.__token[1]
		self.match('NONTERM')
		
	def productions(self):
		self.match('BLOCK')
		while self.__token[0] != 'BLOCK':
			self.left_side()
		self.grammar = Grammar(self.__production_list)
		self.grammar.start = self.start
		self.grammar.tokens = self.terminal_list
		
	def left_side(self):
		self.__rule_list = []
		production_name = self.__token[1]
		self.match('NONTERM')
		self.match('COLON')
		self.right_side()
		self.__rule_list.append(Rule(self.__rule))
		self.__production_list.append(Production(production_name, self.__rule_list))
		self.match('SEMI')
	
	def right_side(self):
		self.__rule = []
		
		while self.__token[0] != 'SEMI':
			if self.__token[0] == 'TERM':
				self.__rule.append(self.__token[1])
				self.match('TERM')
			elif self.__token[0] == 'NONTERM':
				self.__rule.append(self.__token[1])
				self.match('NONTERM')
			elif self.__token[0] == 'CHAR':
				self.__rule.append(self.__token[1])
				self.match('CHAR')
			elif self.__token[0] == 'PIPE':
				self.__rule_list.append(Rule(self.__rule))
				self.match('PIPE')
				self.right_side()
			else:
				print 'right_side:', self.__token
		
	def match(self, m):
		if m == self.__token[0]:
			self.__token = lex(self.source)
		else:
			print 'match error', m	
	
	def parser(self):
		self.__token = lex(self.source)
		self.yygrammar()
	
	def lf(self):
		Transform(self.grammar).lf()
		
	def pa(self):
		Transform(self.grammar).pa()
	
	def printer(self):
		PrettyPrint(self.grammar).printer()
	        
def main(argv):
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
    
	grammar = Parser(G)
	grammar.printer()

if __name__ == "__main__":
    main(sys.argv[1:])
