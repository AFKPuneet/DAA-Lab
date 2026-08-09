#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* ============================= Array-based ============================= */

typedef struct {
    int *data;
    int size;
} ArrayDS;

ArrayDS arrayCreate(int capacity) {
    ArrayDS a;
    a.data = malloc(capacity * sizeof(int));
    a.size = 0;
    return a;
}

/* --- Unsorted array --- */
void auInsert(ArrayDS *a, int x) { a->data[a->size++] = x; }

int auSearch(ArrayDS *a, int k) {
    for (int i = 0; i < a->size; i++) if (a->data[i] == k) return i;
    return -1;
}

void auDeleteAt(ArrayDS *a, int idx) {          /* O(1): swap-with-last trick */
    a->data[idx] = a->data[a->size - 1];
    a->size--;
}

int auMax(ArrayDS *a) {
    int m = a->data[0];
    for (int i = 1; i < a->size; i++) if (a->data[i] > m) m = a->data[i];
    return m;
}
int auMin(ArrayDS *a) {
    int m = a->data[0];
    for (int i = 1; i < a->size; i++) if (a->data[i] < m) m = a->data[i];
    return m;
}
int auPredecessor(ArrayDS *a, int idx) {        /* value just below a->data[idx] */
    int x = a->data[idx], best = -1;
    for (int i = 0; i < a->size; i++)
        if (a->data[i] < x && (best == -1 || a->data[i] > best)) best = a->data[i];
    return best;
}
int auSuccessor(ArrayDS *a, int idx) {
    int x = a->data[idx], best = -1;
    for (int i = 0; i < a->size; i++)
        if (a->data[i] > x && (best == -1 || a->data[i] < best)) best = a->data[i];
    return best;
}

/* --- Sorted array --- */
int saSearch(ArrayDS *a, int k) {               /* binary search */
    int lo = 0, hi = a->size - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (a->data[mid] == k) return mid;
        if (a->data[mid] < k) lo = mid + 1; else hi = mid - 1;
    }
    return -1;
}
void saInsert(ArrayDS *a, int x) {              /* find position + shift: O(n) */
    int i = a->size - 1;
    a->size++;
    while (i >= 0 && a->data[i] > x) { a->data[i + 1] = a->data[i]; i--; }
    a->data[i + 1] = x;
}
void saDeleteAt(ArrayDS *a, int idx) {          /* shift left: O(n) */
    for (int i = idx; i < a->size - 1; i++) a->data[i] = a->data[i + 1];
    a->size--;
}
int saMax(ArrayDS *a) { return a->data[a->size - 1]; }
int saMin(ArrayDS *a) { return a->data[0]; }
int saPredecessor(ArrayDS *a, int idx) { return idx > 0 ? a->data[idx - 1] : -1; }
int saSuccessor(ArrayDS *a, int idx)   { return idx < a->size - 1 ? a->data[idx + 1] : -1; }

/* ============================= Singly linked ============================= */

typedef struct SNode { int key; struct SNode *next; } SNode;
typedef struct { SNode *head; } SLL;

SNode *newSNode(int x) { SNode *n = malloc(sizeof(SNode)); n->key = x; n->next = NULL; return n; }

/* --- Singly unsorted list --- */
void suInsert(SLL *l, int x) { SNode *n = newSNode(x); n->next = l->head; l->head = n; }  /* O(1) */
SNode *suSearch(SLL *l, int k) { for (SNode *p = l->head; p; p = p->next) if (p->key == k) return p; return NULL; }
void suDelete(SLL *l, SNode *target) {          /* O(n): must find predecessor */
    if (l->head == target) { l->head = target->next; free(target); return; }
    SNode *p = l->head;
    while (p->next != target) p = p->next;
    p->next = target->next;
    free(target);
}
int suMax(SLL *l) { int m = l->head->key; for (SNode *p = l->head; p; p = p->next) if (p->key > m) m = p->key; return m; }
int suMin(SLL *l) { int m = l->head->key; for (SNode *p = l->head; p; p = p->next) if (p->key < m) m = p->key; return m; }
int suPredecessor(SLL *l, int x) { int best = -1; for (SNode *p = l->head; p; p = p->next) if (p->key < x && (best == -1 || p->key > best)) best = p->key; return best; }
int suSuccessor(SLL *l, int x)   { int best = -1; for (SNode *p = l->head; p; p = p->next) if (p->key > x && (best == -1 || p->key < best)) best = p->key; return best; }

