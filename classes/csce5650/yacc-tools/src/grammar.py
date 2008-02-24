import sys

class Rule:
    __slots__ = ('rule',)
    def __init__(self, rule):
        self.rule = rule
        
    #def __getitem__(self, key): return self.rule[key]
    #def __setitem__(self, key, item): self.rule[key] = item

class Production:
    __slots__ = ('ls', 'rule_list')
    def __init__(self, ls, rule_list):
        self.ls = ls
        self.rule_list = rule_list
    
    def __iter__(self):
        return iter(self.rule_list)
    
class Grammar:
    def __init__(self, production_list):
        self.start = None
        self.token_list = []
        self.production_list = production_list
    
    def __iter__(self):
        return iter(self.production_list)
        
    def add_token(self, token):
        self.token_list.append(token)
        
    def start_token(self, token):
        self.start = token
        
class Transform:
    suffix = '_prime'
    
    def __init__(self, grammar):
        self.grammar = grammar
        
    def lf(self):
        '''Left factor the grammar to combine rules with redundant starting non-terminals'''
        for A_i in self.grammar.production_list:
            for A_i_gamma in A_i.rule_list:
                print A_i_gamma.rule
                
        
    def pa(self):
        '''M.C. Paull's Algorithm for the removal of left recursion in a CFG
        input: a grammar with no cycles or epsilon productions
        output: an equivalent grammar with no left recursion'''
        i = 0
        # Go through the list of A_i productions
        prod_dict = dict([(x.ls, n) for n, x in enumerate(self.grammar.production_list)])
        num_productions = len(self.grammar.production_list)
        while i < num_productions:
            #print i, 'A_i:', self.grammar.production_list[i].ls
            print i
            # For the production prior to A_i in the list called A_j
                
            k = 0
            i_rule_list = self.grammar.production_list[i].rule_list
            num_i_rules = len(i_rule_list)
            while k < num_i_rules:
                
                #print k,'\t\trule:', self.grammar.production_list[i].rule_list[k].rule
                
                # Evaluate each rule to see if it's left most element is a match to a previous production
                # compare the first subpart of the rule to the jth production
                A_i_gamma = i_rule_list[k].rule
                j = prod_dict.get(A_i_gamma[0], 1e200)
                if j < i:
                    j_rule_list = self.grammar.production_list[j].rule_list
                    assert A_i_gamma[0] == self.grammar.production_list[j].ls
                    # Pop off the beginning that will be replaced
                    #print 'Popping', A_i_gamma[0]
                    A_i_gamma.pop(0)
                    # Remove the rule that will be replaced
                    #print 'Popping', self.grammar.production_list[i].rule_list[k].rule
                    del i_rule_list[k]
                    k -= 1
                    num_i_rules -= 1
                    l = 0
                    num_j_rules = len(self.grammar.production_list[j].rule_list)
                    while l < num_j_rules:
                        A_j_delta = j_rule_list[l].rule
                        #print '\t\t\tNew:', A_j_delta + A_i_gamma
                        i_rule_list.append(Rule(A_j_delta + A_i_gamma))
                        l += 1
                k += 1
            i += 1

class PrettyPrint:
    def __init__(self, grammar):
        self.grammar = grammar
        
    def printer(self, fo = None):
        if fo is None:
            fo = sys.stdout
            
        print >>fo, '%token', ' '.join(self.grammar.token_list)
        print >>fo, '%start', self.grammar.start
        print >>fo
        print >>fo, '%%'
        for p in self.grammar:
            print >>fo, p.ls
            print >>fo, '\t:',
            print >>fo, '\n\t| '.join(["%s" % (' '.join(v.rule)) for v in p.rule_list])
            print >>fo, '\t;'
        print >>fo, '%%'