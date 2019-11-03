#!/usr/bin/env python3

from runacq import runacq  # Warning because it's a local file!

rangelen: int = 10
procnum: int = 4
printedvalues: int = 3

# Serial run of `pi.c`
inprogram_accumulator = 0

for i in range(rangelen):
    acquired = runacq("../src/a.out", procnum, 100000000, isserial=False)
    candidate_max: int = 0
    for elem in range(procnum):
        if float(acquired[0][2 + 3 * elem][2]) >= float(candidate_max):
            candidate_max = float(acquired[0][2 + 3 * elem][2])

    inprogram_accumulator += candidate_max

print(float(inprogram_accumulator) / float(rangelen))
