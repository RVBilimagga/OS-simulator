#include <stdio.h>
#include <string.h>

#define MAX_BLOCKS 256

static void sequential_allocation(int blocks[], int n, int start, int len) {
    int i;
    int ok = 1;
    for (i = start; i < start + len && i < n; i++) {
        if (blocks[i]) ok = 0;
    }
    if (start + len > n) ok = 0;
    if (!ok) {
        printf("RESULT: ALLOCATION_FAILED\n");
        return;
    }
    for (i = start; i < start + len; i++) blocks[i] = 1;
    printf("RESULT: ALLOCATION_SUCCESS\n");
    printf("ALLOCATED_BLOCKS:");
    for (i = start; i < start + len; i++) printf(" %d", i);
    printf("\n");
}

static void indexed_allocation(int blocks[], int n, int index_block, int len) {
    int i, used = 0;
    int allocated[MAX_BLOCKS];
    if (index_block < 0 || index_block >= n || blocks[index_block]) {
        printf("RESULT: ALLOCATION_FAILED\n");
        return;
    }
    for (i = 0; i < n && used < len; i++) {
        if (!blocks[i] && i != index_block) allocated[used++] = i;
    }
    if (used < len) {
        printf("RESULT: ALLOCATION_FAILED\n");
        return;
    }
    blocks[index_block] = 1;
    for (i = 0; i < len; i++) blocks[allocated[i]] = 1;
    printf("RESULT: ALLOCATION_SUCCESS\n");
    printf("INDEX_BLOCK: %d\n", index_block);
    printf("DATA_BLOCKS:");
    for (i = 0; i < len; i++) printf(" %d", allocated[i]);
    printf("\n");
}

static void linked_allocation(int blocks[], int n, int len) {
    int i, used = 0;
    int allocated[MAX_BLOCKS];
    for (i = 0; i < n && used < len; i++) {
        if (!blocks[i]) allocated[used++] = i;
    }
    if (used < len) {
        printf("RESULT: ALLOCATION_FAILED\n");
        return;
    }
    for (i = 0; i < len; i++) blocks[allocated[i]] = 1;
    printf("RESULT: ALLOCATION_SUCCESS\n");
    printf("LINKED_LIST:\n");
    for (i = 0; i < len - 1; i++) {
        printf("%d -> %d\n", allocated[i], allocated[i + 1]);
    }
    printf("%d -> NULL\n", allocated[len - 1]);
}

int main(void) {
    char mode[32];
    int n, i;
    int blocks[MAX_BLOCKS];
    if (scanf("%31s", mode) != 1) return 1;
    if (scanf("%d", &n) != 1 || n <= 0 || n > MAX_BLOCKS) return 1;
    for (i = 0; i < n; i++) {
        if (scanf("%d", &blocks[i]) != 1) return 1;
    }
    if (strcmp(mode, "sequential") == 0) {
        int start, len;
        if (scanf("%d%d", &start, &len) != 2) return 1;
        sequential_allocation(blocks, n, start, len);
    } else if (strcmp(mode, "indexed") == 0) {
        int index_block, len;
        if (scanf("%d%d", &index_block, &len) != 2) return 1;
        indexed_allocation(blocks, n, index_block, len);
    } else if (strcmp(mode, "linked") == 0) {
        int len;
        if (scanf("%d", &len) != 1) return 1;
        linked_allocation(blocks, n, len);
    } else {
        printf("ERROR: Unsupported mode\n");
        return 1;
    }

    printf("DISK_MAP:");
    for (i = 0; i < n; i++) printf(" %d", blocks[i]);
    printf("\n");
    return 0;
}
