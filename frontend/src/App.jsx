import { useEffect, useMemo, useRef, useState } from "react";

const API_BASE = "http://127.0.0.1:5000";

const SIMULATORS = [
  { label: "CPU - SJF", endpoint: "/cpu/sjf", payload: { processes: [{ arrival: 0, burst: 7 }, { arrival: 2, burst: 4 }, { arrival: 4, burst: 1 }] } },
  { label: "CPU - Multilevel Queue", endpoint: "/cpu/mlq", payload: { queues: 3, processes: [{ arrival: 0, burst: 5, queue: 0 }, { arrival: 1, burst: 4, queue: 1 }, { arrival: 2, burst: 3, queue: 2 }] } },
  { label: "CPU - MLFQ", endpoint: "/cpu/mlfq", payload: { tq1: 2, tq2: 4, processes: [{ arrival: 0, burst: 9 }, { arrival: 1, burst: 5 }, { arrival: 2, burst: 3 }] } },
  { label: "File Allocation - Sequential", endpoint: "/file/sequential", payload: { blocks: [0, 1, 0, 0, 0, 1, 0], start: 2, length: 3 } },
  { label: "File Allocation - Indexed", endpoint: "/file/indexed", payload: { blocks: [0, 0, 1, 0, 0, 0], index_block: 1, length: 3 } },
  { label: "File Allocation - Linked", endpoint: "/file/linked", payload: { blocks: [0, 0, 1, 0, 1, 0, 0], length: 4 } },
  { label: "File System - DAG", endpoint: "/filesystem/dag", payload: { directories: ["root", "docs", "bin"], files: ["a.txt", "b.txt"], shares: [{ directory: "root", file: "a.txt" }, { directory: "docs", file: "a.txt" }, { directory: "bin", file: "b.txt" }] } },
  { label: "Disk - C-SCAN", endpoint: "/disk/cscan", payload: { requests: [176, 79, 34, 60, 92, 11, 41, 114], head: 50, max_cylinder: 199 } },
  { label: "Disk - LOOK", endpoint: "/disk/look", payload: { requests: [176, 79, 34, 60, 92, 11, 41, 114], head: 50, max_cylinder: 199 } },
  { label: "Disk - C-LOOK", endpoint: "/disk/clook", payload: { requests: [176, 79, 34, 60, 92, 11, 41, 114], head: 50, max_cylinder: 199 } },
  { label: "Sync - Critical Section", endpoint: "/sync/critical", payload: { steps: 5 } },
  { label: "Sync - Readers Writers", endpoint: "/sync/readers-writers", payload: { readers: 3, writers: 2 } },
  { label: "Sync - Dining Philosophers", endpoint: "/sync/dining", payload: { philosophers: 5 } },
  { label: "Deadlock - RAG Cycle Detection", endpoint: "/deadlock/rag", payload: { processes: 2, resources: 2, edges: [{ from_type: "P", from_id: 0, to_type: "R", to_id: 0 }, { from_type: "R", from_id: 0, to_type: "P", to_id: 1 }, { from_type: "P", from_id: 1, to_type: "R", to_id: 1 }, { from_type: "R", from_id: 1, to_type: "P", to_id: 0 }] } },
  { label: "Memory - Fixed Partitioning", endpoint: "/memory/fixed", payload: { process_sizes: [70, 110, 90, 30], partition_size: 120 } },
  { label: "Memory - Variable Partitioning", endpoint: "/memory/variable", payload: { process_sizes: [120, 180, 160], memory_size: 500 } },
  { label: "Contiguous - Best Fit", endpoint: "/memory/bestfit", payload: { holes: [100, 500, 200, 300, 600], request: 212 } },
  { label: "Contiguous - Next Fit", endpoint: "/memory/nextfit", payload: { holes: [100, 500, 200, 300, 600], request: 212, start_index: 2 } },
  { label: "Paging - FIFO", endpoint: "/paging/fifo", payload: { references: [1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5], frames: 3 } },
  { label: "Paging - LRU", endpoint: "/paging/lru", payload: { references: [7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2], frames: 4 } },
  { label: "Paging - Optimal", endpoint: "/paging/optimal", payload: { references: [7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2], frames: 4 } }
];

