#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Oct  3 15:00:19 2022

@author: jcharles-rebuffe
"""

import matplotlib.pyplot as plt
import numpy as np
import math

fig, ax = plt.subplots(figsize=(13, 9))

x_max = 5
N = -1
lamb = -1
n = 100 # number of intervals
x = np.linspace(0, x_max, n+1)
interval_length = x_max / n
x = x[1:]
y_exp = [0] * n
y_th = [0] * n

with open("res.txt", "r") as file:
    b = 0
    for r in file:
        if b == 0:
            b = 1
            N = int(r)
            continue;
        if b == 1:
            b = 2
            lamb = float(r)
            continue;
            
        if(float(r) <= x_max):
            i_r = int(float(r) / x_max * float(n))
            y_exp[i_r] += 1

for i in range(n):
    y_exp[i] = y_exp[i] / (N * interval_length)
    y_th[i] = lamb * math.exp(- lamb * x[i])
    
    
ax.set_xlabel("x")
ax.set_ylabel("Density")
ax.plot(x, y_th)
ax.plot(x, y_exp)
ax.legend(['Theoretical distribution function', f"Experimental distribution function with {N} attempts"])

plt.savefig("plot.png")
