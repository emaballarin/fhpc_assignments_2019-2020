#!/usr/bin/env python3
#
# (C) 2019-* Emanuele Ballarin <emanuele@ballarin.cc>
# Distribution: MIT License.

from runacq import runacq  # Warning because it's a local file!

# Loop variables
averaging_range_len: int = 10
proc_num: int = 20
in_program_printed_values: int = 3

# Test-specific variables
proc_list = [2, 4, 8, 16, 20]
niter = 100000000

# Plotting-specific variables
outplot_inprogram = []  # To be filled
outplot_gnutime = []  # To be filled

# Varying number of processors
for proc in proc_list:

    # Averaging loop
    inprogram_accumulator = 0
    gnutime_accumulator = 0
    for trial in range(averaging_range_len):

        # Actual execution
        current_run = runacq("../src/a.out", proc, niter, isserial=False)

        # Manipulating output data
        candidate_max = 0
        for elem in range(proc):
            if float(current_run[0][2 + 3 * elem][2]) >= float(candidate_max):
                candidate_max = float(current_run[0][2 + 3 * elem][2])

        inprogram_accumulator += candidate_max
        gnutime_accumulator += float(current_run[1][0])

    # Actual averaging
    inprogram_average = float(inprogram_accumulator) / float(averaging_range_len)
    gnutime_average = float(gnutime_accumulator) / float(averaging_range_len)

    outplot_inprogram.append(inprogram_average)
    outplot_gnutime.append(gnutime_average)


print(outplot_inprogram)
print(outplot_gnutime)
