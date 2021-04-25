First step: directly port to c++.
0) OA has flaw in implementation (can't insert in the end of table, so we get 14744 instead of 14745 inserts!).
1) we measure perf incorrectly (we need make several runs and get minimums). Basically on most of HW we would be measuring noise.
2) ineffecient data structures and algorithms:
  * Our values (which we never used) are in same struct as keys and hashes. It actually makes our HashTables - HashSets.
  * Our hash function is default (and it isn't good or portable)
  * Our 'invalid hash' is -1 (not 0)
  * we have to mask hashes to key location. which is simply silly, we need only one invalid hash
  * our capacity is not pow^2
  * we just waste data in our OA structure
  * actually, we put integers as keys. that's stupid
  * it creates FIXED capacity size HT, no growth and rehashing.
    While it is probably okayish for testing grounds, it is obviously not anyhow ready to be used in any real-life case-scenario.
  * it uses bad (default idiomatic) string implementation.