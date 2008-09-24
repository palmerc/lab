module TreeThings where

import Data.Char

-- The provided data structure
data Term a = Atom String
        | Term String [Term a] deriving (Eq,Ord,Read,Show)

-- These are the sample trees
s1 = "f(one,g(two,three),h3(four))"
s2 = "term1(atom1,term2(atom2,atom3))"
t1 = Term "f" [Atom "one", Term "g" [Atom "two", Atom "three"], Term "h3" [Atom "four"]]
t2 = Term "term1" [Atom "atom1", Term "term2" [Atom "atom2", Atom "atom3"]]

-- Parse the string representation of the tree
--stringToTerm :: String -> Term a
split p [] = []
split p s = p1 : split (not . p) p2 where
	(p1,p2) = span p s
