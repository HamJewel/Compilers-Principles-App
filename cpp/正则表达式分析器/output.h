//*****************************************************************************
// 版权信息：
// 版权所有 (C) 2024 [黄俊銓]。保留所有权利。
//*****************************************************************************
// 文件名称：output.h
// 模块功能说明：
// 该模块实现对重要结果的打印和导出方法。
//
// 华南师范大学-计算机学院-计算机科学与技术-2班
// 作者：黄俊銓-20222131035
// 完成日期：2024年12月3日
//*****************************************************************************
#pragma once
#include "autom.h"
#include "regex.h"
#include "lexical.h"
void Run() {
    processRegex();  // 处理正则表达式
    regex2NFA();  //正则表达式转NFA
    NFA2DFA();  // NFA转DFA
    minimizeDFA();  // 最小化DFA
    genNFA(), genDFA(), genMinDFA(), genLexer();
    delNFAs();
}
void Print() {
    string s;
    cout << "---NFA---\n";
    for (const auto& x : NFATable) {
        for (const auto& y : x)
            s += y + '\t';
        s.pop_back();
        s += '\n';
    }
    cout << s;
    s.clear();
    cout << "---DFA---\n";
    for (const auto& x : DFATable) {
        for (const auto& y : x)
            s += y + '\t';
        s.pop_back();
        s += '\n';
    }
    cout << s;
    s.clear();
    cout << "---MinDFA---\n";
    for (const auto& x : MinDFATable) {
        for (const auto& y : x)
            s += y + '\t';
        s.pop_back();
        s += '\n';
    }
    cout << s;
}
// 导出词法分析源程序和单词编码映射表
void Export(const string& path) {
    ofstream out1(path + "/Lexer.c"), out2(path + "/Map.txt");
    out1 << LexCode;
    for (int i = 0; i < MainCd[0].size(); ++i)
        out2 << MainCd[0][i] << (i < MainCd[0].size() - 1 ? ' ' : '\n');
    for (const auto& p : Cd2Token)
        out2 << p.first << '\t' << p.second << '\n';
    out1.close(), out2.close();
}
