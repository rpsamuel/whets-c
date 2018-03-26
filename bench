module Main where
import qualified Data.Text as T
import qualified Data.Text.Unsafe as TU
import qualified Data.Text.Array as TA
import Data.Text.Internal (Text(Text))
import Data.Char (chr)
import Criterion.Main
import Control.DeepSeq

stringTo :: Int -> T.Text
stringTo n = T.pack . map (chr . (`mod` 2^16)) $ [1..n]

derpA ::T.Text
derpB ::T.Text
derpA = stringTo 1000000
derpB = stringTo 1000000

tenMilA ::T.Text
tenMilB ::T.Text
tenMilA = stringTo 10000000
tenMilB = stringTo 10000000

hundredMilA ::T.Text
hundredMilB ::T.Text
hundredMilA = stringTo 100000
hundredMilB = stringTo 100000

hamming :: T.Text -> T.Text -> Maybe Int
hamming a b =
  if T.length a == T.length b
      then Just (go 0 0 0)
      else Nothing
  where
    go !na !nb !r =
      let !(TU.Iter cha da) = TU.iter a na
          !(TU.Iter chb db) = TU.iter b nb
      in if | na  == len -> r
            | cha /= chb -> go (na + da) (nb + db) (r + 1)
            | otherwise  -> go (na + da) (nb + db) r
    len = TU.lengthWord16 a

hammingFast :: T.Text -> T.Text -> Int
hammingFast a b =
    let
        Text arra ofa lena = a
        Text arrb ofb lenb = b
        loop !i !distance =
            if i < lena then
                if TA.unsafeIndex arra (ofa + i) /= TA.unsafeIndex arrb (ofb + i) then
                    loop (i + 1) (distance + 1)
                else
                    loop (i + 1) distance
            else
                distance
    in
        loop 0 0

main = deepseq (derpA, derpB) $ defaultMain [ bgroup "hamming code fast" [bench "100 thousand" $ nf (uncurry hammingFast) $! (hundredMilA, hundredMilB),
                                                                          bench "1 Million" $ nf (uncurry hammingFast) $! (derpA, derpB),
                                                                          bench "10 million" $ nf (uncurry hammingFast) $! (tenMilA, tenMilB)]]
