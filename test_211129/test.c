#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE 

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include<stdio.h>
#include <stdbool.h>
#include "hash.h"

#define WORD_MAX        30
#define NUM_WORD        3
#define LABEL 0 
#define OPCODE 1
#define OPERAND 2

#define BUCKET_SIZE 600


//OPTAB static 형태로 선언. (추가 or 삭제 X)
char opcode_s[59][7] = { "ADD", "ADDF","ADDR","AND","CLEAR","COMP","COMPF","COMPR","DIV","DIVF","DIVR","FIX","FLOAT","HIO","J","JEQ","JGT","JLT","JSUB","LDA","LDB","LDCH","LDF","LDL","LDS","LDT","LDX","LPS","MUL","MULF","MULR","NORM","OR","RD","RMO","RSUB","SHIFTL","SHIFTR","SIO","SSK","STA","STB","STCH","STF","STI","STL","STS","STSW","STT","STX","SUB","SUBF","SUBR","SVC","TD","TIO","TIX","TIXR","WD" };
char opcode_n[59][3] = { "18","58","90","40","B4","28","88","A0","24","64","9C","C4","C0","F4","3C","30","34","38","48","00","68","50","70","08","6C","74","04","D0","20","60","98","C8","44","D8","AC","4C","A4","A8","F0","EC","0C","78","54","80","D4","14","7C","E8","84","10","1C","5C","94","B0","E0","F8","2C","B8","DC" };

struct bucket* STMTAB = NULL;



//함수 정의

// line 읽어와서 word별 분리 (word[0:2]). 어떤 line 값이 있고, 없는지 확인 (error_check[0:2])
int word_split(const char* line, char(*word)[WORD_MAX], int* error_check);

// int형 값 input, 자릿수 return 
int num_of_digits(int n);

// 0 채우기. 값에 관계없이 6자리로 표현되도록.
void fill_zero(char* str, int n);


int main() {
	char line[40], obj_code[82];
	char word[NUM_WORD][WORD_MAX];    /* 단어들을 저장할 이차원 배열 */
	int i, num_words, program_len_n;
	int locctr = 0;
	char start_address[8], program_len[8];
	char buffer[50];
	int start_address_n=0;
	int error_check[3] = {0,};
	long long key = 0;
	STMTAB = (struct bucket*)malloc(BUCKET_SIZE * sizeof(struct bucket));
	

	//file open --- fp1: INPUT, fp2: INTER, fp3: OUTPUT
	FILE *fp1, *fp2, *fp3, *fp4;
	fp1 = fopen("TEST1.txt", "r");
	fp2 = fopen("INTR1.txt", "w");

	//PASS 1
	
	//첫 줄에 START 있는지 확인, locctr = if (OPCODE == "START") ? operand : 0 
	fgets(line, 40, fp1);
	num_words = word_split(line, word, error_check);
	if (strcmp(word[1], "START")==0) {
		strcpy(start_address, word[2]);
		start_address_n = locctr = atoi(word[2]);
	}
	add(encode(word[LABEL]), word[LABEL], locctr);
	//locctr + symbol, opcode, operand 형태로 intermediate file에 저장  /////////////////이거 하면 word split 함수 각각만들어야함
	//strcpy(buffer, strcat(start_address, " "));
	//fputs(strcat(buffer, line), fp2);

	//읽은 line intermediate file에 저장
	fputs(line, fp2);

	// OPCODE == "END" 까지 loop
	while (strcmp(word[1],"END")) {
		
		//한 줄 읽어와서 단어 단위로 쪼갬
		fgets(line, 40, fp1);
		num_words = word_split(line, word, error_check);
		key = encode(word[LABEL]);
		//LABEL에 Symbol이 있다.
		if (error_check[LABEL] == 1) {
			//symbol table에 있다면 error
			if (!search(5)) {
				printf("duplicate symbol error\n\n");
				return 0;
			}
		}
		else {
			//symbol table에 {LABEL, LOCCTR} 형태로 저장
			add(key, word[LABEL], locctr);
		}
		display();
		//OPCODE가 있다
		if (error_check[OPCODE] == 1) {
			//OPCODE가 OPTABLE에 있다 : LOCCTR +=3
			//OPCODE가 "WORD"다 : LOCCTR +=3
			//OPCODE가 "RESW"다 : LOCCTR +=3*operand
			//OPCODE가 "RESB"다 : LOCCTR +=operand
			//OPCODE가 "BYTE"다 : LOCCTR +=길이
			//else : error
		}
		//intermediate file에 넣고 다음 줄 읽기
	}
	//프로그램 길이: LOCCTR-시작 주소
	program_len_n = locctr - start_address_n;
	sprintf(program_len, "%d", program_len_n);
	fill_zero(program_len, program_len_n);


	fclose(fp1);
	fclose(fp2);
	// PASS 1 종료



	//PASS 2
	//intermediate file, OUTOUT file open
	fp3 = fopen("INTR1.txt", "r");
	fp4 = fopen("OUTPUT1.txt", "w");


	//intr file 첫 줄에 START 있는지 확인, locctr = if (OPCODE == "START") ? operand : 0 
	fgets(line, 40, fp3);
	num_words = word_split(line, word, error_check);
	if (strcmp(word[1], "START") == 0) {
		strcpy(start_address, word[2]);
		start_address_n = locctr = atoi(word[2]);
		fill_zero(start_address, start_address_n);
	}

	//Header 작성
	fputs("H^", fp4);//H
	fputs(strcat(word[0],"^"), fp4);//program name 
	fputs(strcat(start_address,"^"), fp4);//시작주소
	fputs(program_len, fp4);//프로그램 길이

	//Text 작성

	//printf("\n %s",obj_code);
	fclose(fp4);
	//fputs(line, fp4);

	
	return 0;
}

