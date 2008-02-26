import sys

class Rule:
    __slots__ = ('rule',)
    
    def __init__(self, rule):
        self.rule = rule
        
class Production:
    __slots__ = ('ls', 'rule_list')
    
    def __init__(self, ls, rule_list):
        self.ls = ls
        self.rule_list = rule_list
    
    def __iter__(self):
        return iter(self.rule_list)
    
class Grammar:
    def __init__(self, production_list):
        self.start_token = None
        self.terminal_list = []
        self.production_list = production_list
    
    def __iter__(self):
        return iter(self.production_list)
        
    def add_terminal(self, token):
        self.terminal_list.append(token)
        
    def set_start(self, token):
        self.start_token = token
        
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
        
        # Provide a quick lookup for rules
        prod_dict = dict([(x.ls, n) for n, x in enumerate(self.grammar.production_list)])
        
        i = 0
        num_productions = len(self.grammar.production_list)
        while i < num_productions:                        
            # Assign the list of rules to i_rule_list for the current production
            Ai_ls = self.grammar.production_list[i].ls
            Ai_rule_list = self.grammar.production_list[i].rule_list
            num_Ai_rules = len(Ai_rule_list)
            
            k = 0
            while k < num_Ai_rules:
                # Each rule k in the current production must me checked
                # to see if the leftmost portion is a non-terminal that
                # matches a production we have already seen
                Ai_rule_k = Ai_rule_list[k].rule
                j = prod_dict.get(Ai_rule_k[0])
                if j < i and j is not None:
                    Aj_rule_list = self.grammar.production_list[j].rule_list
                    assert Ai_rule_k[0] == self.grammar.production_list[j].ls
                    # Pop off the leftmost non-terminal that will be replaced
                    Ai_rule_k.pop(0)
                    # Remove the rule that will be replaced and subtract one from the counters
                    del Ai_rule_list[k]
                    
                    l = 0
                    num_Aj_rules = len(self.grammar.production_list[j].rule_list)
                    num_Ai_rules = num_Aj_rules - 1
                    while l < num_Aj_rules:
                        Aj_rule_l = Aj_rule_list[l].rule
                        Ai_rule_list.insert(k, Rule(Aj_rule_l + Ai_rule_k))
                        k += 1
                        l += 1
                k += 1

            new_rule_list = []
            immediate_lr = False
            Ai_ls_prime = Ai_ls + self.suffix
            k = 0
            while k < num_Ai_rules:
                Ai_rule_k = Ai_rule_list[k].rule
                # This time we want to eliminate the immediate left recursion
                if Ai_rule_k[0] == Ai_ls:
                    immediate_lr = True
                    print 'Popping', Ai_rule_k[0]
                    Ai_rule_k.pop(0)
                    del Ai_rule_list[k]
                    Ai_rule_k.append(Ai_ls_prime)
                    new_rule_list.append(Ai_rule_k)
                    num_Ai_rules -= 1
                k += 1
            if immediate_lr == True:
                self.grammar.production_list.insert(i+1, Production(Ai_ls_prime, new_rule_list))
                num_productions += 1
                i += 1
            i += 1

class PrettyPrint:
    def __init__(self, grammar):
        self.grammar = grammar
        
    def printer(self, fo = None):
        if fo is None:
            fo = sys.stdout
        
        for tok in self.grammar.terminal_list:
            print >>fo, '%token', ' '.join(tok)
        print >>fo, '%start', self.grammar.start_token
        print >>fo
        print >>fo, '%%'
        for p in self.grammar.production_list:
            print >>fo, p.ls
            print >>fo, '\t:',
            print >>fo, '\n\t| '.join(["%s" % (' '.join(v.rule)) for v in p.rule_list])
            print >>fo, '\t;'
        print >>fo, '%%'