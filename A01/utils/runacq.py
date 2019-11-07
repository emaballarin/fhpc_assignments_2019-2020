#!/usr/bin/env python3

# IMPORTS:
import subprocess as proc
from subprocess import PIPE


def runacq(binary_name, mpiprocs, iterations=None, isserial=False, isintel=False):
    """ Run and Acquire: scriptable, parametrized, parallel execution of binaries. """

    # Preallocation
    retlist = []  # What the function should return
    execution_lol = []  # Execution List Of Lists (a.k.a a "trace")
    gnutimes = []  # List of times acquired with GNU Time

    # User-defined variables
    binary = binary_name  # Or whatever
    mpiprocs: int = mpiprocs  # Or whatever

    if iterations is not None:
        iterations: int = iterations
    else:
        iterations = ""

    # Actual constants
    binpath = "../src/"
    time = "/usr/bin/time"
    if isintel:
        mpirun = "../src/impirun"
    else:
        mpirun = "../src/mpirun"

    # Deduced variables
    mpiargs = str(mpiprocs)
    par_n = str(iterations)

    if isserial:
        execlist = [time, "-p", binpath + binary, par_n]
    else:
        execlist = [time, "-p", mpirun, "-np", mpiargs, binpath + binary, par_n]

    # BASH CALL(S):
    ranproc = proc.run(execlist, stdout=PIPE, stderr=PIPE, check=True)

    # ANALYSIS:

    # Capture stderr (in this case: GNU Time output) and decode:
    ranproc_stderr = str(ranproc.stderr.decode())
    ranproc_lineserr = list(filter(None, ranproc_stderr.split("\n")))

    # Put such times in a tractable list
    for element in ranproc_lineserr:
        gnutimes.append(element.split(" ")[1])

    # Capture output and decode:
    ranproc_output = str(ranproc.stdout.decode())
    ranproc_linesout = list(filter(None, ranproc_output.split("\n")))

    # Sort output lines by procId / execution order
    ranproc_linesout = sorted(ranproc_linesout)

    # Make sorted lines a tractable list
    for elem in ranproc_linesout:
        # Split
        linesplit_out = elem.split(" ")

        # Data-clean
        linesplit_out = list(filter(None, linesplit_out))

        # Append
        partial_new_list = []

        partial_new_list.append(
            linesplit_out[1][0]
        )  # Location of procId; second character (first is a brace!)

        partial_new_list.append(
            linesplit_out[3][0]
        )  # Location of execution ordinal; second character (first is a brace!)

        partial_new_list.append(linesplit_out[-1])  # Execution time

        execution_lol.append(partial_new_list)

    # NOTE: execution_lol is a list of lists;
    #       For each line:
    #                      [0] : the processor Id
    #                      [1] : the ordinal for execution ordering
    #                      [2] : the time for such operation

    # NOTE: gnutimes is a list:
    #                      [0] : "real" time (a.k.a. actual measured walltime)
    #                      [1] : "user" time (a.k.a. sum of actual process times)
    #                      [2] : "sys" time

    retlist.append(execution_lol)
    retlist.append(gnutimes)

    return retlist
