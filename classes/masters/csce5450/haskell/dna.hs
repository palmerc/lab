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
     then DNAstrand P3x5 (nat2dna(n `div` 2))
     else DNAstrand P5x3 (nat2dna ((n+1) `div` 2))

dna_down :: DNA -> DNAstrand
dna_down = DNAstrand P3x5

dna_up :: DNA -> DNAstrand
dna_up = DNAstrand P5x3

data DoubleHelix = DoubleHelix DNAstrand DNAstrand

dna_double_helix :: DNA -> DoubleHelix
dna_double_helix s = 
  DoubleHelix (dna_up s) (dna_down s) 

to_base base n = d : 
  (if q==0 then [] else (to_base base q)) where
     (q,d) = quotRem n base

from_base base [] = 0
from_base base (x:xs) | x>=0 && x<base = 
   x+base*(from_base base xs)
