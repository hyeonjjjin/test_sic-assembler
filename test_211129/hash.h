#pragma once
#include <stdio.h>
#include <stdlib.h>

#define WORD_MAX        30
#define BUCKET_SIZE 600
//��ó
/* https://velog.io/@jewelrykim/C%EB%A1%9C-%ED%95%B4%EC%8B%9C%ED%85%8C%EC%9D%B4%EB%B8%94-%EA%B5%AC%ED%98%84%ED%95%98%EA%B8%B0  */

struct bucket* hashTable = NULL;

// ��� ����ü ����
struct node {
	long long key; // �ؽ� �Լ��� ���� Ű
    char label[WORD_MAX];
	int locctr; // key �� ������ �ִ� ������
	struct node* next; // ���� ��带 ����Ű�� ������
};
// ���� ����ü ����
struct bucket {
	struct node* head; // ���� ���� �տ� �ִ� ����� ������
	int count; // ���Ͽ� ����ִ� ����� ����
};

//�Լ� ����

// �ؽ����̺� ���Ե� �� ���� ��带 �������ִ� �Լ�(�ʱ�ȭ)
struct node* createNode(long long key, char label[WORD_MAX], int locctr);
// �ؽ��Լ� �����. ���⼭�� �ܼ��� key�� ���� ���̷� ���� �������� �Լ��� ����.
int hashFunction(long long key);
// ���� Ű �߰��ϴ� �Լ�
void add(long long key, char label[WORD_MAX], int locctr);
// Ű�� �������ִ� �Լ�
void remove_key(long long key);
// Ű�� �ָ� locctr�� �˷��ִ� �Լ�
int search(long long key);
//�ؽ����̺� ��ü ������ִ� �Լ�
void display();

//label ���ڿ� ��ȣȭ, key ����
long long encode(char* label);

//void decode(char* src, char* key, char* result);

//�Լ� ����

struct node* createNode(long long key, char label[WORD_MAX], int locctr) {
    struct node* newNode;
    // �޸� �Ҵ�
    newNode = (struct node*)malloc(sizeof(struct node));
    // ����ڰ� ������ ���� ����
    newNode->key = key;
    strcpy(newNode->label, label);
    newNode->locctr = locctr;
    newNode->next = NULL; // ������ ���� next�� NULL�� �ʱ�ȭ

    return newNode;
}

int hashFunction(long long key) {
    return key % BUCKET_SIZE;
}
 
void add(long long key, char label[WORD_MAX], int locctr) {
    // ��� ���Ͽ� �߰����� �ε����� ���
    long long hashIndex = hashFunction(key);
    // ���� ��� ����
    struct node* newNode = createNode(key, label,locctr);
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

void remove_key(long long key) {
    long long hashIndex = hashFunction(key);
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
        printf("\n [ %lld ] ��/�� �����Ǿ����ϴ�. \n", key);
    }
    else { // Ű�� ��� ������ �ȵ� ���
        printf("\n [ %lld ] ��/�� �������� �ʾ� �������� ���߽��ϴ�.\n", key);
    }
}

int search(long long key) {
    long long hashIndex = hashFunction(key);
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
        printf("\n Ű�� [ %lld ], ���� [%s, %d ] �Դϴ�. \n", node->key, node->label, node->locctr);
        return 1;
    }
    else {
        printf("\n �������� ���� Ű�� ã�� �� �����ϴ�. \n");
        return 0;
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
            printf("(key : %lld, label : %s,val : %d)  -> ", iterator->key, iterator->label, iterator->locctr);
            iterator = iterator->next;
        }
        printf("\n");
    }
    printf("\n========= display complete ========= \n");
}

long long encode(char* label) {
    int i = 0;
    char len = 0;
    long long key_n = 0;
    int encoder_buff_n = 0;
    char encoder_buff[WORD_MAX * 2] = { 0, };//��� ������, ����, ���ڴ� 16������ ���ڸ�����
    char key[WORD_MAX * 2] = { 0, };

    //line�� ���� �� ���� �� ���ھ� Ȯ��
    for (i = 0; label[i] != '\0'; i++) {
        encoder_buff_n += label[i];
    }
    key[0] = i+ '0';
    sprintf(encoder_buff, "%X", encoder_buff_n);
    strcat(key, encoder_buff);
    key_n = strtol(key, NULL, 16);
    //printf("%s   ", key);
    //printf("%X", key_n);
    return key_n;
}
/*
void decode(char* src, char* key, char* result) {

    encode(src, key, result);

}*/