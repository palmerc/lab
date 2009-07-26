data T = C [T] deriving (Eq,Ord,Read,Show)

-- Sample input
t1 = "()"
t2 = "(()())"

parser :: String->T
parser cs = parse_pars '(' ')' cs

parse_pars :: Char->Char->String->T
parse_pars l r cs | newcs == [] = t where
  (t,newcs)=pars_expr l r cs

pars_expr :: Char -> Char -> String -> (T, String)
pars_expr l r (c:cs) | c==l = ((C ts),newcs) where 
     (ts,newcs) = pars_list l r cs

pars_list :: Char -> Char -> String -> ([T], String)     
pars_list l r (c:cs) | c==r = ([],cs)
pars_list l r (c:cs) = ((t:ts),cs2) where 
    (t,cs1)=pars_expr l r (c:cs)
    (ts,cs2)=pars_list l r cs1

printer = collect_pars "(" ")"

collect_pars l r (C ns) =
    l++ 
      (concatMap (collect_pars l r) ns)
    ++r 
