step5: Algorithms

Let's go back to Step3, i.e. hot cache.
By now we hadn't changed the original algorithm, and all the way kept 0.9 LF, and allowing any capacity.

In reality, if we care for performance, we would keep Pow-of-2 capacity, as it allows simple bit masking to get bucket location.
We would also never allow LF to grow more than we fill comfortable with.
I am not changing SC implementation, as we are keeping same memory requirements for both implementations,
  which means it's Load Factor is higher (but memory usage is same).
Still, amount of probes is higher in our OA implementation, and yet our performance is better.
We win 9% in HIT, and 7.5% in MISS scenarios.

Practically speaking, both implementations requires same amount of memory, and yet OA is faster in unrealistic "hot cache scenario",

By that time we are dominated by our strings implementation.