//함수 정의

int word_split(const char* line, char(*word)[WORD_MAX], int* error_check)
{
	int i, words = 0, chars, count_space = 0;
	int start_word = 0;
	error_check[0] = 0; error_check[1] = 0; error_check[2] = 0;
	word[0][0] = '\0'; word[1][0] = '\0'; word[2][0] = '\0';

	//line이 끝날 때 까지 한 글자씩 확인
	for (i = 0, chars = 0; line[i] != '\n'; i++) {

		//tab 이면 비워진 liner
		if (line[i] == '\t') {
			count_space = 0; word[words][0] = '\0'; error_check[words++] = 0;
		}
		//space 
		else if (line[i] == ' ') {
			if (chars) { // 글자 뒤 space이면 null입력
				word[words][chars] = '\0';
				error_check[words++] = 1;
				chars = 0;
				count_space++;
			}
			else {
				// space 8회이면 비워진 line
				if (++count_space == 4) {
					word[words][0] = '\0'; error_check[words++] = 0; count_space = 0;
				}
			}
		}
		//글자가 있으면 word 변수에 넣어줌
		else {
			count_space = 0;
			word[words][chars++] = line[i];
		}
	}
	word[words][chars] = '\0';    // 마지막 단어의 NULL 문자 추가, 개수 추가
	error_check[words++] = 1;
	return words;                // 값이 있는 line 수
}

int num_of_digits(int n) {
	if (n == 0) return 1;
	return floor(log10(n) + 1);
}

void fill_zero(char* str, int n) {
	int digit = num_of_digits(n);
	char zero[7];
	switch (digit) {
	case 6: return;
	case 5: strcpy(zero, "0"); break;
	case 4: strcpy(zero, "00"); break;
	case 3: strcpy(zero, "000"); break;
	case 2: strcpy(zero, "0000"); break;
	case 1: strcpy(zero, "00000"); break;
	}
	strcat(zero, str);
	printf("length: %d", sizeof(zero));
	strcpy(str, zero);
}