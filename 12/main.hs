{-# LANGUAGE QuasiQuotes #-}
{-# LANGUAGE ViewPatterns #-}

import qualified Data.Map as Map
import Data.Map (Map)
import Text.Scanf

p :: String -> (String, Map String Char)
p (lines -> s:[]:m) = (st s, Map.fromList $ map kv m)
  where 
    kv (scanf [fmt|%s => %s|] -> Just (k :+ v:[] :+ ())) = (k, v)
    st (scanf [fmt|initial state: %s|] -> Just (s :+ ())) = s

step :: Map String Char -> (Int, String)  -> (Int, String)
step m (i, s) = (i - 2, f ("...." ++ s ++ "...."))
  where
    f (a:b:c:d:e:xs) = m Map.! [a, b, c, d, e] : f (b:c:d:e:xs)
    f xs = []

psum :: (Int, String) -> Int
psum (i, s) = sum $ map fst $ filter ((=='#').snd) $ zip [i..] s

main = do
  (s, m) <- p <$> getContents
  let sums = map psum $ iterate (step m) (0, s)
  print (sums !! 20)
  print ((50000000000-1000)*(sums!!1000-sums!!999)+sums!!1000)
