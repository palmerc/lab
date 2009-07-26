module FISO where
import Data.List
import Data.Bits
import Data.Char
import Ratio
import Random

data Iso a b = Iso (a->b) (b->a)

from (Iso f _) = f
to (Iso _ g) = g

compose :: Iso a b -> Iso b c -> Iso a c
compose (Iso f g) (Iso f' g') = Iso (f' . f) (g . g')
itself = Iso id id
invert (Iso f g) = Iso g f

borrow :: Iso t s -> (t -> t) -> s -> s
borrow (Iso f g) h x = f (h (g x))
borrow2 (Iso f g) h x y = f (h (g x) (g y))
borrowN (Iso f g) h xs = f (h (map g xs))

lend :: Iso s t -> (t -> t) -> s -> s
lend = borrow . invert
lend2 = borrow2 . invert
lendN = borrowN . invert

fit :: (b -> c) -> Iso a b -> a -> c
fit op iso x = op ((from iso) x)

retrofit :: (a -> c) -> Iso a b -> b -> c
retrofit op iso x = op ((to iso) x)

type Nat = Integer
type Root = [Nat]

type Encoder a = Iso a Root

with :: Encoder a->Encoder b->Iso a b
with this that = compose this (invert that)

as :: Encoder a -> Encoder b -> b -> a
as that this thing = to (with that this) thing

fun :: Encoder [Nat]
fun = itself

set :: Encoder [Nat]
set = Iso set2fun fun2set

set2fun is | is_set is = 
  map pred (genericTake l ys) where 
    ns=sort is
    l=genericLength ns
    next n | n>=0 = succ n
    xs =(map next ns)
    ys=(zipWith (-) (xs++[0]) (0:xs))

is_set ns = ns==nub ns

fun2set ns = 
  map pred (tail (scanl (+) 0 (map next ns))) where
  next n | n>=0 = succ n

nat_set = Iso nat2set set2nat 

nat2set n | n>=0 = nat2exps n 0 where
  nat2exps 0 _ = []
  nat2exps n x = 
    if (even n) then xs else (x:xs) where
      xs=nat2exps (n `div` 2) (succ x)

set2nat ns | is_set ns = sum (map (2^) ns)

nat :: Encoder Nat
nat = compose nat_set set

bits :: Encoder [Nat]
bits = compose (Iso bits2nat nat2bits) nat

nat2bits = drop_last . (to_base 2) . succ where
  drop_last bs=
    genericTake ((genericLength bs)-1) bs
    
to_base base n = d : 
  (if q==0 then [] else (to_base base q)) where
     (q,d) = quotRem n base
    
bits2nat bs = (from_base 2 (bs ++ [1]))-1

from_base base [] = 0
from_base base (x:xs) | x>=0 && x<base = 
   x+base*(from_base base xs)

b x = pred x -- begin
o x = 2*x+0  -- bit 0
i x = 2*x+1  -- bit 1
e = 1        -- end

data D = E | O D | I D deriving (Eq,Ord,Show,Read)
data B = B D deriving (Eq,Ord,Show,Read)

funbits2nat :: B -> Nat
funbits2nat = bfold b o i e

bfold fb fo fi fe (B d) = fb (dfold d) where
  dfold E = fe
  dfold (O x) = fo (dfold x)
  dfold (I x) = fi (dfold x)

b' x = succ x
o' x | even x = x `div` 2
i' x | odd x = (x-1) `div` 2
e' = 1

nat2funbits :: Nat -> B
nat2funbits = bunfold b' o' i' e'

bunfold fb fo fi fe x = B (dunfold (fb x)) where
  dunfold n | n==fe = E
  dunfold n | even n = O (dunfold (fo n))
  dunfold n | odd n = I (dunfold (fi n))

funbits :: Encoder B
funbits = compose (Iso funbits2nat nat2funbits) nat

bsucc (B d) = B (dsucc d) where
  dsucc E = O E
  dsucc (O x) = I x
  dsucc (I x) = O (dsucc x) 

data T = H Ts deriving (Eq,Ord,Read,Show)
type Ts = [T]

unrank f n = H (unranks f (f n))
unranks f ns = map (unrank f) ns