/* --- Singly sorted list --- */
void ssInsert(SLL *l, int x) {                  /* O(n): find sorted position */
    SNode *n = newSNode(x);
    if (!l->head || l->head->key >= x) { n->next = l->head; l->head = n; return; }
    SNode *p = l->head;
    while (p->next && p->next->key < x) p = p->next;
    n->next = p->next;
    p->next = n;
}
SNode *ssSearch(SLL *l, int k) { for (SNode *p = l->head; p; p = p->next) if (p->key == k) return p; return NULL; }
void ssDelete(SLL *l, SNode *target) { suDelete(l, target); }   /* same O(n) reasoning */
int ssMin(SLL *l) { return l->head->key; }                       /* O(1): sorted, head is smallest */
int ssMax(SLL *l) { SNode *p = l->head; while (p->next) p = p->next; return p->key; }  /* O(n): no tail ptr */
int ssPredecessor(SLL *l, SNode *target) {       /* O(n): walk from head */
    if (l->head == target) return -1;
    SNode *p = l->head;
    while (p->next != target) p = p->next;
    return p->key;
}
int ssSuccessor(SLL *l, SNode *target) { return target->next ? target->next->key : -1; }  /* O(1) */

/* ============================= Doubly linked ============================= */

typedef struct DNode { int key; struct DNode *prev, *next; } DNode;
typedef struct { DNode *head, *tail; } DLL;

DNode *newDNode(int x) { DNode *n = malloc(sizeof(DNode)); n->key = x; n->prev = n->next = NULL; return n; }

/* --- Doubly unsorted list --- */
void duInsert(DLL *l, int x) {                  /* O(1): insert at head */
    DNode *n = newDNode(x);
    n->next = l->head;
    if (l->head) l->head->prev = n;
    l->head = n;
    if (!l->tail) l->tail = n;
}
DNode *duSearch(DLL *l, int k) { for (DNode *p = l->head; p; p = p->next) if (p->key == k) return p; return NULL; }
void duDelete(DLL *l, DNode *target) {          /* O(1): direct unlink via prev/next */
    if (target->prev) target->prev->next = target->next; else l->head = target->next;
    if (target->next) target->next->prev = target->prev; else l->tail = target->prev;
    free(target);
}
int duMax(DLL *l) { int m = l->head->key; for (DNode *p = l->head; p; p = p->next) if (p->key > m) m = p->key; return m; }
int duMin(DLL *l) { int m = l->head->key; for (DNode *p = l->head; p; p = p->next) if (p->key < m) m = p->key; return m; }
int duPredecessor(DLL *l, int x) { int best = -1; for (DNode *p = l->head; p; p = p->next) if (p->key < x && (best == -1 || p->key > best)) best = p->key; return best; }
int duSuccessor(DLL *l, int x)   { int best = -1; for (DNode *p = l->head; p; p = p->next) if (p->key > x && (best == -1 || p->key < best)) best = p->key; return best; }

/* --- Doubly sorted list --- */
void dsInsert(DLL *l, int x) {                  /* O(n): find position, O(1) link */
    DNode *n = newDNode(x);
    if (!l->head || l->head->key >= x) {
        n->next = l->head;
        if (l->head) l->head->prev = n; else l->tail = n;
        l->head = n;
        return;
    }
    DNode *p = l->head;
    while (p->next && p->next->key < x) p = p->next;
    n->next = p->next;
    n->prev = p;
    if (p->next) p->next->prev = n; else l->tail = n;
    p->next = n;
}
DNode *dsSearch(DLL *l, int k) { for (DNode *p = l->head; p; p = p->next) if (p->key == k) return p; return NULL; }
void dsDelete(DLL *l, DNode *target) { duDelete(l, target); }    /* O(1) */
int dsMax(DLL *l) { return l->tail->key; }                        /* O(1) */
int dsMin(DLL *l) { return l->head->key; }                        /* O(1) */
int dsPredecessor(DLL *l, DNode *target) { return target->prev ? target->prev->key : -1; }  /* O(1) */
int dsSuccessor(DLL *l, DNode *target)   { return target->next ? target->next->key : -1; }  /* O(1) */

/* ============================= Timing harness ============================= */

double nowSeconds(void) { return (double) clock() / CLOCKS_PER_SEC; }

/* volatile sink: forces the compiler to actually keep every operation's
   result instead of optimizing away "unused" computations under -O2 */
volatile int sink;

