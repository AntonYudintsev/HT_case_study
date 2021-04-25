step4: Cache effiency

On last step we almost got on par with SC on a ~same memory usage (and LF).
But we are sill loosing.
Or do we...?

That test was extremely benefical to SC, simply the way we do it.
All data set completely fits in L2 cache (several times probably), even with a strings contents, and we do not do anything else, so it _is_ in L2 cache!
Most likely, even L1 cache isnt that useless, as 'buckets' array fits there.
While it is well-known fact, that in real-life scenarios the cache will be cold :)
You usually will be doing something else, besides attack your HT, and in other threads as well, so even small HT will be usually 'cold'.
So, let's prune cache (L2 cache).
The test become painfully slow, but even with same LF we already win over SC in both HIT (by 6%) and MISS (by 27%)!
And on same memory usage win is 26% on HIT, and 37.5% on MISS.

Even without completely pruning our cache, just by increasing working set/HT size to 256k (and so, memory usage to 16mb+, which is still keeps L2 cache somewhat helping),
we get on a same mem usage on HIT 2% win, on MISS 36% win

The funny fact, now MISS is actually where we win more (which is absolutely as it should be, as the biggest win of OA is cache effiecency on scan).