rank g (H ts) = g (ranks g ts)
ranks g ts = map (rank g) ts

tsize = rank (\xs->1 + (sum xs))

hylo :: Iso b [b] -> Iso T b
hylo (Iso f g) = Iso (rank g) (unrank f)

hylos :: Iso b [b] -> Iso Ts [b]
hylos (Iso f g) = Iso (ranks g) (unranks f)

hfs :: Encoder T
hfs = compose (hylo nat_set) nat

hfs_union = borrow2 (with set hfs) union
hfs_succ = borrow (with nat hfs) succ
hfs_pred = borrow (with nat hfs) pred

ackermann = as nat hfs
inverse_ackermann = as hfs nat

hff :: Encoder T
hff = compose (hylo nat) nat

data UT a = A a | F (UTs a) deriving (Eq,Ord,Read,Show)
type UTs a = [UT a]

ulimit = 4

unrankU _ n | n>=0 && n<ulimit = A n
unrankU f n = F (unranksU f (f (n-ulimit)))

unranksU f ns =  map (unrankU f) ns

rankU _ (A n) | n>=0 && n<ulimit = n
rankU g (F ts) = ulimit+(g (ranksU g ts))

ranksU g ts = map (rankU g) ts

hyloU (Iso f g) = Iso (rankU g) (unrankU f)
hylosU (Iso f g) = Iso (ranksU g) (unranksU f)

uhfs :: Encoder (UT Nat)
uhfs = compose (hyloU nat_set) nat

uhff :: Encoder (UT Nat)
uhff = compose (hyloU nat) nat

fr 0 = [0]
fr n = f 1 n where
   f _ 0 = []
   f j k = (k `mod` j) : 
           (f (j+1) (k `div` j))

fl = reverse . fr

rf ns = sum (zipWith (*) ns factorials) where
  factorials=scanl (*) 1 [1..]

lf = rf . reverse

perm2nth ps = (l,lf ls) where 
  ls=perm2lehmer ps
  l=genericLength ls

perm2lehmer [] = []
perm2lehmer (i:is) = l:(perm2lehmer is) where
  l=genericLength [j|j<-is,j<i]  

nth2perm (size,n) = 
  apply_lehmer2perm (zs++xs) [0..size-1] where 
    xs=fl n
    l=genericLength xs
    k=size-l
    zs=genericReplicate k 0

apply_lehmer2perm [] [] = []
apply_lehmer2perm (n:ns) ps@(x:xs) = 
   y : (apply_lehmer2perm ns ys) where
   (y,ys) = pick n ps

pick i xs = (x,ys++zs) where 
  (ys,(x:zs)) = genericSplitAt i xs

sf n = rf (genericReplicate n 1)

to_sf n = (k,n-m) where 
  k=pred (head [x|x<-[0..],sf x>n])
  m=sf k

nat2perm 0 = []
nat2perm n = nth2perm (to_sf n)

perm2nat ps = (sf l)+k where 
  (l,k) = perm2nth ps

perm :: Encoder [Nat]
perm = compose (Iso perm2nat nat2perm) nat

nat2hfp = unrank nat2perm
hfp2nat = rank perm2nat

hfp :: Encoder T
hfp = compose (Iso hfp2nat nat2hfp) nat

data Nat2 = P Nat Nat deriving (Eq,Ord,Read,Show)

bitpair ::  Nat2 -> Nat
bitpair (P i j) = 
  set2nat ((evens i) ++ (odds j)) where
    evens x = map (2*) (nat2set x)
    odds y = map succ (evens y)

bitunpair :: Nat->Nat2  
bitunpair n = P (f xs) (f ys) where 
  (xs,ys) = partition even (nat2set n)
  f = set2nat . (map (`div` 2))

nat2 :: Encoder Nat2
nat2 = compose (Iso bitpair bitunpair) nat

data BT a = B0 | B1 | D a (BT a) (BT a) 
             deriving (Eq,Ord,Read,Show)

data BDD a = BDD a (BT a) deriving (Eq,Ord,Read,Show)

