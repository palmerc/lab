import sys

class Lexer:
    def __init__(self, text = None):
        if text is None:
            text = sys.stdin
            
        self.text = iter(text)

    def next(self):
        # open file
        s = []
        token = ''
        for char in self.text:
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
                    char = self.text.next()
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
                    char = self.text.next()
                token = ''.join(s)
                if len(s) == 3:
                    return 'CHAR', token
                else:
                    raise "Error", "unexpected character", s
            elif char == '<':
                while not char.isspace():
                    s.append(char)
                    char = self.text.next()
                token = ''.join(s)
            elif char.islower():
                while not char.isspace():
                    s.append(char)
                    char = self.text.next()
                token = ''.join(s)
                return 'NONTERM', token
            elif char.isupper():
                while not char.isspace():
                    s.append(char)
                    char = self.text.next()
                token = ''.join(s)
                return 'TERM', token
            else:
                while not char.isspace():
                    s.append(char)
                    char = self.text.next()
                token = ''.join(s)
                print "Error unexpected token", token
