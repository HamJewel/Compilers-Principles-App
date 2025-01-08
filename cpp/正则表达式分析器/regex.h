//*****************************************************************************
// ��Ȩ��Ϣ��
// ��Ȩ���� (C) 2024 [�ƿ��]����������Ȩ����
//*****************************************************************************
// �ļ����ƣ�lexical.h
// ģ�鹦��˵����
// ��ģ��ʵ��DFA��С���ʹʷ�����������
//
// ����ʦ����ѧ-�����ѧԺ-�������ѧ�뼼��-2��
// ���ߣ��ƿ��-20222131035
// ������ڣ�2024��12��3��
//*****************************************************************************
#pragma once
#include "globals.h"
// �����������
void genSubCode(string& r) {
    if (r.empty()) return;
    for (auto& x : split(r, '|')) {
        OutRegex += '(' + x + ')' + EXIT + '|';
        replace(x, "\\", "");  // ȥ����б��ת���
        int k = Cd2Token.size();
        CdPrec[k] = 2;
        Cd2Token[k] = x;
    }
}
// ���ɴ������(ʵ��/��ʵ��)
void genMainCode(string& l, string& r) {
    if (r.empty()) return;
    OutRegex += '(' + r + ')' + EXIT + '|';  //ƴ�ӵ��洢ʽ�ӵ��ַ�������|��ʽ������
    bool flag = l[1] == '!';
    int k = Cd2Token.size();
    CdPrec[k] = 1 - flag;
    Cd2Token[k] = l.substr(1 + flag);
    MainCd[flag].push_back(k);
}
// �����ַ���
void genCharSet(string& l, string& r) {
    if (r.size() < 3) return;
    MapStr.push_back(l);
    r = r.substr(1, r.size() - 2);  // ȥ��������
    replace(r, "\\-", STR(char(-2)));  // �滻ת�����\-
    replace(r, STR(RANGE), STR(char(-1)));  // �滻��Χ�����
    replace(r, STR(char(-2)), "-");  // ת�����\-תΪ����-
    int n = r.size(), i = -1;
    vector<bool> v(n);
    // ���뷶Χ������ַ�
    while ((i = r.find(char(-1), i + 1)) != -1) {
        v[i - 1] = v[i] = v[i + 1] = true;
        for (char c = r[i - 1]; c <= r[i + 1]; ++c)
            CharSet[l].insert(c);
    }
    // ����ʣ����ַ�
    for (i = 0; i < n; ++i)
        if (!v[i]) CharSet[l].insert(r[i]);
}
// ӳ�䴦��������ʽ
void mapRegex() {
    // ��ʼ��ӳ���(-128~-1��Ϊ����ӳ���ַ�ֵ, �����ͻ)
    for (int i = 0; i < MapStr.size(); ++i) {
        C2S[char(EXIT - 1 - i)] = MapStr[i];
        S2C[MapStr[i]] = char(EXIT - 1 - i);
    }
    for (const auto& x : CharSet)
        CharSetSym.insert(S2C[x.first]);
    // ʹ��mapӳ���ϵ�滻������ʽ�еķ���
    for (const auto& pair : S2C)
        replace(OutRegex, pair.first, STR(pair.second));
    //�ȴ���+��
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
// ��������ʽ���д���
void processRegex() {
    // ȥ���ո���Ʊ��
    replace(InRegex, " ", ""), replace(InRegex, "\t", "");
    // ���ı����ݰ��зָ��һ���ַ����б�
    vector<string> lines = split(InRegex, '\n');
    // ƴ������NFA��DFAͼ��������ʽ
    for (const auto& line : lines) {
        if (line.empty()) continue;
        int k = line.find('=');
        string l = line.substr(0, k); // ʽ������
        string r = line.substr(k + 1);  // ʽ������
        if (l[0] == '_')  // �������ǰ���»��ߣ�������������ʽ��Ҫ����NFAͼ��DFAͼ
            l[1] == '_' ? genSubCode(r) : genMainCode(l, r);
        else  // xxx=[a-zA-Z]��ʽ
            genCharSet(l, r);
    }
    OutRegex.pop_back();  // ȥ�����һ��'|'
    //cout << "ƴ�Ӻ��������ʽ:\n" << outRegex << '\n';
    mapRegex();
    //cout << "������������ʽ:\n" << outRegex << '\n';
}