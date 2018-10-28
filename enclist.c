/* This is an implementation of a linked list that encodes the
 * forward and backward pointers into a single list item.
 * Why? Because I can.
 * Because a gauntlet was thrown down, and I picked it up.
 * Simple XOR encoding forward and back.
 * This can be simplified and optimized, but the implementation here is intended to illustrate the point.
 *
 * Copyright 2018 Douglas Fraser.
 * Creative Commons, have fun with it.

 * If you tweak it and do something interesting with it, let me know.
 */
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

    

typedef struct s_list_item t_list_item;
struct s_list_item {
    // LXR is an exlusive OR of the Left and Right links.
    unsigned long lxr;
    int data1;
    int data2;
};

typedef struct s_lxr_info t_lxr_info;
struct s_lxr_info {
    t_list_item * left;
    t_list_item * right;
};

// Here is our list header
t_list_item * lh = NULL;

void print_at(t_lxr_info * p_lxri, t_list_item * pli)
{
        if(pli != NULL) {
            printf("at pli %#08lx, llxr %#08lx, left %#08lx, right %#08lx, d1 %d, d2, %d\n",
                (unsigned long)pli, (unsigned long)(pli->lxr), 
                (unsigned long)(p_lxri->left), (unsigned long)(p_lxri->right),
                pli->data1, pli->data2);
        } else {
            printf("at pli %#08lx, left %#08lx, right %#08lx\n",
                (unsigned long)pli, (unsigned long)(p_lxri->left), (unsigned long)(p_lxri->right));
        }
}

t_list_item * extract_address(t_list_item *lxr, t_list_item *other)
{
    return (t_list_item*)(lxr->lxr ^ (unsigned long)other);

}

void shift_lxr(t_list_item *lxr, t_list_item *known, t_list_item *update)
{
    lxr->lxr = (lxr->lxr ^ (unsigned long)known) ^ (unsigned long)update;
}

t_list_item * lxr_move_right(t_lxr_info * p_lxri, t_list_item *pli)
{
    t_list_item *pcur = pli;
    // printf("move right\n");
    if(p_lxri == NULL || p_lxri->right == NULL || pli == NULL) {
        return NULL;
    }
    pli = p_lxri->right;
    p_lxri->left = pcur;
    p_lxri->right = extract_address(pli, pcur);
    return pli;
}

t_list_item * lxr_move_left(t_lxr_info * p_lxri, t_list_item *pli)
{
    t_list_item *pcur = pli;
    // printf("move left\n");
    if(p_lxri == NULL || p_lxri->left == NULL || pli == NULL) {
        return NULL;
    }
    pli = p_lxri->left;
    p_lxri->right = pcur;
    p_lxri->left = extract_address(pli, pcur);
    return pli;
}

// Walk the list starting at the header
void list_walk(void)
{
    t_list_item *pcur = lh;
    t_lxr_info lxri;

    printf("\n:: List Walk\n\n");
    lxri.left=NULL;
    lxri.right=NULL;
    pcur = lh;
    lxri.left = NULL;
    lxri.right = extract_address(pcur, lxri.left);
    while (pcur != NULL) {
        print_at(&lxri, pcur);
        pcur = lxr_move_right(&lxri, pcur);
    }
}

// Walk the list starting at the header, but with serious jiggling
void list_stumble(void)
{
    t_list_item *pcur = lh;
    t_lxr_info lxri;
    int i;

    printf("\n:: List Stumble\n\n");
    lxri.left=NULL;
    lxri.right=NULL;
    pcur = lh;
    lxri.left = NULL;
    lxri.right = extract_address(pcur, lxri.left);
    print_at(&lxri, pcur);
    while (pcur != NULL) {
        for(i = 0; i < 5 && pcur != NULL; i++) {
            pcur = lxr_move_right(&lxri, pcur);
        }
        if(pcur != NULL) { print_at(&lxri, pcur); }
        for(i = 0; i < 3 && pcur != NULL; i++) {
            pcur = lxr_move_left(&lxri, pcur);
        }
        if(pcur != NULL) { print_at(&lxri, pcur); }
    }
}

// Append an item where we are
t_list_item * append_item(t_lxr_info * p_lxri, t_list_item *p_current, int d1, int d2)
{
    t_list_item * p_this;
    t_list_item * p_left;
    t_list_item * p_right;
    if((p_this = calloc(1, sizeof(t_list_item))) == NULL) {
        printf("ZOINKS! calloc NULL! (my oh my....)\n");
    }
    else {
        p_left = p_lxri->left = p_current;
        p_right = p_lxri->right;
        printf("append item at this %#08lx after %#08lx", (unsigned long)p_this, (unsigned long)p_current);
        printf(": item left at %#08lx, right %#08lx", (unsigned long)p_left, (unsigned long)p_right);
        printf(": d1 %d d2 %d\n", d1, d2);
        if(p_left != NULL) {
            shift_lxr(p_left, p_right, p_this);
        }
        if(p_right != NULL) {
            shift_lxr(p_right, p_left, p_this);
        }
        p_this->lxr = (unsigned long)p_current ^ (unsigned long)p_lxri->right;
        p_this->data1 = d1;
        p_this->data2 = d2;
    }
    return p_this;
}

// Insert a data item in order
t_list_item * insert_data_item(t_lxr_info * p_lxri, t_list_item *pli, int d1, int d2)
{
    t_list_item *pcur;

    printf("\n:: insert data item\n\n");
    while(d1 > pli->data1 && pli != NULL && p_lxri->right != NULL) {
            pli = lxr_move_right(p_lxri, pli);
    }
    pli = lxr_move_left(p_lxri, pli);
    print_at(p_lxri, pli);
    pcur = pli;
    pli = append_item(p_lxri, pcur, d1, d2);

    return pli;
}

int
main(int argc, char ** argv)
{
    int index = 0;
    int maxrun = 20;
    t_list_item *pcur = lh;
    t_list_item *pnli;
    t_lxr_info lxri;

    lxri.left=NULL;
    lxri.right=NULL;

    while (index < maxrun) {
        if((pnli = append_item(&lxri, pcur, 3*index, 3*index + 1)) == NULL) {
            printf("append_item returned NULL. Toasted.\n");
            exit(2);
        }
        pcur = pnli;
        if(lh == NULL) { lh = pcur; }
        index++;
    }

    // now walk that list...
    list_walk();

    // stumble back and forth
    list_stumble();

    lxri.left=NULL;
    lxri.right=(t_list_item*)(lh->lxr ^ 0);
    insert_data_item(&lxri, lh, 47, 48);

    list_walk();
}
