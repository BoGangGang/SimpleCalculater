#pragma warning (disable : 4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

								// 전역 변수 및 구조체
char LINE[100];					// txt 파일 한 줄을 읽기 위한 배열
int R[10] = { 0, };				// 레지스터 0~9

typedef struct _instruction {   // instruction
	char OPcode[3];
	char op[3][10];
} Instruction;

								// 사용할 함수
void init();					// LINE 초기화
int HextoDec(char hex[10]);     // 16진수를 10진수로 바꿔주는 함수

void ADD(Instruction a);
void SUB(Instruction a);
void MUL(Instruction a);
void DIV(Instruction a);

void MOV(Instruction a);
void LW(Instruction a);
void SW(Instruction a);
void RST();

int JMP(char hex[10]);
int BEQ(Instruction a, int pc);
int BNE(Instruction a, int pc);
void SLT(Instruction a);

int main(){
	Instruction code[100];
	int pc = 1;
	int jumppc = 1;

	freopen("input.txt", "r", stdin);

	while (1) {
		init();
		gets(LINE);
		if (pc == jumppc) printf("PC : %d\tOPCODE : %s\n", pc, LINE);

		// instruction format
		if (pc <= jumppc) {
			int i = 0;

			char* temp = strtok(LINE, " ");
			strcpy(code[pc].OPcode, temp);
			temp = strtok(NULL, " ");

			while (temp != NULL) {
				strcpy(code[pc].op[i], temp);
				temp = strtok(NULL, " ");
				i++;
			}
		}
		else if (pc > jumppc) { 
			pc = jumppc; 
		}
		
		if (pc == jumppc) {
			// run operator
			if (strncmp(code[pc].OPcode, "EOF", 3) == 0) break;

			else if (strncmp(code[pc].OPcode, "ADD", 3) == 0) { ADD(code[pc]); }
			else if (strncmp(code[pc].OPcode, "SUB", 3) == 0) { SUB(code[pc]); }
			else if (strncmp(code[pc].OPcode, "MUL", 3) == 0) { MUL(code[pc]); }
			else if (strncmp(code[pc].OPcode, "DIV", 3) == 0) { DIV(code[pc]); }

			else if (strncmp(code[pc].OPcode, "MOV", 3) == 0) { MOV(code[pc]); }
			else if (strncmp(code[pc].OPcode, "LW", 2) == 0) { LW(code[pc]); }
			else if (strncmp(code[pc].OPcode, "SW", 2) == 0) { SW(code[pc]); }
			else if (strncmp(code[pc].OPcode, "RST", 3) == 0) { RST(); }

			else if (strncmp(code[pc].OPcode, "JMP", 3) == 0) { jumppc = JMP(code[pc].op[0]); }

			else if (strncmp(code[pc].OPcode, "BEQ", 3) == 0) { jumppc = BEQ(code[pc], pc); }
			else if (strncmp(code[pc].OPcode, "BNE", 3) == 0) { jumppc = BNE(code[pc], pc); }
			else if (strncmp(code[pc].OPcode, "SLT", 3) == 0) { SLT(code[pc]); }

			if (jumppc == pc) { jumppc = jumppc + 1; }
			pc = pc + 1;
		}
		else if (pc != jumppc) { pc = pc + 1; }
	}

	return 0;
}


// 함수 생성
void init() {
	for (int i = 0; i < 100; i++) LINE[i] = '\0';
}

int HextoDec(char hex[10]) {
	int dec = 0;

	for (int i = 2; hex[i] != '\0'; i++)
	{
		if (hex[i] >= 'A' && hex[i] <= 'F')                // hex값이 'A'(65)~'F'(70) 일때
			dec = dec * 16 + hex[i] - 'A' + 10;
		else if (hex[i] >= 'a' && hex[i] <= 'f')           // hex값이 'a'(97)~'f'(102) 일때
			dec = dec * 16 + hex[i] - 'a' + 10;
		else if (hex[i] >= '0' && hex[i] <= '9')           // hex값이 '0'(48)~'9'(57) 일때
			dec = dec * 16 + hex[i] - '0';
	}

	return dec;
}


void ADD(Instruction a) {
	int num[3] = { 0, };
	for (int i = 0; i < 3; i++) {
		num[i] = a.op[i][1] - '0';
	}

	R[num[0]] = R[num[1]] + R[num[2]];

	printf("[ACTION] ADD : R[%d] = R[%d] + R[%d] (%d = %d + %d)\n\n", num[0], num[1], num[2], R[num[0]], R[num[1]], R[num[2]]);
}

