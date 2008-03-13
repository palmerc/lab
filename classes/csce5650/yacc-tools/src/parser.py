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
		self.production_list = []
	
		self.start_token = ''
		self.terminal_list = []
		self.grammar = None
		
		self.l = Lexer(source)
		self.parse()
	
	def match(self, m):
		if m == self.token[0]:
			self.token = self.l.next()
		else:
			print 'match error', m
	
	def parse(self):
		self.token = self.l.next()
		self.terminals()
		self.start()
		self.productions()
	
	def terminals(self):	
		while self.token[0] == 'TOKEN':
			tlist = []
			self.match('TOKEN')
			while self.token[0] == 'TERM':
				tlist.append(self.token[1])
				self.match('TERM')
			self.terminal_list.append(tlist)
		
	def start(self):	
		self.match('START')
		self.start_token = self.token[1]
		self.match('NONTERM')
		
	def productions(self):
		self.match('BLOCK')
		while self.token[0] != 'BLOCK':
			self.left_side()
		self.grammar = Grammar(self.production_list)
		self.grammar.start_token = self.start_token
		self.grammar.terminal_list = self.terminal_list
		
	def left_side(self):
		self.rule_list = []
		production_name = self.token[1]
		self.match('NONTERM')
		self.match('COLON')
		# If a token is a blank then it is an epsilon
		if self.token[0] == 'PIPE':
			self.rule_list.append(Rule(['']))
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
				# If a token is a blank then it is an epsilon
				if self.token[0] == 'PIPE' or self.token[0] == 'SEMI':
					self.rule_list.append(Rule(['']))
				self.right_side()
			else:
				print 'right_side:', self.token
	
	def printer(self, fo = None):
		pp = PrettyPrint(self.grammar)
		pp.printer(fo)
