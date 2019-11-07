#!/usr/bin/env python3

from runacq import runacq  # Warning because it's a local file!

rangelen: float = float(10)

# Serial run of `pi.c`
inprogram_accumulator = 0
gnutime_accumulator = 0

for i in range(10):
    acquired = runacq("../src/a.out", 1, 100000000, isserial=True)
    inprogram_accumulator += float(acquired[0][0][2])
    gnutime_accumulator += float(acquired[1][0])

print("IN-PROGRAM AVG.:", inprogram_accumulator / rangelen)
print("GNU TIME AVG.:", gnutime_accumulator / rangelen)
