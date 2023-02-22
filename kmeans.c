# include <math.h>
# include <stdio.h>
# include <stdlib.h>
# include "cap.h"

struct cord
{
    double value;
    struct cord *next;
};
struct vector
{
    struct vector *next;
    struct cord *cords;
};
int invalidMalloc = 0;
int getClosestCentroid(struct vector *curr,struct vector* centroids, int K, int vectorLen);
double distance(struct vector *x, struct vector *y, int vectorLength);
void insertToCluster(struct vector *curr,struct vector* centroids, int index);
void getMeanedCentroid(struct vector *cluster, int *clusterSize, int K, int vectorLen);
int getVectorLength(struct vector vec);
int dataLength(struct vector *vec);
void freeVectors(struct vector* head);
void freeCords(struct cord* head, int vectorLen);
void createNewCentroids(struct vector *newCentroids,int K,int vectorLen);
void zeroClusterSize(int *clusterSize, int K);
int validArg(char* str);
int is_digit(char c);
double** kmeans(int K, int maxiter, double EPS, int vectorLength,int N, double** clustersArr ,double** dataPointsArr); 
void insert_vector(struct vector **head, struct vector **tail);
void insert_cords(double value, struct cord **head, struct cord **tail);
struct vector* fill_dataPoints(double** dataPointsArr,int numOfDataPoints,int vectorLength);
void fillBackCentroids(double** oldCentroids, struct vector *centroidsVectorArr,int K,int vectorLength);

int main(int argc, char* argv[]){
    return 0;
    }

double** kmeans(int K, int maxiter, double EPS, int vectorLength,int N, double** clustersArr ,double** dataPointsArr){
    struct vector* centroidsVectorArr;
    int i=0;
    int j=0;
    int iter = 0;
    int numOfNewCentroids;
    int *clusterSize;
    struct vector *currVec;
    struct vector *head;
    head = fill_dataPoints(dataPointsArr,N,vectorLength);
    numOfNewCentroids = K;
    clusterSize = malloc(K * sizeof(int));
    centroidsVectorArr = malloc(sizeof(struct vector) * (K));
    if (clusterSize==NULL || centroidsVectorArr==NULL){
       printf("An Error Has Occurred\n");
        return NULL;
    };
    createNewCentroids(centroidsVectorArr, K, vectorLength);
    if (invalidMalloc == 1){
        return NULL;
    };
    for (i=0;i<K;i++){
        struct cord *currCentCords = centroidsVectorArr[i].cords;
            for (j=0; j<vectorLength; j++){
                currCentCords->value = clustersArr[i][j];
                currCentCords = currCentCords->next;}
    } 
    while (iter < maxiter && numOfNewCentroids > 0){
        struct vector* newCentroidsVectorArr = malloc(sizeof(struct vector) * (K));
        struct vector *currVec = head;
        struct vector* tmp;
        if (newCentroidsVectorArr==NULL){
            printf("An Error Has Occurred\n");
            return NULL;
            };
        zeroClusterSize(clusterSize,K);
        createNewCentroids(newCentroidsVectorArr, K , vectorLength);
        numOfNewCentroids = 0;
        while (currVec != NULL){
            int closestIndex = getClosestCentroid(currVec, centroidsVectorArr, K,vectorLength);
            clusterSize[closestIndex]++;
            insertToCluster(currVec, newCentroidsVectorArr, closestIndex);
            currVec = currVec->next;
    }
    getMeanedCentroid(newCentroidsVectorArr,clusterSize, K, vectorLength);
    for (i = 0; i <K; i++){
        if (distance(&newCentroidsVectorArr[i],&centroidsVectorArr[i],vectorLength)>=EPS){
            numOfNewCentroids++;
        }}
        tmp = centroidsVectorArr;
        centroidsVectorArr = newCentroidsVectorArr;
        newCentroidsVectorArr = tmp;
        for (i=0; i<K;i++){
            freeCords(newCentroidsVectorArr[i].cords,vectorLength);
        }
    
    free(newCentroidsVectorArr);
    iter++;
}

fillBackCentroids(clustersArr,centroidsVectorArr, K,vectorLength);

for (i=0; i<K;i++){
    freeCords(centroidsVectorArr[i].cords,vectorLength);
    }
currVec = head;
free(centroidsVectorArr);
free(clusterSize);
for (i = 0; i <N; i++){
    freeCords(currVec->cords,vectorLength);
    currVec = currVec->next;
}
freeVectors(head->next);
free(head);
return clustersArr;
}

int getClosestCentroid(struct vector *curr,struct vector* centroids, int K, int vectorLen){
    int closestIndex=0;
    double minimum = distance(curr,&centroids[0],vectorLen);
    int i;
    for (i=0;i<K;i++){
        double dist = distance(curr,&centroids[i],vectorLen);
        if (dist < minimum){
            minimum = dist;
            closestIndex = i;
        }
    }
    return closestIndex;
}

