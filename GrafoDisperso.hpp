#ifndef GRAFODISPERSO_HPP
#define GRAFODISPERSO_HPP

#include "GrafoBase.hpp"
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <map>
#include <set>

class GrafoDisperso : public GrafoBase {
private:
    // CSR Representation
    // Since it's an unweighted graph for this problem, we might not need 'values' if all are 1.
    // But for standard CSR, we usually have:
    // - values: edge weights (can be omitted if unweighted)
    // - col_indices: column index for each non-zero value
    // - row_ptr: index in col_indices where each row starts
    
    // For unweighted graph, we just need col_indices and row_ptr.
    // However, the input is often not sorted and nodes might not be sequential 0..N-1.
    // We need to map original Node IDs to internal indices 0..N-1 for CSR to work efficiently.
    
    std::vector<int> col_indices;
    std::vector<int> row_ptr;
    
    // Mapping from external ID to internal index and vice-versa
    std::map<int, int> id_map;      // External -> Internal
    std::vector<int> reverse_map;   // Internal -> External
    
    int num_nodos;
    int num_aristas;

public:
    GrafoDisperso();
    ~GrafoDisperso();

    void cargarDatos(const std::string& archivo) override;
    int obtenerGrado(int nodoID) override;
    std::vector<int> getVecinos(int nodoID) override;
    std::pair<std::vector<int>, std::vector<std::pair<int, int>>> BFS(int nodoInicio, int profundidad) override;
    
    int obtenerNumeroNodos() override;
    int obtenerNumeroAristas() override;
    
    // Helper to check memory usage (approximation)
    long long estimarMemoria();
};

#endif // GRAFODISPERSO_HPP
