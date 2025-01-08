//*****************************************************************************
// ��Ȩ��Ϣ��
// ��Ȩ���� (C) 2024 [�ƿ��]����������Ȩ����
//*****************************************************************************
// �ļ����ƣ�globals.h
// ģ�鹦��˵����
// ��ģ�鶨����Ҫȫ�ֱ�����ʵ�ֳ��÷�������Ϊ��������ͷ�ļ���
//
// ����ʦ����ѧ-�����ѧԺ-�������ѧ�뼼��-2��
// ���ߣ��ƿ��-20222131035
// ������ڣ�2024��12��3��
//*****************************************************************************
#pragma once
#include <iostream>
#include <map>
#include <vector>
#include <queue>
#include <stack>
#include <set>
#include <algorithm>
#include <string>
#include <sstream>
#include <fstream>
using namespace std;
/*==================================������ʽ����==================================*/
// ���ַ�
#define EMPTY char(0)
// �����ַ�
#define ANY '~'
// �ս��
#define END '$'
// ���������(������Ҫ���������ʽ/���ʺ�)
#define EXIT char(-1)
// ��Χ�����(���ַ�����ʽ��ʹ��)
#define RANGE '-'
// charתstring
#define STR(x) string(1, x)
// �ַ�����ӳ���(xxx=[a-zA-Z])
map<string, set<char>> CharSet;
// �ַ����ϵ�ӳ��(��ʶ)������
set<char> CharSetSym;
// ��Ҫӳ���char��string����
vector<string> MapStr{ "\\+", "\\|", "\\(", "\\)", "\\*", "\\?", "\\n", string("\\") + ANY};
// ����map��ֹ�ַ���ͻ
map<char, string> C2S;
map<string, char> S2C;
// ���ʱ���ӳ��Token
map<int, string> Cd2Token{ {0, STR(END)} };
// �������(��ʵ��0 ��ʵ��1)
vector<int> MainCd[2];
// ���ʱ������ȼ�(����:2 > ʵ�����:1 > ��ʵ�����:0)
map<int, int> CdPrec;
// ������ʽ��������ȼ�
map<char, int> OpPrec{ {'|', 1}, {'@', 2}, {'*', 3}, {'?', 3} };
// NFA��������
int NfaCnt = 0;
// token������(0Ϊ�ս��$)
int CdCnt = 1;
// ȫ���ַ�ͳ��
set<char> Chars;
// �����������ʽ
string InRegex;
// ����NFA��DFA��������ʽ
string OutRegex;
// �ʷ������������
string LexCode;
// �������
vector<vector<string>> NFATable, DFATable, MinDFATable;
/*==================================������ʽתNFA==================================*/ 
struct NFA; // ������һ��NFAͼ�Ľ��Ľṹ�壬�����ʱ��Ҫ�õ�
//�ṹ�壬NFAͼ�ı�
struct Edge {
    char c;
    NFA* next;
};
//�ṹ�壬NFAͼ�Ľ��
struct NFA {
    int id = -1; // ���Ψһ���
    int cd = 0;  // ����̬���õ�token��Ӧ�ĵ��ʱ���(0�������ս��$)
    bool isStart = false;   // ��̬��ʶ
    bool isEnd = false;  // ��̬��ʶ
    bool isExit = false;  //���ڱ�ʶ
    vector<Edge> edges;  // �ߣ���vector��Ϊ�п���һ������ж����߿���
    NFA() : id(NfaCnt++) {}
};
// NFA��ʼ�ڵ��
struct NFAP {
    NFA* start;
    NFA* end;
    NFAP(NFA* s = nullptr, NFA* e = nullptr) :start(s), end(e) {}
};
// ���ٹ���NFA�ڵ�
stack<NFA*> NFAs;
// ���յ�NFA
NFAP NFAp;
// ״̬ת���������
struct State {
    int id = -1; // Ψһidֵ
    string flag;  // ��ǳ�̬������̬
    map<char, set<int>> m;  // ��Ӧ�ַ��ܵ����״̬
};
// ״̬ת����
map<int, State> StateTable;
// stateTable����˳���¼������������
vector<int> InsertOrder;
set<int> StartNFA;
set<int> EndNFA;
map<int, int> ExitNFA;  // idӳ��cd
/*============================NFAתDFA==================================*/
struct DFA {
    int id = -1;  // ���
    int cd = 0;
    string flag; // �Ƿ������̬��+�����̬��-��
    set<int> states; // ��DFA״̬������NFA״̬�ļ���
    map<char, set<int>> trans; // �ַ�����һ״̬��ӳ��
};
// DFA����
int DfaCnt;
// DFA״̬ȥ�ؼ�
set<set<int>> DfaStateSet;
// DFA��NFA״̬��ӳ��Ϊ���
map<set<int>, int> DfaSetMap;
// DFA���ս��
vector<DFA> DfaList;
//��������DFA��С��
// DFA��̬����
set<int> DfaEndState;
// DFA����̬����
set<int> DfaNotEndState;
/*============================��С��DFA==================================*/
struct MDFA {
    int id = -1;
    int cd = 0;
    string flag; // �Ƿ������̬��+�����̬��-��
    map<char, int> trans; // �ַ�����һ״̬��ӳ��
};
// ��СDFA�ĳ�̬���
int MDfaStart;
// ��СDFA����
int MDfaCnt;
vector<MDFA> MDfaList;
// ���ڷָ��
vector<set<int>> DivideVec;
// ���±�
map<int, int> Dfa2MDfa;
/*============================ͨ�ú�������==================================*/
// ���ַ��з��ַ���
vector<string> split(const string& str, const char& delimiter) {
    vector<string> result;
    string token;
    istringstream tokenStream(str);
    while (getline(tokenStream, token, delimiter))
        result.push_back(token);
    return result;
}
//ȥ��ǰ���õ�ת���'\'�����ڽ��չʾ
string trim(string str) {
    if (str.empty() || str == "\\n") return str;
    str.erase(0, str.find_first_not_of('\\'));
    str.erase(str.find_last_not_of('\\') + 1);
    return str;
}
//setתstring�����ڽ��չʾ
string set2string(const set<int>& s) {
    string result;
    for (const auto& i : s) {
        result.append(to_string(i));
        result += ',';
    }
    if (result.size() != 0)
        result.pop_back(); //�������һ������
    return result;
}
string escape(const char& c) {
    switch (c) {
    case('\n'): return "\\n";
    case('\\'): return "\\\\";
    case('\''): return "\\\'";
    case('\"'): return "\\\"";
    default: return STR(c);
    }
}
// �ַ����Ӵ��滻
void replace(string& str, const string& x, const string& y) {
    int i = 0;
    while ((i = str.find(x, i)) != -1) {
        str.replace(i, x.size(), y);
        i += y.size();
    }
}
//�ж��ǲ����ַ�������ǰ���Ѿ��Գ�����������ַ�������ϣ����ĸת��������һ���������������ַ�
bool isChar(const char& c) {
    return !(c == '+' || c == '|' || c == '*' || c == '('
        || c == ')' || c == '?' || c == '@' || c == EXIT);
}
// �������������ַ��ͷ����������ַ�
vector<vector<char>> sortCharSet(const set<char>& s) {
    char l = *s.begin(), r = l - 1;
    vector<vector<char>> v(2);
    for (const auto& c : s) {
        if (c == r + 1) ++r;
        else {
            if (l == r) v[1].push_back(l);
            else v[0].push_back(l), v[0].push_back(r);
            l = r = c;
        }
    }
    if (l == r) v[1].push_back(l);
    else v[0].push_back(l), v[0].push_back(r);
    return v;
}