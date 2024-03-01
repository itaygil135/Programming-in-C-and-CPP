#include "library.h"
#include <stdlib.h>
#include <stdio.h>
typedef struct Node {
    int data;
    struct Node* next;
} Node;

    void printList(Node *head) {
        Node* p = head;
        while (p !=NULL) {
            printf("%d ", p->data);
            p = p->next;
        }
        printf("\n");
    }

    void push(Node* head, int new_data)
    {
        Node* new_node = (Node*)malloc(sizeof(Node));
        new_node->data  = new_data;
        new_node->next = head;
        head = new_node;
    }

    void deleteList(Node* head) {
        Node* p = head;
        while (p !=NULL) {
            Node *next = p->next;
            free(p);
            p = next;
        }
        head = NULL;
    }

    void removeDuplicates(Node* head) {
        Node *ptr1, *ptr2, *ptr3;
        ptr1 = head;

        while (ptr1 != NULL ) {
            ptr2 = ptr1;

            while (ptr2->next != NULL) { // iterate the list from ptr2

                if (ptr1->data == ptr2->next->data) { // need to delete
                    if(ptr2->next->next != NULL)
                    {
                        ptr3 = ptr2->next;
                        ptr2->next = ptr2->next->next;
                        free(ptr3);
                        ptr3 = NULL;

                    }
                    else{
                        free(ptr2->next);
                        ptr2->next = NULL;



                    }
                }
                else
                    {
                    ptr2 = ptr2->next;
                }
            }
            ptr1 = ptr1->next; // move to next node
        }

    }

    void myListTest() {
        Node *phead = NULL;
        for (int i = 0; i < 10; i++) {
            push(phead, i);
            push(phead, 10 - i);
        }
        printList(phead);
        removeDuplicates(phead);
        printList(phead);
        deleteList(phead);
    }
int main() {
    myListTest();
return 0;}



