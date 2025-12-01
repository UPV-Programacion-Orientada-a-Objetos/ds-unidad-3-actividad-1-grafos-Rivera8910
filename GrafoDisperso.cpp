#include "GrafoDisperso.hpp"
#include <iostream>
#include <queue>
#include <set>

GrafoDisperso::GrafoDisperso() : num_nodos(0), num_aristas(0) {}

GrafoDisperso::~GrafoDisperso() {}

void GrafoDisperso::cargarDatos(const std::string& archivo) {
    std::cout << "[C++ Core] Cargando dataset '" << archivo << "'..." << std::endl;
    
    std::ifstream file(archivo);
    if (!file.is_open()) {
        std::cerr << "[C++ Core] Error: No se pudo abrir el archivo." << std::endl;
        return;
    }

    std::string line;
    std::vector<std::pair<int, int>> edges;
    std::set<int> unique_nodes;

    // 1. Read file and collect all edges and unique nodes
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue; // Skip comments
        
        std::stringstream ss(line);
        int u, v;
        if (ss >> u >> v) {
            edges.push_back({u, v});
            unique_nodes.insert(u);
            unique_nodes.insert(v);
            
            // If undirected, add reverse edge? 
            // The prompt says "Networks like Internet, Social". Usually undirected or directed.
            // "Edge List (NodoOrigen NodoDestino)". 
            // Let's assume directed for CSR simplicity, or treat as undirected by adding both.
            // For robustness analysis (connectivity), usually undirected is implied unless specified.
            // Let's add both directions to be safe for "connectivity" unless it's strictly directed.
            // Wait, "web-Google" is directed. "amazon" is directed.
            // But "Robustness... failure propagation" often implies flow.
            // Let's stick to the input file format. If the user wants undirected, the file usually has both or we treat it as such.
            // For now, I will store exactly what is in the file. If it's a directed graph, CSR is perfect.
        }
    }
    file.close();

    num_aristas = edges.size();
    num_nodos = unique_nodes.size();

    std::cout << "[C++ Core] Archivo leido. Nodos unicos: " << num_nodos << ", Aristas: " << num_aristas << std::endl;

    // 2. Map external IDs to internal 0..N-1
    int current_idx = 0;
    id_map.clear();
    reverse_map.resize(num_nodos);
    
    for (int id : unique_nodes) {
        id_map[id] = current_idx;
        reverse_map[current_idx] = id;
        current_idx++;
    }

    // 3. Convert edges to adjacency list (temporary) to sort and build CSR
    std::vector<std::vector<int>> adj(num_nodos);
    for (const auto& edge : edges) {
        int u_idx = id_map[edge.first];
        int v_idx = id_map[edge.second];
        adj[u_idx].push_back(v_idx);
        // If we want undirected, we would uncomment:
        // adj[v_idx].push_back(u_idx); 
    }

    // 4. Build CSR
    row_ptr.clear();
    col_indices.clear();
    row_ptr.push_back(0);

    for (int i = 0; i < num_nodos; ++i) {
        // Sort neighbors for faster access/consistency
        std::sort(adj[i].begin(), adj[i].end());
        
        for (int neighbor : adj[i]) {
            col_indices.push_back(neighbor);
        }
        row_ptr.push_back(col_indices.size());
    }

    std::cout << "[C++ Core] Estructura CSR construida. Memoria estimada: " << estimarMemoria() / (1024*1024) << " MB." << std::endl;
}

int GrafoDisperso::obtenerGrado(int nodoID) {
    if (id_map.find(nodoID) == id_map.end()) return 0;
    int u_idx = id_map[nodoID];
    return row_ptr[u_idx + 1] - row_ptr[u_idx];
}

std::vector<int> GrafoDisperso::getVecinos(int nodoID) {
    std::vector<int> vecinos;
    if (id_map.find(nodoID) == id_map.end()) return vecinos;
    
    int u_idx = id_map[nodoID];
    int start = row_ptr[u_idx];
    int end = row_ptr[u_idx + 1];
    
    for (int i = start; i < end; ++i) {
        vecinos.push_back(reverse_map[col_indices[i]]);
    }
    return vecinos;
}

std::pair<std::vector<int>, std::vector<std::pair<int, int>>> GrafoDisperso::BFS(int nodoInicio, int profundidad) {
    std::vector<int> visitados;
    std::vector<std::pair<int, int>> aristas_visitadas;
    
    if (id_map.find(nodoInicio) == id_map.end()) return {visitados, aristas_visitadas};

    int start_idx = id_map[nodoInicio];
    
    // Queue stores {node_idx, current_depth}
    std::queue<std::pair<int, int>> q;
    q.push({start_idx, 0});
    
    std::set<int> visited_set;
    visited_set.insert(start_idx);
    
    visitados.push_back(nodoInicio);

    while (!q.empty()) {
        int u_idx = q.front().first;
        int depth = q.front().second;
        q.pop();

        if (depth >= profundidad) continue;

        int start = row_ptr[u_idx];
        int end = row_ptr[u_idx + 1];

        for (int i = start; i < end; ++i) {
            int v_idx = col_indices[i];
            
            // Add edge to result
            aristas_visitadas.push_back({reverse_map[u_idx], reverse_map[v_idx]});
            
            if (visited_set.find(v_idx) == visited_set.end()) {
                visited_set.insert(v_idx);
                visitados.push_back(reverse_map[v_idx]);
                q.push({v_idx, depth + 1});
            }
        }
    }
    
    return {visitados, aristas_visitadas};
}

int GrafoDisperso::obtenerNumeroNodos() {
    return num_nodos;
}

int GrafoDisperso::obtenerNumeroAristas() {
    return num_aristas;
}

long long GrafoDisperso::estimarMemoria() {
    long long mem = 0;
    mem += col_indices.size() * sizeof(int);
    mem += row_ptr.size() * sizeof(int);
    mem += id_map.size() * (sizeof(int) * 2 + 16); // Rough map overhead
    mem += reverse_map.size() * sizeof(int);
    return mem;
}
