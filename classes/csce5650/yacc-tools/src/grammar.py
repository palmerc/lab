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
        # For each production cycle through each rule and check for other rules in the same production that start with the same non-terminal 
        # If you find a rule with the same starting non-terminal save the rules off adding rule_prime to the end of the rule
        # Create a single matching rule that goes to the production : rule rule_prime ; 
        # Generate a new production rule_prime that has all the rules that were picked up
        
        prod_dict = dict([(x.ls, n) for n, x in enumerate(self.grammar.production_list)])
        i = 0
        # For each production
        for production in self.grammar.production_list:
            print 'production:', i
            i += 1
            # Go through each rule object in the current production
            for rule in production.rule_list:
                rule_prefix_token = rule.rule[0]
                # This is a quick check to see if the item is a non-terminal
                if prod_dict.get(rule_prefix_token) is not None:
                    # Set the prefix to be the non-terminal
                    new_production_name = rule_prefix_token + self.suffix 
                    new_rules = []
                    # Compare the other rules first element to the current rule and see if they have the same first token
                    for other in production.rule_list:
                        other_prefix_token = other.rule[0]
                        # This should check if we are looking at the same Rule object which should be skipped
                        if rule == other:
                            continue
                        # This is the test that checks if current rule matches the first token of the others
                        if rule_prefix_token == other_prefix_token:
                            # remove the beginning non-terminal
                            other.rule.pop(0)
                            # tack on the new production name
                            other.rule.append(new_production_name)
                            # Add the new rule to the list of rules for the new production
                            new_rules.append(other)
                            # del the rule
                            del other
                    if len(new_rules) > 0:
                        self.grammar.production_list.append(Production(new_production_name, new_rules))


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
            num_Ai_rules = len(self.grammar.production_list[i].rule_list)
            while k < num_Ai_rules:
                Ai_rule_k = Ai_rule_list[k].rule
                # This time we want to eliminate the immediate left recursion
                if Ai_rule_k[0] == Ai_ls:
                    immediate_lr = True
                    Ai_rule_k.pop(0)
                    del Ai_rule_list[k]
                    Ai_rule_k.append(Ai_ls_prime)
                    new_rule_list.append(Rule(Ai_rule_k))
                    num_Ai_rules -= 1
                    continue
                k += 1
            if immediate_lr == True:
                self.grammar.production_list.append(Production(Ai_ls_prime, new_rule_list))
                num_productions += 1
                for remaining in self.grammar.production_list[i].rule_list:
                    remaining.rule.append(Ai_ls_prime)
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
