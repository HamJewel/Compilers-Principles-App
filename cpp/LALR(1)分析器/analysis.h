//*****************************************************************************
// 版权信息：
// 版权所有 (C) 2024 [黄俊銓]。保留所有权利。
//*****************************************************************************
// 文件名称：analysis.h
// 模块功能说明：
// 该模块定义全局变量和方法，以及关键的数据结构。
//
// 华南师范大学-计算机学院-计算机科学与技术-2班
// 作者：黄俊銓-20222131035
// 完成日期：2024年12月3日
//*****************************************************************************
#pragma once
#include "fset.h"

// LR(1)构建顺序：
// 构建项目集→构建DFA→构建分析表
vector<LR1ITEM> LR1Items;
vector<LALR1ITEM> LALR1Items;
// LR1的id映射LALR1的id
map<int, int> LR2LA;
// LALR1的项目数量
int LALR1Cnt;

vector<EDGE> LR1Edges;
vector<EDGE> LALR1Edges;

// 在go函数中使用
vector<string> nextTokens;

// 在mergeCores函数中使用
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
// 求CLOSURE(I)
void closure(int id) {
	int lastSize = -1, newSize = LR1Items[id].size();
	while (lastSize != newSize) {  // 如果该项目的文法数量不再变化，就跳出循环
		lastSize = newSize;
		for (int i = 0; i < newSize; ++i) {
			if (LR1Items[id].LRGrams[i].gram.right.empty()) continue;  // 如果没有待接收的tokens了，就跳过
			GRAM gram = LR1Items[id].LRGrams[i].gram;  // 创建GRAM变量记录当前项目的LR(1)文法[i]的文法规则
			string newName = gram.right[0];  // 记录要接收的第一个
			if (isTerminal(newName)) continue;  // 首个token是终结符就不用再求closure
			for (auto& newGram : Grams) {  // 遍历原有的全部文法规则
				if (newGram.left != newName) continue;  // 如果当前文法的左部不等于newName，跳过
				// 求下一个可以接收的终结符
				// 如果当前的token已经是最后一个待接收token了，那就继承之前的下一个终结符
				set<string> terms = gram.right.size() > 1 ?
					First[gram.right[1]] : set<string>({ LR1Items[id].LRGrams[i].term });
				for (auto& newTerm : terms) {  // 新增文法
					LR1GRAM newLR1({ newGram, newTerm });
					if (isLR1InItem(id, newLR1)) continue;  // 如果当前文法已经存在该项目，跳过
					LR1Items[id].LRGrams.push_back(newLR1);
				}
			}
		}
		newSize = LR1Items[id].size();  // 更新当前的文法数量
	}
}
// LR1Items[id]接收token后，得到新的newItem=LR1Items[newID]
void go(int id, const string& token) {
	bool flag = false;  // 跳转是否成功的标识符
	int newID = LR1Items.size();  // 创建新的ID
	LR1ITEM newItem({ newID });  // 新建一个LR(1)项目
	for (auto& x : LR1Items[id].LRGrams) {  // 遍历起点项目的所有文法
		// 如果待接收的tokens为空 或者 第一个待接收的token不等于当前的目标token，则跳过
		if (x.gram.right.empty() || x.gram.right[0] != token) continue;
		flag = true;  // 跳转成功
		GRAM newGram = x.gram;  // 记录变量
		vector<string> newFormer = x.former;  // 记录变量
		newFormer.push_back(newGram.right[0]);  // 把原来待接收的tokens第一个token移到新的former中
		newGram.right.erase(newGram.right.begin());  // 删除待接收的tokens中第一个token
		newItem.LRGrams.push_back({ newGram, x.term , newFormer });  // 向新的item中加入新的文法
	}
	if (!flag) return;  // 跳转失败，直接结束
	LR1Items.push_back(newItem);  // 加入新项目item
	closure(newID);  // 紧接着求这个新项目的CLOSURE
	int tmpID = getItemID(LR1Items[newID]);  // 求新的item在项目集中第一次出现时的下标
	// 如果找到的下标不等于新的ID，说明该item在接收token后成环，然后去掉这个新加的item
	if (newID != tmpID) LR1Items.pop_back();
	LR1Edges.push_back({ id, tmpID, token });  // 添加边数据
}

