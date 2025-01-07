//*****************************************************************************
// 版权信息：
// 版权所有 (C) 2024 [黄俊銓]。保留所有权利。
//*****************************************************************************
// 文件名称：loader.h
// 模块功能说明：
// 该模块实现文法、分析表、Lex文件的加载方法，读取必要数据。
//
// 华南师范大学-计算机学院-计算机科学与技术-2班
// 作者：黄俊銓-20222131035
// 完成日期：2024年12月3日
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
		while (getline(ss, token, ' '))  // 按'\t'分割
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
	istringstream iss(str);  // 使用 stringstream 来逐个处理字符串
	while (iss >> pairStr) {  // 按空格分隔每一对数字
		int k = pairStr.find('-');  // 查找横杆的位置
		if (k != -1) {
			x = stoi(pairStr.substr(0, k));
			y = stoi(pairStr.substr(k + 1));
			edge.pairs.push_back({ x, y });  // 将数字对添加到结果中
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
		if (j == -1) {  // -1表示该行及以后的文法不定义语法树结构
			while (k < Grams.size())
				Edges[k++].root = 0;
			break;
		}
		Edges[k].root = j;
		getPairs(line.substr(i + 1), Edges[k++]);
	}
	file.close();
}