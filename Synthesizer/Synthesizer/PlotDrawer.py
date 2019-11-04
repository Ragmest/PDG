# -*- coding: utf-8 -*-

import matplotlib as mp
import numpy as np
import matplotlib.pyplot as plt

def regular(lines):
	labelX = lines[1]
	labelY = lines[2]
	valuesX = np.array(lines[3].split(',')).astype(np.float)
	valuesY = np.array(lines[4].split(',')).astype(np.float)

	plt.xlabel(labelX)
	plt.ylabel(labelY)
	plt.grid(color='black', linestyle='-', linewidth=0.5)

	plt.plot(valuesX, valuesY)

	plt.show()
	
def grid(lines):

	matrix = []
	for i in range(1, len(lines)):
		row = np.array(lines[i].split(',')).astype(np.float)
		matrix.append(row)

	plt.pcolor(matrix, cmap='Greys')
	plt.show()
		
parameters = open("parameters.txt")

input = parameters.readlines()

if input[0].rstrip() == "grid":
	grid(input)
if input[0].rstrip() == "regular":
	regular(input)