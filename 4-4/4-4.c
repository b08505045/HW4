#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

typedef struct Command{
    int cmd, p, l, r, x, y;
    long long k;
}Command;

typedef struct treap{
    int size, val, pri; // key{size, val}, prioty
    long long sum;
    bool rev;
    struct treap *left, *right, *next; // ptr next is used in Stack
}treap;

typedef struct Stack{
    treap *top;
}Stack;

bool stack_is_empty(Stack *self){
    return !(self->top) ? true : false;
}

bool stack_push(Stack *self, treap *t){
    t -> next = self -> top;
    self -> top = t;
    return true;
}

treap * stack_peek(Stack *self){
    return self -> top;
}

void stack_pop(Stack *self){
    self -> top = self -> top -> next;
}

treap * new(int val){
    treap *t = malloc(sizeof(treap));
    t -> left = t -> right = NULL;
    t -> val = t -> sum = val;
    t -> size = 1;
    t -> pri = rand();
    t -> rev = false;
    return t;
}

void push(treap *t){
    if(t){
        treap *tmp = t -> left;
        t -> left = t -> right;
        t -> right = tmp;
        if(t -> left)
            t -> left -> rev ^= t -> rev;
        if(t -> right)
            t -> right -> rev ^= t -> rev;
        t -> rev = false;
    }
}

void pull(treap *t){
    t -> size = 1;
    t -> sum = t -> val;
    if(t -> left){
        t -> size += t -> left -> size;
        t -> sum += t -> left -> sum;
    }
    if(t -> right){
        t -> size += t -> right -> size;
        t -> sum += t -> right -> sum;
    }
    printf("%d's sum : %lld, size : %d\n", t -> val, t -> sum, t -> size);
}

treap * merge(treap *a, treap *b){
    // printf("merge \n");
    treap *t = NULL, *tmp_t = NULL, *tmp_a = a, *tmp_b = b, *tmp;
    Stack *stk = calloc(1, sizeof(Stack));
    int condition = 0;
    bool find = 0, left = 0; // find = 1 when root decided, left = 1 when finding left child
    while(1){
        if(tmp_a){
            if(tmp_b){
                if(tmp_a -> pri > tmp_b -> pri)
                    condition = 1;
                else
                    condition = 2;
            }
            // b is NULL
            else{
                printf("b is NULL\n");
                condition = 1;
            }
        }
        // a is NULL
        else if(tmp_b){
            printf("a is NULL\n");
            condition = 2;
        }
        // a and b are NULL
        else{
            printf("both null\n");
            condition = 3;
        }
            
        // found node belongs to a, next find right child
        if(condition == 1){
            printf("tmp_a's pri > tmp_b'spri\n");
            if(tmp_a -> rev){
                printf("push a\n");
                push(tmp_a);
            } 
            stack_push(stk, tmp_a);
            if(!find){
                find = 1;
                t = tmp_a;
                printf("t : %d\n", tmp_a -> val);
                tmp_t = t;
            }
            else{
                if(left){
                    printf("left\n");
                    tmp_t -> left = tmp_a;
                    tmp_t = tmp_t -> left;
                }
                else{
                    printf("right\n");
                    tmp_t -> right = tmp_a;
                    tmp_t = tmp_t -> right;
                }
            }
            tmp_a = tmp_a -> right;
            left = 0;
        }
        // found node belongs to b, next find left child
        else if(condition == 2){
            printf("tmp_b's pri > tmp_a'spri\n");
            if(tmp_b -> rev){
                printf("push b\n");
                push(tmp_b);
            }
            stack_push(stk, tmp_b);
            if(!find){
                find = 1;
                t = tmp_b;
                printf("t : %d\n", tmp_b -> val);
                tmp_t = t;
            }
            else{
                if(left){
                    printf("left\n");
                    tmp_t -> left = tmp_b;
                    tmp_t = tmp_t -> left;
                }
                else{
                    printf("right\n");
                    // printf("tmp_t : %d", tmp_t -> val);
                    tmp_t -> right = tmp_b;
                    tmp_t = tmp_t -> right;
                    // if(tmp_t -> left) printf(", tmp_t's left : %d\n", tmp_t -> left -> val);
                    // if(tmp_t -> right) printf(", tmp_t's right : %d\n", tmp_t -> right -> val);
                }
            }
            tmp_b = tmp_b -> left;
            left = 1;
        }
        else if(condition = 3){
            if(left)
                tmp_t -> left = NULL;
            else
                tmp_t -> right = NULL;
            break;
        }
    }
    printf("t : %d", t -> val);
    if(t -> left){
        printf(", t's left : %d, ", t -> left -> val);
        if(t -> left -> left){
            printf(", t's left left : %d, ", t -> left -> left -> val);
            if(t -> left -> left -> right){
                printf(", t's left left right: %d, ", t -> left -> left -> right -> val);
            }
        }
        if(t -> left -> right){
            printf(", t's left right : %d, ", t -> left -> right -> val);
        }
    }
    if(t -> right) printf(", t's right : %d", t -> right -> val);
    printf("\n");
    printf("pull\n");
    while(!stack_is_empty(stk)){
        pull(stack_peek(stk));
        stack_pop(stk);
    }
    free(stk);
    printf("done\n");
    return t;
}

