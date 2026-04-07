#include <stdio.h>

#define MAX 32

static int n, m;
static int graph[MAX][MAX];
static int visited[MAX], rec[MAX];

static int dfs(int u) {
    int v;
    visited[u] = 1;
    rec[u] = 1;
    for (v = 0; v < n + m; v++) {
        if (graph[u][v]) {
            if (!visited[v] && dfs(v)) return 1;
            if (rec[v]) return 1;
        }
    }
    rec[u] = 0;
    return 0;
}

int main(void) {
    int e, i;
    if (scanf("%d%d%d", &n, &m, &e) != 3) return 1;
    for (i = 0; i < e; i++) {
        char t1, t2;
        int a, b;
        int u, v;
        if (scanf(" %c%d %c%d", &t1, &a, &t2, &b) != 4) return 1;
        u = (t1 == 'P') ? a : (n + a);
        v = (t2 == 'P') ? b : (n + b);
        graph[u][v] = 1;
    }

    printf("RAG_ADJACENCY:\n");
    for (i = 0; i < n + m; i++) {
        int j;
        for (j = 0; j < n + m; j++) printf("%d ", graph[i][j]);
        printf("\n");
    }

    for (i = 0; i < n + m; i++) {
        if (!visited[i] && dfs(i)) {
            printf("CYCLE_DETECTED: YES\n");
            printf("DEADLOCK: POSSIBLE\n");
            return 0;
        }
    }
    printf("CYCLE_DETECTED: NO\n");
    printf("DEADLOCK: NOT_PRESENT\n");
    return 0;
}
