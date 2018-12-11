{-# LANGUAGE QuasiQuotes #-}
{-# LANGUAGE ViewPatterns #-}

import Text.Scanf
import Data.Semigroup
import qualified Data.Set as Set

data P a = P { x :: a, y :: a, xv :: a, yv :: a }

p :: String -> [P Int]
p = map pp . lines
pp (scanf [fmt|position=<%d, %d> velocity=<%d, %d>|] ->
   (Just (x :+ y :+ xv :+ yv :+ ()))) = P x y xv yv

t :: [P Int] -> Int
t ps = (f (-v) - f v) `div` (2 * v)
  where v = maximum (map yv ps)
        f x = maximum $ map y $ filter ((==x).yv) ps

s :: Int -> [P Int] -> String
s t ps = unlines $
  [[if Set.member (x, y) s then '#' else '.'
    | x <- [xmin..xmax]] | y <- [ymin..ymax]]
  where
    s = Set.fromList $ map (\p -> (x p+(xv p*t), y p+(yv p*t))) ps
    (Min xmin, Min ymin, Max xmax, Max ymax) =
      foldMap (\(x, y) -> (Min x, Min y, Max x, Max y)) s

main = do
  ps <- p <$> getContents
  putStr $ s (t ps) ps
  print $ t ps
