module Homework1 where

-- The provided data structure
-- data Tree a = Atom String
--	| Term String (Tree a) deriving (Eq,Ord,Read,Show)
data Tree = Term [Tree] deriving (Eq,Ord,Read,Show)

-- These are the sample trees
t1 = "f(one,g(two,three),h3(four))"
t2 = "term1(atom1,term2(atom2,atom3))"

-- Parse the string representation of the tree
stringToTree chars | newcs == [] = t where
	(t,newcs)=pars_expr chars

	pars_expr (char:chars) | char=='(' = ((Term ts),newcs) where
		(ts,newcs) = pars_list chars

	pars_list (char:chars) | char==')' = ([],chars)

	pars_list (char:chars) = ((t:ts),chars2) where
		(t,chars1)=pars_expr (char:chars)
		(ts,chars2)=pars_list chars1

-- Generate the string representation of the tree
-- toString :: Tree -> String
