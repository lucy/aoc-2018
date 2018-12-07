{-# LANGUAGE OverloadedStrings #-}

import Control.Applicative
import Data.Attoparsec.ByteString
import Data.Attoparsec.ByteString.Char8 as C
import Data.Either
import Data.Foldable
import Data.Function
import Data.Int
import Data.List
import Data.Map.Strict (Map)
import qualified Data.ByteString as B
import qualified Data.ByteString.Char8 as B8
import qualified Data.Map.Strict as Map

data Line = Guard Int | Sleep Int | Wake Int
  deriving (Show)

p :: Parser Line
p = do
  c '['; d; c '-'; d; c '-'; d; c ' '; d; c ':'
  m <- d
  s "] "
  (Guard <$ s "Guard #" <*> d <* s " begins shift") <|>
    (Sleep m <$ s "falls asleep") <|>
    (Wake  m <$ s "wakes up")
  where c = C.char; d = decimal; s = string

f :: [Line] -> Map Int (Map Int Int)
f l = f' l 0 0 Map.empty
  where     
    f' :: [Line] -> Int -> Int -> Map Int (Map Int Int) -> Map Int (Map Int Int)
    f' (Guard id:xs) _  t m = f' xs id t m
    f' (Sleep t :xs) id _ m = f' xs id t m
    f' (Wake  t :xs) id f m = f' xs id t $
      flip (Map.insert id) m $
      foldr (\t -> Map.insertWith (+) t 1) 
        (Map.findWithDefault Map.empty id m)
        [f..t]
    f' [] _ _ m = m

main = do
  raw <- sort . B8.lines <$> B.getContents
  let lines = rights $ map (parseOnly p) raw
  mapM_ print lines
  let guards = f lines
  let id = fst $ maximumBy (compare `on` snd)
               $ map (\(k, v) -> (k, sum v))
               $ Map.toList $ f lines

  print guards
  print $ id * (fst $ maximumBy (compare `on` snd) $ reverse $ Map.toList $ (guards Map.! id))
  where
    g (id, sched) =  Map.toList sched
    

