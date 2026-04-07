#include <stdio.h>
#include <string.h>

#define MAX_HOLES 128

static void best_fit(int holes[], int n, int req) {
    int i, idx = -1, best = 1 << 30;
    for (i = 0; i < n; i++) {
        if (holes[i] >= req && holes[i] < best) {
            best = holes[i];
            idx = i;
        }
    }
    if (idx == -1) {
        printf("RESULT: ALLOCATION_FAILED\n");
    } else {
        printf("RESULT: ALLOCATED\n");
        printf("HOLE_INDEX: %d\n", idx);
        printf("FRAGMENT_LEFT: %d\n", holes[idx] - req);
    }
}

static void next_fit(int holes[], int n, int req, int start) {
    int i;
    for (i = 0; i < n; i++) {
        int idx = (start + i) % n;
        if (holes[idx] >= req) {
            printf("RESULT: ALLOCATED\n");
            printf("HOLE_INDEX: %d\n", idx);
            printf("FRAGMENT_LEFT: %d\n", holes[idx] - req);
            return;
        }
    }
    printf("RESULT: ALLOCATION_FAILED\n");
}

int main(void) {
    char mode[16];
    int n, i, req, start = 0;
    int holes[MAX_HOLES];
    if (scanf("%15s", mode) != 1) return 1;
    if (scanf("%d", &n) != 1 || n <= 0 || n > MAX_HOLES) return 1;
    for (i = 0; i < n; i++) scanf("%d", &holes[i]);
    if (scanf("%d", &req) != 1) return 1;

    if (strcmp(mode, "bestfit") == 0) best_fit(holes, n, req);
    else if (strcmp(mode, "nextfit") == 0) {
        if (scanf("%d", &start) != 1) return 1;
        next_fit(holes, n, req, start);
    } else return 1;
    return 0;
}
