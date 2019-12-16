#!/usr/bin/env python3
#
# (C) 2019-* Emanuele Ballarin <emanuele@ballarin.cc>
# Distribution: MIT License.

# IMPORTS:
from runacq import *

# GLOBALS
procnum_list = [2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20]
averaging_num = 5
# averaging_num = 2
probsize = 3000000000  # Do not use scientific notation
# probsize = 3000000000  # Do not use scientific notation
# probsize = 30  # Do not use scientific notation


##################################################################################################

print("\n\n\n\n\n\n\n")

##################################################################################################


# Strong scaling of "Touch First"

tf_xaxis = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20]

tf_serial_run = acqavg(
    averaging_num, runacq_touchfirst, "tf_serial.x", 1, probsize, True
)
tf_serial_time = tf_serial_run[0]

tf_parallel_time = []
tf_parallel_overh = []
for proc in procnum_list:
    procrun = acqavg(
        averaging_num, runacq_touchfirst, "tf_parallel.x", proc, probsize, False
    )
    tf_parallel_time.append(procrun[0])
    tf_parallel_overh.append(esf(float(tf_serial_time / procrun[0]), proc))

print("\n\n")
print("###########################################################################")
print("TOUCH FIRST SUM, up to ", probsize, " numbers")
print("Serial time: ", tf_serial_time)
print("Parallel time vector (2 to 20 threads): ", tf_parallel_time)
print("Parallel OVERHEAD vector (2 to 20 threads): ", tf_parallel_overh)
print("###########################################################################")
print("\n\n\n\n\n")


##################################################################################################

print("\n\n\n\n\n\n\n")

##################################################################################################


# Strong scaling of "Touch By All"

tba_xaxis = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20]

tba_serial_run = acqavg(
    averaging_num, runacq_touchbyall, "tba_serial.x", 1, probsize, True
)
tba_serial_time = tba_serial_run[0]

tba_parallel_time = []
tba_parallel_overh = []
for proc in procnum_list:
    procrun = acqavg(
        averaging_num, runacq_touchbyall, "tba_parallel.x", proc, probsize, False
    )
    tba_parallel_time.append(procrun[0])
    tba_parallel_overh.append(esf(float(tba_serial_time / procrun[0]), proc))

print("\n\n")
print("###########################################################################")
print("TOUCH BY ALL SUM, up to ", probsize, " numbers")
print("Serial time: ", tba_serial_time)
print("Parallel time vector (2 to 20 threads): ", tba_parallel_time)
print("Parallel OVERHEAD vector (2 to 20 threads): ", tba_parallel_overh)
print("###########################################################################")
print("\n\n\n\n\n")


##################################################################################################

print("\n\n\n\n\n\n\n")

##################################################################################################


# Strong scaling of "Prefix Sum"

psum_xaxis = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20]

psum_serial_run = acqavg(averaging_num, runacq_prefixsum, "psum.x", 2, probsize, True)
psum_serial_time = psum_serial_run[0]

psum_parallel_time = []
psum_parallel_overh = []
for proc in procnum_list:
    procrun = acqavg(averaging_num, runacq_prefixsum, "psum.x", proc, probsize, False)
    psum_parallel_time.append(procrun[1])
    psum_parallel_overh.append(esf(float(psum_serial_time / procrun[1]), proc))

print("\n\n")
print("###########################################################################")
print("PREFIX SUM, up to ", probsize, " numbers")
print("Serial time: ", psum_serial_time)
print("Parallel time vector (2 to 20 threads): ", psum_parallel_time)
print("Parallel OVERHEAD vector (2 to 20 threads): ", psum_parallel_overh)
print("###########################################################################")
print("\n\n\n\n\n")
