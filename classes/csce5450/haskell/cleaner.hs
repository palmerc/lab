module TreeThings where

data T = C [T] deriving (Eq,Ord,Read,Show)
data Term a = Atom String
	| Term String [Term a] deriving (Eq,Ord,Read,Show)

s1 = "f(one,g(two,three),h3(four))"
s2 = "term1(atom1,term2(atom2,atom3))"
t1 = Term "f" [Atom "one", Term "g" [Atom "two", Atom "three"], Term "h3" [Atom "four"]]
t2 = Term "term1" [Atom "atom1", Term "term2" [Atom "atom2", Atom "atom3"]]

parse_pars cs = case pars_expr cs of
                  Just (t, []) -> Just t
                  _            -> Nothing
  where pars_expr (c:cs) | c=='(' = do (ts, cs') <- pars_list cs
                                       return (C ts, cs')
        pars_expr _ = Nothing

        pars_list (c:cs) | c==')' = return ([],cs)

        pars_list (c:cs) = do (t, cs1) <- pars_expr (c:cs)
                              (ts,cs2) <- pars_list cs1
                              return ((t:ts),cs2)
        pars_list _ = Nothing