double distance(struct vector *x, struct vector *y, int vectorLength){
    struct cord *yCord = y->cords;
    struct cord *xCord = x->cords;
    double sum = 0.0;
    int i=0;
    for (i = 0; i < vectorLength;i++){
        sum += pow((yCord->value-xCord->value),2);
        yCord = yCord->next;
        xCord = xCord->next;
    }
    return sqrt(sum);
}

void insertToCluster(struct vector *curr,struct vector* centroids, int index){
    struct cord *tmp; 
    struct cord *centroidCords;
    tmp = curr->cords;
    centroidCords = centroids[index].cords;
    while (tmp != NULL){
        centroidCords->value+=tmp->value;
        centroidCords = centroidCords->next;
        tmp = tmp->next;
    }
}

void getMeanedCentroid(struct vector *cluster, int *clusterSize, int K, int vectorLen){
    int i;
    int j;
    for (i = 0; i <K; i++){
        struct cord *curr = cluster[i].cords; 
        for (j = 0; j < vectorLen; j++){
            curr->value/=clusterSize[i];
            curr = curr->next;
        }
    }
}

int getVectorLength(struct vector vec){
    struct cord *pt;
    int len = 0;
    pt = vec.cords;
    while (pt!=NULL){
        len++;
        pt = pt->next;
    }
    return len;
}

int dataLength(struct vector *vec){
    struct vector *curr;
    int len = 0;
    curr = vec;
    while (curr!=NULL){
        len++;
        curr = curr->next;
    }
    return len;
}

void freeVectors(struct vector* headVec)
{
   struct vector* tmp;

   while (headVec != NULL)
    {
               
       tmp = headVec;
       headVec = headVec->next;
       free(tmp);

       
    }

}
void freeCords(struct cord* headCord, int vectorLen)
{
    struct cord* tmp;
    int i;
   for (i = 0; i < vectorLen; i++)
    {
       tmp = headCord;
       headCord = headCord->next;
       free(tmp);
    }
    free(headCord);
}

void createNewCentroids(struct vector *newCentroids,int K,int vectorLen){
    int i;
    int j;
    struct cord *curr;
    for (i=0;i<K;i++){
        newCentroids[i].cords = malloc(sizeof(struct cord));
        if (newCentroids[i].cords==NULL){
            printf("An Error Has Occurred\n");
            invalidMalloc = 1;
            return;
            };
        curr = newCentroids[i].cords;
        for (j=0;j<vectorLen;j++){
            curr->value = 0;
            curr->next = malloc(sizeof(struct cord));
            if (curr->next==NULL){
            printf("An Error Has Occurred\n");
            invalidMalloc = 1;
            return;
            };
            curr = curr->next;
        }
    }
    } 

void zeroClusterSize(int *clusterSize, int K){
    int i;
    for (i = 0; i < K; i++){
        clusterSize[i] = 0;
    }
}

int validArg(char* str){
    int i=0;
    char curr = *(str);
    while (curr){
        if (!is_digit(curr)){
            return 0;
        }
        i++;
        curr = *(str + i);
    }
    return 1;
}

int is_digit(char c){
    if (c >= '0' && c <= '9'){
        return 1;
    }
    else{
        return 0;
    }
}

void insert_vector(struct vector **head, struct vector **tail) {
  struct vector *new_vec = malloc(sizeof(struct vector));
  new_vec->cords = NULL;
  new_vec->next = NULL;

  if (*head == NULL) {
    *head = new_vec;
  } else {
    (*tail)->next = new_vec;
  }
  *tail = new_vec;
}

void insert_cords(double value, struct cord **head, struct cord **tail) {
  struct cord *new_cords = malloc(sizeof(struct cord));
  new_cords->value = value;
  new_cords->next = NULL;

  if (*head == NULL) {
    *head = new_cords;
  } else {
    (*tail)->next = new_cords;
  }
  *tail = new_cords;
}

struct vector* fill_dataPoints(double** dataPointsArr,int numOfDataPoints,int vectorLength){
    int i, j;
    struct vector *vec_head = NULL, *vec_tail = NULL;
    for (i = 0; i < numOfDataPoints; i++) {
        insert_vector(&vec_head, &vec_tail);
        struct cord *cords_tail = NULL;
        for (j = 0; j < vectorLength; j++) {
            insert_cords(dataPointsArr[i][j], &(vec_tail->cords), &cords_tail);
  }
}
  return vec_head;
}

void fillBackCentroids(double** oldCentroids, struct vector *centroidsVectorArr,int K,int vectorLength){
    int i=0;
    int j=0;
    struct cord *curr_cord;
    for (i = 0; i < K; i++){
        curr_cord = centroidsVectorArr[i].cords;
        for (j = 0; j < vectorLength; j++) {
            oldCentroids[i][j] = curr_cord->value;
            curr_cord = curr_cord->next;
        }
    }
}

