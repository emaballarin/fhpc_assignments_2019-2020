#!/usr/bin/env python3

from runacq import runacq  # Warning because it's a local file!

for i in range(10):
    pippo = runacq("../bin/a.out", 4, 1000)
    print(pippo[0])
    print(pippo[1])
    print("\n")
