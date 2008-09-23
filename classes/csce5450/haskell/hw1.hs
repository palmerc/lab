module Homework1 where

-- The provided data structure
data Tree = Atom String
	| Term String [Tree] deriving (Eq,Ord,Read,Show)

-- These are the sample trees
t1 = "f(one,g(two,three),h3(four))"
t2 = "term1(atom1,term2(atom2,atom3))"

-- Parse the string representation of the tree
toTree :: String -> Tree
toTree tstring = 

-- Generate the string representation of the tree
toString :: Tree -> String
