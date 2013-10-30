#!/usr/local/bin/python3 
import os
import numpy as np
import matplotlib.pyplot as plt 

nodes=np.genfromtxt("../utils/dump_msh_example.nodes",dtype="float")
triangles=np.genfromtxt("../utils/dump_msh_example.triangles",dtype="int")
p=nodes
t=triangles 

print( len(p) )
print( len(t) )
print( t[3] )
print( p[t[3,2]] )



#plt.axes([0,1,0,1])
#plt.Line2d([0,1,2,0])
#plt.show()
