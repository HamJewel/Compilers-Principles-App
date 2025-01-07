//*****************************************************************************
// ��Ȩ��Ϣ��
// ��Ȩ���� (C) 2024 [�ƿ��]����������Ȩ����
//*****************************************************************************
// �ļ����ƣ�scan.h
// ģ�鹦��˵����
// ��ģ��ʵ���ķ���ɨ����������ķ����ݡ�
//
// ����ʦ����ѧ-�����ѧԺ-�������ѧ�뼼��-2��
// ���ߣ��ƿ��-20222131035
// ������ڣ�2024��12��3��
//*****************************************************************************
#pragma once
#include "globals.h"


typedef enum { START, DONE, KEEP, NOTEND, ENDLINE, ENDLINES } State;
// START ��ʼ�����ַ�
// DONE ���һ��Token�Ľ���
// NONTERM ��ǰҪ���յ�Token�Ƿ��ս��
// TERMINAL ��ǰҪ���յ�Token���ս��
// KEEP ���������ַ��������ո�ֹͣ(�ڵڶ�����д��ʽ��ʹ��)
// OR '|'�������ַ�
// NOTEND ��û����ɨ���״̬
// ENDLINE ɨ���굱ǰ�е�״̬
// ENDLINES ɨ����ȫ����lines��״̬

State scanState = START, endState = NOTEND;
int lineID = 0, charID = 0;
GRAM newGram;


int nextChar() {
	if (charID >= lines[lineID].size()) {
		endState = (lineID >= lines.size() - 1 ? ENDLINES : ENDLINE);
		return EOF;
	}
	return lines[lineID][charID++];
}
// ����͵������ֱ�Ӱ�"->"��ߵķǿո��ַ��������ķ�����
void getLeft() {
	int i = lines[lineID].find("->");
	charID = i + 2;
	newGram.left = stripBlank(lines[lineID].substr(0, i));
}
// ��ȡ�ķ��Ҳ�
void getRight() {
	bool save;
	string token;
	scanState = START;  // ��ʼ��ɨ��״̬
	endState = NOTEND;  // ��ʼ������״̬
	while (scanState != DONE) {
		char c = nextChar();
		save = true;  // �����ַ���c׷�ӵ�token��
		switch (scanState) {
		case START:
			if (isBlank(c))
				save = false;
			else if (c == '|')
				scanState = DONE;
			else if (c == EOF)  // һ��ʼ�ͽ�����˵���ǿմ�
				return;
			else
				scanState = KEEP;
			break;
		case KEEP:
			if (isBlank(c) || c == EOF)
				save = false, scanState = DONE;
			break;
		case DONE:
		default: /* should never happen */
			//cout << "Scanner Bug: state = " << scanState << '\n';
			scanState = DONE;
			break;
		}
		if (save) token += c;
	}
	// �����ķ��Ҳ�
	newGram.right.push_back(token);
}
void scanGrams() {
	for (auto& line : lines)  // �滻ת���'|'
		replace(line, "\\|", OR);
	while (endState != ENDLINES) {
		endState = NOTEND;
		getLeft();
		while (endState == NOTEND)
			getRight();
		Grams.push_back(newGram);
		newGram.right.clear();
		++lineID, charID = 0;
	}
}
// ������Ż����ķ�
void splitGrams() {
	vector<GRAM> grams = Grams;  // ��ʱ����
	Grams.clear();  // ���ԭ�����ķ�
	for (auto& gram : grams) {
		vector<string> tokens;  // ��¼��ǰ����ֺ��õ��Ҳ�tokens
		for (auto& x : gram.right) {
			if (x != "|") {  // û����"|"�ͼ�������Ҳ�
				tokens.push_back(x);
				continue;
			}
			Grams.push_back({ gram.left, tokens });  // ����"|"�������ķ�
			tokens.clear();  // ��յ�ǰ�Ҳ���¼��׼����һ�εı���
		}
		// (����ĩβ����)�������ʣ����Ҳ���ֱ�Ӽ��뵽Grams��
		if (!tokens.empty()) Grams.push_back({ gram.left, tokens });
	}
	for (auto& gram : Grams)  // �滻��ת���'|'
		for (auto& x : gram.right)
			if (x == OR) x = "|";
}
// ���俪ʼ�ķ�����A->a������A'->A  (��������ݹ飬�������ķ�)
void extendStartGram() {
	string newName = Grams[0].left;
	while (isNonterm(newName))  // ����������Ѵ��ڣ���һֱ����׷��suf
		newName += SUFFIX;
	Grams.insert(Grams.begin(), { newName, {Grams[0].left} });
}
