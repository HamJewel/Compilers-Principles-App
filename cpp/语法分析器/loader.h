//*****************************************************************************
// ��Ȩ��Ϣ��
// ��Ȩ���� (C) 2024 [�ƿ��]����������Ȩ����
//*****************************************************************************
// �ļ����ƣ�loader.h
// ģ�鹦��˵����
// ��ģ��ʵ���ķ���������Lex�ļ��ļ��ط�������ȡ��Ҫ���ݡ�
//
// ����ʦ����ѧ-�����ѧԺ-�������ѧ�뼼��-2��
// ���ߣ��ƿ��-20222131035
// ������ڣ�2024��12��3��
//*****************************************************************************
#pragma once
#include "globals.h"
void loadGrams(const string& path) {
	string line, token;
	ifstream file(path);
	if (!file.is_open()) {
		cout << "Error opening grams file\n";
		exit(0);
	}
	while (getline(file, line)) {
		if (line.empty()) continue;
		vector<string> v;
		stringstream ss(line);
		while (getline(ss, token, ' '))  // ��'\t'�ָ�
			v.push_back(token);
		Grams.push_back({ v[0], vector<string>(v.begin() + 1, v.end()) });
	}
	file.close();
}
void loadTable(const string& path) {
	string line;
	ifstream file(path);
	if (!file.is_open()) {
		cout << "Error opening table file\n";
		exit(0);
	}
	while (getline(file, line)) {
		if (line.empty()) continue;
		int i = line.find(' '), j = line.rfind(' ');
		string a = line.substr(0, i), b = line.substr(i + 1, j - i - 1), c = line.substr(j + 1);
		Table[{a, b}] = c;
	}
	file.close();
}
void loadMap(const string& path) {
	string line, cd;
	ifstream file(path);
	if (!file.is_open()) {
		cout << "Error opening map file\n";
		exit(0);
	}
	while (getline(file, line)) {
		if (line.empty()) continue;
		if (cd.empty()) {
			istringstream ss(line);
			while (ss >> cd)
				mainCd.insert(cd);
			continue;
		}
		int i = line.find('\t');
		string k = line.substr(0, i), v = line.substr(i + 1);
		idMap[k] = v;
	}
	file.close();
}
void loadLex(const string& path) {
	string line;
	ifstream file(path);
	if (!file.is_open()) {
		cout << "Error opening lex file\n";
		exit(0);
	}
	while (getline(file, line)) {
		if (line.empty()) continue;
		int i = line.find('\t');
		if (i == -1)
			Lexs.push_back(Lex(line, idMap[line]));
		else
			Lexs.push_back(Lex(line.substr(0, i), line.substr(i + 1)));
	}
	file.close();
}
void getPairs(const string& str, Edge& edge) {
	int k, x, y;
	string pairStr;
	istringstream iss(str);  // ʹ�� stringstream ����������ַ���
	while (iss >> pairStr) {  // ���ո�ָ�ÿһ������
		int k = pairStr.find('-');  // ���Һ�˵�λ��
		if (k != -1) {
			x = stoi(pairStr.substr(0, k));
			y = stoi(pairStr.substr(k + 1));
			edge.pairs.push_back({ x, y });  // �����ֶ���ӵ������
		}
	}
}
void loadTree(const string& path) {
	string line;
	ifstream file(path);
	if (!file.is_open()) {
		cout << "Error opening tree file\n";
		exit(0);
	}
	Edges.resize(Grams.size());
	int i, j, k = 0;
	while (getline(file, line)) {
		if (line.empty()) continue;
		i = line.find(' ');
		j = stoi(line.substr(0, i));
		if (j == -1) {  // -1��ʾ���м��Ժ���ķ��������﷨���ṹ
			while (k < Grams.size())
				Edges[k++].root = 0;
			break;
		}
		Edges[k].root = j;
		getPairs(line.substr(i + 1), Edges[k++]);
	}
	file.close();
}