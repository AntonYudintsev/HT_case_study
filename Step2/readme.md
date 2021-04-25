Second step: fixes of main flaws

* first, fix OA implementation flaw. It is rather simple fix.
  We have to search till we find empty, and in the end of table we should start from begining.
  Now it is restored, we have our 14745 inserts in both tables.
* remove 'deleted' state of entry. It is useless, and can't be possibly used.
* we measure best result out of N runs. That is significantly more stable performance measurements.

Now we established our test bed, and can go for optimizing OA.
