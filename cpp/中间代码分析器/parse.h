//*****************************************************************************
// ��Ȩ��Ϣ��
// ��Ȩ���� (C) 2024 [�ƿ��]����������Ȩ����
//*****************************************************************************
// �ļ����ƣ�parse.h
// ģ�鹦��˵����
// ��ģ��ʵ�ֶ�Lex�����﷨�����Ķ����ʹ�ӡ�������̡�
//
// ����ʦ����ѧ-�����ѧԺ-�������ѧ�뼼��-2��
// ���ߣ��ƿ��-20222131035
// ������ڣ�2024��12��3��
//*****************************************************************************
#pragma once
#include "interm.h"
// ����Lex�ļ�
void loadLex(const string& path) {
	string line;
	ifstream file(path);
	if (!file.is_open()) {
		cout << "Error opening lex file\n";
		exit(0);
	}
	while (getline(file, line)) {
		if (line.empty()) continue;
		int i = line.find('\t');
		if (i == -1)
			Lexs.push_back(Lex{ line, idMap[line] });
		else
			Lexs.push_back(Lex{ line.substr(0, i), line.substr(i + 1) });
	}
	file.close();
}
// ��ѯ������
string Lookup(State key) {
	if (Table.find(key) != Table.end()) return Table[key];
	key.token = EMPTY;  // ��ѯ�ܷ���տմ�
	return 'e' + Table[key];
}
// ��ǰ����״̬, LALR1���յ�token, ����+id
void shift() {  // �ƽ���״̬[id](ֱ�ӽ���������)
	isEmpty = mv[0] == 'e';
	Lex lex = isEmpty ? Lex{ "�մ�", EMPTY } : Lexs[lexi++];  // �����ܽ��տմ������
	string s = mv.substr(1 + isEmpty);
	Ta.push_back(lex.v);
	Lc.push_back(lex);
	Lc.push_back(Lex{ "״̬", s });
}
// ���ķ�[id]��Լ(��Լ��ת)
void reduce() {
	Gx = stoi(mv.substr(1)), Gy = Grams[Gx], Gn = Gy.r.size();
	Lc.erase(Lc.end() - 2 * Gn, Lc.end());
	id = Lc.back().token();
	mv = Lookup(State{ id, Gy.l });
	Lc.push_back(Lex{ "����ʽ", Gy.l });
	Lc.push_back(Lex{ "״̬", mv });  // ��ת����״̬;
	genToken();
}
// ���ķ�[0]��Լ(��Լ����)
void accept() {
	Gx = 0, Gy = Grams[Gx], Gn = Gy.r.size();
	++lexi;
	genToken();
}
void parse() {
	Lc.push_back(Lex{ "״̬", "0" });  // ��ʼ״̬
	int n = Lexs.size();
	while (lexi < n) {
		id = Lc.back().token();
		in = Lexs[lexi].token();
		mv = Lookup(State{ id, in });
		switch (mv[0]) {
		case 'e':
		case 's':  // �ƽ�
			shift(); break;
		case 'r':  // ��Լ
			reduce(); break;
		case 'a':  // ���� 
			accept(); break;
		default:
			cout << "δ֪����: " << mv << '\n';
			exit(1);
		}
	}
}