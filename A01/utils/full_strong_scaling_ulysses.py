#!/usr/bin/env python3
#
# (C) 2019-* Emanuele Ballarin <emanuele@ballarin.cc>
# Distribution: MIT License.

from runacq import runacq  # Warning because it's a local file!

# Loop variables
averaging_range_len: int = 5
proc_num: int = 20
in_program_printed_values: int = 3

# Test-specific variables
proc_list = [2, 4, 8, 16, 20]
niter_list = [1e7, 1e8, 3e8, 5e8, 7e8, 9e8, 1e9]

final_plot_matrix = []

for niter in niter_list:

    # Plotting-specific variables
    outplot_gnutime = []  # To be filled

    # Varying number of processors
    for proc in proc_list:

        # Averaging loop
        gnutime_accumulator = 0
        for trial in range(averaging_range_len):

            # Actual execution
            current_run = runacq("../src/a.out", proc, niter, isserial=False)
            gnutime_accumulator += float(current_run[1][0])

        # Actual averaging
        gnutime_average = float(gnutime_accumulator) / float(averaging_range_len)
        outplot_gnutime.append(gnutime_average)

    final_plot_matrix.append(outplot_gnutime)

print(final_plot_matrix)
