//*****************************************************************************
// ��Ȩ��Ϣ��
// ��Ȩ���� (C) 2024 [�ƿ��]����������Ȩ����
//*****************************************************************************
// �ļ����ƣ�globals.h
// ģ�鹦��˵����
// ��ģ��ʵ�ָ���ؼ����ݽṹ��������Ҫȫ�ֱ����ͷ�����
//
// ����ʦ����ѧ-�����ѧԺ-�������ѧ�뼼��-2��
// ���ߣ��ƿ��-20222131035
// ������ڣ�2024��12��3��
//*****************************************************************************
#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <set>
#include <map>
using namespace std;
#define EMPTY "@"
#define END "$"
#define ACCEPT "acc"
set<string> mainCd;  // �������
map<string, string> idMap;
map<string, string> tkMap{ {"(", "()"}, {")", "()"}, {"[", "[]"}, {"]", "[]"}, {"{", "{}"}, {"}", "{}"} };
struct Pair {
	int x, y;  // x: ���ڵ��� y: �ӽڵ���
};
struct Edge {
	int root;
	vector<Pair> pairs;
};
struct Node {
	string token;
	vector<Node*> kids;
	Node(string str = "") : token(tkMap.find(str) != tkMap.end() ? tkMap[str] : str) {}
};
struct Gram {
	string l;
	vector<string> r;
	string str() {
		string s = l + " ��";
		for (auto& x : r)
			s += ' ' + x;
		return s;
	}
};
struct Lex {  // �ʷ�
	string k, v;
	string token() {  // ��LALR1����ʱ�Ľ���token
		return mainCd.find(k) != mainCd.end() ? idMap[k] : v;
	}  // ��������v�ǵ������ݣ���Ҫȡ�䵥������
	string str(bool full) {
		string s = k == "״̬" ? '[' + v + ']' : v;
		return full ? '<' + k + ' ' + s + '>' : s;
	}  // ��ӡ���õ��ַ���
};
struct State {
	string id, token;
	// ���� operator< ������Hash�Ƚ�
	bool operator<(const State& other) const {
		return id != other.id ? stoi(id) < stoi(other.id) : token < other.token;
	}
};
bool isEmpty;  // �Ƿ���տմ�
int lexi, step;  // ��ǰ��ȡ�Ĵʷ��±� ��ǰ�﷨�����Ĳ�����
string id, in, mv;  // ״̬��ţ�����token������
int Gx, Gn;  // ��ǰ��ԼҪ�õ��ķ�����ʽ�±ꡢ�ķ��Ҳ�token��
Gram Gy;  // ��ǰ��ԼҪ�õ��ķ�����ʽ
Node* Root;  // �﷨�����ڵ�
vector<Node*> Na, Nb, Nc;  // �﷨���ڵ��¼
vector<Lex> Lc;  // �������ջ
vector<Lex> Lexs;  // ��ȡ��Lex�ļ��е����д���
vector<Gram> Grams;  // �����ķ�
vector<Edge> Edges;  // �﷨���ı�
map<State, string> Table;  // ������