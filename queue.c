#include <stdint.h>
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
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    if (!head || list_empty(head) || list_is_singular(head)) {
        return false;
    }

    element_t *curr = NULL, *next = NULL;
    bool check = false;
    list_for_each_entry_safe (curr, next, head, list) {
        if (&next->list != head && !strcmp(curr->value, next->value)) {
            list_del_init(&curr->list);
            q_release_element(curr);
            check = true;
        } else if (check) {
            list_del_init(&curr->list);
            q_release_element(curr);
            check = false;
        }
    }
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
    if (!head || list_empty(head) || list_is_singular(head) || k <= 2) {
        return;
    }

    int list_length = q_size(head);
    struct list_head **change = NULL;
    struct list_head *header = NULL, *curr = head->next;
    while (curr != head && curr->next != head) {
        change = &curr->next;
        header = curr->prev;
        for (int i = 1; i < k; i++) {
            if (list_length >= k) {
                list_move(*change, header);
            }
        }
        list_length -= k;
        curr = curr->next;
    }
    return;
}

struct list_head *merge_two_list(struct list_head *left,
                                 struct list_head *right)
{
    struct list_head *head = NULL, **ptr = &head;
    element_t *left_node = NULL, *right_node = NULL;
    for (; left && right; ptr = &(*ptr)->next) {
        left_node = list_entry(left, element_t, list);
        right_node = list_entry(right, element_t, list);
        if (strcmp(left_node->value, right_node->value) < 0) {
            *ptr = left;
            left = left->next;
        } else {
            *ptr = right;
            right = right->next;
        }
    }
    *ptr = (struct list_head *) ((uintptr_t) left | (uintptr_t) right);
    return head;
}

struct list_head *merge_sort(struct list_head *head)
{
    if (!head || !head->next) {
        return head;
    }

    struct list_head *fast, *slow = head;
    for (fast = head->next; fast && fast->next; fast = fast->next->next) {
        slow = slow->next;
    }

    struct list_head *left, *right;

    right = slow->next;
    slow->next = NULL;
    left = merge_sort(head);
    right = merge_sort(right);
    return merge_two_list(left, right);
}

/* Sort elements of queue in ascending order */
void q_sort(struct list_head *head)
{
    if (!head || list_empty(head) || list_is_singular(head)) {
        return;
    }

    head->prev->next = NULL;
    head->next = merge_sort(head->next);

    struct list_head *curr = head, *node = head->next;
    while (node) {
        node->prev = curr;
        curr = node;
        node = node->next;
    }
    curr->next = head;
    head->prev = curr;
    return;
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    if (head == NULL || list_empty(head)) {
        return 0;
    }
    struct list_head *node = head->prev;
    int total = 1, delete = 0;
    element_t *curr = NULL, *check = NULL;
    char *num = NULL;
    while (&check->list != head && node->prev != head) {
        curr = list_entry(node, element_t, list);
        check = list_entry(node->prev, element_t, list);
        int str_len = strlen(curr->value);
        num = malloc(sizeof(char) * (str_len + 1));
        strncpy(num, curr->value, str_len);
        *(num + str_len) = '\0';
        if (strcmp(check->value, num) < 0) {
            list_del(&check->list);
            q_release_element(check);
            delete += 1;
        } else {
            node = node->prev;
        }
        free(num);
        total += 1;
    }
    return total - delete;
}

/* Merge all the queues into one sorted queue, which is in ascending order */
int q_merge(struct list_head *head)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    if (!head || list_empty(head)) {
        return 0;
    }
    if (list_is_singular(head)) {
        return list_entry(head->next, queue_contex_t, chain)->size;
    }
    queue_contex_t *merged_list = list_entry(head->next, queue_contex_t, chain);
    struct list_head *node = NULL, *safe = NULL;
    list_for_each_safe (node, safe, head) {
        if (node == head->next) {
            continue;
        }
        queue_contex_t *tmp = list_entry(node, queue_contex_t, chain);
        list_splice_init(tmp->q, merged_list->q);
    }
    q_sort(merged_list->q);
    return merged_list->size;
}