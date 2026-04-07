from flask import Flask, jsonify, request
from flask_cors import CORS
import os
import subprocess

app = Flask(__name__)
CORS(app)

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
BIN = os.path.join(ROOT, "c_code", "bin")


def run_binary(binary: str, stdin_text: str):
    exe = os.path.join(BIN, f"{binary}.exe" if os.name == "nt" else binary)
    if not os.path.exists(exe):
        return {"error": f"Binary not found: {exe}"}, 500
    try:
        proc = subprocess.run(
            [exe],
            input=stdin_text,
            text=True,
            capture_output=True,
            check=False,
            timeout=10,
        )
        if proc.returncode != 0:
            return {
                "error": "Execution failed",
                "stderr": proc.stderr,
                "stdout": proc.stdout,
                "returncode": proc.returncode,
            }, 400
        return {"output": proc.stdout.strip().splitlines()}, 200
    except Exception as exc:
        return {"error": str(exc)}, 500


def body():
    data = request.get_json(silent=True)
    return data if isinstance(data, dict) else {}


@app.route("/cpu/sjf", methods=["POST"])
def cpu_sjf():
    data = body()
    processes = data.get("processes", [])
    lines = [f"sjf {len(processes)}"]
    for p in processes:
        lines.append(f"{p.get('arrival', 0)} {p.get('burst', 1)}")
    return run_binary("cpu_scheduling", "\n".join(lines) + "\n")


@app.route("/cpu/mlq", methods=["POST"])
def cpu_mlq():
    data = body()
    processes = data.get("processes", [])
    queues = data.get("queues", 3)
    lines = [f"mlq {len(processes)}", str(queues)]
    for p in processes:
        lines.append(f"{p.get('arrival', 0)} {p.get('burst', 1)} {p.get('queue', 0)}")
    return run_binary("cpu_scheduling", "\n".join(lines) + "\n")


@app.route("/cpu/mlfq", methods=["POST"])
def cpu_mlfq():
    data = body()
    processes = data.get("processes", [])
    tq1, tq2 = data.get("tq1", 2), data.get("tq2", 4)
    lines = [f"mlfq {len(processes)}", f"{tq1} {tq2}"]
    for p in processes:
        lines.append(f"{p.get('arrival', 0)} {p.get('burst', 1)}")
    return run_binary("cpu_scheduling", "\n".join(lines) + "\n")


@app.route("/file/sequential", methods=["POST"])
def file_seq():
    data = body()
    blocks = data.get("blocks", [])
    lines = [f"sequential {len(blocks)}", " ".join(map(str, blocks))]
    lines.append(f"{data.get('start', 0)} {data.get('length', 1)}")
    return run_binary("file_allocation", "\n".join(lines) + "\n")


@app.route("/file/indexed", methods=["POST"])
def file_indexed():
    data = body()
    blocks = data.get("blocks", [])
    lines = [f"indexed {len(blocks)}", " ".join(map(str, blocks))]
    lines.append(f"{data.get('index_block', 0)} {data.get('length', 1)}")
    return run_binary("file_allocation", "\n".join(lines) + "\n")


@app.route("/file/linked", methods=["POST"])
def file_linked():
    data = body()
    blocks = data.get("blocks", [])
    lines = [f"linked {len(blocks)}", " ".join(map(str, blocks))]
    lines.append(str(data.get("length", 1)))
    return run_binary("file_allocation", "\n".join(lines) + "\n")


@app.route("/filesystem/dag", methods=["POST"])
def filesystem_dag():
    data = body()
    directories = data.get("directories", [])
    files = data.get("files", [])
    shares = data.get("shares", [])
    lines = [f"{len(directories)} {len(files)} {len(shares)}"]
    lines.extend(directories)
    lines.extend(files)
    for item in shares:
        lines.append(f"{item.get('directory', '')} {item.get('file', '')}")
    return run_binary("file_system_dag", "\n".join(lines) + "\n")


@app.route("/disk/cscan", methods=["POST"])
def disk_cscan():
    data = body()
    req = data.get("requests", [])
    lines = [f"cscan {len(req)} {data.get('head', 0)} {data.get('max_cylinder', 199)}"]
    lines.append(" ".join(map(str, req)))
    return run_binary("disk_scheduling", "\n".join(lines) + "\n")