unfold_bdd :: Nat2 -> BDD Nat
unfold_bdd (P n tt) = BDD n bt where 
  bt=if tt<max then shf bitunpair n tt
     else error 
       ("unfold_bdd: last arg "++ (show tt)++
       " should be < " ++ (show max))
     where max = 2^2^n

  shf _ n 0 | n<1 =  B0
  shf _ n 1 | n<1 =  B1
  shf f n tt = D k (shf f k tt1) (shf f k tt2) where
    k=pred n
    P tt1 tt2=f tt

fold_bdd :: BDD Nat -> Nat2
fold_bdd (BDD n bt) = 
  P n (rshf bitpair bt) where
    rshf rf B0 = 0
    rshf rf B1 = 1
    rshf rf (D _ l r) = 
      rf (P (rshf rf l) (rshf rf r))

eval_bdd (BDD n bt) = eval_with_mask (bigone n) n bt
 
eval_with_mask m _ B0 = 0 
eval_with_mask m _ B1 = m
eval_with_mask m n (D x l r) = 
  ite_ (var_mn m n x) 
         (eval_with_mask m n l) 
         (eval_with_mask m n r)
         
var_mn mask n k = mask `div` (2^(2^(n-k-1))+1)
bigone nvars = 2^2^nvars - 1         

ite_ x t e = ((t `xor` e).&.x) `xor` e

bsum 0 = 0
bsum n | n>0 = bsum1 (n-1)

bsum1 0 = 2
bsum1 n | n>0 = bsum1 (n-1)+ 2^2^n

bsums = map bsum [0..]

to_bsum n = (k,n-m) where 
  k=pred (head [x|x<-[0..],bsum x>n])
  m=bsum k

nat2bdd n = unfold_bdd (P k n_m)
  where (k,n_m)=to_bsum n

bdd2nat bdd@(BDD nv _) =  ((bsum nv)+tt) where
  P _ tt =fold_bdd bdd

pbdd :: Encoder (BDD Nat)
pbdd = compose (Iso bdd2nat nat2bdd) nat

ev_bdd2nat bdd@(BDD nv _) = (bsum nv)+(eval_bdd bdd)

bdd :: Encoder (BDD Nat)
bdd = compose (Iso ev_bdd2nat nat2bdd) nat

bdd_reduce (BDD n bt) = BDD n (reduce bt) where
  reduce B0 = B0
  reduce B1 = B1
  reduce (D _ l r) | l == r = reduce l
  reduce (D v l r) = D v (reduce l) (reduce r)

unfold_rbdd = bdd_reduce . unfold_bdd  

nat2rbdd = bdd_reduce . nat2bdd 

rbdd :: Encoder (BDD Nat)
rbdd = compose (Iso ev_bdd2nat nat2rbdd) nat

bdd_size (BDD _ t) = 1+(size t) where
  size B0 = 1
  size B1 = 1
  size (D _ l r) = 1+(size l)+(size r)

to_bdd vs tt | 0<=tt && tt <= m = 
  BDD n (to_bdd_mn vs tt m n) where
    n=genericLength vs
    m=bigone n
to_bdd _ tt = error 
   ("bad arg in to_bdd=>" ++ (show tt)) 

to_bdd_mn []      0 _ _ = B0
to_bdd_mn []      _ _ _ = B1
to_bdd_mn (v:vs) tt m n = D v l r where
  cond=var_mn m n v
  f0= (m `xor` cond) .&. tt
  f1= cond .&. tt 
  l=to_bdd_mn vs f1 m n
  r=to_bdd_mn vs f0 m n

to_rbdd vs tt = bdd_reduce (to_bdd vs tt)
from_bdd bdd = eval_bdd bdd

to_min_bdd n tt = snd $ foldl1 min 
 (map (sized_rbdd tt) (all_permutations n)) where
    sized_rbdd tt vs = (bdd_size b,b) where 
      b=to_rbdd vs tt
 
all_permutations n = if n==0 then [[]] else
  [nth2perm (n,i)|i<-[0..(factorial n)-1]] where
     factorial n=foldl1 (*) [1..n]

data MT a = L a | M a (MT a) (MT a) 
             deriving (Eq,Ord,Read,Show)
data MTBDD a = MTBDD a a (MT a) deriving (Show,Eq)

