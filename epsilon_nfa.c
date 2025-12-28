#include <stdio.h>
#include <stdlib.h>

#define MAX_STATES 20
#define MAX_SYMBOLS 10

/* المتغيرات العامة */
int n_states;
int n_symbols;

char symbols[MAX_SYMBOLS];   // الأبجدية
int start_state;

int is_final[MAX_STATES];    // الحالات النهائية القديمة
int transitions[MAX_STATES][MAX_SYMBOLS + 1][MAX_STATES];
// العمود الأخير خاص بـ ε

int new_transitions[MAX_STATES][MAX_SYMBOLS][MAX_STATES];
int new_final[MAX_STATES];

/* ================= Task 1 ================= */
void readNFA() {
    int i, j, k;

    printf("Enter number of states: ");
    scanf("%d", &n_states);

    printf("Enter number of symbols: ");
    scanf("%d", &n_symbols);

    printf("Enter symbols (example: a b):\n");
    for (i = 0; i < n_symbols; i++) {
        scanf(" %c", &symbols[i]);
    }

    printf("Enter start state: ");
    scanf("%d", &start_state);

    for (i = 0; i < n_states; i++)
        is_final[i] = 0;

    int nb_final;
    printf("Enter number of final states: ");
    scanf("%d", &nb_final);

    printf("Enter final states:\n");
    for (i = 0; i < nb_final; i++) {
        scanf("%d", &j);
        is_final[j] = 1;
    }

    for (i = 0; i < n_states; i++)
        for (j = 0; j <= n_symbols; j++)
            for (k = 0; k < n_states; k++)
                transitions[i][j][k] = 0;

    printf("\nEnter transitions (from symbol to)\n");
    printf("Use 'e' for epsilon, -1 to stop\n");

    int from, to;
    char sym;

    while (1) {
        scanf("%d", &from);
        if (from == -1) break;

        scanf(" %c %d", &sym, &to);

        int index;
        if (sym == 'e')
            index = n_symbols;   // ε
        else
            index = sym - 'a';  // نفترض الرموز a,b,c...

        transitions[from][index][to] = 1;
    }
}

/* ================= Task 2 ================= */
void epsilonClosure(int state, int closure[]) {
    int stack[MAX_STATES];
    int top = -1;
    int i;

    for (i = 0; i < n_states; i++)
        closure[i] = 0;

    stack[++top] = state;
    closure[state] = 1;

    while (top != -1) {
        int s = stack[top--];

        for (i = 0; i < n_states; i++) {
            if (transitions[s][n_symbols][i] && !closure[i]) {
                closure[i] = 1;
                stack[++top] = i;
            }
        }
    }
}

/* ================= Task 3 ================= */
void computeNewTransitions() {
    int q, a, i, j, k;
    int closure_q[MAX_STATES];
    int closure_next[MAX_STATES];

    for (q = 0; q < n_states; q++)
        for (a = 0; a < n_symbols; a++)
            for (i = 0; i < n_states; i++)
                new_transitions[q][a][i] = 0;

    for (q = 0; q < n_states; q++) {
        epsilonClosure(q, closure_q);

        for (a = 0; a < n_symbols; a++) {
            for (i = 0; i < n_states; i++) {
                if (closure_q[i]) {
                    for (j = 0; j < n_states; j++) {
                        if (transitions[i][a][j]) {
                            epsilonClosure(j, closure_next);
                            for (k = 0; k < n_states; k++) {
                                if (closure_next[k])
                                    new_transitions[q][a][k] = 1;
                            }
                        }
                    }
                }
            }
        }
    }
}

/* ================= Task 4 ================= */
void computeNewFinalStates() {
    int q, i;
    int closure[MAX_STATES];

    for (q = 0; q < n_states; q++) {
        new_final[q] = 0;
        epsilonClosure(q, closure);

        for (i = 0; i < n_states; i++) {
            if (closure[i] && is_final[i]) {
                new_final[q] = 1;
                break;
            }
        }
    }
}

/* ================= Task 5 ================= */
void displayNFA() {
    int i, j, k;

    printf("\n===== NEW NFA (WITHOUT EPSILON) =====\n");

    printf("\nStart state: %d\n", start_state);

    printf("Final states: ");
    for (i = 0; i < n_states; i++)
        if (new_final[i])
            printf("%d ", i);
    printf("\n");

    printf("\nTransitions:\n");
    for (i = 0; i < n_states; i++) {
        for (j = 0; j < n_symbols; j++) {
            for (k = 0; k < n_states; k++) {
                if (new_transitions[i][j][k]) {
                    printf("%d --%c--> %d\n", i, symbols[j], k);
                }
            }
        }
    }
}

/* ================= MAIN ================= */
int main() {
    readNFA();
    computeNewTransitions();
    computeNewFinalStates();
    displayNFA();
    return 0;
}
