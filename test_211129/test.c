#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE 

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include<stdio.h>
#include <stdbool.h>

#define WORD_MAX        30
#define NUM_WORD        3
#define LABEL 0 
#define OPCODE 1
#define OPERAND 2


char opcode_s[59][7] = { "ADD", "ADDF","ADDR","AND","CLEAR","COMP","COMPF","COMPR","DIV","DIVF","DIVR","FIX","FLOAT","HIO","J","JEQ","JGT","JLT","JSUB","LDA","LDB","LDCH","LDF","LDL","LDS","LDT","LDX","LPS","MUL","MULF","MULR","NORM","OR","RD","RMO","RSUB","SHIFTL","SHIFTR","SIO","SSK","STA","STB","STCH","STF","STI","STL","STS","STSW","STT","STX","SUB","SUBF","SUBR","SVC","TD","TIO","TIX","TIXR","WD" };
char opcode_n[59][3] = { "18","58","90","40","B4","28","88","A0","24","64","9C","C4","C0","F4","3C","30","34","38","48","00","68","50","70","08","6C","74","04","D0","20","60","98","C8","44","D8","AC","4C","A4","A8","F0","EC","0C","78","54","80","D4","14","7C","E8","84","10","1C","5C","94","B0","E0","F8","2C","B8","DC" };


// line �о�ͼ� word�� �и� (word[0:2]). � line ���� �ְ�, ������ Ȯ�� (error_check[0:2])
int word_split(const char *line, char(*word)[WORD_MAX], int *error_check)
{
	int i, words=0, chars, count_space=0;
	int start_word = 0;
	error_check[0] = 0; error_check[1] = 0; error_check[2] = 0;
	word[0][0] = '\0'; word[1][0] = '\0'; word[2][0] = '\0';

	//line�� ���� �� ���� �� ���ھ� Ȯ��
	for (i = 0, chars = 0; line[i] != '\n'; i++) {

		//tab �̸� ����� liner
		if (line[i] == '\t') { 
			count_space = 0; word[words][0] = '\0'; error_check[words++] = 0;
		} 
		//space 
		else if (line[i] == ' ') { 
			if (chars) { // ���� �� space�̸� null�Է�
				word[words][chars] = '\0';
				error_check[words++] = 1;
				chars = 0;
				count_space++;
			} 
			else {
				// space 8ȸ�̸� ����� line
				if (++count_space ==4) {
					word[words][0] = '\0'; error_check[words++] = 0; count_space = 0;
				}
			}
		}
		//���ڰ� ������ word ������ �־���
		else {
			count_space = 0;
			word[words][chars++] = line[i];
		}
	}
	word[words][chars] = '\0';    // ������ �ܾ��� NULL ���� �߰�, ���� �߰�
	error_check[words++] = 1;
	return words;                // ���� �ִ� line ��
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
	case 4: strcpy(zero, "00"); break; // �̰� �� ������ str�� starting_address �־��µ� 
	case 3: strcpy(zero, "000"); break;
	case 2: strcpy(zero, "0000"); break;
	case 1: strcpy(zero, "00000"); break;
	}
	strcat(zero, str);
	printf("length: %d", sizeof(zero));
	strcpy(str, zero);
}

int main() {
	char line[40], obj_code[82];
	char word[NUM_WORD][WORD_MAX];    /* �ܾ���� ������ ������ �迭 */
	int i, num_words, program_len = 0;
	int locctr = 0;
	char start_address[8];
	char buffer[7];
	int start_address_n=0;
	int error_check[3] = {0,};

	//file open --- fp1: INPUT, fp2: INTER, fp3: OUTPUT
	FILE *fp1, *fp2, *fp3, *fp4;
	fp1 = fopen("TEST1.txt", "r");
	fp2 = fopen("INTR1.txt", "w");

	//PASS 1
	
	//ù �ٿ� START �ִ��� Ȯ��, locctr = if (OPCODE == "START") ? operand : 0 
	fgets(line, 40, fp1);
	num_words = word_split(line, word, error_check);
	if (strcmp(word[1], "START")==0) {
		strcpy(start_address, word[2]);
		//printf("%d", sizeof(word[2]));

		start_address_n = locctr = atoi(word[2]);
	}
	fputs(line, fp2);

	// OPCODE == "END" ���� loop
	while (strcmp(word[1],"END")) {
		
		//�� �� �о�ͼ� �ܾ� ������ �ɰ�
		fgets(line, 40, fp1);
		num_words = word_split(line, word, error_check);

		//LABEL�� Symbol�� �ִ�.
		if (error_check[LABEL] == 1) {
			//symbol table�� �ִٸ� error
		}
		else {
			//symbol table�� {LABEL, LOCCTR} ���·� ����
		}

		//OPCODE�� �ִ�
		if (error_check[OPCODE] == 1) {
			//OPCODE�� OPTABLE�� �ִ� : LOCCTR +=3
			//OPCODE�� "WORD"�� : LOCCTR +=3
			//OPCODE�� "RESW"�� : LOCCTR +=3*operand
			//OPCODE�� "RESB"�� : LOCCTR +=operand
			//OPCODE�� "BYTE"�� : LOCCTR +=����
			//else : error
		}
		//intermediate file�� �ְ� ���� �� �б�
	}
	//���α׷� ����: LOCCTR-���� �ּ�
	program_len = locctr - start_address_n;
	fclose(fp1);
	fclose(fp2);


	//PASS 2
	fp3 = fopen("INTR1.txt", "r");
	fp4 = fopen("OUTPUT1.txt", "w");
	fgets(line, 40, fp3);
	num_words = word_split(line, word, error_check);

	//ù �ٿ� START �ִ��� Ȯ��, locctr = if (OPCODE == "START") ? operand : 0 
	if (strcmp(word[1], "START") == 0) {
		strcpy(start_address, word[2]);
		start_address_n = locctr = atoi(word[2]);
	}

	//Header �ۼ�
	obj_code[0] = 'H'; obj_code[1] = ' ';
	fputs("H ", fp4);
	//program name 
	fputs(strcat(word[0]," "), fp4);

	//�����ּ�
	//�տ� 0ä���
	fill_zero(start_address, start_address_n);
	fputs(strcat(start_address," "), fp4);
	printf("%s", start_address);


	sprintf(buffer, "%d", program_len);
	fill_zero(buffer, program_len);
	fputs(buffer, fp4);
	int checker = 0, k=0,h=0;

	for (i =0,checker = 2+i; i<strlen(word[0]); i++)
		obj_code[checker]=word[0][i];
	obj_code[checker++] = ' ';

	for (i=0, checker +=i ; i < strlen(word[2]); i++)
		obj_code[checker] = start_address[i];
	obj_code[checker++] = ' ';
	/*
	itoa(program_len, buffer, 10);
	for (i = 0, checker += i; i < strlen(buffer); i++)
		obj_code[checker] = buffer[i];
	obj_code[checker] = '\0';
*/
	//Text �ۼ�

	//printf("\n %s",obj_code);
	fclose(fp4);
	//fputs(line, fp4);

	
	return 0;
}