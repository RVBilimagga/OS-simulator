#include <stdio.h>
#include <string.h>

static void critical_section(int steps) {
    int shared = 0, i;
    printf("CRITICAL_SECTION_SIMULATION\n");
    printf("RACE_CONDITION_TRACE:\n");
    for (i = 0; i < steps; i++) {
        int temp1 = shared;
        int temp2 = shared;
        temp1++;
        temp2++;
        shared = temp2;
        printf("Step %d -> shared=%d\n", i + 1, shared);
    }
    printf("SEMAPHORE_SOLUTION_TRACE:\n");
    shared = 0;
    for (i = 0; i < steps; i++) {
        shared++;
        shared++;
        printf("Step %d -> shared=%d\n", i + 1, shared);
    }
}

static void readers_writers(int readers, int writers) {
    int i;
    printf("READERS_WRITERS_TRACE\n");
    for (i = 1; i <= readers; i++) {
        printf("Reader%d entering shared data\n", i);
    }
    for (i = 1; i <= readers; i++) {
        printf("Reader%d leaving shared data\n", i);
    }
    for (i = 1; i <= writers; i++) {
        printf("Writer%d writing exclusively\n", i);
    }
}

static void dining_philosophers(int n) {
    int i;
    printf("DINING_PHILOSOPHERS_TRACE\n");
    for (i = 0; i < n; i++) {
        printf("P%d THINKING\n", i);
    }
    for (i = 0; i < n; i += 2) {
        printf("P%d EATING\n", i);
    }
    for (i = 1; i < n; i += 2) {
        printf("P%d EATING\n", i);
    }
    printf("DEADLOCK_AVOIDED_BY_ORDERING_FORKS\n");
}

int main(void) {
    char mode[32];
    if (scanf("%31s", mode) != 1) return 1;
    if (strcmp(mode, "critical") == 0) {
        int steps;
        if (scanf("%d", &steps) != 1) return 1;
        critical_section(steps);
    } else if (strcmp(mode, "rw") == 0) {
        int r, w;
        if (scanf("%d%d", &r, &w) != 2) return 1;
        readers_writers(r, w);
    } else if (strcmp(mode, "dining") == 0) {
        int n;
        if (scanf("%d", &n) != 1) return 1;
        dining_philosophers(n);
    } else {
        return 1;
    }
    return 0;
}
