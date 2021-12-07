#pragma once
#include <stdio.h>
#include <stdlib.h>

#define WORD_MAX        30
#define BUCKET_SIZE 600
//출처
/* https://velog.io/@jewelrykim/C%EB%A1%9C-%ED%95%B4%EC%8B%9C%ED%85%8C%EC%9D%B4%EB%B8%94-%EA%B5%AC%ED%98%84%ED%95%98%EA%B8%B0  */

struct bucket* hashTable = NULL;

// 노드 구조체 선언
struct node {
	long long key; // 해시 함수에 사용될 키
    char label[WORD_MAX];
	int locctr; // key 가 가지고 있는 데이터
	struct node* next; // 다음 노드를 가르키는 포인터
};
// 버켓 구조체 선언
struct bucket {
	struct node* head; // 버켓 가장 앞에 있는 노드의 포인터
	int count; // 버켓에 들어있는 노드의 개수
};

//함수 선언

// 해쉬테이블 삽입될 때 새로 노드를 생성해주는 함수(초기화)
struct node* createNode(long long key, char label[WORD_MAX], int locctr);
// 해쉬함수 만들기. 여기서는 단순히 key를 버켓 길이로 나눈 나머지로 함수를 만듦.
int hashFunction(long long key);
// 새로 키 추가하는 함수
void add(long long key, char label[WORD_MAX], int locctr);
// 키를 삭제해주는 함수
void remove_key(long long key);
// 키를 주면 locctr를 알려주는 함수
int search(long long key);
//해시테이블 전체 출력해주는 함수
void display();

//label 문자열 암호화, key 생성
long long encode(char* label);

//void decode(char* src, char* key, char* result);

//함수 정의

struct node* createNode(long long key, char label[WORD_MAX], int locctr) {
    struct node* newNode;
    // 메모리 할당
    newNode = (struct node*)malloc(sizeof(struct node));
    // 사용자가 전해준 값을 대입
    newNode->key = key;
    strcpy(newNode->label, label);
    newNode->locctr = locctr;
    newNode->next = NULL; // 생성할 때는 next를 NULL로 초기화

    return newNode;
}

int hashFunction(long long key) {
    return key % BUCKET_SIZE;
}
 
void add(long long key, char label[WORD_MAX], int locctr) {
    // 어느 버켓에 추가할지 인덱스를 계산
    long long hashIndex = hashFunction(key);
    // 새로 노드 생성
    struct node* newNode = createNode(key, label,locctr);
    // 계산한 인덱스의 버켓에 아무 노드도 없을 경우
    if (hashTable[hashIndex].count == 0) {
        hashTable[hashIndex].count = 1;
        hashTable[hashIndex].head = newNode; // head를 교체
    }
    // 버켓에 다른 노드가 있을 경우 한칸씩 밀고 내가 헤드가 된다(실제로는 포인터만 바꿔줌)
    else {
        newNode->next = hashTable[hashIndex].head;
        hashTable[hashIndex].head = newNode;
        hashTable[hashIndex].count++;
    }
}

void remove_key(long long key) {
    long long hashIndex = hashFunction(key);
    // 삭제가 되었는지 확인하는 flag 선언
    int flag = 0;
    // 키를 찾아줄 iterator 선언
    struct node* node = NULL;
    struct node* before = NULL; // 노드가 지나간 바로 전 노드
    // 버켓의 head부터 시작
    node = hashTable[hashIndex].head;
    // 버켓을 순회하면서 key를 찾는다.
    while (node != NULL) // NULL 이 나올때까지 순회
    {
        if (node->key == key) {
            // 포인터 바꿔주기 노드가 1 . 헤드인 경우 2 . 헤드가 아닌경우
            if (node == hashTable[hashIndex].head) {
                hashTable[hashIndex].head = node->next; // 다음 노드가 이제 헤드
            }
            else {
                before->next = node->next; // 전노드가 이제 내 다음 노드를 가르킴
            }
            // 나머지 작업 수행
            hashTable[hashIndex].count--;
            free(node);
            flag = 1;
        }
        before = node; // 노드 바꿔주기 전에 저장
        node = node->next;
    }
    // flag의 값에 따라 출력 다르게 해줌
    if (flag == 1) { // 삭제가 되었다면
        printf("\n [ %lld ] 이/가 삭제되었습니다. \n", key);
    }
    else { // 키가 없어서 삭제가 안된 경우
        printf("\n [ %lld ] 이/가 존재하지 않아 삭제하지 못했습니다.\n", key);
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
    if (flag == 1) { // 키를 찾았다면
        printf("\n 키는 [ %lld ], 값은 [%s, %d ] 입니다. \n", node->key, node->label, node->locctr);
        return 1;
    }
    else {
        printf("\n 존재하지 않은 키는 찾을 수 없습니다. \n");
        return 0;
    }
}

void display() {
    // 반복자 선언
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
    char encoder_buff[WORD_MAX * 2] = { 0, };//모든 영문자, 숫자, 문자는 16진수로 두자리씩임
    char key[WORD_MAX * 2] = { 0, };

    //line이 끝날 때 까지 한 글자씩 확인
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