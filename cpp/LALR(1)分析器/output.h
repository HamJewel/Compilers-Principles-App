//*****************************************************************************
// ��Ȩ��Ϣ��
// ��Ȩ���� (C) 2024 [�ƿ��]����������Ȩ����
//*****************************************************************************
// �ļ����ƣ�output.h
// ģ�鹦��˵����
// ��ģ��ʵ�ָ�����Ҫ��������������
//
// ����ʦ����ѧ-�����ѧԺ-�������ѧ�뼼��-2��
// ���ߣ��ƿ��-20222131035
// ������ڣ�2024��12��3��
//*****************************************************************************
#pragma once
#include "analysis.h"
#include "scan.h"
// ����name��ӡ����First����
void printFirst(string name) {
	cout << name << '\t';
	for (auto& s : First[name])
		cout << s << ' ';
	cout << '\n';
}
// ��ӡȫ��First����
void printFirstAll() {
	for (auto& x : First)
		printFirst(x.first);
}
// ����name��ӡ����Follow����
void printFollow(string name) {
	cout << name << '\t';
	for (auto& s : Follow[name])
		cout << s << ' ';
	cout << '\n';
}
// ��ӡȫ��Follow����
void printFollowAll() {
	for (auto& x : Follow)
		printFollow(x.first);
}
// ����ID��ӡ����LR(1)��Ŀ
void printLR1Item(int id) {
	cout << "--Item[" << id << "]--\n";
	for (auto& x : LR1Items[id].LRGrams) {
		cout << x.gram.left << " ->";
		for (auto& y : x.former)
			cout << ' ' << y;
		cout << "��";
		for (auto& y : x.gram.right)
			cout << ' ' << y;
		cout << ", " << x.term << '\n';
	}
}
// ��ӡLR(1)��Ŀ��
void printLR1ItemAll() {
	for (int i = 0; i < LR1Items.size(); ++i)
		printLR1Item(i);
}
// ��ӡLALR(1)������
void printLALR1Table() {
	cout << "--ACTION��--\n";
	for (auto& x : ACTION)
		cout << x.first.first << '\t' << x.first.second << '\t' << x.second << '\n';
	cout << "--GOTO��--\n";
	for (auto& x : GOTO)
		cout << x.first.first << '\t' << x.first.second << '\t' << x.second << '\n';
}
// ��ӡLALR(1)��Ŀ��
void printLALR1ItemAll() {
	for (auto& item : LALR1Items) {
		cout << "--Item[" << item.id << "]--\n";
		for (auto& x : item.LRGrams) {
			cout << x.gram.left << " ->";
			for (auto& y : x.former)
				cout << ' ' << y;
			cout << "��";
			for (auto& y : x.gram.right)
				cout << ' ' << y;
			cout << ", ";
			string str;
			for (auto& term : x.terms)
				str += term + '/';
			str.pop_back();
			cout << str << '\n';
		}
	}
}
// ��ӡLR(1)DFA�ı�
void printLR1Edges() {
	for (auto& e : LR1Edges)
		cout << e.x << '\t' << e.s << '\t' << e.y << '\n';
}
// ��ӡLALR(1)DFA�ı�
void printLALR1Edges() {
	for (auto& e : LALR1Edges)
		cout << e.x << '\t' << e.s << '\t' << e.y << '\n';
}
void Run() {
	scanGrams();  // ɨ��ȫ���ķ�
	splitGrams();  // ��|����ķ�
	//extendStartGram();  // ���俪ʼ�ķ�
	buildFirstAll();  // ��ȫ��First����
	buildFollowAll();  // ��ȫ��Follow����
	buildLR1Item();  // ����LR(1)��Ŀ��
	buildLALR1Item();  // ����LALR(1)��Ŀ��
	buildLALR1Table();  // ����LALR(1)������
}
void Print() {
	cout << "---First����---\n";
	printFirstAll();
	cout << "---END---\n";
	cout << "---Follow����---\n";
	printFollowAll();
	cout << "---END---\n";
	cout << "---LR(1)��Ŀ��---\n";
	printLR1ItemAll();
	cout << "---END---\n";
	cout << "---LR(1)�߼�---\n";
	printLR1Edges();
	cout << "---END---\n";
	cout << "---LALR(1)��Ŀ��---\n";
	printLALR1ItemAll();
	cout << "---END---\n";
	cout << "---LALR(1)�߼�---\n";
	printLALR1Edges();
	cout << "---END---\n";
	cout << "---LALR(1)������---\n";
	printLALR1Table();
	cout << "---END---\n";
}
// �����ķ���LALR1������
void Export(const string& root) {
	ofstream out(root + "/Grams.txt");
	for (auto& x : Grams) {
		out << x.left;
		for (auto& y : x.right)
			out << ' ' << y;
		out << '\n';
	}
	out = ofstream(root + "/Table.txt");
	for (auto& x : ACTION)
		out << x.first.first << ' ' << x.first.second << ' ' << x.second << '\n';
	for (auto& x : GOTO)
		out << x.first.first << ' ' << x.first.second << ' ' << x.second << '\n';
	out.close();
}