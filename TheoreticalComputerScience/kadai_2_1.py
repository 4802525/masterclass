#/usr/bin/python
#coding: UTF-8 #python kadai_1.py 1 1 1 1.0 0.9
import numpy as np
import sys
import random as rand
argvs = sys.argv
argc = len(argvs)
vals = [-1, 1]
var = {-1:"■",1:"□"}
n_skip = 10
x = [np.array((i,j,k)) for i in vals for j in vals for k in vals]
if argc == 3:
  theta = float(argvs[1])
  n_rand = int(argvs[2])
else:
  print("%s theta n_rand"%argvs[0])
  exit(1)

#確率計算
def prob(theta,x_):
  return np.exp(theta*x_[0]*x_[1] + theta*x_[1]*x_[2] + theta*x_[2]*x_[0])

#y = (x_1,x_2,x_3)生成した乱数からそのindexを算出
def get_index_from_vars(y):
  for i in range(len(x)):
    if (x[i] == y).all():
      index = i
  return index

def main():
  print(var)
  #確率分布の計算
  Prob_d = [ prob(theta,x_) for x_ in x ]
  Prob_d = Prob_d/np.sum(Prob_d)

  print("乱数の生成")
 
  #初期値y
  y = x[rand.randint(0,7)]
 
  #生成した乱数(x_1,x_2,x_3)の割合
  Prob_rand = [ 0 for i in range(len(x))]
  
  for i in range(n_skip+n_rand):
    #状態を変更する変数の選択
    rand_var = rand.randint(0,2)
    #rand_var = i%3  #順番にする場合
    
    #候補の作成
    y_ = y.copy()
    y_[rand_var] = -1*y_[rand_var]

    #確率の比rと一様乱数R
    r = prob(theta,y_)/prob(theta,y)
    R = rand.random()

    #採択条件
    if R < r:
      y = y_

    #一定回数無視して乱数(x_1,x_2,x_3)生成
    if i >= n_skip:
      print("%d:%s%s%s"%(i+1-n_skip,var[y[0]],var[y[1]],var[y[2]]))
      Prob_rand[get_index_from_vars(y)] += 1
  Prob_rand = Prob_rand/np.sum(Prob_rand)

  #確率分布の出力
  print("確率分布P(X)")
  for i in range(len(x)):
    print("%s%s%s:%.4f"%(var[x[i][0]],var[x[i][1]],var[x[i][2]],Prob_d[i]))

  #生成した乱数(x_1,x_2,x_3)の割合の出力
  print("生成した乱数の割合")
  for i in range(len(x)):
    print("%s%s%s:%0.4f"%(var[x[i][0]],var[x[i][1]],var[x[i][2]],Prob_rand[i]))

if __name__=="__main__":
  main()
