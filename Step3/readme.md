step3: fix algorithm and data structure.

* first, simplest: let's use FNV1 (it is not THE fastest hash, but it is simple, portable, and better distributed)
  -3% speed up, less probes

* second, we need only one incorrect hash value - 0. gives us -31% on miss, -0.5% on hit.
  That is very good change, for several reasons: a) it is faster to compare with zero, b) we have better hashes (as we don't waste one bit)

* third, let's use separate hashes, keys, and values to different arrays -21% on hit and -23% on miss.
  Keep in mind, we have absolutely same amount of probes, which is like 2.5 times higher than in required in SC,
  and yet our performance is almost same in OA (we still loose 12%)!

  Why this helps performance, is obvious. It is just much more cache friendly to iterate on smaller array.
  Which is not that obvious, is that it is actually way better structure, and it allows to save more memory.
  Keys, values, and hashes all have different alignment (in real life scenario).

  Original implementation was wasting 4 bytes on aligning int to alignment of string (which is >=8 on 64 bit platforms).
  so we waster our mem bandwidth on downloading 4 bytes of bullshit data, AND wasted memory.
  that alone saves us 128kb of memory (on ~16384 capacity).
  We still loose ~2.82 times on MISS.
  One can argue that keys should be in same array as value (as when we finally make a hit, we usually should return the data of value and we don't here).
  Well, I didn't made that test and data set, and the whole point of this analysis is that in any real scenario OA is (probably) a better solution.
  If we would need to return the value which is different from key and we get many hits, then fine (and in this particular test that wouldn't matter much).

* Fourth:
  Load Factor IS _NOT_ an actual production/platform requirement. It _is_ part of a specific algorithm.
  The actual requirement is memory usage.
  So, if we want compare apples to apples, we should compare performance of HT on same MEMORY footprint, not on the same LF
  (which, is again, doesn't matter).
  With that change we would loos by 4% on HIT and loose 4% on MISS.

