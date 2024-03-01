#include <stdio.h>
void push(Node** head, int new_data)
{
    Node* new_node = (Node*) malloc(sizeof(Node));
    new_node->data = new_data;
    new_node->next = *head;
    new_node->prev = NULL;

    if(*head != NULL) {
        (*head)->prev = new_node;
    }

    *head = new_node;
}
void deleteList(Node** head)
{
    Node* p = *head;
    while (p !=NULL) {
        Node *next = p->next;
        free(p);
        p = next;
    }
    *head = NULL;
}

void deleteNode(Node **head, Node *del)
{
    if(*head != NULL && del != NULL) {
        if(*head == del) {  // head is deleted
            *head = del->next;
        }
        // update the prev pointer for next
        del->next->prev = del->prev;

        // update the next pointer for prev
        del->prev->next = del->next;

        free(del);
    }
}

void myListTest() {
    Node* phead = NULL;
    for(int i=0; i<10; i++) {
        push(&phead, i);
    }
    printList(phead);

    deleteNode(&phead, phead->next->next);
    printList(phead);

    deleteNode(&phead, phead);
    printList(phead);

    deleteList(&phead);
}