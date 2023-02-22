import numpy as np
import pandas as pd
import sys
import mykmeanssp as k





def main(K, Iter,EPS,df1, df2):
    joined = df1.set_index(df1.columns[0]).join(df2.set_index(df2.columns[0]), how='inner', lsuffix='_first', rsuffix='_second')
    sorted = joined.sort_index()
    dataPoints = sorted.values.tolist()
    indices = sorted.index.tolist()
    centroidsIndices = initializeCentroids(dataPoints,K,indices)
    centroids = []
    for i in range(K):
        centroids.append(dataPoints[indices.index(centroidsIndices[i])])
    final_centroid = k.fit(K,Iter,EPS,len(centroids[0]),len(dataPoints),centroids,dataPoints)
    for i in range(K):
        print(centroidsIndices[i],end='')
        if i!=K-1:
            print(",", end='')
    print()
    for i in range(K):
        for j in range(len(centroids[0])):
            print(round(final_centroid[i][j], 4), end='')
            if j!=len(centroids[0])-1:
                print(",", end='')
        print()

def calc_distance(list1,list2):
    assert(len(list1) == len(list2))
    return sum([(list1[ind] - list2[ind]) ** 2 for ind in range(len(list1))])**0.5

def initializeCentroids(dataPoints,K,indices):
    np.random.seed(0)
    N = len(dataPoints)
    centroidIndices = []
    centroidIndices.append(int(np.random.choice(indices)))
    while (len(centroidIndices) < K):
        distances = [0 for i in range(N)]
        for vector in dataPoints:
            minDistance = float('inf')
            for ind in centroidIndices:
                distance = calc_distance(vector,dataPoints[indices.index(ind)])
                if distance < minDistance:
                    minDistance = distance
            distances[dataPoints.index(vector)] = minDistance
        total = np.sum(distances)
        probabilty = [dist/total for dist in distances]
        chosen = (np.random.choice(indices,p=probabilty))
        centroidIndices.append(int(chosen))
    return centroidIndices


def assertValidInput(cond):
	if not cond:
		exit(1)

def checkPositiveNumstr(string):
	"""
	Checks if the input string represents a valid positive integer,
	and returns both the integer and whether it is valid.
	The value of the integer is 0 if it could not be parsed correctly.
	"""
	if not string.isdecimal():
		return 0, False
	x=int(string)
	return x, (x > 1)

if __name__ == "__main__":
    num_args=len(sys.argv)
    assertValidInput(num_args in [5,6])
    K,valid = checkPositiveNumstr(sys.argv[1])
    if not valid:
        print("Invalid number of clusters!")
    assertValidInput(valid)
    i=2  
    maxiter=300  # default value
    if num_args == 6:  # the max_iter argument is present
        i+=1
        maxiter, valid=checkPositiveNumstr(sys.argv[2])
        if not valid or maxiter >= 1000:
            print("Invalid maximum iteration!")
            valid = False
        assertValidInput(valid)
    try:
        EPS = float(sys.argv[i])
    except ValueError:
        assertValidInput(False)
    assertValidInput(EPS >= 0)
    file1=sys.argv[i+1]
    file2=sys.argv[i+2]
    assertValidInput((file1.endswith((".txt", ".csv"))) and (file2.endswith((".txt", ".csv"))))
    df1 = pd.read_csv(file1, header=None)
    df2 = pd.read_csv(file2,header=None)
    try:
        df1 = pd.read_csv(file1, header=None)
        df2 = pd.read_csv(file2, header=None)
    except FileNotFoundError:
        assertValidInput(False)
    if K>=len(df1.index):
        print("Invalid number of clusters!")
        assertValidInput(False)
    main(K,maxiter,EPS,df1,df2)