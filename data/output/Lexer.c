#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#define MaxCd 34
int n, m;
bool flag;
char c, str[128];
int curr[MaxCd], next[MaxCd];
FILE* input, * output;
struct State { int cd, pr; }
S[] = {
	{0, 0}, {1, 1}, {2, 1}, {3, 2}, {4, 2}, 
	{5, 2}, {6, 2}, {7, 2}, {8, 2}, {9, 2}, 
	{10, 2}, {11, 2}, {12, 2}, {13, 2}, {14, 2}, 
	{15, 2}, {16, 2}, {17, 2}, {18, 2}, {19, 2}, 
	{20, 2}, {21, 2}, {22, 2}, {23, 2}, {24, 2}, 
	{25, 2}, {26, 2}, {27, 2}, {28, 2}, {29, 2}, 
	{30, 2}, {31, 2}, {32, 2}, {33, 0}, {0, 0}, 
	{0, 0}, {2, 1}, {33, 0}, {0, 0}, {0, 0}, 
	{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, 
	{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, 
	{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, 
	{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, 
	{0, 0}, {0, 0}, {0, 0}
};
struct Entry { char c; int id; }
E0[] = { {'*', 14}, {')', 28}, {'(', 27}, {'+', 12}, {'!', 34}, {'%', 16}, {',', 26}, {'-', 13}, {'/', 15}, {';', 25}, {'<', 18}, {'=', 24}, {'>', 21}, {'[', 29}, {']', 30}, {'^', 17}, {'_', 1}, {'d', 38}, {'e', 46}, {'f', 57}, {'i', 43}, {'r', 42}, {'v', 48}, {'w', 44}, {'{', 31}, {'}', 32} },
E1[] = { {'_', 1} },
E2[] = { {'.', 35} },
E10[] = { {'u', 40} },
E15[] = { {'/', 33} },
E18[] = { {'=', 19} },
E21[] = { {'=', 20} },
E24[] = { {'=', 22} },
E33[] = { {'\n', 37} },
E34[] = { {'=', 23} },
E38[] = { {'o', 10} },
E39[] = { {'a', 62} },
E40[] = { {'b', 60} },
E41[] = { {'d', 9} },
E42[] = { {'e', 59} },
E43[] = { {'f', 4}, {'n', 56} },
E44[] = { {'h', 50} },
E45[] = { {'i', 41} },
E46[] = { {'l', 52} },
E47[] = { {'n', 8} },
E48[] = { {'o', 45} },
E49[] = { {'e', 3} },
E50[] = { {'i', 51} },
E51[] = { {'l', 53} },
E52[] = { {'s', 49} },
E53[] = { {'e', 11} },
E54[] = { {'o', 39} },
E55[] = { {'r', 47} },
E56[] = { {'t', 5} },
E57[] = { {'l', 54} },
E58[] = { {'e', 7} },
E59[] = { {'t', 61} },
E60[] = { {'l', 58} },
E61[] = { {'u', 55} },
E62[] = { {'t', 6} };
void append() {
	size_t len = strlen(str);
	str[len] = c, str[len + 1] = '\0';
}
int get_cd() {
	int cd = 0, pr = -1;
	for (int i = 0; i < n; ++i)
		if (S[curr[i]].pr > pr)
			cd = S[curr[i]].cd, pr = S[curr[i]].pr;
	return cd;
}
bool has_end(int id) {
	for (int i = 0; i < n; ++i)
		if (curr[i] != id && S[curr[i]].cd > 0)
			return true;
	return false;
}
int get_entry(struct Entry* e, int n) {
	for (int i = 0; i < n; ++i)
		if (e[i].c == c) return e[i].id;
	return -1;
}
bool is_digit() {
	return (c >= '0' && c <= '9');
}
bool is_letter() {
	return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}
void new_state() {
	str[0] = '\0';
	n = 1, curr[0] = 0;
}
void end_state() {
	int cd = get_cd();
	switch (cd) {
	case 0: return;
	case 33: break;
	case 1: fprintf(output, "%d\t%s\n", cd, str); break;
	case 2: fprintf(output, "%d\t%s\n", cd, str); break;
	default: fprintf(output, "%d\n", cd);
	}
	ungetc(c, input);
	n = 0;
}
bool next_state(int id) {
	int k = m, x = -1;
	switch (id) {
	case 0:
		if (is_digit()) next[m++] = 2;
		if (is_letter()) next[m++] = 1;
		x = get_entry(E0, 26);
		break;
	case 1:
		if (is_digit()) next[m++] = 1;
		if (is_letter()) next[m++] = 1;
		x = get_entry(E1, 1);
		break;
	case 2:
		if (is_digit()) next[m++] = 2;
		x = get_entry(E2, 1);
		break;
	case 10:
		x = get_entry(E10, 1);
		break;
	case 15:
		x = get_entry(E15, 1);
		break;
	case 18:
		x = get_entry(E18, 1);
		break;
	case 21:
		x = get_entry(E21, 1);
		break;
	case 24:
		x = get_entry(E24, 1);
		break;
	case 33:
		if (has_end(id)) return false;
		next[m++] = 33;
		x = get_entry(E33, 1);
		break;
	case 34:
		x = get_entry(E34, 1);
		break;
	case 35:
		if (is_digit()) next[m++] = 36;
		break;
	case 36:
		if (is_digit()) next[m++] = 36;
		break;
	case 38:
		x = get_entry(E38, 1);
		break;
	case 39:
		x = get_entry(E39, 1);
		break;
	case 40:
		x = get_entry(E40, 1);
		break;
	case 41:
		x = get_entry(E41, 1);
		break;
	case 42:
		x = get_entry(E42, 1);
		break;
	case 43:
		x = get_entry(E43, 2);
		break;
	case 44:
		x = get_entry(E44, 1);
		break;
	case 45:
		x = get_entry(E45, 1);
		break;
	case 46:
		x = get_entry(E46, 1);
		break;
	case 47:
		x = get_entry(E47, 1);
		break;
	case 48:
		x = get_entry(E48, 1);
		break;
	case 49:
		x = get_entry(E49, 1);
		break;
	case 50:
		x = get_entry(E50, 1);
		break;
	case 51:
		x = get_entry(E51, 1);
		break;
	case 52:
		x = get_entry(E52, 1);
		break;
	case 53:
		x = get_entry(E53, 1);
		break;
	case 54:
		x = get_entry(E54, 1);
		break;
	case 55:
		x = get_entry(E55, 1);
		break;
	case 56:
		x = get_entry(E56, 1);
		break;
	case 57:
		x = get_entry(E57, 1);
		break;
	case 58:
		x = get_entry(E58, 1);
		break;
	case 59:
		x = get_entry(E59, 1);
		break;
	case 60:
		x = get_entry(E60, 1);
		break;
	case 61:
		x = get_entry(E61, 1);
		break;
	case 62:
		x = get_entry(E62, 1);
		break;
	}
	if (x != -1) next[m++] = x;
	return k != m;
}
void coding() {
	while (c = fgetc(input)) {
		if (n == 0) new_state();
		m = 0, flag = false;
		for (int i = 0; i < n; ++i)
			flag |= next_state(curr[i]);
		flag ? append() : end_state();
		n = m;
		for (int i = 0; i < n; ++i)
			curr[i] = next[i];
		if (c == EOF) break;
	}
	fprintf(output, "0");
}
int main(int argc, char* argv[]) {
	if (argc < 3) exit(1);
	input = fopen(argv[1], "r");
	if (input == NULL) {
		printf("Failed to open input file");
		exit(1);
	}
	output = fopen(argv[2], "w");
	if (output == NULL) {
		printf("Failed to open output file");
		fclose(input);
		exit(1);
	}
	coding();
	fclose(input);
	fclose(output);
	return 0;
}