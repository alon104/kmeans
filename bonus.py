from matplotlib import pyplot as plt
from matplotlib.patches import Circle, FancyArrowPatch
from sklearn.datasets import load_iris
from sklearn.cluster import KMeans
import numpy as np

iris_data = load_iris().data
inertia_array = []
k_array = [k for k in range(1,11)]
for k in k_array:
    inertia = KMeans(n_clusters = k,  init = 'k-means++', random_state=0).fit(iris_data).inertia_/len(iris_data)
    inertia_array.append(inertia)
plt.plot(k_array, inertia_array)

circle = Circle(tuple([2, inertia_array[1]]), 0.3, facecolor="#ffffff", linewidth=1, alpha=1, linestyle = '-', edgecolor='black')
arrow = FancyArrowPatch((2 + 0.35, inertia_array[1] + 0.35), (2 + 2.5, inertia_array[1] + 2.5), mutation_scale=30, arrowstyle="<|-", linewidth=2, linestyle="--")
x = plt.gca()
x.add_patch(circle)
x.add_patch(arrow)
plt.text(4.5, inertia_array[1]+2.6, "Elbow Point", fontsize = 13)

plt.title("Elbow Method for Selection of optimal \"K\" clusters")
plt.ylabel("Average Dispresion \n ------>")
plt.xlabel("------> K")
plt.locator_params(axis='x', nbins=10)
plt.savefig("elbow.png")