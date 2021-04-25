step6: what a heck are we are actually doing with our data set?!

Our strings and how we "look" into HT for them are completely unrealistic.
We generate numbers and put them to strings, which means all our strings are basically of a same and small size.
Strings, which are just numbers. Anyone sane in that situation would use just numbers as keys :))
All of them fit in SSO, which makes it very cache-friendly.
We can optimize our string implementation (we actually better do that!), but let's just generate longer strings (x4 longer).

They will still be of the same size, which agains, begs for specific optimizations, but at least that would be more 'realistic' _strings_ themselves.

We are also 'looking for' strings in a very cache friendly way, as they are in the same order as what used to insert them into HT.
That makes test especially biased in favor of chains, as it makes them appear inside chains exactly in same order we are looking for them.

If our strings implementations allows immutable strings/string interning (like most managed platforms),
that basically replaces key comparison with a simple pointer comparison!

It is impossible to imagine any real-life scenario, where we are looking in HT for strings we 100% sure are already there,
  and more over, use exactly same strings (not identical by content, but exactly the same, in case of copy-on-write/string interning)
  and even more, in exactly same order we put them inside that same table!

So, let's back on Step3 (again!) and just makes longer strings and look for them in some stochastic order.
On our c++ platform we know there is no string interning, so they are identical strings, not 'same' strings.

Now, on same Load Factor (and same algorithm), and on hot cache (no cache pruning) we get:
 26% win on LOOSE, 26% win on MISS (and save 10% of memory!).
 and on same memory usage, again
 45% win on LOOSE, and 45% win on MISS

There still much more probes (2.5x on HIT, 9x on MISS) in our OA implementations, and yet it always win.

This is still that same unrealistic "hot cache scenario", but now we measure not _completely_ synthetic and unrealistic scenario.
Ofc, that changes our data set/testing code, even though that testing code didn't made any sense.
So that's not optimization step per se.