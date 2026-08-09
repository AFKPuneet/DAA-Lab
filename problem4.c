#include <stdio.h>

long moveCount = 0;

/* Recursively "solve" ToH for n discs; set printMoves=1 to also
   print each individual move (A -> C style). */
void towersOfHanoi(int n, char from, char aux, char to, int printMoves) {
    if (n == 0) return;

    towersOfHanoi(n - 1, from, to, aux, printMoves);

    moveCount++;
    if (printMoves) {
        printf("Move disc %d from %c to %c\n", n, from, to);
    }

    towersOfHanoi(n - 1, aux, from, to, printMoves);
}

int main(void) {
    int MAX_DISCS = 20;   /* keep this modest -- moves grow as 2^n - 1 */

    /* Demo: show the actual moves for a small case, n = 3 */
    printf("Example solution for n = 3 discs (pegs A, B, C):\n");
    printf("--------------------------------------------------\n");
    moveCount = 0;
    towersOfHanoi(3, 'A', 'B', 'C', 1);
    printf("Total moves for n = 3: %ld\n\n", moveCount);

    /* Main experiment: moves required for n = 1..MAX_DISCS */
    printf("n,MovesRequired,Formula_2^n-1\n");
    for (int n = 1; n <= MAX_DISCS; n++) {
        moveCount = 0;
        towersOfHanoi(n, 'A', 'B', 'C', 0);

        long formula = 1;
        for (int i = 0; i < n; i++) formula *= 2;
        formula -= 1;

        printf("%d,%ld,%ld\n", n, moveCount, formula);
    }

    printf("\nConclusion: the number of moves grows as 2^n - 1, i.e.\n");
    printf("EXPONENTIALLY with the number of discs n. This matches the\n");
    printf("recurrence T(n) = 2T(n-1) + 1, T(0) = 0, whose closed form\n");
    printf("is T(n) = 2^n - 1. A plot of moves vs n on a normal scale\n");
    printf("shoots up sharply; on a semi-log (log-y) scale it becomes a\n");
    printf("straight line, confirming exponential growth.\n");

    return 0;
}