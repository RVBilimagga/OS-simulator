#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_REQ 256

static int cmp(const void *a, const void *b) {
    return (*(int *)a) - (*(int *)b);
}

static void print_path(int path[], int len) {
    int i, total = 0;
    printf("SEEK_PATH:");
    for (i = 0; i < len; i++) printf(" %d", path[i]);
    printf("\n");
    for (i = 1; i < len; i++) total += abs(path[i] - path[i - 1]);
    printf("TOTAL_SEEK: %d\n", total);
}

static void run_cscan(int req[], int n, int head, int max_cyl) {
    int arr[MAX_REQ], i, k = 0, p = 0;
    for (i = 0; i < n; i++) arr[i] = req[i];
    qsort(arr, n, sizeof(int), cmp);
    int path[MAX_REQ + 4];
    path[p++] = head;
    for (i = 0; i < n; i++) if (arr[i] >= head) path[p++] = arr[i];
    path[p++] = max_cyl;
    path[p++] = 0;
    for (k = 0; k < n; k++) if (arr[k] < head) path[p++] = arr[k];
    print_path(path, p);
}

static void run_look(int req[], int n, int head) {
    int arr[MAX_REQ], i, p = 0;
    for (i = 0; i < n; i++) arr[i] = req[i];
    qsort(arr, n, sizeof(int), cmp);
    int path[MAX_REQ + 1];
    path[p++] = head;
    for (i = 0; i < n; i++) if (arr[i] >= head) path[p++] = arr[i];
    for (i = n - 1; i >= 0; i--) if (arr[i] < head) path[p++] = arr[i];
    print_path(path, p);
}

static void run_clook(int req[], int n, int head) {
    int arr[MAX_REQ], i, p = 0;
    for (i = 0; i < n; i++) arr[i] = req[i];
    qsort(arr, n, sizeof(int), cmp);
    int path[MAX_REQ + 2];
    path[p++] = head;
    for (i = 0; i < n; i++) if (arr[i] >= head) path[p++] = arr[i];
    for (i = 0; i < n; i++) if (arr[i] < head) path[p++] = arr[i];
    print_path(path, p);
}

int main(void) {
    char mode[16];
    int n, i, head, max_cyl;
    int req[MAX_REQ];
    if (scanf("%15s", mode) != 1) return 1;
    if (scanf("%d%d%d", &n, &head, &max_cyl) != 3 || n <= 0 || n > MAX_REQ) return 1;
    for (i = 0; i < n; i++) scanf("%d", &req[i]);

    if (strcmp(mode, "cscan") == 0) run_cscan(req, n, head, max_cyl);
    else if (strcmp(mode, "look") == 0) run_look(req, n, head);
    else if (strcmp(mode, "clook") == 0) run_clook(req, n, head);
    else return 1;
    return 0;
}
