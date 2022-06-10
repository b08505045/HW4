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
    t -> left = NULL;
    t -> right = NULL;
    t -> size = 1;
    t -> val = val;
    t -> sum = val;
    t -> pri = rand();
    t -> rev = false;
    return t;
}

void push(treap *t){
    if (!t) return;
    treap *tmp = t -> left;
    t -> left = t -> right;
    t -> right = tmp;
    if (t -> left)
        t -> left -> rev ^ t -> rev;
    if (t -> right)
        t -> right -> rev ^ t -> rev;
    t -> rev = false;
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
    // leaf node
    if(!a) return b;
    if(!b) return a;
    // check priority
    if(a -> pri > b -> pri){
        if(a -> rev)
            push(a);
        a -> right = merge(a -> right, b);
        pull(a); // update size and sum
        return a;
    }
    if(b -> rev)
        push(b);
    b -> left = merge(a, b -> left);
    pull(b);
    return b;
}

// split to two array, with a whose key <= p and b whose key > p
void split(treap* t, int p, treap *a, treap *b){
    Stack *stk = calloc(1, sizeof(Stack));
    treap *tmp_t = t, *tmp_a = a, *tmp_b = b;
    bool find_a = 0, find_b = 0;
    while(tmp_t){
        printf("currnet node : %d\n", tmp_t -> val);
        if(tmp_t -> rev)
            push(tmp_t);
        if(tmp_t -> left){
            if(tmp_t -> left -> size >= p){
                tmp_b = tmp_t;
                if(!find_b){
                    b = tmp_b;
                    find_b = 1;
                }
                printf("b : %d\n", tmp_b -> val);
                if(tmp_b -> rev)
                    push(tmp_b);
                stack_push(stk, tmp_b); // used to pull after
                tmp_t = tmp_t -> left;
                tmp_b = tmp_b -> left;
            }
            else{
                tmp_a = tmp_t;
                if(!find_a){
                    a = tmp_a;
                    find_a = 1;
                }
                printf("a : %d\n", tmp_a -> val);
                push(tmp_a);
                stack_push(stk, tmp_a); // used to pull after
                p -= (tmp_t -> left -> size) - 1;
                tmp_t = tmp_t -> right;
                tmp_a = tmp_a -> right; 
                printf("move to %d\n", tmp_t -> val);
            }
        }
        // current node has no left child
        else{
            printf("current node has no left child\n");
            if(tmp_t -> right){
                if(tmp_t -> right -> size < p){
                    tmp_b = tmp_t;
                    if(!find_b){
                        b = tmp_b;
                        find_b = 1;
                    }
                    printf("b : %d\n", tmp_b -> val);
                    push(tmp_b);
                    stack_push(stk, tmp_b);
                    tmp_t = tmp_t -> left;
                    tmp_b = tmp_b -> left;
                }
                else{
                    tmp_a = tmp_t;
                    if(!find_a){
                        a = tmp_a;
                        find_a = 1;
                    }
                    printf("a : %d\n", tmp_a -> val);
                    push(tmp_a);
                    stack_push(stk, tmp_a);
                    p -=(tmp_t -> left -> size) - 1;
                    tmp_t = tmp_t -> right;
                    tmp_a = tmp_a -> right;
                }
            }
            // current node is leaf
            else{
                printf("current node is leaf\n");
                if(p > 0){
                    tmp_a = tmp_t;
                    if(!find_a){
                        a = tmp_a;
                        find_a = 1;
                    }
                    printf("a : %d\n", tmp_a -> val);
                    push(tmp_a);
                    stack_push(stk, tmp_a);
                    tmp_t = tmp_t -> right;
                    tmp_a = tmp_a -> right;
                    p--;
                }
                else{
                    tmp_b = tmp_t;
                    if(!find_b){
                        b = tmp_b;
                        find_b = 1;
                    }
                    printf("b : %d\n", tmp_b -> val);
                    push(tmp_b);
                    stack_push(stk, tmp_b);
                    tmp_t = tmp_t -> left;
                    tmp_b = tmp_b -> left;
                }
            }
        }
    }
    printf("pull\n");
    tmp_a = tmp_b = NULL;
    while(!stack_is_empty(stk)){
        pull(stack_peek(stk));
        stack_pop(stk);
    }
}

// insert val k at location p
void insert(treap *t, int p, int k){
    treap *lt, *rt;
    split(t, p, lt, rt);
    merge(merge(lt, new(k)), rt);
}

