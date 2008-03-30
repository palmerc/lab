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
    def __init__(self):
        self.start_token = None
        self.terminal_group = {}
        self.terminal_name = {}
        self.production_name = {}
        self.production_list = []
        
    def __str__(self):
        view = ''
        for group in self.terminal_group:
            view += '%token '
            view += ' '.join(self.terminal_group[group])
            view += '\n'
                
        view += '%start ' + self.start_token + '\n'
        view += '%%\n'
        for production in self.production_list:
            view += production.ls + '\n'
            view += '\t: '
            view += '\n\t| '.join(["%s" % (' '.join(v.rule)) for v in production.rule_list])
            view += '\n\t;\n'
        view += '%%\n'
        return view
    
    def __iter__(self):
        return iter(self.production_list)
        
    def add_terminal(self, token, group):
        if not self.terminal_group.has_key(group):
            self.terminal_group[group] = []
            self.terminal_group[group].append(token)
        else:
            self.terminal_group[group].append(token)
        self.terminal_name[token] = group
    
    def add_production(self, ls, rule_list):
        self.production_list.append(Production(ls, rule_list))
        self.production_name[ls] = rule_list
              
    def set_start(self, token):
        self.start_token = token
        
class Transform:
    suffix = '_prime'
    
    def __init__(self, parse):
        self.grammar = parse.grammar    
                
    def lf(self):
        '''Left factor the grammar to combine rules with redundant starting non-terminals'''
        # For each production cycle through each rule and check for other rules in the same production that start with the same non-terminal 
        # If you find a rule with the same starting non-terminal save the rules off adding rule_prime to the end of the rule
        # Create a single matching rule that goes to the production : rule rule_prime ; 
        # Generate a new production rule_prime that has all the rules that were picked up

        # For each production
        for pindex, production in enumerate(self.grammar.production_list):
            matches = []
            # Identify the rules that begin with the same production, if any.
            for rindex, rule in enumerate(production.rule_list):
                for oindex, other in enumerate(production.rule_list):
                    if rule == other:
                        continue
                    elif len(rule.rule) == 0:
                        del rule
                    elif rule.rule[0] == other.rule[0] and self.grammar.production_name.get(rule.rule[0]) is not None:
                        if matches.count(rule.rule[0]) == 0:
                            matches.append(rule.rule[0])
            # if there are rules with duplicate productions
            if len(matches) > 0:
                # process each item in the match list
                for match in matches:
                    new_name = None
                    new_rules = []
                    removal_list = []
                    epsilon_removal = []
                    for rindex, rule in enumerate(production.rule_list):
                        # in each rule check the left hand non-terminal to see if it is in the match list
                        if rule.rule[0] == match:
                            # generate the new name for the production
                            new_name = production.ls + '_' + match + self.suffix
                            # pop off the leftmost non-terminal
                            if len(rule.rule) == 1:
                                rule.rule.append('')
                            rule.rule.pop(0)
                            new_rules.append(rule)
                            removal_list.append(rindex)
                    production.rule_list.append(Rule([match, new_name]))
                    if len(new_rules) > 0:
                        self.grammar.add_production(new_name, new_rules)
                        removal_list.sort()
                        removal_list.reverse()
                        for i in removal_list:
                            del production.rule_list[i]

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

class Analyze:
    def __init__(self, grammar):
        self.grammar = grammar.grammar
        
    def mark_epsilon(self):
        changes = True

        for P in self.grammar.production_list:
            P.derives_epsilon = False

        while changes:
            changes = False
            for P in self.grammar.production_list:
                RHS_derives_epsilon = True
                for rule in P.rule_list:
                    if rule.rule[0] == '':
                        epsilon = True
                    else:
                        epsilon = False
                    RHS_derives_epsilon = RHS_derives_epsilon and epsilon
                if RHS_derives_epsilon and not P.derives_epsilon:
                    changes = True
                    P.derives_epsilon = True
                    print 'yields epsilon:', P.ls
    def compute_first(self, x):
        k = len(x)
        if k == 0:
            result.append(lambda)
        else:
            result.append(first_set[x[1]])
            i = 1
        while i < k and lambda is in first_set[x[i]]:
            i += 1
            result.extend(first_set[x[i]] - lambda)
        if i == k and lambda is in first_set[x[k]]:
            result.append(lambda)
        return result
    
    def fill_first_set(self):
        for A in self.grammar.production_name:
            if DerivesLambda(A):
                first_set(A) = { lambda }
            else:
                first_set(A) = nullset
        for a in terminals:
            first_set(a) = { a }
            for A in nonterminal:
                if there exists a production A -> a...:
                    first_set(A) = first_set(A) + {a}
        while True:
            for p in productions:
                first_set(lhs(p)) = first_set(lhs(p)) + compute_first(rhs(p))
            if no changes:
                last

class PrettyPrint:
    def __init__(self, grammar):
        self.grammar = grammar
        
    def printer(self, fo = None):
        if fo is None:
            fo = sys.stdout
        
        for group in self.grammar.terminal_group:
            print >>fo, '%token',
            for tok in self.grammar.terminal_group[group]:
                print >>fo, tok,
            print >>fo
        print >>fo, '%start', self.grammar.start_token
        print >>fo
        print >>fo, '%%'
        for p in self.grammar.production_list:
            print >>fo, p.ls
            print >>fo, '\t:',
            print >>fo, '\n\t| '.join(["%s" % (' '.join(v.rule)) for v in p.rule_list])
            print >>fo, '\t;'
        print >>fo, '%%'
