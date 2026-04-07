#include <stdio.h>
#include <string.h>

#define MAX_REF 512
#define MAX_FRAMES 32

static void print_frames(int frames[], int f) {
    int i;
    printf("[");
    for (i = 0; i < f; i++) {
        if (i) printf(" ");
        if (frames[i] == -1) printf("-");
        else printf("%d", frames[i]);
    }
    printf("]");
}

static void fifo(int ref[], int n, int f) {
    int frames[MAX_FRAMES], i, ptr = 0, faults = 0;
    for (i = 0; i < f; i++) frames[i] = -1;
    printf("FIFO_TRACE:\n");
    for (i = 0; i < n; i++) {
        int j, hit = 0;
        for (j = 0; j < f; j++) if (frames[j] == ref[i]) hit = 1;
        if (!hit) {
            frames[ptr] = ref[i];
            ptr = (ptr + 1) % f;
            faults++;
        }
        printf("%d ", ref[i]); print_frames(frames, f); printf(" %s\n", hit ? "HIT" : "FAULT");
    }
    printf("PAGE_FAULTS: %d\n", faults);
}

static void lru(int ref[], int n, int f) {
    int frames[MAX_FRAMES], used[MAX_FRAMES], i, faults = 0;
    for (i = 0; i < f; i++) { frames[i] = -1; used[i] = -1; }
    printf("LRU_TRACE:\n");
    for (i = 0; i < n; i++) {
        int j, hit = -1;
        for (j = 0; j < f; j++) if (frames[j] == ref[i]) hit = j;
        if (hit != -1) {
            used[hit] = i;
            printf("%d ", ref[i]); print_frames(frames, f); printf(" HIT\n");
        } else {
            int idx = -1, minv = 1 << 30;
            for (j = 0; j < f; j++) if (frames[j] == -1) { idx = j; break; }
            if (idx == -1) {
                for (j = 0; j < f; j++) if (used[j] < minv) { minv = used[j]; idx = j; }
            }
            frames[idx] = ref[i];
            used[idx] = i;
            faults++;
            printf("%d ", ref[i]); print_frames(frames, f); printf(" FAULT\n");
        }
    }
    printf("PAGE_FAULTS: %d\n", faults);
}

static void optimal(int ref[], int n, int f) {
    int frames[MAX_FRAMES], i, faults = 0;
    for (i = 0; i < f; i++) frames[i] = -1;
    printf("OPTIMAL_TRACE:\n");
    for (i = 0; i < n; i++) {
        int j, hit = 0;
        for (j = 0; j < f; j++) if (frames[j] == ref[i]) hit = 1;
        if (!hit) {
            int idx = -1;
            for (j = 0; j < f; j++) if (frames[j] == -1) { idx = j; break; }
            if (idx == -1) {
                int farthest = -1, k;
                for (j = 0; j < f; j++) {
                    int next = 1 << 30;
                    for (k = i + 1; k < n; k++) if (ref[k] == frames[j]) { next = k; break; }
                    if (next > farthest) { farthest = next; idx = j; }
                }
            }
            frames[idx] = ref[i];
            faults++;
        }
        printf("%d ", ref[i]); print_frames(frames, f); printf(" %s\n", hit ? "HIT" : "FAULT");
    }
    printf("PAGE_FAULTS: %d\n", faults);
}

int main(void) {
    char mode[16];
    int n, f, i;
    int ref[MAX_REF];
    if (scanf("%15s", mode) != 1) return 1;
    if (scanf("%d%d", &n, &f) != 2 || n <= 0 || n > MAX_REF || f <= 0 || f > MAX_FRAMES) return 1;
    for (i = 0; i < n; i++) scanf("%d", &ref[i]);

    if (strcmp(mode, "fifo") == 0) fifo(ref, n, f);
    else if (strcmp(mode, "lru") == 0) lru(ref, n, f);
    else if (strcmp(mode, "optimal") == 0) optimal(ref, n, f);
    else return 1;
    return 0;
}