void getNextTokens(int id) {
	nextTokens.clear();
	for (auto& x : LR1Items[id].LRGrams) {
		if (x.gram.right.empty()) continue;
		if (!isInVec(x.gram.right[0], nextTokens))
			nextTokens.push_back(x.gram.right[0]);
	}
}
// 构建LR(1)的项目集
void buildLR1Item() {
	LR1Items.push_back({ 0, {{Grams[0], END}} });  // 用扩充的开始文法初始化LR(1)项目集
	closure(0);  // 立即求新item的CLOSURE
	int id = 0;
	while (id < LR1Items.size()) {  // 如果没有新增的item，id会到达临界值而跳出循环
		getNextTokens(id);  // 获取Items[id]下一次可以接收的tokens
		for (auto& token : nextTokens)
			go(id, token);  // 跳转到新item
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
// 将LR(1)文法转换为字符串
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
// 判断两个LR(1)项目A和B是否具有相同核心
bool isSameCore(const LR1ITEM& A, const LR1ITEM& B) {
	set<string> s1, s2;
	for (auto& x : A.LRGrams)
		s1.insert(gram2Str(x));
	for (auto& x : B.LRGrams)
		s2.insert(gram2Str(x));
	return s1 == s2;
}
// 对LR(1)项目A合并相同核心的文法，成为LALR(1)项目B
void mergeLRToLALRItem(LR1ITEM& A, LALR1ITEM& B) {
	for (auto& x : A.LRGrams) {
		bool flag = true;  // 如果B中不存在与x具有相同核心的文法，可以直接把文法x加入B中
		for (auto& y : B.LRGrams) {
			if (x.former != y.former || x.gram != y.gram) continue;  // 如果文法y不具有和x相同的核心，跳过
			flag = false;  // x和y的核心相同，就不要直接把x加入B中(避免重复)，我们只需要追加终结符term就行
			y.terms.insert(x.term);  // 加入新的终结符
		}
		if (flag) B.LRGrams.push_back({ x.gram, {x.term}, x.former });  // 全新的文法核心，直接把文法x加入B中
	}
}
// 将当前记录的相同核心的LR(1)项目合并成LALR(1)项目，并加入到LALR(1)项目集中
void addLALR1Item() {
	LALR1ITEM Item;
	for (auto& item : sameCores) {
		LR2LA[item.id] = LALR1Cnt;
		mergeLRToLALRItem(item, Item);
	}
	Item.id = LALR1Cnt++;
	LALR1Items.push_back(Item);
}
// 不断遍历LR(1)项目集，合并相同核心项
void mergeCores() {
	int n = LR1Items.size();
	vector<bool> v(n, false);  // 访问标记，根据id判断哪些item已经访问过
	while (true) {
		sameCores.clear();  // 清空上一次的相同核心列表
		for (auto& item : LR1Items) {
			if (v[item.id]) continue;  // 如果已访问，就跳过
			if (sameCores.empty()) {  // 如果当前没有记录过任何一个LR(1)项目
				sameCores.push_back(item);  // 直接把item加入到samCores中
				v[item.id] = true;  // 更新标记
				continue;
			}
			if (isSameCore(sameCores[0], item)) {  // 如果当前项目的核心和sameCores中的第一个相同
				sameCores.push_back(item);  // 追加新item到sameCores中
				v[item.id] = true;  // 更新标记
			}
		}
		if (sameCores.empty()) return;  // 如果所有的item都进行过合并操作，就结束
		addLALR1Item();  // 添加新的LALR(1)项目到LALR(1)项目集中
	}
}

void buildLALR1Edges() {  // 构造LALR1边
	// 二维数组v[n][n]，v[x][y]=true说明项目x到项目y的边已经记录过了
	vector<vector<bool>> v(LALR1Cnt, vector<bool>(LALR1Cnt, false));
	for (auto& e : LR1Edges) {
		// 获取当前的LR(1)边的端点项目对应的起点LALR(1)项目的id
		int x = LR2LA[e.x], y = LR2LA[e.y];
		if (v[x][y]) continue;  // 如果已经遍历过，则跳过
		v[x][y] = true;  // 更新标记
		LALR1Edges.push_back({ x, y, e.s });  // 添加新边
	}
}

void buildLALR1Item() {
	mergeCores();  // 合并相同核心
	buildLALR1Edges();  // 去重 + 构造边
}
// 构建LALR(1)分析表
void buildLALR1Table() {
	// 归约动作（！！！注意，先记录归约动作，因为LALR1仍存在移进-归约冲突，此时优先进行移进动作，所以后记录移进动作，可以覆盖掉归约动作）
	// 如果某一LALR(1)文法没有待接收tokens(已经接收完毕)，那就进入归约动作
	for (auto& item : LALR1Items) {  // 遍历每一个项目
		for (auto& LR1 : item.LRGrams) {  // 遍历每一个项目的每一条文法
			if (!LR1.gram.right.empty()) continue;  // 如果还有待接收的tokens，则跳过
			GRAM gram({ LR1.gram.left, LR1.former });  // 记录该文法信息
			int id = getGramID(gram);  // 获取该文法在全部文法Grams中的下标ID
			for (auto& term : LR1.terms)  // 要对所有可接收的终结符都进行归约
				ACTION[{item.id, term}] = 'r' + to_string(id);  // 更新表格
		}
	}
	// 移进动作 or 跳转动作
	for (auto& e : LALR1Edges) {  // 遍历每一条边
		if (isTerminal(e.s))  // 如果接收的token是终结符，则为移进动作
			ACTION[{e.x, e.s}] = 's' + to_string(e.y);  // 更新表格
		else   // 如果接收的token是非终结符，则为跳转动作
			GOTO[{e.x, e.s}] = to_string(e.y);  // 更新表格(跳转动作前缀不用标记)
	}
	// 接受动作
	// 例如，扩充开始文法[S'->S, $]，它是位于LALR1Items[0]的，在接收一个S后，跳转至LALR1Items[1]
	// 此时跳转至LALR1Items[1]再接收一个结束符$，就完成了整体文法的归约(相当于文法归约的统一出口)
	// 我们定义这一个出口为接受动作，用"acc"表示
	ACTION[{1, END}] = ACCEPT;
}
