import sys
from lexer import lex
from grammar import Rule
from grammar import Production
from grammar import Grammar
from grammar import PrettyPrint
from grammar import Transform

class Parser:
	def __init__(self, source):
		self.token = ''
		self.rule = []
		self.rule_list = []
		self.production_list = []
	
		self.source = None
		self.start_token = ''
		self.terminal_list = []
		self.grammar = None
		
		self.source = iter(source)
		self.parser()
	
	def yygrammar(self):
		self.tokens()
		self.start()
		self.productions()
	
	def tokens(self):	
		while self.token[0] == 'TOKEN':
			self.match('TOKEN')
			while self.token[0] == 'TERM':
				self.terminal_list.append(self.token[1])
				self.match('TERM')
		
	def start(self):	
		self.match('START')
		self.start_token = self.token[1]
		self.match('NONTERM')
		
	def productions(self):
		self.match('BLOCK')
		while self.token[0] != 'BLOCK':
			self.left_side()
		self.grammar = Grammar(self.production_list)
		self.grammar.start = self.start_token
		self.grammar.tokens = self.terminal_list
		
	def left_side(self):
		self.rule_list = []
		production_name = self.token[1]
		self.match('NONTERM')
		self.match('COLON')
		self.right_side()
		self.rule_list.append(Rule(self.rule))
		self.production_list.append(Production(production_name, self.rule_list))
		self.match('SEMI')
	
	def right_side(self):
		self.rule = []
		
		while self.token[0] != 'SEMI':
			if self.token[0] == 'TERM':
				self.rule.append(self.token[1])
				self.match('TERM')
			elif self.token[0] == 'NONTERM':
				self.rule.append(self.token[1])
				self.match('NONTERM')
			elif self.token[0] == 'CHAR':
				self.rule.append(self.token[1])
				self.match('CHAR')
			elif self.token[0] == 'PIPE':
				self.rule_list.append(Rule(self.rule))
				self.match('PIPE')
				self.right_side()
			else:
				print 'right_side:', self.token
		
	def match(self, m):
		if m == self.token[0]:
			self.token = lex(self.source)
		else:
			print 'match error', m	
	
	def parser(self):
		self.token = lex(self.source)
		self.yygrammar()
	
	def lf(self):
		Transform(self.grammar).lf()
		
	def pa(self):
		Transform(self.grammar).pa()
	
	def printer(self, fo = None):
		PrettyPrint(self.grammar).printer(fo)
