GEM5
====

Integration of correlator Branch Predictor into gem5 cycle accurate architectural simulator

Add the source files in SConcript

In bpred_unit.cc create a new object for predictor

Add params to Branchpredictor.py

Change the predType in O3_ARM_v7a.py


build the system:
 scons -j8 build/ARM/gem5.debug

compiling a program for se mode:
arm-linux-gnueabi-gcc -DUNIX -o sort sort.c -static


Run the system in se mode:
./build/ARM/gem5.debug --debug-file=trace.out --debug-flags=Fetch,Branch configs/example/se.py -c ./tests/test-progs/hello/bin/arm/linux/sort -o 16 --caches --cpu-type=arm_detailed

Run full system simulation using PARSEC:
./build/ALPHA/gem5.opt --debug-file=trace.out --debug-flags=Fetch,Branch ./configs/example/fs.py --script=./PARSEC/runscript.rcS


For statistics files go to /gem5/m5out
