//*****************************************************************************
// 版权信息：
// 版权所有 (C) 2024 [黄俊銓]。保留所有权利。
//*****************************************************************************
// 文件名称：globals.h
// 模块功能说明：
// 该模块定义重要全局变量和实现常用方法，作为基础引用头文件。
//
// 华南师范大学-计算机学院-计算机科学与技术-2班
// 作者：黄俊銓-20222131035
// 完成日期：2024年12月3日
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
/*==================================正则表达式处理==================================*/
// 空字符
#define EMPTY char(0)
// 任意字符
#define ANY '~'
// 终结符
#define END '$'
// 出口运算符(加在需要编码的正则式/单词后)
#define EXIT char(-1)
// 范围运算符(在字符集合式中使用)
#define RANGE '-'
// char转string
#define STR(x) string(1, x)
// 字符集合映射表(xxx=[a-zA-Z])
map<string, set<char>> CharSet;
// 字符集合的映射(标识)符集合
set<char> CharSetSym;
// 需要映射成char的string类型
vector<string> MapStr{ "\\+", "\\|", "\\(", "\\)", "\\*", "\\?", "\\n", string("\\") + ANY};
// 下面map防止字符冲突
map<char, string> C2S;
map<string, char> S2C;
// 单词编码映射Token
map<int, string> Cd2Token{ {0, STR(END)} };
// 大类编码(有实义0 无实义1)
vector<int> MainCd[2];
// 单词编码优先级(子类:2 > 实义大类:1 > 无实义大类:0)
map<int, int> CdPrec;
// 正则表达式运算符优先级
map<char, int> OpPrec{ {'|', 1}, {'@', 2}, {'*', 3}, {'?', 3} };
// NFA结点计数器
int NfaCnt = 0;
// token计数器(0为终结符$)
int CdCnt = 1;
// 全局字符统计
set<char> Chars;
// 输入的正则表达式
string InRegex;
// 生成NFA和DFA的正则表达式
string OutRegex;
// 词法分析程序代码
string LexCode;
// 表格数据
vector<vector<string>> NFATable, DFATable, MinDFATable;
/*==================================正则表达式转NFA==================================*/ 
struct NFA; // 先声明一下NFA图的结点的结构体，定义边时需要用到
//结构体，NFA图的边
struct Edge {
    char c;
    NFA* next;
};
//结构体，NFA图的结点
struct NFA {
    int id = -1; // 结点唯一编号
    int cd = 0;  // 出终态后获得的token对应的单词编码(0保留给终结符$)
    bool isStart = false;   // 初态标识
    bool isEnd = false;  // 终态标识
    bool isExit = false;  //出口标识
    vector<Edge> edges;  // 边，用vector因为有可能一个结点有多条边可走
    NFA() : id(NfaCnt++) {}
};
// NFA起始节点对
struct NFAP {
    NFA* start;
    NFA* end;
    NFAP(NFA* s = nullptr, NFA* e = nullptr) :start(s), end(e) {}
};
// 开辟过的NFA节点
stack<NFA*> NFAs;
// 最终的NFA
NFAP NFAp;
// 状态转换表单个结点
struct State {
    int id = -1; // 唯一id值
    string flag;  // 标记初态还是终态
    map<char, set<int>> m;  // 对应字符能到达的状态
};
// 状态转换表
map<int, State> StateTable;
// stateTable插入顺序记录，方便后续输出
vector<int> InsertOrder;
set<int> StartNFA;
set<int> EndNFA;
map<int, int> ExitNFA;  // id映射cd
/*============================NFA转DFA==================================*/
struct DFA {
    int id = -1;  // 编号
    int cd = 0;
    string flag; // 是否包含终态（+）或初态（-）
    set<int> states; // 该DFA状态包含的NFA状态的集合
    map<char, set<int>> trans; // 字符到下一状态的映射
};
// DFA数量
int DfaCnt;
// DFA状态去重集
set<set<int>> DfaStateSet;
// DFA的NFA状态集映射为编号
map<set<int>, int> DfaSetMap;
// DFA最终结果
vector<DFA> DfaList;
//下面用于DFA最小化
// DFA终态集合
set<int> DfaEndState;
// DFA非终态集合
set<int> DfaNotEndState;
/*============================最小化DFA==================================*/
struct MDFA {
    int id = -1;
    int cd = 0;
    string flag; // 是否包含终态（+）或初态（-）
    map<char, int> trans; // 字符到下一状态的映射
};
// 最小DFA的初态编号
int MDfaStart;
// 最小DFA数量
int MDfaCnt;
vector<MDFA> MDfaList;
// 用于分割集合
vector<set<int>> DivideVec;
// 存下标
map<int, int> Dfa2MDfa;
/*============================通用函数方法==================================*/
// 按字符切分字符串
vector<string> split(const string& str, const char& delimiter) {
    vector<string> result;
    string token;
    istringstream tokenStream(str);
    while (getline(tokenStream, token, delimiter))
        result.push_back(token);
    return result;
}
//去除前后置的转义符'\'，用于结果展示
string trim(string str) {
    if (str.empty() || str == "\\n") return str;
    str.erase(0, str.find_first_not_of('\\'));
    str.erase(str.find_last_not_of('\\') + 1);
    return str;
}
//set转string，用于结果展示
string set2string(const set<int>& s) {
    string result;
    for (const auto& i : s) {
        result.append(to_string(i));
        result += ',';
    }
    if (result.size() != 0)
        result.pop_back(); //弹出最后一个逗号
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
// 字符串子串替换
void replace(string& str, const string& x, const string& y) {
    int i = 0;
    while ((i = str.find(x, i)) != -1) {
        str.replace(i, x.size(), y);
        i += y.size();
    }
}
//判断是不是字符，由于前面已经对出现在下面的字符进行了希腊字母转换，所以一定不会出现下面的字符
bool isChar(const char& c) {
    return !(c == '+' || c == '|' || c == '*' || c == '('
        || c == ')' || c == '?' || c == '@' || c == EXIT);
}
// 分类连续区间字符和非连续区间字符
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