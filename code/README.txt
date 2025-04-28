Note we did timing tests without the code able to use non-constant functions.

Once we added that functionality, which involves conditional checking to apply the right functions to compute range sums, timing increased very marginally.

This should be fine since most of our graphs are speedup graphs anyways.