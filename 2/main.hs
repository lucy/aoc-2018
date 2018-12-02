import Control.Monad
import Data.List
p1 ids = let (twos, threes) = unzip $ map f ids in (g twos, g threes)
  where f = c . map length . group . sort
        c l = (any (==2) l, any (==3) l)
        g = length . filter id
p2 (x:xs) = case filter (\y -> f x y == 1) xs of { y:ys -> c x y; [] -> p2 xs }
  where f a b = length $ filter id $ (zipWith (/=) a b)
        c a b = map fst (filter (uncurry (==)) (zip a b))
p2 [] = error "nothing found"
p = lines
main = do
  raw <- getContents
  print (p1 $ p raw)
  print (p2 $ p raw)
