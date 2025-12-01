import tkinter as tk
from tkinter import filedialog, messagebox
import matplotlib.pyplot as plt
import networkx as nx
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import neuronet
import time
import os
import sys

# Add project root to sys.path to allow importing neuronet
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), "../..")))

class NeuroNetApp:
    def __init__(self, root):
        self.root = root
        self.root.title("NeuroNet: Massive Graph Analysis")
        self.root.geometry("1000x800")
        
        self.engine = neuronet.NeuroNetEngine()
        self.graph_loaded = False
        
        self._setup_ui()
        
    def _setup_ui(self):
        # Control Panel
        control_frame = tk.Frame(self.root, padx=10, pady=10, bg="#f0f0f0")
        control_frame.pack(side=tk.TOP, fill=tk.X)
        
        # Load Button
        self.btn_load = tk.Button(control_frame, text="Cargar Dataset (SNAP)", command=self.load_dataset, bg="#4CAF50", fg="white")
        self.btn_load.pack(side=tk.LEFT, padx=5)
        
        # Info Labels
        self.lbl_status = tk.Label(control_frame, text="Estado: Esperando dataset...", bg="#f0f0f0")
        self.lbl_status.pack(side=tk.LEFT, padx=20)
        
        self.lbl_stats = tk.Label(control_frame, text="", bg="#f0f0f0", font=("Arial", 9, "italic"))
        self.lbl_stats.pack(side=tk.LEFT, padx=20)

        # Analysis Panel
        analysis_frame = tk.LabelFrame(self.root, text="Análisis y Simulación", padx=10, pady=10)
        analysis_frame.pack(side=tk.TOP, fill=tk.X, padx=10, pady=5)
        
        tk.Label(analysis_frame, text="Nodo Inicio ID:").pack(side=tk.LEFT)
        self.entry_node = tk.Entry(analysis_frame, width=10)
        self.entry_node.pack(side=tk.LEFT, padx=5)
        
        tk.Label(analysis_frame, text="Profundidad BFS:").pack(side=tk.LEFT)
        self.entry_depth = tk.Entry(analysis_frame, width=5)
        self.entry_depth.insert(0, "2")
        self.entry_depth.pack(side=tk.LEFT, padx=5)
        
        self.btn_bfs = tk.Button(analysis_frame, text="Ejecutar BFS & Visualizar", command=self.run_bfs, bg="#2196F3", fg="white")
        self.btn_bfs.pack(side=tk.LEFT, padx=15)

        self.btn_degree = tk.Button(analysis_frame, text="Ver Grado Nodo", command=self.show_degree)
        self.btn_degree.pack(side=tk.LEFT, padx=5)

        # Visualization Area
        self.viz_frame = tk.Frame(self.root, bg="white")
        self.viz_frame.pack(side=tk.BOTTOM, fill=tk.BOTH, expand=True, padx=10, pady=10)
        
        self.fig, self.ax = plt.subplots(figsize=(8, 6))
        self.canvas = FigureCanvasTkAgg(self.fig, master=self.viz_frame)
        self.canvas.get_tk_widget().pack(fill=tk.BOTH, expand=True)

    def load_dataset(self):
        file_path = filedialog.askopenfilename(filetypes=[("Text Files", "*.txt"), ("All Files", "*.*")])
        if not file_path:
            return
        
        self.lbl_status.config(text="Cargando...", fg="orange")
        self.root.update()
        
        start_time = time.time()
        try:
            self.engine.cargar_datos(file_path)
            elapsed = time.time() - start_time
            
            n_nodes = self.engine.obtener_numero_nodos()
            n_edges = self.engine.obtener_numero_aristas()
            mem = self.engine.estimar_memoria() / (1024*1024)
            
            self.lbl_status.config(text="Dataset Cargado Correctamente", fg="green")
            self.lbl_stats.config(text=f"Nodos: {n_nodes:,} | Aristas: {n_edges:,} | Tiempo: {elapsed:.2f}s | RAM CSR: ~{mem:.1f} MB")
            self.graph_loaded = True
            
        except Exception as e:
            messagebox.showerror("Error", f"Error al cargar dataset: {str(e)}")
            self.lbl_status.config(text="Error en carga", fg="red")

    def run_bfs(self):
        if not self.graph_loaded:
            messagebox.showwarning("Aviso", "Primero cargue un dataset.")
            return
            
        try:
            start_node = int(self.entry_node.get())
            depth = int(self.entry_depth.get())
        except ValueError:
            messagebox.showerror("Error", "Ingrese valores numéricos válidos.")
            return
            
        start_time = time.time()
        nodes, edges = self.engine.bfs(start_node, depth)
        elapsed = time.time() - start_time
        
        print(f"[Python GUI] BFS Result: {len(nodes)} nodes, {len(edges)} edges found in {elapsed:.4f}s")
        
        if not nodes:
            messagebox.showinfo("Info", "No se encontraron nodos (¿ID incorrecto o aislado?).")
            return

        self.visualize_subgraph(nodes, edges, start_node)

    def show_degree(self):
        if not self.graph_loaded: return
        try:
            node_id = int(self.entry_node.get())
            degree = self.engine.obtener_grado(node_id)
            messagebox.showinfo("Grado del Nodo", f"El nodo {node_id} tiene grado: {degree}")
        except ValueError:
            pass

    def visualize_subgraph(self, nodes, edges, start_node):
        self.ax.clear()
        
        G = nx.Graph() # Or DiGraph if directed, but for viz usually Graph is cleaner unless arrows needed
        G.add_nodes_from(nodes)
        G.add_edges_from(edges)
        
        pos = nx.spring_layout(G, seed=42)
        
        # Draw nodes
        node_colors = ['red' if n == start_node else 'skyblue' for n in G.nodes()]
        node_sizes = [300 if n == start_node else 50 for n in G.nodes()]
        
        nx.draw(G, pos, ax=self.ax, with_labels=True, node_color=node_colors, node_size=node_sizes, font_size=8, edge_color="gray", alpha=0.7)
        self.ax.set_title(f"Subgrafo BFS desde Nodo {start_node}")
        
        self.canvas.draw()

if __name__ == "__main__":
    root = tk.Tk()
    app = NeuroNetApp(root)
    root.mainloop()
