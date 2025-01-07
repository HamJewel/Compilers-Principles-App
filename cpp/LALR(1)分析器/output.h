//*****************************************************************************
// 版权信息：
// 版权所有 (C) 2024 [黄俊銓]。保留所有权利。
//*****************************************************************************
// 文件名称：output.h
// 模块功能说明：
// 该模块实现各类重要结果的输出方法。
//
// 华南师范大学-计算机学院-计算机科学与技术-2班
// 作者：黄俊銓-20222131035
// 完成日期：2024年12月3日
//*****************************************************************************
#pragma once
#include "analysis.h"
#include "scan.h"
// 根据name打印单个First集合
void printFirst(string name) {
	cout << name << '\t';
	for (auto& s : First[name])
		cout << s << ' ';
	cout << '\n';
}
// 打印全部First集合
void printFirstAll() {
	for (auto& x : First)
		printFirst(x.first);
}
// 根据name打印单个Follow集合
void printFollow(string name) {
	cout << name << '\t';
	for (auto& s : Follow[name])
		cout << s << ' ';
	cout << '\n';
}
// 打印全部Follow集合
void printFollowAll() {
	for (auto& x : Follow)
		printFollow(x.first);
}
// 根据ID打印单个LR(1)项目
void printLR1Item(int id) {
	cout << "--Item[" << id << "]--\n";
	for (auto& x : LR1Items[id].LRGrams) {
		cout << x.gram.left << " ->";
		for (auto& y : x.former)
			cout << ' ' << y;
		cout << "·";
		for (auto& y : x.gram.right)
			cout << ' ' << y;
		cout << ", " << x.term << '\n';
	}
}
// 打印LR(1)项目集
void printLR1ItemAll() {
	for (int i = 0; i < LR1Items.size(); ++i)
		printLR1Item(i);
}
// 打印LALR(1)分析表
void printLALR1Table() {
	cout << "--ACTION表--\n";
	for (auto& x : ACTION)
		cout << x.first.first << '\t' << x.first.second << '\t' << x.second << '\n';
	cout << "--GOTO表--\n";
	for (auto& x : GOTO)
		cout << x.first.first << '\t' << x.first.second << '\t' << x.second << '\n';
}
// 打印LALR(1)项目集
void printLALR1ItemAll() {
	for (auto& item : LALR1Items) {
		cout << "--Item[" << item.id << "]--\n";
		for (auto& x : item.LRGrams) {
			cout << x.gram.left << " ->";
			for (auto& y : x.former)
				cout << ' ' << y;
			cout << "·";
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
// 打印LR(1)DFA的边
void printLR1Edges() {
	for (auto& e : LR1Edges)
		cout << e.x << '\t' << e.s << '\t' << e.y << '\n';
}
// 打印LALR(1)DFA的边
void printLALR1Edges() {
	for (auto& e : LALR1Edges)
		cout << e.x << '\t' << e.s << '\t' << e.y << '\n';
}
void Run() {
	scanGrams();  // 扫描全部文法
	splitGrams();  // 按|拆分文法
	//extendStartGram();  // 扩充开始文法
	buildFirstAll();  // 求全部First集合
	buildFollowAll();  // 求全部Follow集合
	buildLR1Item();  // 构建LR(1)项目集
	buildLALR1Item();  // 构建LALR(1)项目集
	buildLALR1Table();  // 构建LALR(1)分析表
}
void Print() {
	cout << "---First集合---\n";
	printFirstAll();
	cout << "---END---\n";
	cout << "---Follow集合---\n";
	printFollowAll();
	cout << "---END---\n";
	cout << "---LR(1)项目集---\n";
	printLR1ItemAll();
	cout << "---END---\n";
	cout << "---LR(1)边集---\n";
	printLR1Edges();
	cout << "---END---\n";
	cout << "---LALR(1)项目集---\n";
	printLALR1ItemAll();
	cout << "---END---\n";
	cout << "---LALR(1)边集---\n";
	printLALR1Edges();
	cout << "---END---\n";
	cout << "---LALR(1)分析表---\n";
	printLALR1Table();
	cout << "---END---\n";
}
// 导出文法和LALR1分析表
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