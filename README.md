# OS Simulator

A full-stack Operating System concepts simulator with a Python backend and React frontend. Visualizes and simulates core OS algorithms interactively.

## Project Structure

```
os-simulator/
├── backend/
│   ├── app.py              # Flask API server
│   └── requirements.txt    # Python dependencies
├── c_code/
│   ├── contiguous_allocation.c
│   ├── cpu_scheduling.c
│   ├── deadlock_detection.c
│   ├── disk_scheduling.c
│   ├── file_allocation.c
│   ├── file_system_dag.c
│   ├── memory_management.c
│   ├── page_replacement.c
│   ├── sjf.c
│   ├── sync_problems.c
│   ├── compile_all.ps1
│   └── compile_all.sh
├── frontend/
│   ├── src/
│   │   ├── App.jsx
│   │   ├── main.jsx
│   │   └── styles.css
│   ├── index.html
│   ├── package.json
│   └── vite.config.js
└── .gitignore
```

## Features

- **CPU Scheduling** — FCFS, SJF, Round Robin, Priority scheduling
- **Memory Management** — Contiguous allocation, paging, segmentation
- **Page Replacement** — FIFO, LRU, Optimal algorithms
- **Disk Scheduling** — SSTF, SCAN, C-SCAN
- **Deadlock Detection** — Banker's algorithm, resource allocation graphs
- **Synchronization** — Producer-consumer, dining philosophers, readers-writers
- **File Systems** — File allocation methods, DAG-based directory structures

## Getting Started

### Prerequisites

- Python 3.8+
- Node.js 18+
- GCC (for compiling C modules)

### Backend Setup

```bash
cd backend
pip install -r requirements.txt
python app.py
```

The API server runs at `http://localhost:5000`.

### Frontend Setup

```bash
cd frontend
npm install
npm run dev
```

The frontend runs at `http://localhost:5173`.

### Compile C Modules

**Linux/macOS:**
```bash
cd c_code
chmod +x compile_all.sh
./compile_all.sh
```

**Windows (PowerShell):**
```powershell
cd c_code
./compile_all.ps1
```

Compiled binaries are output to `c_code/bin/`.

## Tech Stack

| Layer | Technology |
|-------|-----------|
| Frontend | React + Vite |
| Backend | Python (Flask) |
| Algorithms | C |
| Styling | CSS |

## Contributing

1. Fork the repo
2. Create a feature branch (`git checkout -b feature/algorithm-name`)
3. Commit your changes (`git commit -m 'add: new scheduling algorithm'`)
4. Push and open a Pull Request

## License

MIT
