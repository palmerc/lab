module Homework1 where

-- The provided data structure
data Tree = Atom String
	| Term String [Tree] deriving (Eq,Ord,Read,Show)

-- These are the sample trees
s1 = "f(one,g(two,three),h3(four))"
s2 = "term1(atom1,term2(atom2,atom3))"
t1 = Term "f" [Atom "one", Term "g" [Atom "two", Atom "three"], Term "h3" [Atom "four"]]
t2 = Term "term1" [Atom "atom1", Term "term2" [Atom "atom2", Atom "atom3"]]

-- Parse the string representation of the tree
-- Generate the string representation of the tree

treeToString :: Tree -> String
treeToString (Atom x) = x
treeToString (Term x xs) = Term x (fmap treeToString xs)
