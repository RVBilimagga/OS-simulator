#include <stdio.h>

#define MAX 100

// Function for Best-Fit Allocation
void bestFit(int blockSize[], int m, int processSize[], int n) {
    int allocation[MAX];

    // Initially no allocation
    for (int i = 0; i < n; i++)
        allocation[i] = -1;

    // Best Fit allocation
    for (int i = 0; i < n; i++) {
        int bestIdx = -1;

        for (int j = 0; j < m; j++) {
            if (blockSize[j] >= processSize[i]) {
                if (bestIdx == -1 || blockSize[j] < blockSize[bestIdx]) {
                    bestIdx = j;
                }
            }
        }

        if (bestIdx != -1) {
            allocation[i] = bestIdx;
            blockSize[bestIdx] -= processSize[i];
        }
    }

    // Display result
    printf("\n--- Best Fit Allocation ---\n");
    printf("Process No.\tProcess Size\tBlock No.\n");

    for (int i = 0; i < n; i++) {
        printf("%d\t\t%d\t\t", i + 1, processSize[i]);
        if (allocation[i] != -1)
            printf("%d\n", allocation[i] + 1);
        else
            printf("Not Allocated\n");
    }
}

// Function for Next-Fit Allocation
void nextFit(int blockSize[], int m, int processSize[], int n) {
    int allocation[MAX];

    for (int i = 0; i < n; i++)
        allocation[i] = -1;

    int j = 0; // last allocated index

    for (int i = 0; i < n; i++) {
        int count = 0;

        while (count < m) {
            if (blockSize[j] >= processSize[i]) {
                allocation[i] = j;
                blockSize[j] -= processSize[i];
                break;
            }

            j = (j + 1) % m;
            count++;
        }
    }

    // Display result
    printf("\n--- Next Fit Allocation ---\n");
    printf("Process No.\tProcess Size\tBlock No.\n");

    for (int i = 0; i < n; i++) {
        printf("%d\t\t%d\t\t", i + 1, processSize[i]);
        if (allocation[i] != -1)
            printf("%d\n", allocation[i] + 1);
        else
            printf("Not Allocated\n");
    }
}

int main() {
    int m, n;
    int blockSize[MAX], processSize[MAX];

    // Input
    printf("Enter number of memory blocks: ");
    scanf("%d", &m);

    printf("Enter sizes of memory blocks:\n");
    for (int i = 0; i < m; i++) {
        scanf("%d", &blockSize[i]);
    }

    printf("Enter number of processes: ");
    scanf("%d", &n);

    printf("Enter sizes of processes:\n");
    for (int i = 0; i < n; i++) {
        scanf("%d", &processSize[i]);
    }

    // Copy arrays because algorithms modify them
    int blockSize1[MAX], blockSize2[MAX];

    for (int i = 0; i < m; i++) {
        blockSize1[i] = blockSize[i];
        blockSize2[i] = blockSize[i];
    }

    // Run algorithms
    bestFit(blockSize1, m, processSize, n);
    nextFit(blockSize2, m, processSize, n);

    return 0;
}