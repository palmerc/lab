import sys
from Stack import Stack

def lex(grammar):
    # open file
    s = []
    token = ''
    for char in grammar:
        if char.isspace():
            continue       
        elif char == '|':
            return 'PIPE', char
        elif char == ':':
            return 'COLON', char
        elif char == ';':
            return 'SEMI', char
        elif char == '%':
            while not char.isspace():
                s.append(char)
                char = grammar.next()
            token = ''.join(s)
            if token == '%token':
                return 'TOKEN', token
            elif token == '%start':
                return 'START', token
            elif token == '%%':
                return 'BLOCK', token
        elif char == '\'':
            while not char.isspace():
                s.append(char)
                char = grammar.next()
            token = ''.join(s)
            if len(s) == 3:
                return 'CHAR', token
            else:
                raise "Error", "unexpected character", s
        elif char.islower():
            while not char.isspace():
                s.append(char)
                char = grammar.next()
            token = ''.join(s)
            return 'NONTERM', token
        elif char.isupper():
            while not char.isspace():
                s.append(char)
                char = grammar.next()
            token = ''.join(s)
            return 'TERM', token
        else:
            while not char.isspace():
                s.append(char)
                char = grammar.next()
            token = ''.join(s)
            print "Error unexpected token", token
        
        
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

    #G = ""
    #for line in f:
    #    if line.startswith('#'):
    #        continue
    #    G += line
    G = ''.join(line for line in f if not line.startswith('#'))
    f.close()
    
    #G = f.read()
    
    
    grammar = iter(G) 
    token = lex(grammar)
    block = 0
    while token:
        print token
        token = lex(grammar)
        if block == 0 and token[0] == 'BLOCK':
            block = 1
        elif block == 1 and token[0] == 'BLOCK':
            sys.exit(0)

if __name__ == "__main__":
    main(sys.argv[1:])
