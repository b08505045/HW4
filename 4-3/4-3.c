#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

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
    bool rank_increase;
    struct Change *next;
}Change;

Change * Change_new(int index, int num){
    Change *C = malloc(sizeof(Change));
    if(!C)
        return C;
    C -> index = index;
    C -> num = num;
    C -> next = NULL;
    C -> rank_increase = false;
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
    if(!self) return false;
    else return !(self -> top) ? true : false;
}

// Change * stack_peek(Stack *self){
//     return self -> top;
// }

bool stack_push(Stack *self, int index, int num){
    Change *n = Change_new(index, num);
    if(!n) 
        return false;
    n -> next = self -> top; // n -> next = NULL
    self -> top = n; // n = top
    return true;
}

void stack_pop(Stack *self){
    Change *temp = self -> top;
    self -> top = temp -> next;
    free(temp);
}

int find_set(Shop *S, int i){
    // find the root
    int root = i;
    // keep tracking until parent = 0, i.e found the root
    while(S[root].parent)
        root = S[root].parent;
    // path compression
    // int j = i;
    // while(i != root){
    //     i = S[i].parent;
    //     S[j].parent = root;
    //     j = i;
    // }
    return root;
}

int merge(Shop *S, int i, int j, int *num_of_shops){
    int root_i = find_set(S, i), root_j = find_set(S, j);
    // printf("root_i : %d, root_j : %d\n", root_i, root_j);
    // union by rank
    // i's root != j's root, return changed index
    if(root_i != root_j){
        int index;
        if(S[root_i].rank >= S[root_j].rank){
            index = root_j; // ori must be 1
            // update parent
            S[root_j].parent = root_i;
            if(S[root_i].rank == S[root_j].rank){
                S[root_i].rank++;
                index *= (-1);
            }
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
    Boom *B = malloc(sizeof(Boom));
    B -> day = day;
    B -> next = NULL;
    return B;
}

Boom * delete_head(Boom *head){
    Boom *tmp = head -> next;
    free(head);
    return tmp;
}

int main() {
    int N, M; // N = number of nodes, M = number of days
    int num_of_query = 0;
    scanf("%d %d", &N, &M);
    // printf("N : %d, M : %d\n", N, M);
    Command *Cmd = calloc((M + 1), sizeof(Command));
    char *c = calloc(6, sizeof(char));
    int *query = malloc((M + 1) * sizeof(int));

    // cmd : 1 = merge, 2 = query, 3 = boom
    // read all commands first
    // printf("Cmd[0]'s num of boom = %d\n", Cmd[0].num_of_boom);
    for(int i = 1; i <= M; i++){
        // printf("i = %d\n", i);
        scanf("%s", c);
        if(!strcmp(c, "merge")){
            // merge = 1
            Cmd[i].cmd = 1;
            scanf("%d %d", &Cmd[i].i, &Cmd[i].j);
            // printf("merge !\n");
        }
        else if(!strcmp(c, "query")){
            // query = 2
            // printf("query !\n");
            Cmd[i].cmd = 2;
            num_of_query++;
            query[i] = num_of_query;
            // printf("query now : %d\n", num_of_query);
        }
        else if(!strcmp(c, "boom")){
            // boom = 3
            Cmd[i].cmd = 3;
            scanf("%d", &Cmd[i].k);
            // printf("\n%d boom to %d\n\n", i, Cmd[i].k);
            // Cmd[i].k is the day will be boomed to
            (Cmd[Cmd[i].k].num_of_boom)++;
            // insert
            if(!Cmd[Cmd[i].k].head){
                // printf("Cmd[0]'s num of boom : %d\n", Cmd[0].num_of_boom);
                Cmd[Cmd[i].k].head = new(i);
                // printf("Cmd[0]'s num of boom : %d\n", Cmd[0].num_of_boom);
                Cmd[Cmd[i].k].tail = Cmd[Cmd[i].k].head;
            }
            else{
                Cmd[Cmd[i].k].tail -> next = new(i);
                Cmd[Cmd[i].k].tail = Cmd[Cmd[i].k].tail -> next;
            }
            // printf("%d boom to %d\n", Cmd[Cmd[i].k].tail -> day, Cmd[i].k);
        }
    }

    // ------------------------------------------------------------- test cmd
    // printf("num of query : %d", num_of_query);
    // printf("\nprintf :\n\n");
    // if(Cmd[0].num_of_boom)
    //     printf("Cmd[0] num of boom : %d\n", Cmd[0].num_of_boom);
    // for(int i = 1; i <= M; i++){
    //     if(Cmd[i].cmd == 1)
    //         printf("merge %d %d", Cmd[i].i, Cmd[i].j);
    //     else if(Cmd[i].cmd == 2){
    //         printf("query");
    //     }
    //     else if(Cmd[i].cmd == 3){
    //         printf("boom %d", Cmd[i].k);
    //     }
    //     if(Cmd[i].num_of_boom)
    //         printf("\nnum of boom : %d\n", Cmd[i].num_of_boom);
    //     printf("\n");
    // }
    // ------------------------------------------------------------- test cmd
    
    // printf("\n---------------------Let's process it offline!----------------------------------------\n\n");
    Shop *S = calloc((N + 1), sizeof(Shop)); // Shop using disjoint set
    int num_of_shops = N; // num_of_shops = number of current shops
    int *ans = malloc((num_of_query + 1) * sizeof(int)); // answer

    bool boom = 0; // boom = 1 means start finding each boom

    // reverse_element record all history of changed, reverse_times record each times need to reverse in each step
    Stack *reverse_element = stack_new(), *reverse_times = stack_new();
    // printf("Let's Start !\n");
    int reverse = 0; // reverse = 1 means it's time to reverse !
    int jump = 0; // jump = 1 means just jump to boom!
    int i = 0;
    int cur_query = num_of_query;

    // for(int a = 1; a <= N; a++){
    //     printf("%d ", S[a].parent);
    // }
    // printf("\n");

    while(cur_query > 0){
        // printf("%d : ", i);
        // i-th day haven't been processed
        if(i <= M && !Cmd[i].processed){
            Cmd[i].processed = 1;
            // -------------------------------------------------------------processe command
            // merge
            if(Cmd[i].cmd == 1){
                // printf("merge %d %d!\n", Cmd[i].i, Cmd[i].j);
                if(!boom){
                    // printf("no boom\n");
                    // printf("no boom\n");
                    merge(S, Cmd[i].i, Cmd[i].j, &num_of_shops);
                }
                // record change
                else{
                    // printf("yes boom\n");
                    int index = merge(S, Cmd[i].i, Cmd[i].j, &num_of_shops);
                    if(index){
                        // printf("index combined : %d\n", index);
                        if(index < 0){
                            index *= (-1);
                            stack_push(reverse_element, index, 0);
                            reverse_element -> top -> rank_increase = true;
                        }
                        else{
                            stack_push(reverse_element, index, 0);
                        }
                        // printf("Shop :\n");
                        // for(int a = 1; a <= N; a++){
                        //     printf("%d ", S[a].parent);
                        // }
                        // printf("\n");
                        (reverse_times -> top -> num)++;
                        // if(!stack_is_empty(reverse_times))
                        //     printf("top : %d\n", reverse_times -> top -> num);
                    }
                }
            }
            // query
            else if(Cmd[i].cmd == 2){
                // printf("query !\n");
                ans[query[i]] = num_of_shops;
                cur_query--;
            }
            // boom
            else if(Cmd[i].cmd == 3){         
                // if encounter boom(not jump), break
                if(!jump){
                    // printf("reverse : encounter boom\n");
                    Cmd[i].processed = 0;
                    reverse = 1;
                    int j = reverse_times -> top -> num;
                    Change *tmp;
                    // reverse
                    for(j; j > 0; j--){
                        // tmp = each reverse element
                        tmp = reverse_element -> top;
                        // printf("%d popped since encounter boom\n", tmp -> index);
                        // this has problem !
                        if(S[S[tmp -> index].parent].rank == S[tmp -> index].rank)
                            S[S[tmp -> index].parent].rank--;
                        //
                        S[tmp -> index].parent = 0;
                        num_of_shops++;
                        stack_pop(reverse_element);
                    }
                    // tmp now is the day which jump from
                    tmp = reverse_times -> top;
                    Cmd[tmp -> index].num_of_boom--;
                    // printf("%d's num of boom : %d\n", tmp -> index, Cmd[tmp -> index].num_of_boom);
                    if(Cmd[tmp -> index].num_of_boom){
                        Cmd[tmp -> index].head = delete_head(Cmd[tmp -> index].head);
                        // printf("delete_head\n");
                        i =  Cmd[tmp -> index].head -> day;
                        jump = 1; // jump to next target
                        // printf("i is now %d\n", i);
                        tmp -> num = 0;
                    }
                    else{
                        // all jumps processed
                        i = tmp -> index + 1;
                        if(!stack_is_empty(reverse_times))
                            stack_pop(reverse_times);
                        if(stack_is_empty(reverse_times)){
                            jump = 0;
                            boom = 0;
                            // printf("reverse_time is empty now\n");
                        }
                        // else
                        //     printf("reverse_time's top is %d now\n", reverse_times -> top -> index);
                        tmp = NULL;
                    }
                }
                else{
                    jump = 0;
                    // printf("just pass\n");
                }
            }
            // else printf("beginning ! \n");
            // -------------------------------------------------------------
            // printf("check reverse\n");
            //--------------------------------------------------------------check if jump
            if(!reverse){
                // printf("no reverse\n");
                // printf("Cmd[%d]'s num of boom : %d\n", i, Cmd[i].num_of_boom);
                if(Cmd[i].num_of_boom){
                    // current day will be boomed int the future
                    // printf("Let's boom !\n");
                    jump = 1;
                    boom = 1;
                    stack_push(reverse_times, i, 0);
                    // printf("reverse_time's top is %d now\n", reverse_times -> top -> index);
                    // printf("jump to %d\n", Cmd[i].head -> day);
                    i = Cmd[i].head -> day;
                    // printf("jump\n");
                }
                else{
                    // printf("%d++\n", i);
                    if(!jump)
                        i++;
                }
            }
            else{
                // printf("%d++", i);
                // i++; // keep going
                reverse = 0;
            }
            //-------------------------------------------------------------------------------
        }
        // encounter processed node or the end of array, reverse !
        else{
            // printf("reverse : encounter processed or ecounter End\n");
            int j = reverse_times -> top -> num;
            // printf("return %d times\n", j);
            Change *tmp;
            // reverse
            for(j; j > 0; j--){
                // tmp = each reverse element
                tmp = reverse_element -> top;
                // this problem!
                if(S[S[tmp -> index].parent].rank == S[tmp -> index].rank)
                    S[S[tmp -> index].parent].rank--;
                S[tmp -> index].parent = 0;
                num_of_shops++;
                // printf("%d popped since encounter processed or end\n", tmp -> index);
                stack_pop(reverse_element);
            }
            // tmp now is the day which jump from
            tmp = reverse_times -> top;
            Cmd[tmp -> index].num_of_boom--;
            // printf("%d\n", Cmd[tmp -> index].num_of_boom);
            if(Cmd[tmp -> index].num_of_boom){
                // printf("delete_head\n");
                Cmd[tmp -> index].head = delete_head(Cmd[tmp -> index].head);
                i =  Cmd[tmp -> index].head -> day;
                tmp -> num = 0;
            }
            else{
                // printf("all %d's booms done\n", tmp -> index);
                i = tmp -> index + 1;
                if(!stack_is_empty(reverse_times))
                    stack_pop(reverse_times);
                if(stack_is_empty(reverse_times)){
                    // printf("reverse_time is empty now\n");
                    jump = 0;
                    boom = 0;
                }
                // else
                //     printf("reverse_time's top is %d now\n", reverse_times -> top -> index);
            }
        }
    }
    for(int i = 1; i <= num_of_query; i++){
        printf("%d\n", ans[i]);
    }
    return 0;
}
