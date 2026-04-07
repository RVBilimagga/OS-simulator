#include <stdio.h>
#include <string.h>

#define MAX_NODES 64
#define MAX_NAME 32

typedef struct {
    char name[MAX_NAME];
    int is_file;
} Node;

static Node nodes[MAX_NODES];
static int adj[MAX_NODES][MAX_NODES];
static int node_count = 0;

static int get_node(const char *name, int is_file) {
    int i;
    for (i = 0; i < node_count; i++) {
        if (strcmp(nodes[i].name, name) == 0) return i;
    }
    strcpy(nodes[node_count].name, name);
    nodes[node_count].is_file = is_file;
    return node_count++;
}

int main(void) {
    int d, f, s, i, j;
    if (scanf("%d%d%d", &d, &f, &s) != 3) return 1;

    for (i = 0; i < d; i++) {
        char name[MAX_NAME];
        scanf("%31s", name);
        get_node(name, 0);
    }
    for (i = 0; i < f; i++) {
        char name[MAX_NAME];
        scanf("%31s", name);
        get_node(name, 1);
    }

    for (i = 0; i < s; i++) {
        char dir[MAX_NAME], file[MAX_NAME];
        int d_idx, f_idx;
        scanf("%31s%31s", dir, file);
        d_idx = get_node(dir, 0);
        f_idx = get_node(file, 1);
        adj[d_idx][f_idx] = 1;
    }

    printf("DAG_FILE_SYSTEM\n");
    printf("DIRECTORY_FILE_RELATIONS:\n");
    for (i = 0; i < node_count; i++) {
        if (!nodes[i].is_file) {
            printf("%s ->", nodes[i].name);
            for (j = 0; j < node_count; j++) {
                if (adj[i][j]) printf(" %s", nodes[j].name);
            }
            printf("\n");
        }
    }
    printf("SHARED_FILES:\n");
    for (i = 0; i < node_count; i++) {
        if (nodes[i].is_file) {
            int ref = 0;
            for (j = 0; j < node_count; j++) {
                if (adj[j][i]) ref++;
            }
            if (ref > 1) printf("%s shared_by %d_directories\n", nodes[i].name, ref);
        }
    }
    return 0;
}
