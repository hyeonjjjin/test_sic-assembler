#pragma once
#include <stdio.h>
#include <stdlib.h>

//��ó
/* https://velog.io/@jewelrykim/C%EB%A1%9C-%ED%95%B4%EC%8B%9C%ED%85%8C%EC%9D%B4%EB%B8%94-%EA%B5%AC%ED%98%84%ED%95%98%EA%B8%B0  */

struct bucket* hashTable = NULL;
int BUCKET_SIZE = 10; // ������ �� ����

// ��� ����ü ����
struct node {
	int key; // �ؽ� �Լ��� ���� Ű
	int value; // key �� ������ �ִ� ������
	struct node* next; // ���� ��带 ����Ű�� ������
};
// ���� ����ü ����
struct bucket {
	struct node* head; // ���� ���� �տ� �ִ� ����� ������
	int count; // ���Ͽ� ����ִ� ����� ����
};

//�Լ� ����

// �ؽ����̺� ���Ե� �� ���� ��带 �������ִ� �Լ�(�ʱ�ȭ)
struct node* createNode(int key, int value);
// �ؽ��Լ� �����. ���⼭�� �ܼ��� key�� ���� ���̷� ���� �������� �Լ��� ����.
int hashFunction(int key);
// ���� Ű �߰��ϴ� �Լ�
void add(int key, int value);
// Ű�� �������ִ� �Լ�
void remove_key(int key);
// Ű�� �ָ� value�� �˷��ִ� �Լ�
void search(int key);
//�ؽ����̺� ��ü ������ִ� �Լ�
void display();


//�Լ� ����

struct node* createNode(int key, int value) {
    struct node* newNode;
    // �޸� �Ҵ�
    newNode = (struct node*)malloc(sizeof(struct node));
    // ����ڰ� ������ ���� ����
    newNode->key = key;
    newNode->value = value;
    newNode->next = NULL; // ������ ���� next�� NULL�� �ʱ�ȭ

    return newNode;
}

int hashFunction(int key) {
    return key % BUCKET_SIZE;
}

void add(int key, int value) {
    // ��� ���Ͽ� �߰����� �ε����� ���
    int hashIndex = hashFunction(key);
    // ���� ��� ����
    struct node* newNode = createNode(key, value);
    // ����� �ε����� ���Ͽ� �ƹ� ��嵵 ���� ���
    if (hashTable[hashIndex].count == 0) {
        hashTable[hashIndex].count = 1;
        hashTable[hashIndex].head = newNode; // head�� ��ü
    }
    // ���Ͽ� �ٸ� ��尡 ���� ��� ��ĭ�� �а� ���� ��尡 �ȴ�(�����δ� �����͸� �ٲ���)
    else {
        newNode->next = hashTable[hashIndex].head;
        hashTable[hashIndex].head = newNode;
        hashTable[hashIndex].count++;
    }
}

void remove_key(int key) {
    int hashIndex = hashFunction(key);
    // ������ �Ǿ����� Ȯ���ϴ� flag ����
    int flag = 0;
    // Ű�� ã���� iterator ����
    struct node* node = NULL;
    struct node* before = NULL; // ��尡 ������ �ٷ� �� ���
    // ������ head���� ����
    node = hashTable[hashIndex].head;
    // ������ ��ȸ�ϸ鼭 key�� ã�´�.
    while (node != NULL) // NULL �� ���ö����� ��ȸ
    {
        if (node->key == key) {
            // ������ �ٲ��ֱ� ��尡 1 . ����� ��� 2 . ��尡 �ƴѰ��
            if (node == hashTable[hashIndex].head) {
                hashTable[hashIndex].head = node->next; // ���� ��尡 ���� ���
            }
            else {
                before->next = node->next; // ����尡 ���� �� ���� ��带 ����Ŵ
            }
            // ������ �۾� ����
            hashTable[hashIndex].count--;
            free(node);
            flag = 1;
        }
        before = node; // ��� �ٲ��ֱ� ���� ����
        node = node->next;
    }
    // flag�� ���� ���� ��� �ٸ��� ����
    if (flag == 1) { // ������ �Ǿ��ٸ�
        printf("\n [ %d ] ��/�� �����Ǿ����ϴ�. \n", key);
    }
    else { // Ű�� ��� ������ �ȵ� ���
        printf("\n [ %d ] ��/�� �������� �ʾ� �������� ���߽��ϴ�.\n", key);
    }
}

void search(int key) {
    int hashIndex = hashFunction(key);
    struct node* node = hashTable[hashIndex].head;
    int flag = 0;
    while (node != NULL)
    {
        if (node->key == key) {
            flag = 1;
            break;
        }
        node = node->next;
    }
    if (flag == 1) { // Ű�� ã�Ҵٸ�
        printf("\n Ű�� [ %d ], ���� [ %d ] �Դϴ�. \n", node->key, node->value);
    }
    else {
        printf("\n �������� ���� Ű�� ã�� �� �����ϴ�. \n");
    }
}

void display() {
    // �ݺ��� ����
    struct node* iterator;
    printf("\n========= display start ========= \n");
    for (int i = 0; i < BUCKET_SIZE; i++) {
        iterator = hashTable[i].head;
        printf("Bucket[%d] : ", i);
        while (iterator != NULL)
        {
            printf("(key : %d, val : %d)  -> ", iterator->key, iterator->value);
            iterator = iterator->next;
        }
        printf("\n");
    }
    printf("\n========= display complete ========= \n");
}

