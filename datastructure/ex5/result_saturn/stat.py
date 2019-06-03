import numpy as np
import argparse
import sys

def calc_stat(filename):
  with open(filename,"r") as fs:
    strs = fs.readlines()
    times = np.array([float(val.split()[1]) for val in strs],dtype = np.float32)

  return np.mean(times), np.std(times)

def main():
  filenames = sys.stdin.readlines()
  for fn in filenames:
    m,s = calc_stat(fn.rstrip())
    fo = fn.split(".")[0]+".res"
    with open(fo,"w") as fs:
      fs.write(str(m) + "\t" + str(s))

if __name__=="__main__":
  main()
