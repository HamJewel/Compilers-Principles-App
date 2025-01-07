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
	Ns.push(x);
	return x;
}
void delNodes() {
	while (!Ns.empty()) {
		delete Ns.top();
		Ns.pop();
	}
}
void trim() {  // 整理分析栈元素，获取并反转节点栈元素
	Nk.clear();
	int n = Gy.r.size();
	Nk.resize(n + 1);
	Nk[0] = newNode();
	Lc.erase(Lc.end() - 2 * n, Lc.end());
	while (n) {
		Nk[n--] = Nc.top();
		Nc.pop();
	}
}
void merge(Node* x, Node* y) {  // 合并节点
	if (y->token.empty()) x->kids.insert(x->kids.end(), y->kids.begin(), y->kids.end());
	else x->kids.push_back(y);
}
Node* genNode() {
	trim();
	for (const auto& p : Edges[Gx].pairs)
		merge(Nk[p.x], Nk[p.y]);
	return Nk[Edges[Gx].root];
}
void _printTree(int n, Node* x) {
	cout << string(n, ' ') << x->token << '\n';
	for (auto& y : x->kids)
		_printTree(n + 1, y);
}
void printTree() {
	_printTree(0, Root);
}