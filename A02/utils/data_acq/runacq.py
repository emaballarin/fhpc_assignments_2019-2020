#!/usr/bin/env python3
#
# (C) 2019-* Emanuele Ballarin <emanuele@ballarin.cc>
# Distribution: MIT License.

# IMPORTS:
import os
import subprocess as proc
from subprocess import PIPE
import time
import numpy as np


###################################################################################################


def esf(speedup_pass, nthreads_pass):
    """Empirical Serial Fraction, given parallel speedup and number of threads
    (after [Tornatore, 2019])"""

    return ((1.0 / float(speedup_pass)) - (1.0 / float(nthreads_pass))) / (
        1.0 - (1.0 / float(nthreads_pass))
    )


###################################################################################################


def runacq_touchfirst(bin_name, nthreads, vector_len, isserial=False):

    bin_path = "../../src/touch_by/"

    call_env = os.environ.copy()
    call_env["OMP_NUM_THREADS"] = str(nthreads)

    execlist = [str(bin_path + bin_name), str(vector_len)]
    ranproc = proc.run(execlist, stdout=PIPE, stderr=PIPE, check=True, env=call_env)

    ranproc_stout = str(ranproc.stdout.decode())
    ranproc_linesout = list(filter(None, ranproc_stout.split("\n")))
    print(ranproc_linesout)
    clean_linesout = [ranproc_linesout[-3], ranproc_linesout[-2], ranproc_linesout[-1]]
    timesout = [
        float(clean_linesout[0]),  # Wall clock time
        float(clean_linesout[1]),  # Average single thread time
        float(clean_linesout[2]),  # Minimum single thread time
    ]

    if isserial:
        timesout = [
            timesout[0]
        ]  # Thread times are undefined and thus contain garbage values!

    time.sleep(1)
    return timesout  # LIST: [walltime, threadavg, threadmin]


###################################################################################################


def runacq_touchbyall(bin_name, nthreads, vector_len, isserial=None):

    bin_path = "../../src/touch_by/"

    call_env = os.environ.copy()
    call_env["OMP_NUM_THREADS"] = str(nthreads)

    execlist = [str(bin_path + bin_name), str(vector_len)]
    ranproc = proc.run(execlist, stdout=PIPE, stderr=PIPE, check=True, env=call_env)

    ranproc_stout = str(ranproc.stdout.decode())
    ranproc_linesout = list(filter(None, ranproc_stout.split("\n")))
    print(ranproc_linesout)
    clean_linesout = [ranproc_linesout[-1]]
    timesout = [
        float(clean_linesout[0]),  # Wall clock time
    ]

    time.sleep(1)
    return timesout  # LIST: [walltime]


###################################################################################################


def runacq_prefixsum(bin_name, nthreads, vector_len, isserial=None):

    bin_path = "../../src/prefix_sum/"

    call_env = os.environ.copy()
    call_env["OMP_NUM_THREADS"] = str(nthreads)

    execlist = [str(bin_path + bin_name), str(vector_len)]
    ranproc = proc.run(execlist, stdout=PIPE, stderr=PIPE, check=True, env=call_env)

    ranproc_stout = str(ranproc.stdout.decode())
    ranproc_linesout = list(filter(None, ranproc_stout.split("\n")))
    print(ranproc_linesout)
    clean_linesout = [ranproc_linesout[-2], ranproc_linesout[-1]]

    timesout = [
        float(clean_linesout[0]),  # Serial runtime - in nanoseconds
        float(clean_linesout[1]),  # Parallel runtime - in nanoseconds
        float(0.0),  # Speedup (still to be computed)
    ]

    timesout = [
        timesout[0] / 10e9,  # Serial runtime - in seconds
        timesout[1] / 10e9,  # Parallel runtime - in seconds
        timesout[0] / timesout[1],  # Speedup
        # Empirical serial fraction
    ]

    timesout = [
        timesout[0],  # Serial runtime - in seconds
        timesout[1],  # Parallel runtime - in seconds
        timesout[2],  # Speedup
        (
            esf(timesout[2], nthreads)  # Empirical serial fraction
            #            ((1.0 / float(timesout[2])) - (1.0 / float(nthreads)))
            #            / (1.0 - (1.0 / float(nthreads)))
        ),
    ]

    time.sleep(1)
    return timesout  # LIST: [serialtime, paralleltime, speedup]


###################################################################################################


def acqavg(nrtimes, funcname, bin_name, nthreads, vector_len, isserial_pass=None):
    """Iterate the acquisition of data and average the results"""

    if (funcname == "runacq_touchfirst") and (isserial_pass is None):
        raise Exception(
            "acqavg: function 'runacq_touchfirst' called with invalid 'isserial=None' argument!"
        )

    # First, out-of-loop call
    ret_result = funcname(bin_name, nthreads, vector_len, isserial=isserial_pass)
    accumulator_vector = np.asarray(ret_result, float)

    for i in range(nrtimes - 1):
        ret_result = funcname(bin_name, nthreads, vector_len, isserial=isserial_pass)
        accumulator_vector += np.asarray(ret_result)

    mean = accumulator_vector / nrtimes

    return list(mean)


###################################################################################################