to_mtbdd m n tt = MTBDD m n r where 
  mlimit=2^m
  nlimit=2^n
  ttlimit=mlimit^nlimit
  r=if tt<ttlimit 
    then (to_mtbdd_ mlimit n tt)
    else error 
      ("bt: last arg "++ (show tt)++
      " should be < " ++ (show ttlimit))

to_mtbdd_ mlimit n tt|(n<1)&&(tt<mlimit) = L tt
to_mtbdd_ mlimit n tt = (M k l r) where 
   P x y=bitunpair tt
   k=pred n
   l=to_mtbdd_ mlimit k x
   r=to_mtbdd_ mlimit k y

from_mtbdd (MTBDD m n b) = from_mtbdd_ (2^m) n b

from_mtbdd_ mlimit n (L tt)|(n<1)&&(tt<mlimit)=tt
from_mtbdd_ mlimit n (M _ l r) = tt where 
   k=pred n
   x=from_mtbdd_ mlimit k l
   y=from_mtbdd_ mlimit k r
   tt=bitpair (P x y)

digraph2set ps = map bitpair ps
set2digraph ns = map bitunpair ns

digraph :: Encoder [Nat2]
digraph = compose (Iso digraph2set set2digraph) set

set2hypergraph = map nat2set
hypergraph2set = map set2nat

hypergraph :: Encoder [[Nat]]
hypergraph = 
  compose (Iso hypergraph2set set2hypergraph) set

dyadic :: Encoder (Ratio Nat)
dyadic = compose (Iso dyadic2set set2dyadic) set

set2dyadic :: [Nat] -> Ratio Nat
set2dyadic ns = rsum (map nexp2 ns) where
  nexp2 0 = 1%2
  nexp2 n = (nexp2 (n-1))*(1%2)

  rsum [] = 0%1
  rsum (x:xs) = x+(rsum xs)

dyadic2set :: Ratio Nat -> [Nat]
dyadic2set n | good_dyadic n = dyadic2exps n 0 where
  dyadic2exps 0 _ = []
  dyadic2exps n x = 
    if (d<1) then xs else (x:xs) where
      d = 2*n
      m = if d<1 then d else (pred d)
      xs=dyadic2exps m (succ x)
dyadic2set _ =  error 
  "dyadic2set: argument not a dyadic rational"

good_dyadic kn = (k==0 && n==1) 
  || ((kn>0%1) && (kn<1%1) && (is_exp2 n)) where 
    k=numerator kn
    n=denominator kn

    is_exp2 1 = True
    is_exp2 n | even n = is_exp2 (n `div` 2)
    is_exp2 n = False

dyadic_dist x y = abs (x-y)

dist_for t x y =  as dyadic t 
  (borrow2 (with dyadic t) dyadic_dist x y)
dsucc = borrow (with nat dyadic) succ
dplus = borrow2 (with nat dyadic) (+)

dconcat = lend2 dyadic (++)

string :: Encoder String
string = Iso string2fun fun2string

string2fun cs = map (fromIntegral . ord) cs

fun2string ns = map (chr . fromIntegral) ns 

pars :: Encoder [Char]
pars = compose (Iso pars2hff hff2pars) hff

pars2hff cs = parse_pars '(' ')' cs

parse_pars l r cs | newcs == [] = t where
  (t,newcs)=pars_expr l r cs

  pars_expr l r (c:cs) | c==l = ((H ts),newcs) where 
     (ts,newcs) = pars_list l r cs
     
  pars_list l r (c:cs) | c==r = ([],cs)
  pars_list l r (c:cs) = ((t:ts),cs2) where 
    (t,cs1)=pars_expr l r (c:cs)
    (ts,cs2)=pars_list l r cs1

hff2pars = collect_pars "(" ")" where
  collect_pars l r (H ns) =
    l++ 
      (concatMap (collect_pars l r) ns)
    ++r 

data Base = Adenine | Cytosine | Guanine | Thymine 
  deriving(Eq,Ord,Show,Read)

type DNA = [Base]

alphabet2code Adenine = 0
alphabet2code Cytosine = 1
alphabet2code Guanine = 2 
alphabet2code Thymine = 3

code2alphabet 0 = Adenine
code2alphabet 1 = Cytosine
code2alphabet 2 = Guanine 
code2alphabet 3 = Thymine

