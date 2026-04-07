#include <stdio.h>
#include <string.h>

#define MAX 128

static void fixed_partitioning(int partition_size, int n, int proc[]) {
    int i;
    int internal = 0, allocated = 0;
    for (i = 0; i < n; i++) {
        if (proc[i] <= partition_size) {
            internal += partition_size - proc[i];
            allocated++;
        }
    }
    printf("FIXED_PARTITIONING\n");
    printf("ALLOCATED_PROCESSES: %d\n", allocated);
    printf("INTERNAL_FRAGMENTATION: %d\n", internal);
}

static void variable_partitioning(int mem_size, int n, int proc[]) {
    int i, used = 0;
    int allocated = 0;
    for (i = 0; i < n; i++) {
        if (used + proc[i] <= mem_size) {
            used += proc[i];
            allocated++;
        }
    }
    printf("VARIABLE_PARTITIONING\n");
    printf("ALLOCATED_PROCESSES: %d\n", allocated);
    printf("EXTERNAL_FRAGMENTATION: %d\n", mem_size - used);
}

int main(void) {
    char mode[16];
    int n, i, x;
    int proc[MAX];
    if (scanf("%15s", mode) != 1) return 1;
    if (scanf("%d", &n) != 1 || n <= 0 || n > MAX) return 1;
    for (i = 0; i < n; i++) scanf("%d", &proc[i]);
    if (scanf("%d", &x) != 1) return 1;

    if (strcmp(mode, "fixed") == 0) fixed_partitioning(x, n, proc);
    else if (strcmp(mode, "variable") == 0) variable_partitioning(x, n, proc);
    else return 1;
    return 0;
}
