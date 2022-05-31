#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <assert.h>

// Boom record the days which will boom to cuurent day
typedef struct Boom{
    int day;
    struct Boom *next;
}Boom;

typedef struct Command{
    // cmd : 1 = merge, 2 = query, 3 = boom, i, j : merge, k : boom
    int cmd, i, j, k;
    bool processed; // processed = 1 if have been processed
    int num_of_boom; // the number of times this day been boomed
    Boom *head, *tail; // record the days which will boom to this day
}Command;

// Disjoint set of Shop
typedef struct Shop{
    int parent, rank;
}Shop;

typedef struct Change{
    int index, num;
    struct Change *next;
}Change;

Change * Change_new(int index, int num){
    Change *C = malloc(sizeof(Change));
    if (!C)
        return C;
    C -> index = index;
    C -> num = num;
    C -> next = NULL;
    return C;
}

typedef struct Stack{
    Change *top;
}Stack;

Stack * stack_new(){
    Stack *s = malloc(sizeof(Stack));
    if (!s)
        return s;
    s -> top = NULL;
    return s;
}

bool stack_is_empty(Stack *self){
    assert(self);
    return !(self -> top) ? true : false;
}

Change * stack_peek(Stack *self){
    assert(!stack_is_empty(self));
    return self -> top;
}

bool stack_push(Stack *self, int index, int num){
    Change *n = Change_new(index, num);
    if (!n) 
        return false;
    n -> next = self -> top; // n -> next = NULL
    self -> top = n; // n = top
    return true;
}

void stack_pop(Stack *self){
    assert(!stack_is_empty(self));
    Change *temp = self -> top;
    // int popped = temp -> data;
    self -> top = temp -> next;
    free(temp);
    // return popped;
}

int find_set(Shop *S, int i){
    // find the root
    int root = i;
    // keep tracking until parent = 0, i.e found the root
    while(S[root].parent)
        root = S[root].parent;
    
    // path compression
    int j = i;
    while(i != root){
        i = S[i].parent;
        S[j].parent = root;
        j = i;
    }
    return root;
}

int merge(Shop *S, int i, int j, int *num_of_shops){
    int root_i = find_set(S, i), root_j = find_set(S, j);
    printf("root_i : %d, root_j : %d\n", root_i, root_j);
    // union by rank
    // i's root != j's root, return changed index
    if(root_i != root_j){
        int index;
        if(S[root_i].rank >= S[root_j].rank){
            index = root_j; // ori must be 1
            // update parent
            S[root_j].parent = root_i;
            if(S[root_i].rank == S[root_j].rank)
                S[root_i].rank++;
        }
        else{
            index = root_i;
            S[root_i].parent = root_j;
        }
        (*num_of_shops)--;
        return index;
    }
    // i's root = j's root, merge do nothing, return 0
    else return 0;
}

Boom * new(int day){
    Boom *B = calloc(1, sizeof(Boom));
    B -> day = day;
    B -> next = NULL;
    return B;
}

void delete_head(Boom *head){
    Boom *del = head;
    head = head -> next;
    free(del);
}

int main() {
    int N, M, num_of_query = 0; // N = number of nodes, M = number of days
    int *query = malloc((M + 1) * sizeof(int)); // record the sequence of query, ex. if now query is called twice and is at day 8, then query[8] = 2; 
    scanf("%d %d", &N, &M);
    printf("N : %d, M : %d\n", N, M);
    Command *Cmd = calloc((M + 1), sizeof(Command));
    char *c = calloc(6, sizeof(char));

    // cmd : 1 = merge, 2 = query, 3 = boom
    // read all commands first
    printf("Cmd[0]'s num of boom = %d\n", Cmd[0].num_of_boom);
    for(int i = 1; i <= M; i++){
        // printf("i = %d\n", i);
        scanf("%s", c);
        if(!strcmp(c, "merge")){
            // merge = 1
            printf("i : %d merge\n", i);
            Cmd[i].cmd = 1;
            scanf("%d %d", &Cmd[i].i, &Cmd[i].j);
        }
        else if(!strcmp(c, "query")){
            // query = 2
            printf("i : %d query\n", i);
            Cmd[i].cmd = 2;
            query[i] = num_of_query++; // start from 0, it's (num_of_query)-th times query at day i  
        }
        else if(!strcmp(c, "boom")){
            printf("i : %d boom\n", i);
            // boom = 3
            Cmd[i].cmd = 3;
            scanf("%d", &Cmd[i].k);
            // Cmd[i].k is the day will be boomed to
            (Cmd[Cmd[i].k].num_of_boom)++;
            // insert
            if(!Cmd[Cmd[i].k].head){
                Cmd[Cmd[i].k].head = new(i);
                Cmd[Cmd[i].k].tail = Cmd[Cmd[i].k].head;
            }
            else{
                Cmd[Cmd[i].k].tail -> next = Cmd[Cmd[i].k].head;
                Cmd[Cmd[i].k].tail = Cmd[Cmd[i].k].tail -> next;
            }
            // printf("%d boom to %d\n", Cmd[Cmd[i].k].tail -> day, Cmd[i].k);
        }
    }
    
    // free(c);
    // printf("Cmd[0]'s num of boom = %d\n", Cmd[0].num_of_boom);
    printf("num of boom :\n");
    for(int i = 0; i <= M; i++){
        printf("%d ", Cmd[i].num_of_boom);
    }
    printf("\n");
    return 0;
}
