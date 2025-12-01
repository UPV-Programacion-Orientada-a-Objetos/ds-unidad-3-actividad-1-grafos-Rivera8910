# NeuroNet: Massive Graph Analysis Tool

## Overview
NeuroNet is a hybrid C++/Python system for analyzing massive networks using Compressed Sparse Row (CSR) matrices for memory efficiency.

## Requirements
- Python 3.8+
- C++ Compiler (MSVC on Windows, g++ on Linux/Mac)
- Python libraries: `cython`, `networkx`, `matplotlib`, `numpy`

## Installation

1. **Install Dependencies**:
   ```bash
   pip install cython networkx matplotlib numpy
   ```

2. **Compile the Extension**:
   Run the following command in the project root:
   ```bash
   python setup.py build_ext --inplace
   ```
   This will generate a `neuronet` shared library (e.g., `neuronet.cp312-win_amd64.pyd`).

## Usage

1. **Run the GUI**:
   ```bash
   python src/python/app.py
   ```

2. **Load Data**:
   - Click "Cargar Dataset" and select a SNAP benchmark file (Edge List format).
   - Example: `web-Google.txt`.

3. **Analyze**:
   - Enter a Start Node ID and BFS Depth.
   - Click "Ejecutar BFS".
   - View the visualization and console output.

## Troubleshooting
- **Missing `setuptools`**: Ensure you have a standard Python installation.
- **Compiler Errors**: Make sure you have Visual Studio Build Tools installed on Windows.
