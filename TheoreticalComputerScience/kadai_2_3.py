#/usr/bin/python
#coding: UTF-8
#python kadai_2_3.py 1000 100 0.3 0.5
import numpy as np
import sys
import random as rand
argvs = sys.argv
argc = len(argvs)
n_skip = 500
if argc == 5:
  n_rand = int(argvs[1])
  n_var = int(argvs[2])
  beta = float(argvs[3])
  sigma = float(argvs[4])
else:
  print("%s n_rand n_variable beta sigma"%argvs[0])
  exit(1)

#-10-10の乱数を作成
x = np.array([ rand.uniform(-10,10) for i in range(n_var)])
#yは適当に決める w_0 w_1は求める理論値
w_0 = rand.uniform(-5,5)
w_1 = rand.uniform(-1,1)
noize = 0.2*np.random.rand(n_var) - 0.1
y = w_0 + w_1*x + noize 

#事前確率計算
def prob(beta,w):
  return np.exp( -1 * (w[0]**2 + w[1]**2)/(2*(beta**2)) )/( np.pi*(beta**2) )

#尤度函数計算
def likelihood(sigma,y,x,w):
  return np.exp( -1 * np.sum((y - (w[0] + w[1]*x))**2)/(2*(sigma**2)))/( (np.pi*(sigma**2))**(n_var/2) )

#比の計算
def rate_prob(beta,sigma,y,x,w,w_):
  buf_1 = np.exp( -1 * (w_[0]**2 + w_[1]**2 - w[0]**2 - w[1]**2)/(2*(beta**2)))/( np.pi*(beta**2) )
  buf_2 = np.exp( -1 * (np.sum((y - (w_[0] + w_[1]*x))**2) - np.sum((y - (w[0] + w[1]*x))**2)))/((np.pi*(sigma**2))**(n_var/2) )
  return buf_1*buf_2

def main():
  #初期値を適当に決める(-1-1)
  w = 2*np.random.rand(2) - 1
  print("理論値 w_0:%.3f, w_1:%.3f" %(w_0, w_1))
  print("w_0, w_1, likelihood")
  w_sum = [0,0]
  for i in range(n_skip+n_rand):
    #候補の作成
    d_w = np.random.normal(loc=0.0, scale=beta, size=2)
    w_ = w + 0.1*d_w
    
    #候補と現在状態の確率の比を計算
    #r = prob(beta,w_)*likelihood(sigma,y,x,w_)/( prob(beta,w)*likelihood(sigma,y,x,w) )
    r = rate_prob(beta,sigma,y,x,w,w_)
    #一様乱数
    R = rand.random()
    
    #採択条件
    if R < r:
      w = w_
    
    #一定回数無視してw生成
    if i >= n_skip:
      print("%.3f,%.3f:%s" %( w[0], w[1], str(likelihood(sigma,y,x,w))))
      w_sum[0] += w[0]
      w_sum[1] += w[1]

  print("理論値 w_0:%.3f, w_1:%.3f" %(w_0, w_1))
  print("期待値 w_0:%.3f, w_1:%.3f" %(w_sum[0]/n_rand, w_sum[1]/n_rand))

if __name__=="__main__":
  main()
