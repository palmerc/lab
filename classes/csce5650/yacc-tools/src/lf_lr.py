import sys
from parser import Parser
from grammar import Transform

def main(argv):
	# open file
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

	G = f.read()
	f.close()
       
	grammar = Parser(G)
	
	t = Transform(grammar)
	t.lf()
	t.pa()
	
	grammar.printer()
	
if __name__ == "__main__":
	main(sys.argv[1:])
