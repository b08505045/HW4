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

Boom * new(int day){
    Boom *B = calloc(1, sizeof(Boom));
    B -> day = day;
    B -> next = NULL;
    return B;
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
            // printf("i : %d merge\n", i);
            Cmd[i].cmd = 1;
            scanf("%d %d", &Cmd[i].i, &Cmd[i].j);
        }
        else if(!strcmp(c, "query")){
            // query = 2
            // printf("i : %d query\n", i);
            Cmd[i].cmd = 2;
            query[i] = num_of_query++; // start from 0, it's (num_of_query)-th times query at day i  
        }
        else if(!strcmp(c, "boom")){
            // boom = 3
            Cmd[i].cmd = 3;
            scanf("%d", &Cmd[i].k);
            printf("\n%d boom to %d\n\n", i, Cmd[i].k);
            // Cmd[i].k is the day will be boomed to
            (Cmd[Cmd[i].k].num_of_boom)++;
            // insert
            if(!Cmd[Cmd[i].k].head){
                // printf("Cmd[0]'s num of boom : %d\n", Cmd[0].num_of_boom);
                Cmd[Cmd[i].k].head = new(i);
                printf("Cmd[0]'s num of boom : %d\n", Cmd[0].num_of_boom);
                Cmd[Cmd[i].k].tail = Cmd[Cmd[i].k].head;
            }
            else{
                Cmd[Cmd[i].k].tail -> next = new(i);
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
