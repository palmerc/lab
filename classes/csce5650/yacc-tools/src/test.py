import unittest
from StringIO import StringIO
from lexer import Lexer
from parser import Parser

class LexerTestCase(unittest.TestCase):
    '''This test checks the lexer'''
    
    def setUp(self):
        f = open('../grammars/' + self.file)
        G = f.read()
        f.close()
        
        self.grammar_one = Lexer(G)
        

class ParserTestCase(unittest.TestCase):
    '''This test check to make sure the internal representation matches the file read'''

    def setUp(self):
        f = open('../grammars/' + self.file)
        G = f.read()
        f.close()
    
        self.grammar_one = Parser(G)
        expected_s = StringIO()
        self.grammar_one.printer(expected_s)
        expected_s.seek(0, 0)
        self.grammar_two = Parser(expected_s.read())
   
    def test_start_token(self):
        '''%start nonterminal'''
        self.assertEquals(self.grammar_one.start_token, self.grammar_two.start_token)
        
    def test_terminals(self):
        '''%token terminals'''
        self.assertEquals(len(self.grammar_one.terminal_list), len(self.grammar_two.terminal_list))
        for (a, b) in zip(self.grammar_one.terminal_list, self.grammar_two.terminal_list):
            self.assertEquals(a, b)
    
    def test_productions(self):
        '''Productions'''
        self.assertEquals(len(self.grammar_one.production_list), len(self.grammar_two.production_list))
        for (a, b) in zip(self.grammar_one.production_list, self.grammar_two.production_list):
            self.assertEquals(a.ls, b.ls)

class ParserTestCaseForE(ParserTestCase):
    file = "p178.yacc"

class ParserTestCaseForC(ParserTestCase):
    file = "c.yacc"

class ParserTestCaseForD(ParserTestCase):
    file = "d.yacc"

def suite():
    suite = unittest.TestSuite()
    suite.addTest(unittest.makeSuite(ParserTestCaseForC))
    suite.addTest(unittest.makeSuite(ParserTestCaseForD))
    suite.addTest(unittest.makeSuite(ParserTestCaseForE))

    return suite
if __name__ == '__main__':
    unittest.main(defaultTest='suite')