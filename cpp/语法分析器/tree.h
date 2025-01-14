//*****************************************************************************
// 版权信息：
// 版权所有 (C) 2024 [黄俊銓]。保留所有权利。
//*****************************************************************************
// 文件名称：tree.h
// 模块功能说明：
// 该模块实现语法树节点的生成方法和各个文法产生式的语法动作。
//
// 华南师范大学-计算机学院-计算机科学与技术-2班
// 作者：黄俊銓-20222131035
// 完成日期：2024年12月3日
//*****************************************************************************
#pragma once
#include "globals.h" 
Node* newNode(string str = "") {
	Node* x = new Node(str);
	Na.push_back(x);
	return x;
}
void delNodes() {
	for (auto& x : Na)
		delete x;
}
void trim() {
	Nc = { newNode() };
	Nc.insert(Nc.end(), Nb.end() - Gn, Nb.end());
	Nb.erase(Nb.end() - Gn, Nb.end());
}
void merge(Node* x, Node* y) {  // 合并节点
	if (y->token.empty()) x->kids.insert(x->kids.end(), y->kids.begin(), y->kids.end());
	else x->kids.push_back(y);
}
void genNode() {
	trim();
	for (const auto& p : Edges[Gx].pairs)
		merge(Nc[p.x], Nc[p.y]);
	Nb.push_back(Nc[Edges[Gx].root]);
}
void _printTree(int n, Node* x) {
	cout << string(n, ' ') << x->token << '\n';
	for (auto& y : x->kids)
		_printTree(n + 1, y);
}
void printTree() {
	_printTree(0, Root);
}