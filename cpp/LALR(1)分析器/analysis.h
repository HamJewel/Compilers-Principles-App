//*****************************************************************************
// ��Ȩ��Ϣ��
// ��Ȩ���� (C) 2024 [�ƿ��]����������Ȩ����
//*****************************************************************************
// �ļ����ƣ�analysis.h
// ģ�鹦��˵����
// ��ģ�鶨��ȫ�ֱ����ͷ������Լ��ؼ������ݽṹ��
//
// ����ʦ����ѧ-�����ѧԺ-�������ѧ�뼼��-2��
// ���ߣ��ƿ��-20222131035
// ������ڣ�2024��12��3��
//*****************************************************************************
#pragma once
#include "fset.h"

// LR(1)����˳��
// ������Ŀ��������DFA������������
vector<LR1ITEM> LR1Items;
vector<LALR1ITEM> LALR1Items;
// LR1��idӳ��LALR1��id
map<int, int> LR2LA;
// LALR1����Ŀ����
int LALR1Cnt;

vector<EDGE> LR1Edges;
vector<EDGE> LALR1Edges;

// ��go������ʹ��
vector<string> nextTokens;

// ��mergeCores������ʹ��
vector<LR1ITEM> sameCores;

//map<pair<int, string>, string> LR1ACTION, LR1GOTO;
map<pair<int, string>, string> ACTION, GOTO;


bool isLR1InItem(int id, const LR1GRAM& A) {
	for (auto& x : LR1Items[id].LRGrams)
		if (x == A) return true;
	return false;
}

int getItemID(const LR1ITEM& A) {
	for (auto& item : LR1Items)
		if (item.LRGrams == A.LRGrams)
			return item.id;
	return -1;
}
// ��CLOSURE(I)
void closure(int id) {
	int lastSize = -1, newSize = LR1Items[id].size();
	while (lastSize != newSize) {  // �������Ŀ���ķ��������ٱ仯��������ѭ��
		lastSize = newSize;
		for (int i = 0; i < newSize; ++i) {
			if (LR1Items[id].LRGrams[i].gram.right.empty()) continue;  // ���û�д����յ�tokens�ˣ�������
			GRAM gram = LR1Items[id].LRGrams[i].gram;  // ����GRAM������¼��ǰ��Ŀ��LR(1)�ķ�[i]���ķ�����
			string newName = gram.right[0];  // ��¼Ҫ���յĵ�һ��
			if (isTerminal(newName)) continue;  // �׸�token���ս���Ͳ�������closure
			for (auto& newGram : Grams) {  // ����ԭ�е�ȫ���ķ�����
				if (newGram.left != newName) continue;  // �����ǰ�ķ����󲿲�����newName������
				// ����һ�����Խ��յ��ս��
				// �����ǰ��token�Ѿ������һ��������token�ˣ��Ǿͼ̳�֮ǰ����һ���ս��
				set<string> terms = gram.right.size() > 1 ?
					First[gram.right[1]] : set<string>({ LR1Items[id].LRGrams[i].term });
				for (auto& newTerm : terms) {  // �����ķ�
					LR1GRAM newLR1({ newGram, newTerm });
					if (isLR1InItem(id, newLR1)) continue;  // �����ǰ�ķ��Ѿ����ڸ���Ŀ������
					LR1Items[id].LRGrams.push_back(newLR1);
				}
			}
		}
		newSize = LR1Items[id].size();  // ���µ�ǰ���ķ�����
	}
}
// LR1Items[id]����token�󣬵õ��µ�newItem=LR1Items[newID]
void go(int id, const string& token) {
	bool flag = false;  // ��ת�Ƿ�ɹ��ı�ʶ��
	int newID = LR1Items.size();  // �����µ�ID
	LR1ITEM newItem({ newID });  // �½�һ��LR(1)��Ŀ
	for (auto& x : LR1Items[id].LRGrams) {  // ���������Ŀ�������ķ�
		// ��������յ�tokensΪ�� ���� ��һ�������յ�token�����ڵ�ǰ��Ŀ��token��������
		if (x.gram.right.empty() || x.gram.right[0] != token) continue;
		flag = true;  // ��ת�ɹ�
		GRAM newGram = x.gram;  // ��¼����
		vector<string> newFormer = x.former;  // ��¼����
		newFormer.push_back(newGram.right[0]);  // ��ԭ�������յ�tokens��һ��token�Ƶ��µ�former��
		newGram.right.erase(newGram.right.begin());  // ɾ�������յ�tokens�е�һ��token
		newItem.LRGrams.push_back({ newGram, x.term , newFormer });  // ���µ�item�м����µ��ķ�
	}
	if (!flag) return;  // ��תʧ�ܣ�ֱ�ӽ���
	LR1Items.push_back(newItem);  // ��������Ŀitem
	closure(newID);  // ���������������Ŀ��CLOSURE
	int tmpID = getItemID(LR1Items[newID]);  // ���µ�item����Ŀ���е�һ�γ���ʱ���±�
	// ����ҵ����±겻�����µ�ID��˵����item�ڽ���token��ɻ���Ȼ��ȥ������¼ӵ�item
	if (newID != tmpID) LR1Items.pop_back();
	LR1Edges.push_back({ id, tmpID, token });  // ��ӱ�����
}