dna2nat  = (from_base 4) . (map alphabet2code)

nat2dna = (map code2alphabet) . (to_base 4)

dna :: Encoder DNA
dna = compose (Iso dna2nat nat2dna)  nat

dna_complement :: DNA -> DNA
dna_complement = map to_compl where
  to_compl Adenine = Thymine
  to_compl Cytosine = Guanine
  to_compl Guanine = Cytosine
  to_compl Thymine = Adenine

dna_reverse :: DNA -> DNA
dna_reverse = reverse

dna_comprev :: DNA -> DNA
dna_comprev = dna_complement . dna_reverse

data Polarity =  P3x5 | P5x3 
  deriving(Eq,Ord,Show,Read)

data DNAstrand = DNAstrand Polarity DNA 
  deriving(Eq,Ord,Show,Read)

strand2nat (DNAstrand polarity strand) = 
  add_polarity polarity (dna2nat strand) where 
    add_polarity P3x5 x = 2*x
    add_polarity P5x3 x = 2*x-1
    
nat2strand n =
  if even n 
     then DNAstrand P3x5 (nat2dna (n `div` 2))
     else DNAstrand P5x3 (nat2dna ((n+1) `div` 2))

dnaStrand :: Encoder DNAstrand
dnaStrand = compose (Iso strand2nat nat2strand) nat

dna_down :: DNA -> DNAstrand
dna_down = (DNAstrand P3x5) . dna_complement

dna_up :: DNA -> DNAstrand
dna_up = DNAstrand P5x3

data DoubleHelix = DoubleHelix DNAstrand DNAstrand
   deriving(Eq,Ord,Show,Read)

dna_double_helix :: DNA -> DoubleHelix
dna_double_helix s = 
  DoubleHelix (dna_up s) (dna_down s)

rannat = rand (2^50)

rand :: Nat->Nat->Nat
rand max seed = n where 
  (n,g)=
    randomR (0,max) (mkStdGen (fromIntegral seed))   

rantest :: Encoder t->Bool
rantest t = and (map (rantest1 t) [0..255])

rantest1 t n = x==(visit_as t x) where  x=rannat n

visit_as t = (to nat) . (from t) . (to t) . (from nat) 

isotest = and (map rt [0..19])

rt 0 = rantest nat
rt 1 = rantest fun
rt 2 = rantest set
rt 3 = rantest bits
rt 4 = rantest funbits
rt 5 = rantest hfs
rt 6 = rantest hff
rt 7 = rantest uhfs
rt 8 = rantest uhff
rt 9 = rantest nat2
rt 10 = rantest pbdd
rt 11 = rantest bdd
rt 12 = rantest rbdd
rt 13 = rantest digraph
rt 14 = rantest hypergraph
rt 15 = rantest dyadic
rt 16 = rantest string
rt 17 = rantest pars
rt 18 = rantest perm
rt 19 = rantest hfp

nth thing = as thing nat
nths thing = map (nth thing)
stream_of thing = nths thing [0..]

random_gen thing seed largest n = genericTake n
  (nths thing (rans seed largest))
  
rans seed largest = 
    randomRs (0,largest) (mkStdGen seed)

length_as t = fit genericLength (with nat t)
sum_as t = fit sum (with nat t)
size_as t = fit tsize (with nat t)

strange_sort = (from nat_set) . (to nat_set)


-- tests

refl1 x=
  as nat set $
  as set fun $
  as fun funbits $
  as funbits pbdd $
  as pbdd hfs $
  as hfs hff $
  as hff uhfs $
  as uhfs bits $
  as bits bdd $ 
  as bdd nat x

refl2 x=
  as nat bdd $
  as bdd bits $ 
  as bits uhfs $
  as uhfs hff $
  as hff hfs $
  as hfs pbdd $
  as pbdd funbits $
  as funbits fun $
  as fun set $
  as set nat x


cross = bitpair . cross2 . bitunpair

cross2 (P a b) = P ab ba where
  P a1 a2  = bitunpair a
  P b1 b2 = bitunpair b
  ab = bitpair (P a1 b2)
  ba = bitpair (P b1 a2)
  

