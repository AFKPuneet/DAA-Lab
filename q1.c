#include <stdio.h>
#include <string.h>

struct Item {
    int number;
    char colour[10];
};

int main() {
    int n;

    printf("Enter number of items: ");
    scanf("%d", &n);

    struct Item items[n];
    struct Item red[n], blue[n], yellow[n];

    int r = 0, b = 0, y = 0;

    printf("Enter %d pairs (number colour):\n", n);

    for (int i = 0; i < n; i++) {
        scanf("%d %s", &items[i].number, items[i].colour);

        if (strcmp(items[i].colour, "red") == 0) {
            red[r++] = items[i];
        }
        else if (strcmp(items[i].colour, "blue") == 0) {
            blue[b++] = items[i];
        }
        else if (strcmp(items[i].colour, "yellow") == 0) {
            yellow[y++] = items[i];
        }
    }

    printf("\nSorted by colour:\n");

    for (int i = 0; i < r; i++)
        printf("%d red\n", red[i].number);

    for (int i = 0; i < b; i++)
        printf("%d blue\n", blue[i].number);

    for (int i = 0; i < y; i++)
        printf("%d yellow\n", yellow[i].number);

    return 0;
}