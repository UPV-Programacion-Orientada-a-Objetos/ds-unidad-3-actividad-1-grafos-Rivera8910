# distutils: language = c++

from libcpp.vector cimport vector
from libcpp.string cimport string
from libcpp.pair cimport pair
from neuronet cimport GrafoDisperso

cdef class NeuroNetEngine:
    cdef GrafoDisperso* c_grafo  # Hold a pointer to the C++ instance

    def __cinit__(self):
        self.c_grafo = new GrafoDisperso()

    def __dealloc__(self):
        del self.c_grafo

    def cargar_datos(self, archivo: str):
        # Convert Python string to C++ string
        cdef string c_archivo = archivo.encode('utf-8')
        self.c_grafo.cargarDatos(c_archivo)

    def obtener_grado(self, nodo_id: int):
        return self.c_grafo.obtenerGrado(nodo_id)

    def get_vecinos(self, nodo_id: int):
        return self.c_grafo.getVecinos(nodo_id)

    def bfs(self, nodo_inicio: int, profundidad: int):
        cdef pair[vector[int], vector[pair[int, int]]] result = self.c_grafo.BFS(nodo_inicio, profundidad)
        
        # Convert C++ vectors to Python lists
        nodes = result.first
        edges = result.second
        
        py_nodes = [n for n in nodes]
        py_edges = [(e.first, e.second) for e in edges]
        
        return py_nodes, py_edges

    def obtener_numero_nodos(self):
        return self.c_grafo.obtenerNumeroNodos()

    def obtener_numero_aristas(self):
        return self.c_grafo.obtenerNumeroAristas()
        
    def estimar_memoria(self):
        return self.c_grafo.estimarMemoria()
