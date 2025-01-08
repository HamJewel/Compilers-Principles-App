//*****************************************************************************
// 版权信息：
// 版权所有 (C) 2024 [黄俊銓]。保留所有权利。
//*****************************************************************************
// 文件名称：globals.h
// 模块功能说明：
// 该模块定义全局变量和方法，以及关键的数据结构，作为基础头文件。
//
// 华南师范大学-计算机学院-计算机科学与技术-2班
// 作者：黄俊銓-20222131035
// 完成日期：2024年12月3日
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
#define SUFFIX "'"  // 扩充文法的后缀名
#define EMPTY "@"  //空串符
#define END "$"  //终结符
#define ACCEPT "acc"  // 接受动作(这个在LALR(1)分析表的时候使用)
#define OR string(1, char(-2))  // 转义或'|'的映射标识
const string Blank = " \t\n";
vector<string> lines;  // 原始输入文法内容，按行获取
// 单条文法结构
struct GRAM {
	string left;  // 文法规则的左部(都为非终结符)
	vector<string> right;  // 文法规则右部的组成tokens(有先后顺序，通过扫描处理，将右部拆解为若干个独立的tokens)
	// 扫描前 A->aB|c
	// 扫描后 GRAM gram = { "A", {"a", "B", "|", "c"} }

	// 用来判定两个文法是否是相同的(后续会用上)
	bool operator==(const GRAM& other) const {
		return left == other.left && right == other.right;
	}
	// 用来判定两个文法是否是不同的(后续会用上)
	bool operator!=(const GRAM& other) const {
		return left != other.left || right != other.right;
	}
};
// Grams中将消除|或符号和直接左递归
vector<GRAM> Grams;
// LR(1)项目集中的文法结构
struct LR1GRAM {
	GRAM gram;
	string term;  // 终结符
	vector<string> former;  // 已经接受过的token
	bool operator==(const LR1GRAM& other) const {
		return term == other.term && former == other.former && gram == other.gram;
	}
};
// LALR(1)项目集中的文法结构
struct LALR1GRAM {
	GRAM gram;
	set<string> terms;  // 前瞻终结符集合
	vector<string> former;  // 已经接受过的token
};
// LR(1)项目集结构
struct LR1ITEM {
	int id;
	vector<LR1GRAM> LRGrams;
	int size() { return LRGrams.size(); }
};
// LALR(1)项目集结构
struct LALR1ITEM {
	int id;
	vector<LALR1GRAM> LRGrams;
	int size() { return LRGrams.size(); }
};
// DFA的边信息
struct EDGE {
	int x, y;
	string s;
};
// 通过文法的左部来查找文法，不存在返回-1
int getGramID(const string& name) {
	for (int i = 0; i < Grams.size(); ++i)
		if (Grams[i].left == name)
			return i;
	return -1;
}
// 通过文法的左部和右部共同查找
int getGramID(const GRAM& gram) {
	for (int i = 0; i < Grams.size(); ++i)
		if (Grams[i] == gram)
			return i;
	return -1;
}
// 判断是终结符还是非终结符
bool isNonterm(const string& name) {
	return getGramID(name) != -1;
}

bool isTerminal(const string& name) {
	return getGramID(name) == -1;
}
// 判断空串
bool isEmpty(const string& name) {
	return name == EMPTY;
}
// 判断元素x是否在集合A中
bool isInSet(const string& x, const set<string>& A) {
	return A.find(x) != A.end();
}
bool isInVec(const string& x, const vector<string>& A) {
	for (auto& y : A)
		if (x == y) return true;
	return false;
}
// 合并两个集合，右集合的元素(除了except外)都添加到左集合中
void mergeSet(set<string>& A, const set<string>& B, string except = "") {
	for (auto& x : B)
		if (x != except) A.insert(x);
}
// 判断字符是否为' ', '\n', '\t'
bool isBlank(char c) {
	return Blank.find(c) != -1;
}
// 清除前后缀空格
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