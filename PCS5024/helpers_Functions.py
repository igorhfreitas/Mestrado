#%%

"""
Este código se refere a funções feitas para facilitar durante o cálculo de exercícios
"""

import pandas as pd
import numpy as np

from sklearn.neighbors import DistanceMetric
from sklearn.metrics import confusion_matrix
from sklearn.metrics.pairwise import pairwise_distances

def simple_linear_regression(x,y):
    """
    simple linear regression with 1 covariate
    x=list of values for covariate
    y=list of values for y

    solve y=a+bx
    """

    df=pd.DataFrame({"X":x,"Y":y})
    df["diff_from_mean_X"]=df["X"].apply(lambda X_: X_-df["X"].mean())
    df["diff_from_mean_y"]=df["Y"].apply(lambda y_: y_-df["Y"].mean())
    df["sqr_diff_from_mean_X"]=df["diff_from_mean_X"]**2
    
    beta1=(df["diff_from_mean_X"]*df["diff_from_mean_y"]).sum()/df["sqr_diff_from_mean_X"].sum()
    beta0=df["Y"].mean()-beta1*df["X"].mean()

    df["RSS"]=(df["Y"]-(beta0+beta1*df["X"]))**2

    return df, [beta0, beta1], df["RSS"].sum()

def simple_binary_distance(x1,x2):
    #
    tn, fp, fn, tp = confusion_matrix(x1, x2).ravel()
    #
    return confusion_matrix(x1,x2), (fn+fp)/(tn+fp+fn+tp)


def simple_perceptron(w,x,f):
    return f(np.dot(w,x))

def wald_statistic(cv_1,cv_2,n,alpha=0.05):
    """
    W, from wald test, is assymptotically Gaussian, so we can compare to z test
    for smaler samples, this assumption fails. So we need to compare against it
     strictaly distribution of t-test with n-1 degrees of freedom
    
    """
    from scipy import stats
    two_tailed_test=0.05/2
    z=stats.norm.ppf(1-two_tailed_test)

    df=pd.DataFrame({"Model_1":cv_1,"Model_2":cv_2})
    num=(df["Model_1"]-df["Model_2"]).sum()/n
    den_1=1/(n*(n-1))
    den_2=(np.square(df["Model_1"]-df["Model_2"]-num)).sum()
    wald=num/np.sqrt(den_1*den_2)

    ci=((df["Model_1"]-df["Model_2"]).sum()/n)-(z*np.sqrt(den_1*den_2))

    print("Reject H0") if abs(wald)>z else print("do not reject h0")
    
    return df,num,den_1,den_2,wald,z,ci

def entropy(x):
    df=pd.DataFrame({"feature":x})
    s=df["feature"].nunique()
    df=df.groupby("feature").count().to_frame().reset_index()
    """
    sum -(si.abs()/s.abs()) * log2(si.abs()/s.abs())
    após calcular a entropia do target sem nenhum split
    temos que calcular a entropia fazendo o split em cada coluna
    após isso temos que fazer a soma ponderada destas entropias
    pela % de dados do split escolhido da coluna escolhida

    o information gain e a diferença entre a entropia inicial e após o split

    """

"""
classification trees e regression trees
classification trees uses information gain usually, but it tends to 
prefer columns with many values, so use gini index instead
for regression trees uses RSS or oder istead of gini index/information gain
to decide wheter to split

post-prunning is hard, so an alternative is cost-complexity prunning
ccp
Ra=RSS (ou MC - missclassification rate) + alpha * |T|

alpha nós que escolhemos
e T é o número de terminal nodes

A ideia é cortar os galhos que não aumentem muito o RA (ou mexam no RSS)
e diminuir o T.


"""


#%%
df,num,den_1,den_2,wald,z,ci=wald_statistic([0.1,0.12,0.11],[0.15,0.1,0.13],3000,0.05)


#%%
def activ_func(a):
    return 1 if a>0 else 0

print(simple_perceptron([0.2,1,-1],[1,0.1,0.7],activ_func))
print(simple_perceptron([0.2,1,-1],[1,0.3,0.6],activ_func))
print(simple_perceptron([0.2,1,-1],[1,0.4,0.2],activ_func))
print(simple_perceptron([0.2,1,-1],[1,0.6,0.3],activ_func))
print(simple_perceptron([0.2,1,-1],[1,0.1,1.0],activ_func))
print(simple_perceptron([0.2,1,-1],[1,0.3,1.0],activ_func))
print(simple_perceptron([0.2,1,-1],[1,0.7,0.7],activ_func))
print(simple_perceptron([0.2,1,-1],[1,0.4,0.9],activ_func))

#%%
matrix,coefs,rss=simple_linear_regression([1,2,3],[1,3,8])


#%%
x1=1
x2=1
x3=1

b1=0.3
b2=-0.2
b3=0.8

w=b1*x1+b2*x2+b3*x3

p_y_1_x1_x2_x3=np.exp(w)/(1+np.exp(w))
print("1") if p_y_1_x1_x2_x3>0.5 else print("0")


#%%
p_y_0=0.8
p_x1_0_y_0=0.2
p_x1_0_y_1=0.7
p_x2_0_y_0=0.3
p_x2_0_y_1=0.6

p_x1_1_y_0=1-p_x1_0_y_0
p_x1_1_y_1=1-p_x1_0_y_1
p_x2_1_y_0=1-p_x2_0_y_0
p_x2_1_y_1=1-p_x2_0_y_1

p_y_0_x1_0_x2_0=p_y_0*p_x1_0_y_0*p_x2_0_y_0/\
                ((p_y_0*p_x1_0_y_0*p_x2_0_y_0)+((1-p_y_0)*p_x1_0_y_1*p_x2_0_y_1))

print("1") if p_y_0_x1_0_x2_0<0.5 else print("0")

p_y_0_x1_1_x2_1=p_y_0*p_x1_1_y_0*p_x2_1_y_0/\
                ((p_y_0*p_x1_1_y_0*p_x2_1_y_0)+((1-p_y_0)*p_x1_1_y_1*p_x2_1_y_1))

print("1") if p_y_0_x1_1_x2_1<0.5 else print("0")

#%%
confusion_matrix_,matching_distance=\
    simple_binary_distance(np.array([1,1,1,0,1,0,0]).reshape(-1,1),
                           np.array([0,1,1,0,0,1,0]).reshape(-1,1))

# %%
