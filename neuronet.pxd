from libcpp.vector cimport vector
from libcpp.string cimport string
from libcpp.pair cimport pair

cdef extern from "../cpp/GrafoBase.hpp":
    cdef cppclass GrafoBase:
        pass

cdef extern from "../cpp/GrafoDisperso.hpp":
    cdef cppclass GrafoDisperso(GrafoBase):
        GrafoDisperso() except +
        void cargarDatos(string archivo)
        int obtenerGrado(int nodoID)
        vector[int] getVecinos(int nodoID)
        pair[vector[int], vector[pair[int, int]]] BFS(int nodoInicio, int profundidad)
        int obtenerNumeroNodos()
        int obtenerNumeroAristas()
        long long estimarMemoria()
