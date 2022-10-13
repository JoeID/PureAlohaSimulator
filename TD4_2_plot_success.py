#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Oct  3 15:00:19 2022

@author: jcharles-rebuffe
"""

import matplotlib.pyplot as plt
import math

fig, ax = plt.subplots(figsize=(13, 9))

N_steps, N_simul = -1, -1
x = []
y_exp = []
y_th = []

with open("res_success.txt", "r") as file:
    b = 0
    for r in file:
        if b == 0:
            b = 1
            N_steps, N_simul = r.split(" ")
            N_steps = int(N_steps)
            N_simul = int(N_simul)
            continue
        
        a, b = r.split(" ")
        x.append(float(a))
        y_exp.append(float(b))
        y_th.append(float(a) * math.exp(- 2 * float(a)))
    
ax.set_xlabel("G the mean number of aggregate arrivals")
ax.set_ylabel("Throughput")
ax.plot(x, y_th)
ax.plot(x, y_exp)
ax.legend(['Theoretical throughput', f"Experimental throughput with {N_steps} packet arrivals for each of the {N_simul} simulations launched"])

plt.savefig("plot_success.png")
