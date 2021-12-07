#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WORD_MAX 30
#define BUCKET_SIZE 600

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

struct bucket* hashTable;

// 함수 선언
void HashTableInit();

// 해쉬테이블 삽입될 때 새로 노드를 생성해주는 함수(초기화)
struct node* createNode(long long key, char label[WORD_MAX], int locctr);

// 해쉬함수 만들기. 여기서는 단순히 key를 버켓 길이로 나눈 나머지로 함수를 만듦.
int hashFunction(long long key);

// 새로 키 추가하는 함수
void add(long long key, char label[WORD_MAX], int locctr);

// 키를 삭제해주는 함수
void remove_key(long long key);

// 키를 주면 value를 알려주는 함수
int search(long long key);

// 해시테이블 전체를 출력해주는 함수
void display();

long long encode(char* label);

// 출처
/* https://velog.io/@jewelrykim/C%EB%A1%9C-%ED%95%B4%EC%8B%9C%ED%85%8C%EC%9D%B4%EB%B8%94-%EA%B5%AC%ED%98%84%ED%95%98%EA%B8%B0  */