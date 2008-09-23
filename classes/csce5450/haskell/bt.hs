data T  = E | C T T
        deriving (Eq, Ord, Show, Read)
        
bt 1 = [E]
bt n = [C l r | (i,j) <- split n, l <- bt i, r <- bt j]

cat n = length (bt n)

cats n = map cat [1..n]

split n = [(i,n-i)|i<-[1..n-1]]
 
mytree = bt 6

main = do
  print "enter a tree:"
  t<-readLn :: IO T
  print t

