//*****************************************************************************
// ��Ȩ��Ϣ��
// ��Ȩ���� (C) 2024 [�ƿ��]����������Ȩ����
//*****************************************************************************
// �ļ����ƣ�tree.h
// ģ�鹦��˵����
// ��ģ��ʵ���﷨���ڵ�����ɷ����͸����ķ�����ʽ���﷨������
//
// ����ʦ����ѧ-�����ѧԺ-�������ѧ�뼼��-2��
// ���ߣ��ƿ��-20222131035
// ������ڣ�2024��12��3��
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
void merge(Node* x, Node* y) {  // �ϲ��ڵ�
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