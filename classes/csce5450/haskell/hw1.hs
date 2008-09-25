-- Cameron Palmer, Jorge Reyes, Kaci Irvin
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
stringToTerm :: String -> Maybe (Term a)
stringToTerm cs = case pars_expr cs of
                  Just (t, []) -> Just t
                  _            -> Nothing
  where pars_expr cs = let (ident, rest) = span isAlphaNum cs
                       in case rest of
                         '(' : rest' -> do (ts, cs') <- pars_list rest'
                                           return (Term ident ts, cs')
                         _           -> return (Atom ident, rest)

        pars_list (c:cs) | c==')' = return ([],cs)
        pars_list (c:cs) = do (t, cs1) <- pars_expr (c:cs)
                              let cs2 = case cs1 of
                                    ',' : x -> x
                                    _       -> cs1
                              (ts,cs3) <- pars_list cs2
                              return ((t:ts),cs3)
        pars_list _ = Nothing

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
