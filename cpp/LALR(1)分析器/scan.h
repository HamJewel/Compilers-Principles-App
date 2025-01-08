//*****************************************************************************
// 版权信息：
// 版权所有 (C) 2024 [黄俊銓]。保留所有权利。
//*****************************************************************************
// 文件名称：scan.h
// 模块功能说明：
// 该模块实现文法的扫描器，拆分文法内容。
//
// 华南师范大学-计算机学院-计算机科学与技术-2班
// 作者：黄俊銓-20222131035
// 完成日期：2024年12月3日
//*****************************************************************************
#pragma once
#include "globals.h"


typedef enum { START, DONE, KEEP, NOTEND, ENDLINE, ENDLINES } State;
// START 开始接收字符
// DONE 完成一个Token的接收
// NONTERM 当前要接收的Token是非终结符
// TERMINAL 当前要接收的Token是终结符
// KEEP 持续接受字符，遇到空格停止(在第二种书写形式中使用)
// OR '|'或，特殊字符
// NOTEND 还没结束扫描的状态
// ENDLINE 扫描完当前行的状态
// ENDLINES 扫描完全部行lines的状态

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
// 这里偷个懒，直接把"->"左边的非空格字符都当做文法的左部
void getLeft() {
	int i = lines[lineID].find("->");
	charID = i + 2;
	newGram.left = stripBlank(lines[lineID].substr(0, i));
}
// 获取文法右部
void getRight() {
	bool save;
	string token;
	scanState = START;  // 初始化扫描状态
	endState = NOTEND;  // 初始化结束状态
	while (scanState != DONE) {
		char c = nextChar();
		save = true;  // 保存字符，c追加到token后
		switch (scanState) {
		case START:
			if (isBlank(c))
				save = false;
			else if (c == '|')
				scanState = DONE;
			else if (c == EOF)  // 一开始就结束，说明是空串
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
	// 更新文法右部
	newGram.right.push_back(token);
}
void scanGrams() {
	for (auto& line : lines)  // 替换转义或'|'
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
// 按或符号划分文法
void splitGrams() {
	vector<GRAM> grams = Grams;  // 临时变量
	Grams.clear();  // 清空原来的文法
	for (auto& gram : grams) {
		vector<string> tokens;  // 记录当前被拆分后获得的右部tokens
		for (auto& x : gram.right) {
			if (x != "|") {  // 没遇到"|"就继续添加右部
				tokens.push_back(x);
				continue;
			}
			Grams.push_back({ gram.left, tokens });  // 遇到"|"，新增文法
			tokens.clear();  // 清空当前右部记录，准备下一次的遍历
		}
		// (处理末尾数据)如果还有剩余的右部，直接加入到Grams中
		if (!tokens.empty()) Grams.push_back({ gram.left, tokens });
	}
	for (auto& gram : Grams)  // 替换回转义或'|'
		for (auto& x : gram.right)
			if (x == OR) x = "|";
}
// 扩充开始文法，如A->a，新增A'->A  (先消除左递归，再扩充文法)
void extendStartGram() {
	string newName = Grams[0].left;
	while (isNonterm(newName))  // 如果新名称已存在，就一直往后追加suf
		newName += SUFFIX;
	Grams.insert(Grams.begin(), { newName, {Grams[0].left} });
}