void getNextTokens(int id) {
	nextTokens.clear();
	for (auto& x : LR1Items[id].LRGrams) {
		if (x.gram.right.empty()) continue;
		if (!isInVec(x.gram.right[0], nextTokens))
			nextTokens.push_back(x.gram.right[0]);
	}
}
// ����LR(1)����Ŀ��
void buildLR1Item() {
	LR1Items.push_back({ 0, {{Grams[0], END}} });  // ������Ŀ�ʼ�ķ���ʼ��LR(1)��Ŀ��
	closure(0);  // ��������item��CLOSURE
	int id = 0;
	while (id < LR1Items.size()) {  // ���û��������item��id�ᵽ���ٽ�ֵ������ѭ��
		getNextTokens(id);  // ��ȡItems[id]��һ�ο��Խ��յ�tokens
		for (auto& token : nextTokens)
			go(id, token);  // ��ת����item
		++id;
	}
}

void mergeGram(LALR1GRAM& LA, const LR1GRAM& LR) {
	if (LA.terms.empty()) {
		LA.former = LR.former;
		LA.gram = LR.gram;
	}
	LA.terms.insert(LR.term);
}
// ��LR(1)�ķ�ת��Ϊ�ַ���
string gram2Str(const LR1GRAM& x) {
	string str;
	str += x.gram.left + "->";
	for (auto& y : x.gram.right)
		str += y + ' ';
	str += "former:";
	for (auto& y : x.former)
		str += ' ' + y;
	return str;
}
// �ж�����LR(1)��ĿA��B�Ƿ������ͬ����
bool isSameCore(const LR1ITEM& A, const LR1ITEM& B) {
	set<string> s1, s2;
	for (auto& x : A.LRGrams)
		s1.insert(gram2Str(x));
	for (auto& x : B.LRGrams)
		s2.insert(gram2Str(x));
	return s1 == s2;
}
// ��LR(1)��ĿA�ϲ���ͬ���ĵ��ķ�����ΪLALR(1)��ĿB
void mergeLRToLALRItem(LR1ITEM& A, LALR1ITEM& B) {
	for (auto& x : A.LRGrams) {
		bool flag = true;  // ���B�в�������x������ͬ���ĵ��ķ�������ֱ�Ӱ��ķ�x����B��
		for (auto& y : B.LRGrams) {
			if (x.former != y.former || x.gram != y.gram) continue;  // ����ķ�y�����к�x��ͬ�ĺ��ģ�����
			flag = false;  // x��y�ĺ�����ͬ���Ͳ�Ҫֱ�Ӱ�x����B��(�����ظ�)������ֻ��Ҫ׷���ս��term����
			y.terms.insert(x.term);  // �����µ��ս��
		}
		if (flag) B.LRGrams.push_back({ x.gram, {x.term}, x.former });  // ȫ�µ��ķ����ģ�ֱ�Ӱ��ķ�x����B��
	}
}
// ����ǰ��¼����ͬ���ĵ�LR(1)��Ŀ�ϲ���LALR(1)��Ŀ�������뵽LALR(1)��Ŀ����
void addLALR1Item() {
	LALR1ITEM Item;
	for (auto& item : sameCores) {
		LR2LA[item.id] = LALR1Cnt;
		mergeLRToLALRItem(item, Item);
	}
	Item.id = LALR1Cnt++;
	LALR1Items.push_back(Item);
}
// ���ϱ���LR(1)��Ŀ�����ϲ���ͬ������
void mergeCores() {
	int n = LR1Items.size();
	vector<bool> v(n, false);  // ���ʱ�ǣ�����id�ж���Щitem�Ѿ����ʹ�
	while (true) {
		sameCores.clear();  // �����һ�ε���ͬ�����б�
		for (auto& item : LR1Items) {
			if (v[item.id]) continue;  // ����ѷ��ʣ�������
			if (sameCores.empty()) {  // �����ǰû�м�¼���κ�һ��LR(1)��Ŀ
				sameCores.push_back(item);  // ֱ�Ӱ�item���뵽samCores��
				v[item.id] = true;  // ���±��
				continue;
			}
			if (isSameCore(sameCores[0], item)) {  // �����ǰ��Ŀ�ĺ��ĺ�sameCores�еĵ�һ����ͬ
				sameCores.push_back(item);  // ׷����item��sameCores��
				v[item.id] = true;  // ���±��
			}
		}
		if (sameCores.empty()) return;  // ������е�item�����й��ϲ��������ͽ���
		addLALR1Item();  // ����µ�LALR(1)��Ŀ��LALR(1)��Ŀ����
	}
}

