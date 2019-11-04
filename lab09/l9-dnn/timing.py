import time
import dnn_timetest

starttime = time.clock()

def measureAlg(numCPUs):
    if numCPUs < 1 and numCPUs > 4:
        print("Error. Invalid number of CPUs given.")
    else:
        startime = time.clock()
        dnn_timetest.run(numCPUs)
        return time.clock() - startime

def measureAlgRepeat(numCPUs, repeats):
    tests = []
    i = 0
    while i < repeats:
        tests += measureAlg(numCPUs)
        i += 1
    return tests

def runtests(repeats):
    tests_by_cpu = []
    i = 1
    while i <= 4:
        tests_by_cpu += measureAlgRepeat(repeats)
        i += 1
    return tests_by_cpu

