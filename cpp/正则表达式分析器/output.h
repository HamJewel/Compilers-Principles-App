//*****************************************************************************
// ��Ȩ��Ϣ��
// ��Ȩ���� (C) 2024 [�ƿ��]����������Ȩ����
//*****************************************************************************
// �ļ����ƣ�output.h
// ģ�鹦��˵����
// ��ģ��ʵ�ֶ���Ҫ����Ĵ�ӡ�͵���������
//
// ����ʦ����ѧ-�����ѧԺ-�������ѧ�뼼��-2��
// ���ߣ��ƿ��-20222131035
// ������ڣ�2024��12��3��
//*****************************************************************************
#pragma once
#include "autom.h"
#include "regex.h"
#include "lexical.h"
void Run() {
    processRegex();  // ����������ʽ
    regex2NFA();  //������ʽתNFA
    NFA2DFA();  // NFAתDFA
    minimizeDFA();  // ��С��DFA
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
// �����ʷ�����Դ����͵��ʱ���ӳ���
void Export(const string& path) {
    ofstream out1(path + "/Lexer.c"), out2(path + "/Map.txt");
    out1 << LexCode;
    for (int i = 0; i < MainCd[0].size(); ++i)
        out2 << MainCd[0][i] << (i < MainCd[0].size() - 1 ? ' ' : '\n');
    for (const auto& p : Cd2Token)
        out2 << p.first << '\t' << p.second << '\n';
    out1.close(), out2.close();
}
