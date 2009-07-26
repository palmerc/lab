-- a Generic Genetic Algorithm Engine in Haskell
-- usage: ghci ga.hs, then type ga (or main, just for the best solution)
-- customize fitness_fun and breading_fun for other problems

module GA where
import Data.List
import Random

-- organism
type Org = Integer

-- population 
data Pop = Pop [Int] [Org] deriving (Eq,Ord,Show,Read)
 
-- creates population
newPop popsize maxsteps seed maxran largest = Pop rs orgs where
  all_rans=newRandoms seed maxran
  rs=if maxsteps==0 
        then all_rans 
        else genericTake (popsize*maxsteps) all_rans
  ranorgs=newRandoms (maxsteps+seed+maxran) largest
  orgs=genericTake popsize ranorgs

-- provides a lazy infinite stream of random numbers
-- to be used as needed by the GP mechanism
newRandoms seed largest = 
    randomRs (0,largest) (mkStdGen seed)
    
-- filter the fittest using random choices rs and organisms orgs  
filterFits fitness breeding rs orgs = map snd fs where
  l=length orgs
  is=[0..l-1]
  xs=nub $ foldl (++) [] (ranBreed breeding is rs orgs)
  fs=genericTake l $ (sort . nub) $ zip (map fitness xs) xs

-- selects subpopulation of gender g from orgs, 
-- driven by random choices rs, using indices in is
gender g is rs orgs = [orgs!!i|i<-is,g==(rs!!i) `mod` 2]

-- applies breeding function to random selection of opposite genders
-- note that only genders 0 and 1 breed, 2,3,..etc. do not
ranBreed breeding is rs orgs = 
  [breeding x y|x<-gender 0 is rs orgs,y<-gender 1 is rs orgs]

-- evolution: recurses while randoms last 
-- or population dies off
evolve  _ _ p@(Pop [] _) = p
evolve  _ _ (Pop _ []) = Pop [] []
evolve fitness breading (Pop rs orgs) = 
  evolve fitness breading (Pop newrs fits) where
    l=genericLength orgs
    (rs0,newrs)=genericSplitAt l rs
    fits=filterFits fitness breading rs orgs

-- picks the best of a population
best (Pop _ (p:_)) = p

run_ga :: (Ord a) =>
            (Org -> a) -> 
            (Org -> Org -> [Org]) -> 
            Int -> Int -> Int -> Int -> Org -> Pop  
              
run_ga 
  fitness -- function to Ord type - to be minimized
  breeding -- function combining 2 organisms into 2 new ones
  popsize -- size of the population
  maxsteps -- number of generations
  seed -- initial random seed - makes it all replicable
  maxran -- sets range for picking candidates for breeding 
         -- if set to 4 we get 0,1 genders that breed 
         -- and 2,3 genders that do not participate in breeding
         -- with equal probability for each gender
  largest = 
    evolve fitness breeding 
     (newPop popsize maxsteps 
      seed maxran largest)

-- end of generic GA engine ---
  
-- a GA INSTANCE: INTEGER sqrt PROBLEM: --

-- fitness: the smaller the delta, the better 
--   slimness actually :-)
sqrt_fitness_fun x = y where
  input = 2008
  delta = input-x^2
  y = if delta>=0 then delta else input

-- breading rule - 2 offsprings
--   one more similar to the "mother"
--   one more similar to the "father"
sqrt_breeding_fun x y = [abs(2*x-y),abs(2*y-x)]  

-- simple genetic algorithm with parameters
ga = run_ga fitness breeding popsize maxsteps seed maxran largest where
  fitness = sqrt_fitness_fun
  breeding = sqrt_breeding_fun
  popsize=10
  maxsteps=3 -- increase for harder problems
  seed=13 -- change for different random runs
  maxran=1 -- if increased, a smaller part of the population will breed
  largest=100 -- increase for harder propblems


-- compile with ghc, then run the executable for harder problems 
main = (putStrLn . show) (best ga)
