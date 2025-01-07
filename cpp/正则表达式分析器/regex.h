//*****************************************************************************
// 版权信息：
// 版权所有 (C) 2024 [黄俊銓]。保留所有权利。
//*****************************************************************************
// 文件名称：lexical.h
// 模块功能说明：
// 该模块实现DFA最小化和词法分析方法。
//
// 华南师范大学-计算机学院-计算机科学与技术-2班
// 作者：黄俊銓-20222131035
// 完成日期：2024年12月3日
//*****************************************************************************
#pragma once
#include "globals.h"
// 生成子类编码
void genSubCode(string& r) {
    if (r.empty()) return;
    for (auto& x : split(r, '|')) {
        OutRegex += '(' + x + ')' + EXIT + '|';
        replace(x, "\\", "");  // 去除反斜杠转义符
        int k = Cd2Token.size();
        CdPrec[k] = 2;
        Cd2Token[k] = x;
    }
}
// 生成大类编码(实义/无实义)
void genMainCode(string& l, string& r) {
    if (r.empty()) return;
    OutRegex += '(' + r + ')' + EXIT + '|';  //拼接到存储式子的字符串后，用|将式子连接
    bool flag = l[1] == '!';
    int k = Cd2Token.size();
    CdPrec[k] = 1 - flag;
    Cd2Token[k] = l.substr(1 + flag);
    MainCd[flag].push_back(k);
}
// 生成字符集
void genCharSet(string& l, string& r) {
    if (r.size() < 3) return;
    MapStr.push_back(l);
    r = r.substr(1, r.size() - 2);  // 去掉中括号
    replace(r, "\\-", STR(char(-2)));  // 替换转义减号\-
    replace(r, STR(RANGE), STR(char(-1)));  // 替换范围运算符
    replace(r, STR(char(-2)), "-");  // 转义减号\-转为减号-
    int n = r.size(), i = -1;
    vector<bool> v(n);
    // 参与范围运算的字符
    while ((i = r.find(char(-1), i + 1)) != -1) {
        v[i - 1] = v[i] = v[i + 1] = true;
        for (char c = r[i - 1]; c <= r[i + 1]; ++c)
            CharSet[l].insert(c);
    }
    // 补充剩余的字符
    for (i = 0; i < n; ++i)
        if (!v[i]) CharSet[l].insert(r[i]);
}
// 映射处理正则表达式
void mapRegex() {
    // 初始化映射表(-128~-1作为可用映射字符值, 避免冲突)
    for (int i = 0; i < MapStr.size(); ++i) {
        C2S[char(EXIT - 1 - i)] = MapStr[i];
        S2C[MapStr[i]] = char(EXIT - 1 - i);
    }
    for (const auto& x : CharSet)
        CharSetSym.insert(S2C[x.first]);
    // 使用map映射关系替换正则表达式中的符号
    for (const auto& pair : S2C)
        replace(OutRegex, pair.first, STR(pair.second));
    //先处理+号
    for (int i = 0; i < OutRegex.size(); i++) {
        if (OutRegex[i] == '+') {
            int k = 0, j = i;
            do {
                --j;
                if (OutRegex[j] == ')') k++;
                else if (OutRegex[j] == '(') k--;
            } while (k != 0);
            string str1 = OutRegex.substr(0, j);
            string kstr = OutRegex.substr(j, i - j);
            string str2 = OutRegex.substr(i + 1, (OutRegex.size() - i));
            OutRegex = str1 + kstr + kstr + '*' + str2;
        }
    }
    for (int i = 0; i < OutRegex.size() - 1; i++) {
        if ((isChar(OutRegex[i]) && isChar(OutRegex[i + 1]))
            || (isChar(OutRegex[i]) && OutRegex[i + 1] == '(')
            || (OutRegex[i] == ')' && (OutRegex[i + 1] == '(' || isChar(OutRegex[i + 1])))
            || (OutRegex[i] == '*' && (OutRegex[i + 1] == '(' || isChar(OutRegex[i + 1])))
            || (OutRegex[i] == '?' && (OutRegex[i + 1] == '(' || isChar(OutRegex[i + 1])))
            || (OutRegex[i] == '+' && (OutRegex[i + 1] == '(' || isChar(OutRegex[i + 1]))))
        {
            string str1 = OutRegex.substr(0, i + 1);
            string str2 = OutRegex.substr(i + 1, (OutRegex.size() - i));
            OutRegex = str1 + '@' + str2;
        }
    }
}
// 对正则表达式进行处理
void processRegex() {
    // 去除空格和制表符
    replace(InRegex, " ", ""), replace(InRegex, "\t", "");
    // 将文本内容按行分割成一个字符串列表
    vector<string> lines = split(InRegex, '\n');
    // 拼接生成NFA和DFA图的正则表达式
    for (const auto& line : lines) {
        if (line.empty()) continue;
        int k = line.find('=');
        string l = line.substr(0, k); // 式子名称
        string r = line.substr(k + 1);  // 式子内容
        if (l[0] == '_')  // 如果命名前有下划线，则代表该正则表达式需要生成NFA图和DFA图
            l[1] == '_' ? genSubCode(r) : genMainCode(l, r);
        else  // xxx=[a-zA-Z]格式
            genCharSet(l, r);
    }
    OutRegex.pop_back();  // 去除最后一个'|'
    //cout << "拼接后的正则表达式:\n" << outRegex << '\n';
    mapRegex();
    //cout << "处理后的正则表达式:\n" << outRegex << '\n';
}