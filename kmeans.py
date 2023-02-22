import sys

def dist(x,y):
    total = 0
    for i in range(len(x)):
        total += (x[i]-y[i])**2
    return total**0.5

def getClosestCentroid(x,centroids):
    minimum = float('inf')
    minCentroid = None
    for centroid in centroids:
        d = dist(centroid,x)
        if d<minimum:
            minimum = d
            minCentroid = centroid
    return minCentroid
        
def getMeanedCentroid(cluster):
    newCentroid = []
    currSum = 0
    for i in range(len(cluster[0])):
        for j in range(len(cluster)):
            currSum += cluster[j][i]
        newCentroid.append(currSum/len(cluster))
        currSum = 0
    return tuple(newCentroid)


def main(K, iter=200, path = ''):
    EPS = 0.001
    dataPoints = []
    maxIter = iter
    with open(path) as f:
        lines = f.readlines()
    for line in lines:
        asList = line.split(',')
        dataPoints.append(tuple(float(asList[i]) for i in range(len(asList))))
    if K<=1 or K>=len(dataPoints):
        print("Invalid number of clusters!")
        exit(1)
    centroids = dict({dataPoints[i]:[] for i in range(K)})
    iter = 0
    numOfNewCentroids = K
    while (iter<maxIter and numOfNewCentroids>0):
        numOfNewCentroids = 0
        for x in dataPoints:
            closest = getClosestCentroid(x,centroids)
            centroids[closest].append(x)
        newCentroids = dict()
        for centroid in centroids:
            if centroids[centroid]==[]:
                currNewCentroid = centroid
            else:
                currNewCentroid = getMeanedCentroid(centroids[centroid])
                newCentroids[currNewCentroid] = []
            if dist(centroid,currNewCentroid)>=EPS:
                numOfNewCentroids+=1
        iter+=1
        centroids = newCentroids
    asStrings = []
    for centroid in centroids:
        for i in range(len(centroid)):
            print(round(centroid[i],4),end = '')
            if i!=len(centroid)-1:
                print(',',end = '')
            else: 
                print("")
    return 
        
        
            

if __name__ == "__main__":
    args = sys.argv[1:]
    if not args[0].isdigit():
        print("Invalid number of clusters!")
        exit(1)
    k = int(args[0])
    if len(args)==2:
        main(k,path = args[1])
        exit(0)
    if len(args)==3:
        if not args[1].isdigit():
            print("Invalid maximum iteration!")
            exit(1)
        iter = int(args[1])
        if iter<=1 or iter>=1000:
            print("Invalid maximum iteration!")
            exit(1)
        main(k,iter,args[2])
        exit(0)
    else:
        print("An Error Has Occurred")
        exit(1)