int main(void) {
    srand(1);   /* fixed seed for reproducibility */

    int sizes[] = {1000, 2000, 4000, 8000, 16000, 32000, 64000};
    int numSizes = sizeof(sizes) / sizeof(sizes[0]);
    int REPS = 5000;   /* repeat read-only ops this many times to get a measurable time */

    printf("structure,operation,n,time_seconds\n");

    for (int s = 0; s < numSizes; s++) {
        int n = sizes[s];

        /* ---------- build each structure with the SAME n sorted-distinct values ---------- */
        ArrayDS au = arrayCreate(n + REPS + 10);
        ArrayDS sa = arrayCreate(n + REPS + 10);
        SLL su = {NULL}, ss = {NULL};
        DLL du = {NULL, NULL}, ds = {NULL, NULL};

        for (int i = 0; i < n; i++) {
            au.data[au.size++] = i;      /* unsorted array: values 0..n-1 in order (order irrelevant) */
            sa.data[sa.size++] = i;      /* sorted array:   already ascending */
            suInsert(&su, i);
            ssInsert(&ss, i);            /* inserted in increasing order -> ends up sorted, still O(n)/insert in general case */
            duInsert(&du, i);
            dsInsert(&ds, i);
        }

        int missingKey = -1;             /* guarantees worst-case (not found) search */
        int midKey = n / 2;

        double t0, t1;

        /* ---- SEARCH (worst case: key absent -> full scan / full binary search) ---- */
        t0 = nowSeconds();
        for (int r = 0; r < REPS; r++) sink = auSearch(&au, missingKey);
        t1 = nowSeconds();
        printf("UnsortedArray,Search,%d,%.8f\n", n, (t1 - t0) / REPS);

        t0 = nowSeconds();
        for (int r = 0; r < REPS; r++) sink = saSearch(&sa, missingKey);
        t1 = nowSeconds();
        printf("SortedArray,Search,%d,%.8f\n", n, (t1 - t0) / REPS);

        t0 = nowSeconds();
        for (int r = 0; r < REPS; r++) sink = (int)(long) suSearch(&su, missingKey);
        t1 = nowSeconds();
        printf("SinglyUnsortedList,Search,%d,%.8f\n", n, (t1 - t0) / REPS);

        t0 = nowSeconds();
        for (int r = 0; r < REPS; r++) sink = (int)(long) ssSearch(&ss, missingKey);
        t1 = nowSeconds();
        printf("SinglySortedList,Search,%d,%.8f\n", n, (t1 - t0) / REPS);

        t0 = nowSeconds();
        for (int r = 0; r < REPS; r++) sink = (int)(long) duSearch(&du, missingKey);
        t1 = nowSeconds();
        printf("DoublyUnsortedList,Search,%d,%.8f\n", n, (t1 - t0) / REPS);

        t0 = nowSeconds();
        for (int r = 0; r < REPS; r++) sink = (int)(long) dsSearch(&ds, missingKey);
        t1 = nowSeconds();
        printf("DoublySortedList,Search,%d,%.8f\n", n, (t1 - t0) / REPS);

        /* ---- MAX ---- */
        t0 = nowSeconds(); for (int r = 0; r < REPS; r++) sink = auMax(&au); t1 = nowSeconds();
        printf("UnsortedArray,Max,%d,%.8f\n", n, (t1 - t0) / REPS);
        t0 = nowSeconds(); for (int r = 0; r < REPS; r++) sink = saMax(&sa); t1 = nowSeconds();
        printf("SortedArray,Max,%d,%.8f\n", n, (t1 - t0) / REPS);
        t0 = nowSeconds(); for (int r = 0; r < REPS; r++) sink = suMax(&su); t1 = nowSeconds();
        printf("SinglyUnsortedList,Max,%d,%.8f\n", n, (t1 - t0) / REPS);
        t0 = nowSeconds(); for (int r = 0; r < REPS; r++) sink = ssMax(&ss); t1 = nowSeconds();
        printf("SinglySortedList,Max,%d,%.8f\n", n, (t1 - t0) / REPS);
        t0 = nowSeconds(); for (int r = 0; r < REPS; r++) sink = duMax(&du); t1 = nowSeconds();
        printf("DoublyUnsortedList,Max,%d,%.8f\n", n, (t1 - t0) / REPS);
        t0 = nowSeconds(); for (int r = 0; r < REPS; r++) sink = dsMax(&ds); t1 = nowSeconds();
        printf("DoublySortedList,Max,%d,%.8f\n", n, (t1 - t0) / REPS);

        /* ---- MIN ---- */
        t0 = nowSeconds(); for (int r = 0; r < REPS; r++) sink = auMin(&au); t1 = nowSeconds();
        printf("UnsortedArray,Min,%d,%.8f\n", n, (t1 - t0) / REPS);
        t0 = nowSeconds(); for (int r = 0; r < REPS; r++) sink = saMin(&sa); t1 = nowSeconds();
        printf("SortedArray,Min,%d,%.8f\n", n, (t1 - t0) / REPS);
        t0 = nowSeconds(); for (int r = 0; r < REPS; r++) sink = suMin(&su); t1 = nowSeconds();
        printf("SinglyUnsortedList,Min,%d,%.8f\n", n, (t1 - t0) / REPS);
        t0 = nowSeconds(); for (int r = 0; r < REPS; r++) sink = ssMin(&ss); t1 = nowSeconds();
        printf("SinglySortedList,Min,%d,%.8f\n", n, (t1 - t0) / REPS);
        t0 = nowSeconds(); for (int r = 0; r < REPS; r++) sink = duMin(&du); t1 = nowSeconds();
        printf("DoublyUnsortedList,Min,%d,%.8f\n", n, (t1 - t0) / REPS);
        t0 = nowSeconds(); for (int r = 0; r < REPS; r++) sink = dsMin(&ds); t1 = nowSeconds();
        printf("DoublySortedList,Min,%d,%.8f\n", n, (t1 - t0) / REPS);

        /* ---- PREDECESSOR / SUCCESSOR of the middle element ---- */
        SNode *ssMidNode = ssSearch(&ss, midKey);
        DNode *dsMidNode = dsSearch(&ds, midKey);

        t0 = nowSeconds(); for (int r = 0; r < REPS; r++) sink = auPredecessor(&au, midKey); t1 = nowSeconds();
        printf("UnsortedArray,Predecessor,%d,%.8f\n", n, (t1 - t0) / REPS);
        t0 = nowSeconds(); for (int r = 0; r < REPS; r++) sink = saPredecessor(&sa, midKey); t1 = nowSeconds();
        printf("SortedArray,Predecessor,%d,%.8f\n", n, (t1 - t0) / REPS);
        t0 = nowSeconds(); for (int r = 0; r < REPS; r++) sink = suPredecessor(&su, midKey); t1 = nowSeconds();
        printf("SinglyUnsortedList,Predecessor,%d,%.8f\n", n, (t1 - t0) / REPS);
        t0 = nowSeconds(); for (int r = 0; r < REPS; r++) sink = ssPredecessor(&ss, ssMidNode); t1 = nowSeconds();
        printf("SinglySortedList,Predecessor,%d,%.8f\n", n, (t1 - t0) / REPS);
        t0 = nowSeconds(); for (int r = 0; r < REPS; r++) sink = duPredecessor(&du, midKey); t1 = nowSeconds();
        printf("DoublyUnsortedList,Predecessor,%d,%.8f\n", n, (t1 - t0) / REPS);
        t0 = nowSeconds(); for (int r = 0; r < REPS; r++) sink = dsPredecessor(&ds, dsMidNode); t1 = nowSeconds();
        printf("DoublySortedList,Predecessor,%d,%.8f\n", n, (t1 - t0) / REPS);

        t0 = nowSeconds(); for (int r = 0; r < REPS; r++) sink = auSuccessor(&au, midKey); t1 = nowSeconds();
        printf("UnsortedArray,Successor,%d,%.8f\n", n, (t1 - t0) / REPS);
        t0 = nowSeconds(); for (int r = 0; r < REPS; r++) sink = saSuccessor(&sa, midKey); t1 = nowSeconds();
        printf("SortedArray,Successor,%d,%.8f\n", n, (t1 - t0) / REPS);
        t0 = nowSeconds(); for (int r = 0; r < REPS; r++) sink = suSuccessor(&su, midKey); t1 = nowSeconds();
        printf("SinglyUnsortedList,Successor,%d,%.8f\n", n, (t1 - t0) / REPS);
        t0 = nowSeconds(); for (int r = 0; r < REPS; r++) sink = ssSuccessor(&ss, ssMidNode); t1 = nowSeconds();
        printf("SinglySortedList,Successor,%d,%.8f\n", n, (t1 - t0) / REPS);
        t0 = nowSeconds(); for (int r = 0; r < REPS; r++) sink = duSuccessor(&du, midKey); t1 = nowSeconds();
        printf("DoublyUnsortedList,Successor,%d,%.8f\n", n, (t1 - t0) / REPS);
        t0 = nowSeconds(); for (int r = 0; r < REPS; r++) sink = dsSuccessor(&ds, dsMidNode); t1 = nowSeconds();
        printf("DoublySortedList,Successor,%d,%.8f\n", n, (t1 - t0) / REPS);

        /* ---- INSERT (append a new max-valued element; worst case for sorted structures) ---- */
        int INSERT_REPS = 50;
        t0 = nowSeconds(); for (int r = 0; r < INSERT_REPS; r++) auInsert(&au, n + r); t1 = nowSeconds();
        printf("UnsortedArray,Insert,%d,%.8f\n", n, (t1 - t0) / INSERT_REPS);

        t0 = nowSeconds(); for (int r = 0; r < INSERT_REPS; r++) saInsert(&sa, n + r); t1 = nowSeconds();
        printf("SortedArray,Insert,%d,%.8f\n", n, (t1 - t0) / INSERT_REPS);

        t0 = nowSeconds(); for (int r = 0; r < INSERT_REPS; r++) suInsert(&su, n + r); t1 = nowSeconds();
        printf("SinglyUnsortedList,Insert,%d,%.8f\n", n, (t1 - t0) / INSERT_REPS);

        t0 = nowSeconds(); for (int r = 0; r < INSERT_REPS; r++) ssInsert(&ss, n + r); t1 = nowSeconds();
        printf("SinglySortedList,Insert,%d,%.8f\n", n, (t1 - t0) / INSERT_REPS);

        t0 = nowSeconds(); for (int r = 0; r < INSERT_REPS; r++) duInsert(&du, n + r); t1 = nowSeconds();
        printf("DoublyUnsortedList,Insert,%d,%.8f\n", n, (t1 - t0) / INSERT_REPS);

        t0 = nowSeconds(); for (int r = 0; r < INSERT_REPS; r++) dsInsert(&ds, n + r); t1 = nowSeconds();
        printf("DoublySortedList,Insert,%d,%.8f\n", n, (t1 - t0) / INSERT_REPS);

        /* ---- DELETE (delete the elements we just inserted, given a pointer/index to each) ---- */
        t0 = nowSeconds();
        for (int r = 0; r < INSERT_REPS; r++) auDeleteAt(&au, au.size - 1);
        t1 = nowSeconds();
        printf("UnsortedArray,Delete,%d,%.8f\n", n, (t1 - t0) / INSERT_REPS);

        t0 = nowSeconds();
        for (int r = 0; r < INSERT_REPS; r++) saDeleteAt(&sa, sa.size - 1);
        t1 = nowSeconds();
        printf("SortedArray,Delete,%d,%.8f\n", n, (t1 - t0) / INSERT_REPS);

        t0 = nowSeconds();
        for (int r = 0; r < INSERT_REPS; r++) suDelete(&su, su.head);   /* delete head node each time */
        t1 = nowSeconds();
        printf("SinglyUnsortedList,Delete,%d,%.8f\n", n, (t1 - t0) / INSERT_REPS);

        t0 = nowSeconds();
        for (int r = 0; r < INSERT_REPS; r++) {
            SNode *last = ss.head; while (last->next) last = last->next;
            ssDelete(&ss, last);                 /* delete last (highest) node: forces O(n) predecessor walk */
        }
        t1 = nowSeconds();
        printf("SinglySortedList,Delete,%d,%.8f\n", n, (t1 - t0) / INSERT_REPS);

        t0 = nowSeconds();
        for (int r = 0; r < INSERT_REPS; r++) duDelete(&du, du.head);
        t1 = nowSeconds();
        printf("DoublyUnsortedList,Delete,%d,%.8f\n", n, (t1 - t0) / INSERT_REPS);

        t0 = nowSeconds();
        for (int r = 0; r < INSERT_REPS; r++) dsDelete(&ds, ds.tail);
        t1 = nowSeconds();
        printf("DoublySortedList,Delete,%d,%.8f\n", n, (t1 - t0) / INSERT_REPS);

        /* ---------- free everything ---------- */
        free(au.data);
        free(sa.data);
        { SNode *p = su.head; while (p) { SNode *nx = p->next; free(p); p = nx; } }
        { SNode *p = ss.head; while (p) { SNode *nx = p->next; free(p); p = nx; } }
        { DNode *p = du.head; while (p) { DNode *nx = p->next; free(p); p = nx; } }
        { DNode *p = ds.head; while (p) { DNode *nx = p->next; free(p); p = nx; } }
    }

    return 0;
}
