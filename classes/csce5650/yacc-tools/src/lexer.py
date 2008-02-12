import sys
from Stack import Stack

def lex(grammar):
    # open file
    for char in grammar:
        s = Stack()
        while char.isspace(): grammar.next()
        
        if char == '|':
            return 'COLON'
        elif char == ';':
            return 'SEMI'
        elif char == '%':
            while not char.isspace():
                s.push(char)
                char = grammar.next()
            if s == '%token':
                return 'TOKEN'
            elif s == '%start':
                return 'START'
            elif s == '%%':
                return 'BLOCK'
        elif char == '\'':
            while not char.isspace():
                s.push(char)
                char = grammar.next()
            if len(s) == 3:
                return 'CHAR,', s
            else:
                raise "Error", "unexpected character", s
        elif char.islower():
            while not char.isspace():
                s.push(char)
                char = grammar.next()
            return 'NONTERM', s
        elif char.isupper():
            while not char.isspace():
                s.push(char)
                char = grammar.next()
            return 'TERM', s
        else:
            while not char.isspace():
                s.push(char)
                char = grammar.next()
            print "Error unexpected token", char
        
        
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

    token = lex(f)
    while token:
        print token
        token = lex(f)

if __name__ == "__main__":
    main(sys.argv[1:])