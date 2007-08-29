import IO
import Data.Map
import Data.List
import Text.Printf

testmaximum :: IO ()
testmaximum = do
		printf "Maximum of list %d\n" x
   where
		x :: Int
		x = foldl1 max [1, 2, 3]

testminimum :: IO ()
testminimum = do
		printf "Minimum of list %d\n" x
	where
		x :: Int
		x = foldl1 min [1, 2, 3]

main :: IO ()
main = do
   testmaximum
   testminimum
