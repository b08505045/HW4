#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

typedef struct Boom{
    int day;
    struct Boom *next;
}Boom;

typedef struct Command{
    // cmd : 1 = merge, 2 = query, 3 = boom, i, j : merge, k : boom
    int cmd, i, j, k;
    bool processed; // processed = 1 if have been processed
    int num_of_boom; // the number of times this day been boomed
    int *data, *group; // data record the day which call boom with this day
    Boom *head, *tail, *tmp; // record the days which will boom to this day
}Command;

typedef struct Shop{
    int parent, rank;
}Shop;

int find_set(Shop *S, int i){
    // find the root
    int root = i;
    // keep tracking until parent = 0, i.e found the root
    while(S[i].parent)
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

void merge(Shop *S, int i, int j, int *num_of_shops){
    int root_i = find_set(S, i), root_j = find_set(S, j);
    // union by rank
    if(root_i != root_j){
        if(S[root_i].rank >= S[root_j].rank){
            S[root_j].parent = root_i;
            if(S[root_i].rank == S[root_j].rank)
                S[root_i].rank++;
        }
        else
            S[root_i].parent = root_j;
        
        (*num_of_shops)--;
    }
}

void insert(Boom *head, Boom *tail, int day){
    if(!head){
        Boom *B = calloc(1, sizeof(Boom));
        B -> day = day;
        head = B;
        tail = B;
    }
    else{
        Boom *B = calloc(1, sizeof(Boom));
        B -> day = day;
        tail -> next = B;
        tail = B;
    }
}

int main() {
    int N, M, num_of_query = 0; // N = number of nodes, M = number of days
    scanf("%d %d", &N, &M);
    printf("N : %d, M : %d\n", N, M);
    Command *Cmd = calloc(M, sizeof(Command));
    char *c = calloc(6, sizeof(char));
    // cmd : 1 = merge, 2 = query, 3 = boom
    // read all commands first
    for(int i = 0; i < M; i++){
        // printf("i = %d\n", i);
        scanf("%s", c);
        if(!strcmp(c, "merge")){
            // merge = 1
            Cmd[i].cmd = 1;
            scanf("%d %d", &Cmd[i].i, &Cmd[i].j);
        }
        else if(!strcmp(c, "query")){
            // query = 2
            Cmd[i].cmd = 2;
            num_of_query++;
        }
        else{
            // boom = 3
            Cmd[i].cmd = 3;
            scanf("%d", &Cmd[i].k);
            // Cmd[i].k is the day will be boomed to
            Cmd[Cmd[i].k].num_of_boom++;
            insert(Cmd[Cmd[i].k].head, Cmd[Cmd[i].k].tail, i);
        }
    }

    int *ans = malloc(num_of_query * sizeof(int)); // answer
    int query = num_of_query, num_of_shops = N; // num_of_shops = number of current shops
    int i = 0, idx_ans = 0;
    Shop *S = calloc(N, sizeof(Shop)); // Shop using disjoint set

    printf("Let's Start !\n");
    while(i < M && query >= 0){
        if(!Cmd[i].processed){
            if(Cmd[i].cmd == 1){
                // merge
                printf("merge !\n");
                merge(S, Cmd[i].i, Cmd[i].j, &num_of_shops);
            }
            else if(Cmd[i].cmd == 2){
                printf("query !\n");
                // query
                ans[idx_ans++] = num_of_shops;
                query--;
            }
            else{
                // boom
                printf("boom !\n");
            }
            // printf("num of shops : %d\n", num_of_shops);
            //---------------------------------------------------------------------------------------
            if(Cmd[i].num_of_boom){
                Cmd[i].group = calloc(N, sizeof(int));
                for(int j = 0; j < N; j++){
                    Cmd[i].group[j] = group[j];
                }
                Cmd[i].tmp = Cmd[i].head;
                i = Cmd[i].tmp -> day;
                Cmd[i].tmp = Cmd[i].head -> next;
            }
            //---------------------------------------------------------------------------------------
            Cmd[i].processed = 1;
        }
        i++;
    }
    for(int i = 0; i < num_of_query; i++){
        printf("%d\n", ans[i]);
    }
}