void delete(treap *t, int k){
    treap *lt, *rt;
    split(t, k - 1, lt, t);
    split(t, k, t, rt);
    t = merge(lt, rt);
}

long long getsum(treap *t, int l, int r){
    printf("l : %d, r : %d\n", l, r);
    treap *a, *b, *c, *d;
    split(t, l - 1, a, b);
    printf("t's val : %d\n", t -> val);
    printf("t's sum : %lld\n", t -> sum);
    printf("a's val : %d\n", a -> val);
    printf("a's sum : %lld\n", a -> sum);
    printf("b's val : %d\n", b -> val);
    printf("b's sum : %lld\n", b -> sum);
    // split(b, r - l + 1, c, d);
    // long long sum = c -> sum;
    // merge(a, merge(c, d));
    // return sum;
}

void reverse (treap *t , int l , int r){
    treap *a, *b, *c, *d;
    split(t, l - 1, a, b);
    split(b, r - l + 1, c, d);
    // Drt x = Split ( rt , l - 1 ) ;
    // Drt y = Split ( x.second , r - l + 1 );
    c -> rev = true;
    t = merge(a, merge(c, d));
    a = b = c = d = NULL;
    return ;
}

void swap(treap *t, int l, int r, int x, int y, int N){
    treap *a, *b, *c, *d, *e, *f, *g, *h;
    if(l == 1 && y == N){
        split(t, r, a, b);
        split(b, x - r, c, d);
        merge(d, merge(c, a));
    }
    else if(l == 1){
        split(t, r, a, b);
        split(b, x - r, c, d);
        split(d, y - x + 1, e, f);
        merge(e, merge(c, merge(a, f)));
    }
    else if(y == N){
        split(t, l - 1, a, b);
        split(b, r - l + 1, c, d);
        split(d, x - r + 1, e, f);
        merge(a, merge(f, merge(e, c)));
    }
    else{
        split(t, l - 1, a, b);
        split(b, r - l + 1, c, d);
        split(d, x - r + 1, e, f);
        split(f, y - x + 1, g, h);
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
        t = merge(t, tmp);
    }
    tmp = NULL;
    printf("t's val : %d, sum : %lld, size : %d\n", t -> val, t -> sum, t -> size);
    printf("t's left val : %d, sum : %lld, size : %d\n", t -> left -> val, t -> left -> sum, t -> left -> size);
    printf("t's left right val : %d, sum : %lld, size : %d\n", t -> left -> right -> val, t -> left -> right -> sum, t -> left -> right -> size);
    printf("t's left rihgt left val : %d\n", t -> left -> right -> left -> val);
    printf("t's left rihgt left left val : %d\n", t -> left -> right -> left -> left -> val);
    
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
            printf("1 : p = %d, k = %lld\n", Cmd[i].p, Cmd[i].k);
            if(!Cmd[i].p){
                t = merge(new(Cmd[i].k), t);
            }
            else if(Cmd[i].p == N){
                t = merge(t, new(Cmd[i].k));
            }
            else
                insert(t, Cmd[i].p, Cmd[i].k);
            N++;
        }
        // delete
        else if(Cmd[i].cmd == 2){
            printf("2 : p = %d\n", Cmd[i].p);
            if(Cmd[i].p == 1){
                split(t, 1, a, b);
                t = b;
            }
            else if(Cmd[i].p == N){
                split(t, N - 1, a, b);
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
            printf("3 : l = %d, r = %d\n", Cmd[i].l, Cmd[i].r);
            if(Cmd[i].l == 1 && Cmd[i].r == N)
                t -> rev = true;
            else if(Cmd[i].l == 1){
                split(t, Cmd[i].r, a, b);
                a -> rev = true;
                t = merge(a, b);
            }
            else if(Cmd[i].r == N){
                split(t, Cmd[i].l - 1, a, b);
                b -> rev = true;
                t = merge(a, b);
            }
            else
                reverse(t , Cmd[i].l, Cmd[i].r);
            a = b = NULL;
        }
        // swap
        else if(Cmd[i].cmd == 4){
            printf("4 : l = %d, r = %d, x = %d, y = %d\n", Cmd[i].l, Cmd[i].r, Cmd[i].x, Cmd[i].y);
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
            printf("6 : l = %d, r = %d\n", Cmd[i].l, Cmd[i].r);
            printf("sum : %lld\n", getsum(t, Cmd[i].l, Cmd[i].r));
        }
    }
    printf("end\n");
    //------------------------------------------------------------------------------print command
}
