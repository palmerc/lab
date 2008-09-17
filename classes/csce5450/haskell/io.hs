main =
   do
      print "Enter something, please!"
      line<-getLine
      print (beautify line)

beautify line = "This is what you typed: >"++ line ++ "<"
