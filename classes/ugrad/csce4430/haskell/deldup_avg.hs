import IO
import Data.Map
import Data.List
import Text.Printf

testdeldup :: IO ()
testdeldup = do
		printf "Delete Duplicates %s\n" (show x)
	where
		x :: [Int]
		x = nub [1, 8, 8, 7, 6, 4, 4, 1, 3]

testavg :: IO ()
testavg = do
		printf "Average a list %f\n" x
   where
		y :: [Float]
		y = [1,2,3]
		x :: Float
		x = (sum y) / fromIntegral (length y)

main :: IO ()
main = do
		testavg
		testdeldup
