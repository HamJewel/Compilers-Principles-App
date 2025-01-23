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
// 查询分析表
string Lookup(State key) {
	if (Table.find(key) != Table.end()) return Table[key];
	key.token = EMPTY;  // 查询能否接收空串
	return 'e' + Table[key];
}
// 当前所在状态, LALR1接收的token, 动作+id
void shift() {  // 移进至状态[id](直接接收新输入)
	isEmpty = mv[0] == 'e';
	Lex lex = isEmpty ? Lex{ "空串", EMPTY } : Lexs[lexi++];  // 特判能接收空串的情况
	string s = mv.substr(1 + isEmpty);
	Lc.push_back(lex);
	Lc.push_back(Lex{ "状态", s });
	Node* x = newNode(lex.str(false));
	Nb.push_back(x);  // 生成树节点
	cout << "移进状态" << s << '\n';
}
// 用文法[id]归约(归约跳转)
void reduce() {
	Gx = stoi(mv.substr(1)), Gy = Grams[Gx], Gn = Gy.r.size();
	Lc.erase(Lc.end() - 2 * Gn, Lc.end());
	id = Lc.back().token();
	mv = Lookup(State{ id, Gy.l });
	Lc.push_back(Lex{ "产生式", Gy.l });
	Lc.push_back(Lex{ "状态", mv });  // 跳转至新状态;
	genNode();
	cout << "用" << Gy.str() << "归约\n";
}
// 用文法[0]归约(归约接受)
void accept() {
	Gx = 0, Gy = Grams[Gx], Gn = Gy.r.size();
	Root = Nb[0];
	Root->token = "start";
	++lexi;
	cout << "用" << Gy.str() << "归约(接受)\n";
}
void parse() {
	Lc.push_back(Lex{ "状态", "0" });  // 初始状态
	int n = Lexs.size();
	while (lexi < n) {
		id = Lc.back().token();
		in = Lexs[lexi].token();
		mv = Lookup(State{ id, in });
		cout << "[步骤" << ++step << "]\n";
		cout << "分析栈:";
		for (auto& x : Lc)
			cout << ' ' << x.str(false);
		cout << '\n';
		cout << "输入: " << Lexs[lexi].str(true) << '\n';
		switch (mv[0]) {
		case 'e':
		case 's': shift(); break;  // 移进
		case 'r': reduce(); break;  // 归约
		case 'a': accept(); break;  // 接受 
		default:
			cout << "未知动作: " << mv << '\n';
			exit(1);
		}
	}
}