// split to two array, with a whose key <= p and b whose key > p
void split(treap *t, int p, treap **a, treap **b){
    Stack *stk = calloc(1, sizeof(Stack));
    treap *tmp_t = t, *tmp_a = NULL, *tmp_b = NULL;
    bool find_a = false, find_b = false; // turn = 0 when end with a -> right, 1 when end with b -> left
    // split until encounter leaf node
    while(tmp_t){
        printf("current node : %d, p : %d\n", tmp_t -> val, p);
        if(tmp_t -> rev)
            push(tmp_t);
        if(tmp_t -> left){
            if(tmp_t -> left -> size >= p){
                // turn = 1;
                if(!find_b){
                    find_b = 1;
                    *b = tmp_t;
                    tmp_b = *b;
                    printf("tmp_b = b = %d\n", (*b) -> val);
                    stack_push(stk, tmp_b);
                }
                else{
                    tmp_b -> left = tmp_t;
                    tmp_b = tmp_b -> left;
                    printf("b's left : %d\n", (*b) -> left -> val);
                    stack_push(stk, tmp_b);
                }
                tmp_t = tmp_t -> left; 
            }
            else{
                // turn = 0;
                p -= (tmp_t -> left -> size + 1);
                if(!find_a){
                    find_a = 1;
                    *a = tmp_t;
                    tmp_a = *a;
                    printf("tmp_a = a = %d\n", (*a) -> val);
                    stack_push(stk, tmp_a);
                }
                else{
                    tmp_a -> right = tmp_t;
                    tmp_a = tmp_a -> right;
                    printf("a's right : %d", (*a) -> right -> val);
                    stack_push(stk, tmp_a);
                }
                tmp_t = tmp_t -> right;
            }
        }
        // current node has no left child
        else if(p > 0){
            printf("current node has no left child, p > 0\n");
            // turn = 0;
            p--;
            if(!find_a){
                find_a = 1;
                *a = tmp_t;
                tmp_a = *a;
                stack_push(stk, tmp_a);
            }
            else{
                tmp_a -> right = tmp_t;
                tmp_a = tmp_a -> right;
                stack_push(stk, tmp_a);
            }
            tmp_t = tmp_t -> right;
        }
        else{
            printf("current node has no left child, p = 0\n");
            // turn = 1;
            if(!find_b){
                find_b = 1;
                *b = tmp_t;
                tmp_b = *b;
                stack_push(stk, tmp_b);
            }
            else{
                tmp_b -> left = tmp_t;
                tmp_b = tmp_b -> left;
                stack_push(stk, tmp_b);
            }
            tmp_t = tmp_t -> left;
        }
    }
    if(tmp_a)
        tmp_a -> right = NULL;
    if(tmp_b)
        tmp_b -> left = NULL;
    
    printf("pull\n");
    while(!stack_is_empty(stk)){
        pull(stack_peek(stk));
        stack_pop(stk);
    }
    free(stk);
}

// insert val k at location p
void insert(treap *t, int p, int k){
    treap *lt = NULL, *rt = NULL;
    split(t, p, &lt, &rt);
    merge(merge(lt, new(k)), rt);
}

void delete(treap *t, int p){
    treap *a = NULL, *b = NULL, *c = NULL, *d = NULL;
    split(t, p - 1, &a, &b);
    split(t, 1, &c, &d);
    t = merge(a, d);
}

