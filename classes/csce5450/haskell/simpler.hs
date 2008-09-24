data T = C [T] deriving (Eq,Ord,Read,Show)

parser cs = parse_pars cs

parse_pars cs | newcs == [] = t where
  (t,newcs)=pars_expr cs

  pars_expr (c:cs) | c=='(' = ((C ts),newcs) where 
     (ts,newcs) = pars_list cs
     
  pars_list (c:cs) | c==')' = ([],cs)

  pars_list (c:cs) = ((t:ts),cs2) where 
    (t,cs1)=pars_expr (c:cs)
    (ts,cs2)=pars_list cs1

printer = collect_pars where
  collect_pars (C ns) =
    "("++ 
      (concatMap collect_pars ns)
    ++")" 
