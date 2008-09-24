-- Cameron Palmer, Jorge Reyes, Kaci Irvin
module TreeThings where

-- The provided data structure
data Term a = Atom String
        | Term String [Term a] deriving (Eq,Ord,Read,Show)

-- These are the sample trees
s1 = "f(one,g(two,three),h3(four))"
s2 = "term1(atom1,term2(atom2,atom3))"
t1 = Term "f" [Atom "one", Term "g" [Atom "two", Atom "three"], Term "h3" [Atom "four"]]
t2 = Term "term1" [Atom "atom1", Term "term2" [Atom "atom2", Atom "atom3"]]

-- Parse the string representation of the tree
{-stringToTerm :: String -> Term a
stringToTerm x = Term x-}

-- Generate the string representation of the tree
termToString :: Term a -> String
termToString (Atom x) = x 
termToString (Term x xs) = x ++ "(" ++ (intercalate "," . fmap termToString $ xs) ++ ")"

intersperse :: a -> [a] -> [a]
intersperse _   []     = []
intersperse _   [x]    = [x]
intersperse sep (x:xs) = x : sep : intersperse sep xs

intercalate :: [a] -> [[a]] -> [a]
intercalate sep = concat . intersperse sep