long long getsum(treap *t, int l, int r){
    // treap *a, *b, *c, *d;
    treap *a = NULL, *b = NULL, *c = NULL, *d = NULL;
    split(t, l - 1, &a, &b);
    // printf("t's val : %d\n", t -> val);
    // printf("t's sum : %lld\n", t -> sum);
    // printf("t's left : %d, sum : %d\n", t -> left -> val, t -> left -> sum);
    // printf("a's val : %d\n", a -> val);
    // printf("a's sum : %lld\n", a -> sum);
    // printf("b's val : %d\n", b -> val);
    // printf("b's sum : %lld\n", b -> sum);
    printf("\n");
    split(b, r - l + 1, &c, &d);
    printf("\n");
    long long sum = c -> sum;
    t = merge(a, merge(c, d));
    printf("getsum done, sum : %lld", sum);
    return sum;
}

void reverse (treap *t , int l , int r){
    treap *a = NULL, *b = NULL, *c = NULL, *d = NULL;
    split(t, l - 1, &a, &b);
    split(b, r - l + 1, &c, &d);
    // Drt x = Split ( rt , l - 1 );
    // Drt y = Split ( x.second , r - l + 1 );
    c -> rev = true;
    printf("c's sum : %d\n", c -> sum);
    printf("d's sum : %d\n", d -> sum);
    t = merge(a, merge(c, d));
    a = b = c = d = NULL;
    return ;
}

void swap(treap *t, int l, int r, int x, int y, int N){
    treap *a = NULL, *b = NULL, *c = NULL, *d = NULL, *e = NULL, *f = NULL, *g = NULL, *h = NULL;
    if(l == 1 && y == N){
        split(t, r, &a, &b);
        split(b, x - r, &c, &d);
        merge(d, merge(c, a));
    }
    else if(l == 1){
        split(t, r, &a, &b);
        split(b, x - r, &c, &d);
        split(d, y - x + 1, &e, &f);
        merge(e, merge(c, merge(a, f)));
    }
    else if(y == N){
        split(t, l - 1, &a, &b);
        split(b, r - l + 1, &c, &d);
        split(d, x - r + 1, &e, &f);
        merge(a, merge(f, merge(e, c)));
    }
    else{
        split(t, l - 1, &a, &b);
        split(b, r - l + 1, &c, &d);
        split(d, x - r + 1, &e, &f);
        split(f, y - x + 1, &g, &h);
        t = merge(a, merge(g, merge(e, merge(c, h))));
    }
    a = b = c = d = e = f = g = h = NULL;
}

