#ifndef GRAFOBASE_HPP
#define GRAFOBASE_HPP

#include <vector>
#include <string>
#include <iostream>

// Abstract Base Class for Graph
class GrafoBase {
public:
    virtual ~GrafoBase() {}

    // Pure virtual methods to be implemented by derived classes
    virtual void cargarDatos(const std::string& archivo) = 0;
    virtual int obtenerGrado(int nodoID) = 0;
    virtual std::vector<int> getVecinos(int nodoID) = 0;
    
    // BFS returns a pair: list of visited nodes and list of edges (u, v) for visualization
    virtual std::pair<std::vector<int>, std::vector<std::pair<int, int>>> BFS(int nodoInicio, int profundidad) = 0;
    
    virtual int obtenerNumeroNodos() = 0;
    virtual int obtenerNumeroAristas() = 0;
};

#endif // GRAFOBASE_HPP
