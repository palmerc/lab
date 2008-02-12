import sys
from lexer import lex

def parser():
	pass
        
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

    G = ""
    for line in f:
        G += line
    f.close()
    
    parser()

if __name__ == "__main__":
    main(sys.argv[1:])
