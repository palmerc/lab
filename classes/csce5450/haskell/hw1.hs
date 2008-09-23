module Homework1 where

-- The provided data structure
-- data Tree a = Atom String
--	| Term String (Tree a) deriving (Eq,Ord,Read,Show)
data Tree = Term [Tree] deriving (Eq,Ord,Read,Show)

-- These are the sample trees
t1 = "f(one,g(two,three),h3(four))"
t2 = "term1(atom1,term2(atom2,atom3))"

-- Parse the string representation of the tree
stringToTree cs | newcs == [] = t where
	(t,newcs)=pars_expr cs

	pars_expr (c:cs) | c=='(' = ((Term ts),newcs) where
		(ts,newcs) = pars_list cs

	pars_list (c:cs) | c==')' = ([],cs)
	pars_list (c:cs) = ((t:ts),cs2) where
		(t,cs1)=pars_expr (c:cs)
		(ts,cs2)=pars_list cs1

-- Generate the string representation of the tree
-- toString :: Tree -> String