int main() {
    int N, Q; // N = number of machines, Q = number of records
    scanf("%d %d", &N, &Q);
    printf("%d %d\n", N, Q);
    Command *Cmd = malloc((Q + 1) * sizeof(Command));
    long long *reboot = malloc((N + 1) * sizeof(long long));
    for(int i = 1; i <= N; i++){
        scanf("%lld", &reboot[i]);
    }
    // print ori machine
    for(int i = 1; i <= N; i++){
        printf("%lld ", reboot[i]);
    }
    printf("\n");

    // build treap
    treap *t = NULL, *tmp = NULL;
    for(int i = 1; i <= N; i++){
        tmp = new(reboot[i]);
        printf("merge %d\n", tmp -> val);
        t = merge(t, tmp);
        printf("t now : %d", t -> val);
        if(t -> left) printf(", left : %d, ", t -> left -> val);
        if(t -> right) printf(", right : %d", t -> right -> val);
        printf("\n");
    }
    tmp = NULL;
    // printf("t : %d, size : %d, sum : %d\n", t -> val, t -> size, t -> sum);
    // printf("t's left : %d, size : %d, sum : %d\n", t -> left -> val, t -> left -> size, t -> left -> sum);
    // printf("t's right : %d, size : %d, sum : %d\n", t -> right -> val, t -> right -> size, t -> right -> sum);
    printf("\nread input\n\n");
    for(int i = 1; i <= Q; i++){
        scanf("%d", &Cmd[i].cmd);
        // insert
        if(Cmd[i].cmd == 1)
            scanf("%d %lld", &Cmd[i].p, &Cmd[i].k);
        // delete
        else if(Cmd[i].cmd == 2)
            scanf("%d", &Cmd[i].p);
        // swap
        else if(Cmd[i].cmd == 4)
            scanf("%d %d %d %d", &Cmd[i].l, &Cmd[i].r, &Cmd[i].x, &Cmd[i].y);
        // min?
        else if(Cmd[i].cmd == 5)
            scanf("%d %d %lld", &Cmd[i].l, &Cmd[i].r, &Cmd[i].k);
        // reverse or getsum
        else if(Cmd[i].cmd == 3 || Cmd[i].cmd == 6)
            scanf("%d %d", &Cmd[i].l, &Cmd[i].r);
    }

    // printf("done\n");
    //------------------------------------------------------------------------------print command
    treap *a, *b, *c, *d, *e, *f, *g, *h;
    for(int i = 1; i <= Q; i++){
        // insert
        if(Cmd[i].cmd == 1){
            printf("INSERT : p = %d, k = %lld\n", Cmd[i].p, Cmd[i].k);
            if(!Cmd[i].p){
                printf("no split since p = 0\n");
                t = merge(new(Cmd[i].k), t);
            }
            else if(Cmd[i].p == N){
                printf("no split since p = N\n");
                t = merge(t, new(Cmd[i].k));
            }
            else
                insert(t, Cmd[i].p, Cmd[i].k);
            N++;
        }
        // delete
        else if(Cmd[i].cmd == 2){
            printf("DELETE : p = %d\n", Cmd[i].p);
            if(Cmd[i].p == 1){
                split(t, 1, &a, &b);
                t = b;
            }
            else if(Cmd[i].p == N){
                split(t, N - 1, &a, &b);
                t = a;
            }
            else
                delete(t, Cmd[i].p);
            N--;
            a = NULL;
            b = NULL;
        }
        // reverse
        else if(Cmd[i].cmd == 3){
            printf("REVERSE : l = %d, r = %d\n", Cmd[i].l, Cmd[i].r);
            if(Cmd[i].l == 1 && Cmd[i].r == N)
                t -> rev = true;
            else if(Cmd[i].l == 1){
                split(t, Cmd[i].r, &a, &b);
                a -> rev = true;
                t = merge(a, b);
            }
            else if(Cmd[i].r == N){
                split(t, Cmd[i].l - 1, &a, &b);
                b -> rev = true;
                t = merge(a, b);
            }
            else
                reverse(t , Cmd[i].l, Cmd[i].r);
            a = b = NULL;
        }
        // swap
        else if(Cmd[i].cmd == 4){
            printf("SWAP : l = %d, r = %d, x = %d, y = %d\n", Cmd[i].l, Cmd[i].r, Cmd[i].x, Cmd[i].y);
            // switch (l, r), (x, y)
            if(Cmd[i].l > Cmd[i].x){
                int temp1 = Cmd[i].l, temp2 = Cmd[i].r;
                Cmd[i].l = Cmd[i].x;
                Cmd[i].r = Cmd[i].y;
                Cmd[i].x = temp1;
                Cmd[i].y = temp2;
            }
            swap(t, Cmd[i].l, Cmd[i].r, Cmd[i].x, Cmd[i].y, N);
        }
        // min?
        else if(Cmd[i].cmd == 5){
            printf("5 : l = %d, r = %d, k = %lld\n", Cmd[i].l, Cmd[i].r, Cmd[i].k);
        }
        // getsum
        else if(Cmd[i].cmd == 6){
            printf("GETSUM : l = %d, r = %d\n", Cmd[i].l, Cmd[i].r);
            printf("sum : %lld\n", getsum(t, Cmd[i].l, Cmd[i].r));
        }
        printf("root : %d", t -> val);
        if(t -> left){
            printf(", t's left : %d, ", t -> left -> val);
            if(t -> left -> left){
                printf(", t's left left: %d, ", t -> left -> left -> val);
                if(t -> left -> left -> right){
                    printf(", t's left left right: %d, ", t -> left -> left -> right -> val);
                }
            }
            if(t -> left -> right){
                printf(", t's left right: %d, ", t -> left -> right -> val);
                if(t -> left -> right -> left){
                    printf(", t's left right left: %d, ", t -> left -> right -> left -> val);
                }
            }
        }
        if(t -> right){
            printf(", t's right : %d", t -> right -> val);
            if(t -> right -> right){
                printf(", t's right right: %d", t -> right -> right -> val);
            }
            if(t -> right -> left){
                printf(", t's right left: %d", t -> right -> left -> val);
                if(t -> right -> left -> right){
                    printf(", t's right left right : %d", t -> right -> left -> right -> val);
                }
            }
        }
        printf("\n");
    }
    printf("end\n");
    //------------------------------------------------------------------------------print command
}
