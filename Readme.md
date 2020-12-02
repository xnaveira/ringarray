# Ringarray

This is a small library I made for an embedded project, we moved on to using
other data structures so I thought to save it here for posterity :)

It is a simple array that overflows into itself 'a la fifo'. The idea is to be
able to save and then retrieve the last **n** elements, in this case a 64 bit
number. It was made for Zephyr OS but should be easy to port due to its
simplicity.

Be warned, I am by no means a C expert developer...
