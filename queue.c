#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* Notice: sometimes, Cppcheck would find the potential NULL pointer bugs,
 * but some of them cannot occur. You can suppress them by adding the
 * following line.
 *   cppcheck-suppress nullPointer
 */


/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *head = malloc(sizeof(struct list_head));
    if (!head) {
        return NULL;
    }
    INIT_LIST_HEAD(head);
    return head;
}

/* Free all storage used by queue */
void q_free(struct list_head *l)
{
    if (!l) {
        return;
    }
    element_t *cur, *tmp;
    list_for_each_entry_safe (cur, tmp, l, list) {
        q_release_element(cur);
    }
    free(l);
    return;
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    int str_len = strlen(s);
    if (!head) {
        return false;
    }
    element_t *element = malloc(sizeof(element_t));
    if (!element) {
        free(element);
        return false;
    }
    element->value = malloc(sizeof(char) * (str_len + 1));
    if (!element->value) {
        free(element);
        return false;
    }
    strncpy(element->value, s, str_len);
    *(element->value + str_len) = '\0';
    list_add(&element->list, head);
    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    int str_len = strlen(s);
    if (!head) {
        return false;
    }
    element_t *element = malloc(sizeof(element_t));
    if (!element) {
        free(element);
        return false;
    }
    element->value = malloc(sizeof(char) * (str_len + 1));
    if (!element->value) {
        free(element);
        return false;
    }
    strncpy(element->value, s, str_len);
    *(element->value + str_len) = '\0';
    list_add_tail(&element->list, head);
    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head)) {
        return NULL;
    }
    element_t *first = list_first_entry(head, element_t, list);
    list_del(&first->list);
    if (sp) {
        strncpy(sp, first->value, bufsize - 1);
        *(sp + bufsize - 1) = '\0';
    }
    return first;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head)) {
        return NULL;
    }
    element_t *last = list_last_entry(head, element_t, list);
    list_del(&last->list);
    if (sp) {
        strncpy(sp, last->value, bufsize - 1);
        *(sp + bufsize - 1) = '\0';
    }
    return last;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    if (!head || list_empty(head)) {
        return 0;
    }
    int cnt = 0;
    struct list_head *node, *safe;
    list_for_each_safe (node, safe, head) {
        cnt++;
    }
    return cnt;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    if (!head || list_empty(head))
        return false;
    struct list_head *forward = head->next;
    struct list_head *backward = head->prev;
    while (backward->next != forward && backward != forward) {
        forward = forward->next;
        backward = backward->prev;
    }
    list_del(forward);

    // delete entry
    element_t *middle = list_entry(forward, element_t, list);
    free(middle);
    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
    if (head == NULL || list_is_singular(head) || list_empty(head)) {
        return;
    }
    struct list_head *first = head->next;
    while (first != head && first->next != head) {
        struct list_head *second = first->next;
        list_del(first);
        list_add(first, second);
        first = first->next;
    }
    return;
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    if (!head || list_empty(head)) {
        return;
    }
    struct list_head *node, *safe;
    list_for_each_safe (node, safe, head) {
        list_move(node, head);
    }
    return;
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
}

/* Sort elements of queue in ascending order */
void q_sort(struct list_head *head) {}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Merge all the queues into one sorted queue, which is in ascending order */
int q_merge(struct list_head *head)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}