@app.route("/disk/look", methods=["POST"])
def disk_look():
    data = body()
    req = data.get("requests", [])
    lines = [f"look {len(req)} {data.get('head', 0)} {data.get('max_cylinder', 199)}"]
    lines.append(" ".join(map(str, req)))
    return run_binary("disk_scheduling", "\n".join(lines) + "\n")


@app.route("/disk/clook", methods=["POST"])
def disk_clook():
    data = body()
    req = data.get("requests", [])
    lines = [f"clook {len(req)} {data.get('head', 0)} {data.get('max_cylinder', 199)}"]
    lines.append(" ".join(map(str, req)))
    return run_binary("disk_scheduling", "\n".join(lines) + "\n")


@app.route("/sync/critical", methods=["POST"])
def sync_critical():
    steps = body().get("steps", 5)
    return run_binary("sync_problems", f"critical\n{steps}\n")


@app.route("/sync/readers-writers", methods=["POST"])
def sync_rw():
    data = body()
    return run_binary("sync_problems", f"rw\n{data.get('readers', 3)} {data.get('writers', 2)}\n")


@app.route("/sync/dining", methods=["POST"])
def sync_dining():
    n = body().get("philosophers", 5)
    return run_binary("sync_problems", f"dining\n{n}\n")


@app.route("/deadlock/rag", methods=["POST"])
def deadlock_rag():
    data = body()
    processes = data.get("processes", 3)
    resources = data.get("resources", 3)
    edges = data.get("edges", [])
    lines = [f"{processes} {resources} {len(edges)}"]
    for e in edges:
        lines.append(f"{e.get('from_type', 'P')}{e.get('from_id', 0)} {e.get('to_type', 'R')}{e.get('to_id', 0)}")
    return run_binary("deadlock_detection", "\n".join(lines) + "\n")


@app.route("/memory/fixed", methods=["POST"])
def mem_fixed():
    data = body()
    processes = data.get("process_sizes", [])
    lines = [f"fixed {len(processes)}", " ".join(map(str, processes)), str(data.get("partition_size", 100))]
    return run_binary("memory_management", "\n".join(lines) + "\n")


@app.route("/memory/variable", methods=["POST"])
def mem_variable():
    data = body()
    processes = data.get("process_sizes", [])
    lines = [f"variable {len(processes)}", " ".join(map(str, processes)), str(data.get("memory_size", 500))]
    return run_binary("memory_management", "\n".join(lines) + "\n")


@app.route("/memory/bestfit", methods=["POST"])
def contig_bestfit():
    data = body()
    holes = data.get("holes", [])
    lines = [f"bestfit {len(holes)}", " ".join(map(str, holes)), str(data.get("request", 10))]
    return run_binary("contiguous_allocation", "\n".join(lines) + "\n")


@app.route("/memory/nextfit", methods=["POST"])
def contig_nextfit():
    data = body()
    holes = data.get("holes", [])
    lines = [f"nextfit {len(holes)}", " ".join(map(str, holes)), str(data.get("request", 10)), str(data.get("start_index", 0))]
    return run_binary("contiguous_allocation", "\n".join(lines) + "\n")


@app.route("/paging/fifo", methods=["POST"])
def paging_fifo():
    data = body()
    refs = data.get("references", [])
    lines = [f"fifo {len(refs)} {data.get('frames', 3)}", " ".join(map(str, refs))]
    return run_binary("page_replacement", "\n".join(lines) + "\n")


@app.route("/paging/lru", methods=["POST"])
def paging_lru():
    data = body()
    refs = data.get("references", [])
    lines = [f"lru {len(refs)} {data.get('frames', 3)}", " ".join(map(str, refs))]
    return run_binary("page_replacement", "\n".join(lines) + "\n")


@app.route("/paging/optimal", methods=["POST"])
def paging_optimal():
    data = body()
    refs = data.get("references", [])
    lines = [f"optimal {len(refs)} {data.get('frames', 3)}", " ".join(map(str, refs))]
    return run_binary("page_replacement", "\n".join(lines) + "\n")


@app.route("/health", methods=["GET"])
def health():
    return jsonify({"status": "ok"})


if __name__ == "__main__":
    app.run(debug=True, port=5000)
