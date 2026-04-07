#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PROCS 128
#define MAX_QUEUES 8

typedef struct {
    int id;
    int arrival;
    int burst;
    int priority;
    int queue_level;
    int remaining;
    int completion;
    int waiting;
    int turnaround;
} Process;

static void print_cpu_result(Process p[], int n, const char *title) {
    double avg_wait = 0.0, avg_tat = 0.0;
    int i;
    printf("%s\n", title);
    printf("PID\tAT\tBT\tCT\tTAT\tWT\n");
    for (i = 0; i < n; i++) {
        avg_wait += p[i].waiting;
        avg_tat += p[i].turnaround;
        printf("%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].id, p[i].arrival, p[i].burst, p[i].completion, p[i].turnaround, p[i].waiting);
    }
    printf("AVG_WAITING_TIME: %.2f\n", avg_wait / n);
    printf("AVG_TURNAROUND_TIME: %.2f\n", avg_tat / n);
}

static void run_sjf(Process p[], int n) {
    int completed = 0, time = 0;
    int done[MAX_PROCS] = {0};

    while (completed < n) {
        int best = -1;
        int i;
        for (i = 0; i < n; i++) {
            if (!done[i] && p[i].arrival <= time) {
                if (best == -1 || p[i].burst < p[best].burst ||
                    (p[i].burst == p[best].burst && p[i].arrival < p[best].arrival)) {
                    best = i;
                }
            }
        }

        if (best == -1) {
            time++;
            continue;
        }

        time += p[best].burst;
        p[best].completion = time;
        p[best].turnaround = p[best].completion - p[best].arrival;
        p[best].waiting = p[best].turnaround - p[best].burst;
        done[best] = 1;
        completed++;
    }

    print_cpu_result(p, n, "SJF_NON_PREEMPTIVE_RESULT");
}

static void run_multilevel_queue(Process p[], int n, int queues) {
    int q;
    int current_time = 0;
    int completed = 0;
    int done[MAX_PROCS] = {0};
    printf("GANTT_CHART:\n");

    for (q = 0; q < queues; q++) {
        while (1) {
            int idx = -1;
            int i;
            for (i = 0; i < n; i++) {
                if (!done[i] && p[i].queue_level == q && p[i].arrival <= current_time) {
                    if (idx == -1 || p[i].arrival < p[idx].arrival) idx = i;
                }
            }

            if (idx == -1) {
                int future = -1;
                for (i = 0; i < n; i++) {
                    if (!done[i] && p[i].queue_level == q) {
                        if (future == -1 || p[i].arrival < future) future = p[i].arrival;
                    }
                }
                if (future == -1) break;
                if (future > current_time) current_time = future;
                continue;
            }

            printf("| P%d [%d-%d] ", p[idx].id, current_time, current_time + p[idx].burst);
            current_time += p[idx].burst;
            p[idx].completion = current_time;
            p[idx].turnaround = p[idx].completion - p[idx].arrival;
            p[idx].waiting = p[idx].turnaround - p[idx].burst;
            done[idx] = 1;
            completed++;
        }
    }

    while (completed < n) {
        int idx = -1, i;
        for (i = 0; i < n; i++) {
            if (!done[i]) {
                if (idx == -1 || p[i].arrival < p[idx].arrival) idx = i;
            }
        }
        if (idx == -1) break;
        if (current_time < p[idx].arrival) current_time = p[idx].arrival;
        printf("| P%d [%d-%d] ", p[idx].id, current_time, current_time + p[idx].burst);
        current_time += p[idx].burst;
        p[idx].completion = current_time;
        p[idx].turnaround = p[idx].completion - p[idx].arrival;
        p[idx].waiting = p[idx].turnaround - p[idx].burst;
        done[idx] = 1;
        completed++;
    }
    printf("|\n");
    print_cpu_result(p, n, "MULTILEVEL_QUEUE_RESULT");
}

static void run_mlfq(Process p[], int n, int tq1, int tq2) {
    int time = 0, completed = 0;
    int q1[MAX_PROCS], q2[MAX_PROCS], q3[MAX_PROCS];
    int f1 = 0, r1 = 0, f2 = 0, r2 = 0, f3 = 0, r3 = 0;
    int enqueued[MAX_PROCS] = {0};
    int i;

    for (i = 0; i < n; i++) p[i].remaining = p[i].burst;
    printf("GANTT_CHART:\n");

    while (completed < n) {
        for (i = 0; i < n; i++) {
            if (!enqueued[i] && p[i].arrival <= time) {
                q1[r1++] = i;
                enqueued[i] = 1;
            }
        }

        if (f1 < r1) {
            int idx = q1[f1++];
            int run = p[idx].remaining < tq1 ? p[idx].remaining : tq1;
            printf("| P%d(Q1) [%d-%d] ", p[idx].id, time, time + run);
            time += run;
            p[idx].remaining -= run;
            for (i = 0; i < n; i++) {
                if (!enqueued[i] && p[i].arrival <= time) {
                    q1[r1++] = i;
                    enqueued[i] = 1;
                }
            }
            if (p[idx].remaining == 0) {
                p[idx].completion = time;
                p[idx].turnaround = p[idx].completion - p[idx].arrival;
                p[idx].waiting = p[idx].turnaround - p[idx].burst;
                completed++;
            } else {
                q2[r2++] = idx;
            }
        } else if (f2 < r2) {
            int idx = q2[f2++];
            int run = p[idx].remaining < tq2 ? p[idx].remaining : tq2;
            printf("| P%d(Q2) [%d-%d] ", p[idx].id, time, time + run);
            time += run;
            p[idx].remaining -= run;
            for (i = 0; i < n; i++) {
                if (!enqueued[i] && p[i].arrival <= time) {
                    q1[r1++] = i;
                    enqueued[i] = 1;
                }
            }
            if (p[idx].remaining == 0) {
                p[idx].completion = time;
                p[idx].turnaround = p[idx].completion - p[idx].arrival;
                p[idx].waiting = p[idx].turnaround - p[idx].burst;
                completed++;
            } else {
                q3[r3++] = idx;
            }
        } else if (f3 < r3) {
            int idx = q3[f3++];
            int run = p[idx].remaining;
            printf("| P%d(Q3) [%d-%d] ", p[idx].id, time, time + run);
            time += run;
            p[idx].remaining = 0;
            p[idx].completion = time;
            p[idx].turnaround = p[idx].completion - p[idx].arrival;
            p[idx].waiting = p[idx].turnaround - p[idx].burst;
            completed++;
        } else {
            time++;
        }
    }

    printf("|\n");
    print_cpu_result(p, n, "MLFQ_RESULT");
}

int main(void) {
    char mode[64];
    int n, i;
    Process p[MAX_PROCS];

    if (scanf("%63s", mode) != 1) {
        printf("ERROR: Missing mode\n");
        return 1;
    }
    if (scanf("%d", &n) != 1 || n <= 0 || n > MAX_PROCS) {
        printf("ERROR: Invalid process count\n");
        return 1;
    }

    for (i = 0; i < n; i++) {
        p[i].id = i + 1;
        p[i].priority = 0;
        p[i].queue_level = 0;
    }

    if (strcmp(mode, "sjf") == 0) {
        for (i = 0; i < n; i++) {
            if (scanf("%d%d", &p[i].arrival, &p[i].burst) != 2) return 1;
        }
        run_sjf(p, n);
    } else if (strcmp(mode, "mlq") == 0) {
        int queues;
        if (scanf("%d", &queues) != 1 || queues <= 0 || queues > MAX_QUEUES) return 1;
        for (i = 0; i < n; i++) {
            if (scanf("%d%d%d", &p[i].arrival, &p[i].burst, &p[i].queue_level) != 3) return 1;
        }
        run_multilevel_queue(p, n, queues);
    } else if (strcmp(mode, "mlfq") == 0) {
        int tq1, tq2;
        if (scanf("%d%d", &tq1, &tq2) != 2 || tq1 <= 0 || tq2 <= 0) return 1;
        for (i = 0; i < n; i++) {
            if (scanf("%d%d", &p[i].arrival, &p[i].burst) != 2) return 1;
        }
        run_mlfq(p, n, tq1, tq2);
    } else {
        printf("ERROR: Unsupported mode\n");
        return 1;
    }

    return 0;
}
