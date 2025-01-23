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
#include "tree.h"
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
	Lc.push_back(lex);
	Lc.push_back(Lex{ "״̬", s });
	Node* x = newNode(lex.str(false));
	Nb.push_back(x);  // �������ڵ�
	cout << "�ƽ�״̬" << s << '\n';
}
// ���ķ�[id]��Լ(��Լ��ת)
void reduce() {
	Gx = stoi(mv.substr(1)), Gy = Grams[Gx], Gn = Gy.r.size();
	Lc.erase(Lc.end() - 2 * Gn, Lc.end());
	id = Lc.back().token();
	mv = Lookup(State{ id, Gy.l });
	Lc.push_back(Lex{ "����ʽ", Gy.l });
	Lc.push_back(Lex{ "״̬", mv });  // ��ת����״̬;
	genNode();
	cout << "��" << Gy.str() << "��Լ\n";
}
// ���ķ�[0]��Լ(��Լ����)
void accept() {
	Gx = 0, Gy = Grams[Gx], Gn = Gy.r.size();
	Root = Nb[0];
	Root->token = "start";
	++lexi;
	cout << "��" << Gy.str() << "��Լ(����)\n";
}
void parse() {
	Lc.push_back(Lex{ "״̬", "0" });  // ��ʼ״̬
	int n = Lexs.size();
	while (lexi < n) {
		id = Lc.back().token();
		in = Lexs[lexi].token();
		mv = Lookup(State{ id, in });
		cout << "[����" << ++step << "]\n";
		cout << "����ջ:";
		for (auto& x : Lc)
			cout << ' ' << x.str(false);
		cout << '\n';
		cout << "����: " << Lexs[lexi].str(true) << '\n';
		switch (mv[0]) {
		case 'e':
		case 's': shift(); break;  // �ƽ�
		case 'r': reduce(); break;  // ��Լ
		case 'a': accept(); break;  // ���� 
		default:
			cout << "δ֪����: " << mv << '\n';
			exit(1);
		}
	}
}