export default function App() {
  const [selected, setSelected] = useState(SIMULATORS[0].label);
  const selectedConfig = useMemo(() => SIMULATORS.find((s) => s.label === selected), [selected]);
  const [jsonInput, setJsonInput] = useState(JSON.stringify(selectedConfig.payload, null, 2));
  const [output, setOutput] = useState([]);
  const [loading, setLoading] = useState(false);
  const [error, setError] = useState("");
  const [pickerOpen, setPickerOpen] = useState(false);
  const pickerRef = useRef(null);

  useEffect(() => {
    const onOutside = (event) => {
      if (pickerRef.current && !pickerRef.current.contains(event.target)) {
        setPickerOpen(false);
      }
    };
    window.addEventListener("mousedown", onOutside);
    return () => window.removeEventListener("mousedown", onOutside);
  }, []);

  const onSelect = (value) => {
    const cfg = SIMULATORS.find((s) => s.label === value);
    setSelected(value);
    setJsonInput(JSON.stringify(cfg.payload, null, 2));
    setOutput([]);
    setError("");
    setPickerOpen(false);
  };

  const runSimulation = async () => {
    setLoading(true);
    setError("");
    setOutput([]);
    try {
      const payload = JSON.parse(jsonInput);
      const res = await fetch(`${API_BASE}${selectedConfig.endpoint}`, {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify(payload)
      });
      const data = await res.json();
      if (!res.ok) {
        setError(data.error || "Request failed");
      } else {
        setOutput(data.output || []);
      }
    } catch (e) {
      setError(`Invalid input or server error: ${e.message}`);
    } finally {
      setLoading(false);
    }
  };

  return (
    <div className="app">
      <div className="ambient" aria-hidden="true" />

      <header className="hero">
        <p className="eyebrow">Laboratory</p>
        <h1 className="hero-title">Operating System Simulator</h1>
        <p className="hero-subtitle">
          Run scheduling, memory, disk, and synchronization models. Edit JSON, then simulate.
        </p>
      </header>

      <main className="main-grid">
        <section className="card card-input">
          <div className="card-header">
            <h2 className="card-title">Configuration</h2>
            <p className="card-desc">Choose an algorithm and supply parameters.</p>
          </div>

          <div className="field">
            <label className="field-label" htmlFor="sim-picker-button">Simulation</label>
            <div className={`picker ${pickerOpen ? "picker--open" : ""}`} ref={pickerRef}>
              <button
                id="sim-picker-button"
                className="picker-button"
                type="button"
                onClick={() => setPickerOpen((v) => !v)}
                aria-haspopup="listbox"
                aria-expanded={pickerOpen}
              >
                <span className="picker-value">{selected}</span>
                <span className="picker-chevron">⌄</span>
              </button>
              <div className={`picker-popover ${pickerOpen ? "picker-popover--open" : ""}`} role="listbox">
                {SIMULATORS.map((s) => (
                  <button
                    key={s.label}
                    type="button"
                    className={`picker-item ${selected === s.label ? "picker-item--active" : ""}`}
                    onClick={() => onSelect(s.label)}
                    role="option"
                    aria-selected={selected === s.label}
                  >
                    <span>{s.label}</span>
                    <span className="picker-check">{selected === s.label ? "✓" : ""}</span>
                  </button>
                ))}
              </div>
            </div>
          </div>

          <div className="field">
            <label className="field-label" htmlFor="json-input">Parameters</label>
            <textarea
              id="json-input"
              className="textarea"
              value={jsonInput}
              onChange={(e) => setJsonInput(e.target.value)}
              rows={14}
              spellCheck={false}
            />
          </div>

          <button
            type="button"
            className="btn-primary"
            onClick={runSimulation}
            disabled={loading}
          >
            <span className="btn-label">{loading ? "Running…" : "Run simulation"}</span>
            {!loading && <span className="btn-hint">↵</span>}
          </button>

          {error && (
            <div className="alert" role="alert">
              <span className="alert-icon">!</span>
              <span className="alert-text">{error}</span>
            </div>
          )}
        </section>

        <section className="card card-output">
          <div className="card-header">
            <h2 className="card-title">Results</h2>
            <p className="card-desc">Structured output from the C engine.</p>
          </div>

          {output.length === 0 ? (
            <div className="empty-state">
              <div className="empty-icon" aria-hidden="true" />
              <p className="empty-title">No run yet</p>
              <p className="empty-copy">Start the Flask API, pick a simulation, then run.</p>
            </div>
          ) : (
            <div className="output-scroll">
              <div className="output">
                {output.map((line, idx) => {
                  const isRow = line.includes("\t");
                  const isGantt = line.includes("GANTT") || line.includes("| P");
                  if (isRow) {
                    const cells = line.split("\t");
                    const isHeader = line.startsWith("PID");
                    return (
                      <div
                        key={idx}
                        className={`table-row ${isHeader ? "table-row--head" : ""}`}
                      >
                        {cells.map((cell, i) => (
                          <span key={i} className="table-cell">{cell}</span>
                        ))}
                      </div>
                    );
                  }
                  return (
                    <pre key={idx} className={isGantt ? "pre pre--gantt" : "pre"}>{line}</pre>
                  );
                })}
              </div>
            </div>
          )}
        </section>
      </main>

      <footer className="footer">
        <span>Backend at {API_BASE}</span>
      </footer>
    </div>
  );
}
