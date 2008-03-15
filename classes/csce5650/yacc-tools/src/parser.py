from lexer import Lexer
from grammar import Rule
from grammar import Production
from grammar import Grammar
from grammar import Transform
from grammar import PrettyPrint

class Parser:
	def __init__(self, source):
		self.token = ''
		self.rule = []
		self.rule_list = []
		self.terminal_name = {}
		self.terminal_group = {}
	
		self.start_token = ''
		self.grammar = None
		
		self.l = Lexer(source)
		self.parse()
	
	def match(self, m):
		if m == self.token[0]:
			self.token = self.l.next()
		else:
			print 'match error', m
	
	def parse(self):
		self.grammar = Grammar()
		self.token = self.l.next()
		self.terminals()
		self.start()
		self.productions()
	
	def terminals(self):	
		group = 0
		while self.token[0] == 'TOKEN':
			self.match('TOKEN')
			while self.token[0] == 'TERM':
				self.grammar.add_terminal(self.token[1], group)
				self.match('TERM')
			group += 1
		
	def start(self):	
		self.match('START')
		self.start_token = self.token[1]
		self.match('NONTERM')
		
	def productions(self):
		self.match('BLOCK')
		while self.token[0] != 'BLOCK':
			self.left_side()
		self.grammar.start_token = self.start_token
		
	def left_side(self):
		self.rule_list = []
		production_name = self.token[1]
		self.match('NONTERM')
		self.match('COLON')
		self.right_side()
		self.grammar.add_production(production_name, self.rule_list)
		self.match('SEMI')

	def right_side(self):
		rule = []
		
		first = True
		while self.token[0] != 'SEMI':
			if self.token[0] == 'TERM':
				if first == True:
					first = False
				rule.append(self.token[1])
				self.match('TERM')
			elif self.token[0] == 'NONTERM':
				if first == True:
					first = False
				rule.append(self.token[1])
				self.match('NONTERM')
			elif self.token[0] == 'CHAR':
				if first == True:
					first = False
				rule.append(self.token[1])
				self.match('CHAR')
			elif self.token[0] == 'PIPE':
				if first == True:
					self.rule_list.append(Rule(['']))
					first = False
					self.match('PIPE')
				else:
					self.rule_list.append(Rule(rule))
					rule = []
					self.match('PIPE')
					if self.token[0] == 'PIPE':
						self.rule_list.append(Rule(['']))
						self.match(self.token[0])
					elif self.token[0] == 'SEMI':
						rule = ['']
			else:
				print 'right_side:', self.token
		self.rule_list.append(Rule(rule))
	
	def printer(self, fo = None):
		pp = PrettyPrint(self.grammar)
		pp.printer(fo)
