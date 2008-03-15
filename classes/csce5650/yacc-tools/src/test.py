import unittest
from StringIO import StringIO
from lexer import Lexer
from parser import Parser
from grammar import Transform

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
        self.assertEquals(len(self.grammar_one.grammar.terminal_name), len(self.grammar_two.grammar.terminal_name))
        for (a, b) in zip(self.grammar_one.grammar.terminal_name, self.grammar_two.grammar.terminal_name):
            self.assertEquals(a, b)
    
    def test_productions(self):
        '''Parser Production Count'''
        self.assertEquals(len(self.grammar_one.grammar.production_list), len(self.grammar_two.grammar.production_list))
        for (a, b) in zip(self.grammar_one.grammar.production_list, self.grammar_two.grammar.production_list):
            self.assertEquals(a.ls, b.ls)

class LeftFactoringTestCase(unittest.TestCase):
    '''This test checks to make sure left factoring is working correctly'''

    def setUp(self):
        f = open('../grammars/' + self.file)
        G = f.read()
        f.close()
        
        f = open('../grammars/' + self.solution)
        G2 = f.read()
        f.close()
    
        self.grammar_one = Parser(G)
        t = Transform(self.grammar_one)
        t.lf()
        
        self.grammar_two = Parser(G2)
        
    def test_productions(self):
        '''LF Production Count'''
        self.assertEquals(len(self.grammar_one.grammar.production_list), len(self.grammar_two.grammar.production_list))
        for (a, b) in zip(self.grammar_one.grammar.production_list, self.grammar_two.grammar.production_list):
            self.assertEquals(a.ls, b.ls)
            self.assertEquals(len(a.rule_list), len(b.rule_list))            
            for (ra, rb) in zip(a.rule_list, b.rule_list):
                self.assertEquals(ra.rule, rb.rule)

class LeftRecursionTestCase(unittest.TestCase):
    '''This test checks to make sure left factoring is working correctly'''

    def setUp(self):
        f = open('../grammars/' + self.file)
        G = f.read()
        f.close()
        
        f = open('../grammars/' + self.solution)
        G2 = f.read()
        f.close()
    
        self.grammar_one = Parser(G)
        t = Transform(self.grammar_one)
        t.pa()
        
        self.grammar_two = Parser(G2)
        
    def test_productions(self):
        '''LR Production Count'''
        self.assertEquals(len(self.grammar_one.production_list), len(self.grammar_two.production_list))
        for (a, b) in zip(self.grammar_one.production_list, self.grammar_two.production_list):
            self.assertEquals(a.ls, b.ls)
            
class LFTestCaseForPostfixExpression(LeftFactoringTestCase):
    file = "postfix_expression.yacc"
    solution = "postfix_expression.yacc.lf"

class LFTestCaseForP178(LeftFactoringTestCase):
    file = "p178.yacc"
    solution = "p178.yacc.lf"
    
class LFTestCaseForC(LeftFactoringTestCase):
    file = "c.yacc"
    solution = "c.yacc.lf"

class LFTestCaseForTestLF(LeftFactoringTestCase):
    file = "testlf.yacc"
    solution = "testlf.yacc.lf"
    
class LRTestCaseForP178(LeftRecursionTestCase):
    file = "p178.yacc"
    solution = "p178.yacc.nolr"
    
class LRTestCaseForKLee(LeftRecursionTestCase):
    file = "klee.yacc"
    solution = "klee.yacc.nolr"

class ParserTestCaseForP178(ParserTestCase):
    file = "p178.yacc"

class ParserTestCaseForC(ParserTestCase):
    file = "c.yacc"

class ParserTestCaseForD(ParserTestCase):
    file = "d.yacc"

def suite():
    suite = unittest.TestSuite()
    suite.addTest(unittest.makeSuite(ParserTestCaseForC))
    suite.addTest(unittest.makeSuite(ParserTestCaseForD))
    suite.addTest(unittest.makeSuite(ParserTestCaseForP178))
    suite.addTest(unittest.makeSuite(LFTestCaseForPostfixExpression))
    suite.addTest(unittest.makeSuite(LFTestCaseForP178))
    suite.addTest(unittest.makeSuite(LFTestCaseForC))
    suite.addTest(unittest.makeSuite(LFTestCaseForTestLF))
    suite.addTest(unittest.makeSuite(LRTestCaseForP178))
    suite.addTest(unittest.makeSuite(LRTestCaseForKLee))

    return suite

if __name__ == '__main__':
    unittest.main(defaultTest='suite')