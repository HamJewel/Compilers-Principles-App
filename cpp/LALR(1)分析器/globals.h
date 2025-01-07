//*****************************************************************************
// ��Ȩ��Ϣ��
// ��Ȩ���� (C) 2024 [�ƿ��]����������Ȩ����
//*****************************************************************************
// �ļ����ƣ�globals.h
// ģ�鹦��˵����
// ��ģ�鶨��ȫ�ֱ����ͷ������Լ��ؼ������ݽṹ����Ϊ����ͷ�ļ���
//
// ����ʦ����ѧ-�����ѧԺ-�������ѧ�뼼��-2��
// ���ߣ��ƿ��-20222131035
// ������ڣ�2024��12��3��
//*****************************************************************************
#pragma once
#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<set>
#include<map>
#include<unordered_map>
using namespace std;
#define SUFFIX "'"  // �����ķ��ĺ�׺��
#define EMPTY "@"  //�մ���
#define END "$"  //�ս��
#define ACCEPT "acc"  // ���ܶ���(�����LALR(1)�������ʱ��ʹ��)
#define OR string(1, char(-2))  // ת���'|'��ӳ���ʶ
const string Blank = " \t\n";
vector<string> lines;  // ԭʼ�����ķ����ݣ����л�ȡ
// �����ķ��ṹ
struct GRAM {
	string left;  // �ķ��������(��Ϊ���ս��)
	vector<string> right;  // �ķ������Ҳ������tokens(���Ⱥ�˳��ͨ��ɨ�账�����Ҳ����Ϊ���ɸ�������tokens)
	// ɨ��ǰ A->aB|c
	// ɨ��� GRAM gram = { "A", {"a", "B", "|", "c"} }

	// �����ж������ķ��Ƿ�����ͬ��(����������)
	bool operator==(const GRAM& other) const {
		return left == other.left && right == other.right;
	}
	// �����ж������ķ��Ƿ��ǲ�ͬ��(����������)
	bool operator!=(const GRAM& other) const {
		return left != other.left || right != other.right;
	}
};
// Grams�н�����|����ź�ֱ����ݹ�
vector<GRAM> Grams;
// LR(1)��Ŀ���е��ķ��ṹ
struct LR1GRAM {
	GRAM gram;
	string term;  // �ս��
	vector<string> former;  // �Ѿ����ܹ���token
	bool operator==(const LR1GRAM& other) const {
		return term == other.term && former == other.former && gram == other.gram;
	}
};
// LALR(1)��Ŀ���е��ķ��ṹ
struct LALR1GRAM {
	GRAM gram;
	set<string> terms;  // ǰհ�ս������
	vector<string> former;  // �Ѿ����ܹ���token
};
// LR(1)��Ŀ���ṹ
struct LR1ITEM {
	int id;
	vector<LR1GRAM> LRGrams;
	int size() { return LRGrams.size(); }
};
// LALR(1)��Ŀ���ṹ
struct LALR1ITEM {
	int id;
	vector<LALR1GRAM> LRGrams;
	int size() { return LRGrams.size(); }
};
// DFA�ı���Ϣ
struct EDGE {
	int x, y;
	string s;
};
// ͨ���ķ������������ķ��������ڷ���-1
int getGramID(const string& name) {
	for (int i = 0; i < Grams.size(); ++i)
		if (Grams[i].left == name)
			return i;
	return -1;
}
// ͨ���ķ����󲿺��Ҳ���ͬ����
int getGramID(const GRAM& gram) {
	for (int i = 0; i < Grams.size(); ++i)
		if (Grams[i] == gram)
			return i;
	return -1;
}
// �ж����ս�����Ƿ��ս��
bool isNonterm(const string& name) {
	return getGramID(name) != -1;
}

bool isTerminal(const string& name) {
	return getGramID(name) == -1;
}
// �жϿմ�
bool isEmpty(const string& name) {
	return name == EMPTY;
}
// �ж�Ԫ��x�Ƿ��ڼ���A��
bool isInSet(const string& x, const set<string>& A) {
	return A.find(x) != A.end();
}
bool isInVec(const string& x, const vector<string>& A) {
	for (auto& y : A)
		if (x == y) return true;
	return false;
}
// �ϲ��������ϣ��Ҽ��ϵ�Ԫ��(����except��)����ӵ��󼯺���
void mergeSet(set<string>& A, const set<string>& B, string except = "") {
	for (auto& x : B)
		if (x != except) A.insert(x);
}
// �ж��ַ��Ƿ�Ϊ' ', '\n', '\t'
bool isBlank(char c) {
	return Blank.find(c) != -1;
}
// ���ǰ��׺�ո�
string stripBlank(const string& s) {
	int i = s.find_first_not_of(Blank);
	int j = s.find_last_not_of(Blank);
	return i == -1 ? "" : s.substr(i, j - i + 1);
}
void replace(string& str, const string& x, const string& y) {
	int i = 0;
	while ((i = str.find(x, i)) != -1) {
		str.replace(i, x.size(), y);
		i += y.size();
	}
}