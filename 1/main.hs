import qualified Data.IntSet as Set
import Data.List
p = map (read . dropWhile (=='+')) . lines
p1 = sum
p2 l =
  fst . head . filter (uncurry Set.member) . tail .
  scanl' (\(v, s) x -> (x + v, Set.insert v s)) (0, Set.empty) .
  cycle
main = do
  raw <- getContents
  print (p1 $ p raw)
  print (p2 $ p raw)
