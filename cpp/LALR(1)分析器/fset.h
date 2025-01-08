//*****************************************************************************
// ��Ȩ��Ϣ��
// ��Ȩ���� (C) 2024 [�ƿ��]����������Ȩ����
//*****************************************************************************
// �ļ����ƣ�fset.h
// ģ�鹦��˵����
// ��ģ��ʵ��First��Follow���ϵļ��㡣
//
// ����ʦ����ѧ-�����ѧԺ-�������ѧ�뼼��-2��
// ���ߣ��ƿ��-20222131035
// ������ڣ�2024��12��3��
//*****************************************************************************
#pragma once
#include "globals.h"

// First��Follow����
unordered_map<string, set<string>> First, Follow;

// ��ȡȫ��First���ϵĴ�С�ܺ�
int FirstAllSize() {
	int size = 0;
	for (auto& x : First)
		size += x.second.size();
	return size;
}
// ��������ȫ��First����
void buildFirstAll() {
	// �������ս����First����(��һ�������٣����������©)
	for (auto& gram : Grams)
		for (auto& token : gram.right)
			if (isTerminal(token)) First[token] = { token };  // �ս����First����ֻ������

	int lastSize = -1, newSize = 0;
	while (lastSize != newSize) {
		for (auto& gram : Grams) {
			bool flag = false;  // ��־�����жϵ�ǰ�Ҳ���token��First(token)�Ƿ����@��������ڣ��͵ü�����������Ҳ�
			for (auto& token : gram.right) {  // �����ķ����Ҳ�
				mergeSet(First[gram.left], First[token], EMPTY);  // ��First[token] - @�ϲ���First[��]
				flag = isInSet(EMPTY, First[token]);  // ���@��First[token]�������������
				if (!flag) break;  // ���@����First[token]��ֹͣ����
			}
			if (flag) First[gram.left].insert(EMPTY);  // ����������Ҳ���flag����true,˵��@��First[�Ҳ�]
		}
		lastSize = newSize, newSize = FirstAllSize();  // ����ȫ��First���ϵĴ�С
	}
}
// ��ȡȫ��Follow���ϵĴ�С�ܺ�
int FollowAllSize() {
	int size = 0;
	for (auto& x : Follow)
		size += x.second.size();
	return size;
}
// ��������ȫ��Follow����
void buildFollowAll() {
	int lastSize = -1, newSize = 0;
	Follow[Grams[0].left] = { END };  // ��ʼ��(Ĭ�ϵ�һ���ķ�����Ϊ��ʼ����)
	newSize = FollowAllSize();  // ��ǰȫ��Follow���ϵĴ�С�ܺ�
	while (lastSize != newSize) {  // ���ȫ��Follow���ϵĴ�С���ٱ仯������������
		for (auto& x : Grams) {  // ����ÿһ���ķ�����
			for (int i = 0; i < x.right.size(); ++i) {  // �����Ҳ���ÿһ��token
				if (isTerminal(x.right[i])) continue;  // ֻ�з��ս������Follow����
				// �ж�@�Ƿ�����First(Xi+1Xi+2...Xn)
				// ����Ҫ��x.tokens[i]�Ƿ�Ϊ���Ҷ˵�token������
				bool flag = (i == x.right.size() - 1);
				for (int j = i + 1; j < x.right.size(); ++j) {  // ����token[i]���tokens
					mergeSet(Follow[x.right[i]], First[x.right[j]], EMPTY);
					flag = isInSet(EMPTY, First[x.right[j]]);  // �ж�@�Ƿ���First[Xj]��
					if (!flag) break;
				}
				if (flag) mergeSet(Follow[x.right[i]], Follow[x.left]);  // ��Follow[A]�ϲ���Follow[Xi]��
			}
		}
		lastSize = newSize, newSize = FollowAllSize();  // �����¾�Follow���ϴ�С���ܺ�
	}
}
