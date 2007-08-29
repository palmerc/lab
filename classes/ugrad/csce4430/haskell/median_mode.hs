import IO
import Data.Map
import Data.List
import Text.Printf

testmode :: IO ()
testmode = do
		printf "Mode is %d\n" mode
	where
		mode :: Int
		mode = head mostoften
	
		myeq :: [Int] -> [Int] -> Ordering
		myeq a b = (compare (length a) (length b))
    
		mostoften :: [Int]
		mostoften = maximumBy myeq (group (sort thelist))
		thelist :: [Int]
		thelist = [1,1,2,2,2,3,1,1]

testmedian :: IO ()
testmedian = do
    printf "Median is %d\n" (median thelist)
   where
    halfpoint :: [Int] -> Int
    halfpoint list = quot (length list) 2
    median :: [Int] -> Int
    
    median list = if (odd (length list)) then (list !! (halfpoint list))
             else quot ((list !! (halfpoint list)-1) + (list !! (halfpoint list))) 2
    thelist = [1,2,4,6,9,9]			

quicksort [] = []
quicksort (x:xs) = do
		quicksort [y | y <- xs, y < x]
		++ [x]
		++ quicksort [y| y <- xs, y >= x]
		
main :: IO ()
main = do
		testmedian
		testmode
		