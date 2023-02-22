#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "cap.h"

/*function that buildes from each vector a python list*/
PyObject* make_list(double* vector, int vector_dimension){
    int i;
    PyObject* py_coordiante;
    PyObject* py_vector = PyList_New(vector_dimension);
    for(i=0; i<vector_dimension; i++){
        py_coordiante = Py_BuildValue("d", vector[i]);
        PyList_SetItem(py_vector, i, py_coordiante);
    }
    return py_vector;
}

static PyObject* kmeans_c(PyObject *self, PyObject *args)
{   
    int K;
    int maxiter;
    double EPS;
    int vector_dimension;
    int N;
    int numOfVectors;
    int i;
    int j;
    double **vectors_list;
    double *vector;
    double **final_centroids;
    double **centroids_list;
    PyObject *Py_centroids;
    PyObject *Py_vectors_list;
    PyObject *Py_vector;
    PyObject* ret_python_list;

    
    if (!PyArg_ParseTuple(args, "iidiiOO",&K, &maxiter, &EPS, &vector_dimension, &N, &Py_centroids, &Py_vectors_list)){
        return NULL;
    }

/*building array of centroids*/
    centroids_list = (double **) malloc(sizeof(double *)*K);
    vector = (double *) malloc(sizeof(double)*vector_dimension);
    for(i=0; i<K; i++){
        Py_vector = PyList_GetItem(Py_centroids, i);
        for(j=0; j<vector_dimension; j++){
            *(vector+j) = PyFloat_AsDouble(PyList_GetItem(Py_vector, j));
        }
        *(centroids_list+i) = vector;
        vector = (double *) malloc(sizeof(double)*vector_dimension);
    }

/*building array of all the vectors*/
    numOfVectors = PyObject_Length(Py_vectors_list);
    vectors_list = (double **) malloc(sizeof(double *)*numOfVectors);
    for(i=0; i<numOfVectors; i++){
        Py_vector = PyList_GetItem(Py_vectors_list, i);
        for(j=0; j<vector_dimension; j++){
            *(vector+j) = PyFloat_AsDouble(PyList_GetItem(Py_vector, j));
        }
        *(vectors_list+i) = vector;
        vector = (double *) malloc(sizeof(double)*vector_dimension);
    }
    free(vector);

    final_centroids = (double **)kmeans(K, maxiter, EPS, vector_dimension, N, centroids_list, vectors_list);

    /*building python list from C array*/
    ret_python_list = PyList_New(K);
    for(i=0; i<K; i++){
        Py_vector = make_list(final_centroids[i], vector_dimension);
        PyList_SetItem(ret_python_list, i, Py_vector);
    }
    
    /*free centroids returned from kmeans algorithm*/
    for(i=0; i<K; i++){
        free(final_centroids[i]);
    }
    free(final_centroids);

    /*return the final list of centroids to python*/
    return Py_BuildValue("O",ret_python_list);
}

static PyMethodDef kmeansMethods[] = {
    {"fit",
    (PyCFunction) kmeans_c,
    METH_VARARGS,
    "A fit method for the kmeans algorithm"
    },
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef kmeansmodule = 
{
    PyModuleDef_HEAD_INIT,
    "mykmeanssp",
    NULL,
    -1,
    kmeansMethods
};

PyMODINIT_FUNC PyInit_mykmeanssp (void)
{
    PyObject *m;
    m = PyModule_Create(&kmeansmodule);
    if (!m){
        return NULL;
    }
    return m;
};

