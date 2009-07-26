data T = C [T] deriving (Eq,Ord,Read,Show)

parser cs = parse_pars '(' ')' cs

parse_pars l r cs | newcs == [] = t where
  (t,newcs)=pars_expr l r cs

  pars_expr l r (c:cs) | c==l = ((C ts),newcs) where 
     (ts,newcs) = pars_list l r cs
     
  pars_list l r (c:cs) | c==r = ([],cs)
  pars_list l r (c:cs) = ((t:ts),cs2) where 
    (t,cs1)=pars_expr l r (c:cs)
    (ts,cs2)=pars_list l r cs1

printer = collect_pars "(" ")" where
  collect_pars l r (C ns) =
    l++ 
      (concatMap (collect_pars l r) ns)
    ++r 
