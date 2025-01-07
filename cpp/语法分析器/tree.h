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
	Ns.push(x);
	return x;
}
void delNodes() {
	while (!Ns.empty()) {
		delete Ns.top();
		Ns.pop();
	}
}
void trim() {  // �������ջԪ�أ���ȡ����ת�ڵ�ջԪ��
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
void merge(Node* x, Node* y) {  // �ϲ��ڵ�
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