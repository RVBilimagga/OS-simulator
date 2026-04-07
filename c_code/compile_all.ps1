New-Item -ItemType Directory -Path "bin" -Force | Out-Null
gcc cpu_scheduling.c -o bin/cpu_scheduling.exe
gcc file_allocation.c -o bin/file_allocation.exe
gcc file_system_dag.c -o bin/file_system_dag.exe
gcc disk_scheduling.c -o bin/disk_scheduling.exe
gcc sync_problems.c -o bin/sync_problems.exe
gcc deadlock_detection.c -o bin/deadlock_detection.exe
gcc memory_management.c -o bin/memory_management.exe
gcc contiguous_allocation.c -o bin/contiguous_allocation.exe
gcc page_replacement.c -o bin/page_replacement.exe
Write-Output "All binaries compiled in c_code/bin"