void SUB(Instruction a) {
	int num[3] = { 0, };
	for (int i = 0; i < 3; i++) {
		num[i] = a.op[i][1] - '0';
	}

	R[num[0]] = R[num[1]] - R[num[2]];

	printf("[ACTION] SUB : R[%d] = R[%d] - R[%d] (%d = %d - %d)\n\n", num[0], num[1], num[2], R[num[0]], R[num[1]], R[num[2]]);
}

void MUL(Instruction a) {
	int num[3] = { 0, };
	for (int i = 0; i < 3; i++) {
		num[i] = a.op[i][1] - '0';
	}

	R[num[0]] = R[num[1]] * R[num[2]];

	printf("[ACTION] MUL : R[%d] = R[%d] * R[%d] (%d = %d * %d)\n\n", num[0], num[1], num[2], R[num[0]], R[num[1]], R[num[2]]);
}

void DIV(Instruction a) {
	int num[3] = { 0, };
	for (int i = 0; i < 3; i++) {
		num[i] = a.op[i][1] - '0';
	}

	if (R[num[2]] == 0) { 
		printf("[ACTION] ERROR : Can not be divided by zero.\n\n"); 
	} else { 
		R[num[0]] = R[num[1]] / R[num[2]]; 
		printf("[ACTION] DIV : R[%d] = R[%d] / R[%d] (%d = %d / %d)\n\n", num[0], num[1], num[2], R[num[0]], R[num[1]], R[num[2]]);
	}
}

void MOV(Instruction a) {
	int num1, num2;

	num1 = a.op[0][1] - '0';
	num2 = a.op[1][1] - '0';

	R[num1] = R[num2];

	printf("[ACTION] Move : R[%d] = R[%d](%d)\n\n", num1, num2, R[num2]);
}

void LW(Instruction a) {
	int num = a.op[0][1] - '0';
	int dec = HextoDec(a.op[1]);

	R[num] = dec;

	printf("[ACTION] LW : R[%d] = %d(%s)\n\n", num, dec, a.op[1]);
}

void SW(Instruction a) {
	int num = a.op[0][1] - '0';

	printf("[ACTION] SW : R[%d]is %d\n\n", num, R[num]);
}

void RST() {
	for (int i = 0; i < 10; i++) { R[i] = 0; }

	printf("[ACTION] Reset all registers\n\n");
}

int JMP(char hex[10]) {
	int dec = HextoDec(hex);

	printf("[ACTION] Jump to %d\n\n", dec);
	return dec;
}

int BEQ(Instruction a, int pc) {
	int op[2] = { 0, };
	int num = HextoDec(a.op[2]);

	for (int i = 0; i < 2; i++) {
		if (a.op[i][0] == 'r') {
			int temp = a.op[i][1] - '0';
			op[i] = R[temp];
		}
		else if (a.op[i][0] == '0') {
			int temp = HextoDec(a.op[i]);
			op[i] = temp;
		}
	}

	int jump = 0;
	if (op[0] == op[1]) { jump = num; }
	else if (op[0] != op[1]) { jump = pc; }

	printf("[ACTION] Jump to %d\n\n", jump);

	return jump;
}

int BNE(Instruction a, int pc) {
	int op[2] = { 0, };
	int num = HextoDec(a.op[2]);

	for (int i = 0; i < 2; i++) {
		if (a.op[i][0] == 'r') {
			int temp = a.op[i][1] - '0';
			op[i] = R[temp];
		}
		else if (a.op[i][0] == '0') {
			int temp = HextoDec(a.op[i]);
			op[i] = temp;
		}
	}

	int jump = 0;
	if (op[0] != op[1]) { jump = num; }
	else if (op[0] == op[1]) { jump = pc; }

	printf("[ACTION] Jump to %d\n\n", jump);

	return jump;
}

void SLT(Instruction a) {
	int op[2] = { 0, };

	for (int i = 1; i < 3; i++) {
		if (a.op[i][0] == 'r') {
			int temp = a.op[i][1] - '0';
			op[i-1] = R[temp];
		}
		else if (a.op[i][0] == '0') {
			int temp = HextoDec(a.op[i]);
			op[i-1] = temp;
		}
	}

	int num = a.op[0][1] - '0';
	
	if (op[0] < op[1]) { R[num] = 1; }
	else { R[num] = 0; }

	printf("[ACTION] SLT : R[%d] = %d\n\n", num, R[num]);
}