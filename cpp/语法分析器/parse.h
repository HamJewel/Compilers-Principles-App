//*****************************************************************************
// 版权信息：
// 版权所有 (C) 2024 [黄俊銓]。保留所有权利。
//*****************************************************************************
// 文件名称：parse.h
// 模块功能说明：
// 该模块实现对Lex进行语法分析的动作和打印分析过程。
//
// 华南师范大学-计算机学院-计算机科学与技术-2班
// 作者：黄俊銓-20222131035
// 完成日期：2024年12月3日
//*****************************************************************************
#pragma once
#include "tree.h"
// 当前所在状态, LALR1接收的token, 动作+id
void shift() {  // 移进至状态[id](直接接收新输入)
	isEmpty = mv[0] == 'e';
	Lex lex = isEmpty ? Lex("空串", EMPTY) : Lexs[lexi++];  // 特判能接收空串的情况
	string s = mv.substr(1 + isEmpty);
	Lc.push_back(lex);
	Lc.push_back(Lex("状态", s));
	Node* x = newNode(lex.str(false));
	Nc.push(x);  // 生成树节点
	cout << "移进状态" << s << '\n';
}
// 用文法[id]归约(归约跳转)
void reduce() {
	Gx = stoi(mv.substr(1));
	Gy = Grams[Gx];
	Node* x = genNode();
	Nc.push(x);
	id = Lc.back().token();
	mv = Lookup(State(id, Gy.l));
	Lc.push_back(Lex("产生式", Gy.l));
	Lc.push_back(Lex("状态", mv));  // 跳转至新状态;
	cout << "用" << Gy.str() << "归约\n";
}
// 用文法[0]归约(归约接受)
void accept() {
	Gx = 0;
	Gy = Grams[Gx];
	Root = genNode();
	Root->token = "start";
	Lc.clear();
	Lc.push_back(Lex("产生式", Gy.l));
	++lexi;
	cout << "用" << Gy.str() << "归约(接受)\n";
}
void parse() {
	Lc.push_back(Lex("状态", "0"));  // 初始状态
	while (lexi < Lexs.size()) {
		id = Lc.back().token();
		in = Lexs[lexi].token();
		mv = Lookup(State(id, in));
		cout << "[步骤" << ++step << "]\n";
		cout << "分析栈: ";
		for (auto& x : Lc)
			cout << x.str(false) << ' ';
		cout << '\n';
		cout << "输入: " << Lexs[lexi].str(true) << '\n';
		switch (mv[0]) {
		case 'e':
		case 's':  // 移进
			shift(); break;
		case 'r':  // 归约
			reduce(); break;
		case 'a':  // 接受 
			accept(); break;
		default:
			cout << "未知动作: " << mv << '\n';
			exit(1);
		}
	}
}