//*****************************************************************************
// 版权信息：
// 版权所有 (C) 2024 [黄俊銓]。保留所有权利。
//*****************************************************************************
// 文件名称：globals.h
// 模块功能说明：
// 该模块实现各类关键数据结构，定义重要全局变量和方法。
//
// 华南师范大学-计算机学院-计算机科学与技术-2班
// 作者：黄俊銓-20222131035
// 完成日期：2024年12月3日
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
set<string> mainCd;  // 大类编码
map<string, string> idMap;
map<string, string> tkMap{ {"(", "()"}, {")", "()"}, {"[", "[]"}, {"]", "[]"}, {"{", "{}"}, {"}", "{}"} };
struct Pair {
	int x, y;  // x: 父节点编号 y: 子节点编号
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
		string s = l + " →";
		for (auto& x : r)
			s += ' ' + x;
		return s;
	}
};
struct Lex {  // 词法
	string k, v;
	string token() {  // 在LALR1分析时的接收token
		return mainCd.find(k) != mainCd.end() ? idMap[k] : v;
	}  // 大类编码的v是单词内容，需要取其单词名称
	string str(bool full) {
		string s = k == "状态" ? '[' + v + ']' : v;
		return full ? '<' + k + ' ' + s + '>' : s;
	}  // 打印所用的字符串
};
struct State {
	string id, token;
	// 重载 operator< 来进行Hash比较
	bool operator<(const State& other) const {
		return id != other.id ? stoi(id) < stoi(other.id) : token < other.token;
	}
};
bool isEmpty;  // 是否接收空串
int lexi, step;  // 当前读取的词法下标 当前语法分析的步骤数
string id, in, mv;  // 状态编号，输入token，动作
int Gx, Gn;  // 当前归约要用的文法产生式下标、文法右部token数
Gram Gy;  // 当前归约要用的文法产生式
Node* Root;  // 语法树根节点
vector<Node*> Na, Nb, Nc;  // 语法树节点记录
vector<Lex> Lc;  // 词组分析栈
vector<Lex> Lexs;  // 读取的Lex文件中的所有词组
vector<Gram> Grams;  // 所有文法
vector<Edge> Edges;  // 语法树的边
map<State, string> Table;  // 分析表