import sys
from parser import parser

def kil_LeR():
	# double for loop Page 177 of 1988 edition of Dragon book
	pass

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

	G = ""
	for line in f:
		G += line
	f.close()
      
	source = iter(G)
    
	parser()
	
	PrettyPrint(grammar).printer()
	
if __name__ == "__main__":
	main(sys.argv[1:])
