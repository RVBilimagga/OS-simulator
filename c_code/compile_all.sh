#!/usr/bin/env bash
set -e
mkdir -p bin
gcc cpu_scheduling.c -o bin/cpu_scheduling
gcc file_allocation.c -o bin/file_allocation
gcc file_system_dag.c -o bin/file_system_dag
gcc disk_scheduling.c -o bin/disk_scheduling
gcc sync_problems.c -o bin/sync_problems
gcc deadlock_detection.c -o bin/deadlock_detection
gcc memory_management.c -o bin/memory_management
gcc contiguous_allocation.c -o bin/contiguous_allocation
gcc page_replacement.c -o bin/page_replacement
echo "All binaries compiled in c_code/bin"
