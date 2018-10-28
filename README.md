# xorlist
A demo implementation of a single element doubly linked list using XOR

Someone asked if it was possible to do a doubly linked list using only one 'address' element.
There were no restrictions on any side data that were stored in the implementation, all that
mattered is that the side data could not grow and rate N (it had to be less) and the complexity
of the algorithm could also not grow at rate N (it had to be less).

So basically, behave within the bounds of a doubly linked list, except that you can only use one
native pointer sized slot for the pointer, and you had to be able to cover the full native address
space of the machine.

Knowing that (A^B)^A == B, I worked from there.

The basic idea is that each data element in the list has a single slot that is the XOR of the element
to the left and the element to the right.

So as long as the code stays within the list, it can move a cursor along the elements, back and forth,
and it can do insertions and deletions, but you cannot just jump on any particular element and
find your way around.

I have to think about that one a little more.

Anyway, here it is. Feel free to poke around.

Copyright 2018 Douglas Fraser
Creative Commons
Feel free to use it as you wish, I make no claims other than I authored this ugly chunk of code.

If you find it interesting or extend it in some way, I would love to hear about it.
