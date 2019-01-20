#/usr/bin/python
#coding: UTF-8
#python kadai_1.py 1 1 1 1.0 0.9
import numpy as np
import sys

argvs = sys.argv
argc = len(argvs)
vals = [-1, 1]
var = {-1:"■",1:"□"}
x = [np.array((i,j,k)) for i in vals for j in vals for k in vals]
if argc == 6:
  y = [ int(i) for i in argvs[1:4]]
  shita = float(argvs[4])
  p = float(argvs[5])
  likelihood = {1:p,-1:1-p}
else:
  print("%s y(3numbers 1 or -1) shita p"%argvs[0])
  exit(1)

def main():
  print(var)
  print("y:%s%s%s, θ:%.3f, p:%.3f"%(var[y[0]],var[y[1]],var[y[2]],shita,p))
  #事前分布の計算
  Ising = [ np.exp(shita*x_[0]*x_[1] + shita*x_[1]*x_[2]) for x_ in x ]
  Ising = Ising/np.sum(Ising)

  #事前分布の出力
  print("事前分布P(x)")
  for i in range(len(x)):
    print("%s%s%s:%.4f"%(var[x[i][0]],var[x[i][1]],var[x[i][2]],Ising[i]))

  #尤度関数の計算
  likefunc = [ likelihood[x_[0]*y[0]]*likelihood[x_[1]*y[1]]*likelihood[x_[2]*y[2]] for x_ in x]
  
  #尤度関数の出力
  print("\n尤度関数P(Y|X)")
  for i in range(len(x)):
    print("%s%s%s:%.4f"%(var[x[i][0]],var[x[i][1]],var[x[i][2]],likefunc[i]))

  #事後分布の計算
  Posdis = [ Ising[i]*likefunc[i] for i in range(len(x))]
  Posdis = Posdis/np.sum(Posdis)

  #事後分布の出力
  print("\n事後分布P(x|y)")
  for i in range(len(x)):
    print("%s%s%s:%.4f"%(var[x[i][0]],var[x[i][1]],var[x[i][2]],Posdis[i]))

if __name__=="__main__":
  main()