void buildLALR1Edges() {  // ����LALR1��
	// ��ά����v[n][n]��v[x][y]=true˵����Ŀx����Ŀy�ı��Ѿ���¼����
	vector<vector<bool>> v(LALR1Cnt, vector<bool>(LALR1Cnt, false));
	for (auto& e : LR1Edges) {
		// ��ȡ��ǰ��LR(1)�ߵĶ˵���Ŀ��Ӧ�����LALR(1)��Ŀ��id
		int x = LR2LA[e.x], y = LR2LA[e.y];
		if (v[x][y]) continue;  // ����Ѿ���������������
		v[x][y] = true;  // ���±��
		LALR1Edges.push_back({ x, y, e.s });  // ����±�
	}
}

void buildLALR1Item() {
	mergeCores();  // �ϲ���ͬ����
	buildLALR1Edges();  // ȥ�� + �����
}
// ����LALR(1)������
void buildLALR1Table() {
	// ��Լ������������ע�⣬�ȼ�¼��Լ��������ΪLALR1�Դ����ƽ�-��Լ��ͻ����ʱ���Ƚ����ƽ����������Ժ��¼�ƽ����������Ը��ǵ���Լ������
	// ���ĳһLALR(1)�ķ�û�д�����tokens(�Ѿ��������)���Ǿͽ����Լ����
	for (auto& item : LALR1Items) {  // ����ÿһ����Ŀ
		for (auto& LR1 : item.LRGrams) {  // ����ÿһ����Ŀ��ÿһ���ķ�
			if (!LR1.gram.right.empty()) continue;  // ������д����յ�tokens��������
			GRAM gram({ LR1.gram.left, LR1.former });  // ��¼���ķ���Ϣ
			int id = getGramID(gram);  // ��ȡ���ķ���ȫ���ķ�Grams�е��±�ID
			for (auto& term : LR1.terms)  // Ҫ�����пɽ��յ��ս�������й�Լ
				ACTION[{item.id, term}] = 'r' + to_string(id);  // ���±��
		}
	}
	// �ƽ����� or ��ת����
	for (auto& e : LALR1Edges) {  // ����ÿһ����
		if (isTerminal(e.s))  // ������յ�token���ս������Ϊ�ƽ�����
			ACTION[{e.x, e.s}] = 's' + to_string(e.y);  // ���±��
		else   // ������յ�token�Ƿ��ս������Ϊ��ת����
			GOTO[{e.x, e.s}] = to_string(e.y);  // ���±��(��ת����ǰ׺���ñ��)
	}
	// ���ܶ���
	// ���磬���俪ʼ�ķ�[S'->S, $]������λ��LALR1Items[0]�ģ��ڽ���һ��S����ת��LALR1Items[1]
	// ��ʱ��ת��LALR1Items[1]�ٽ���һ��������$��������������ķ��Ĺ�Լ(�൱���ķ���Լ��ͳһ����)
	// ���Ƕ�����һ������Ϊ���ܶ�������"acc"��ʾ
	ACTION[{1, END}] = ACCEPT;
}
