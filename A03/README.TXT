##################################################
##                                              ##
## README                                       ##
## ******                                       ##
##                                              ##
## Third Assignment - FHPC Course 2019/2020     ##
##                                              ##
## Emanuele Ballarin <emanuele@ballarin.cc>     ##
## DSSC M.Sc. @ (UniTS | UniUD | SISSA | ICTP)  ##
##                                              ##
##################################################

This folder contains the source file(s) fulfilling the requirements of 2nd
execrise (i.e. the "N-bodies with gravitational potential" simulation) of the
3rd assignment for the FHPC course.

At the moment of the writing, the code is still undergoing heavy development and
may be changed - even dramatically.
However, at its final version, it should contain as comments all the necessary
information required to compile/use it.

Given the many assumptions required by the assignment text, the lack of
precision of the integration scheme suggested, and the ultimately non-physical
nature of the simulation outcomes, this exercise might also serve as a terse
coding "kata". By exploiting modern C++ features, it is in fact possible to
develop code that - while ensuring high performance in the final executable
by performing verifications and pre-calculations at compile-time - is also
readable and apt to fulfil its purpose of mean of communication among human
developers.
