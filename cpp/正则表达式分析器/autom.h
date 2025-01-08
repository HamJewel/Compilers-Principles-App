//*****************************************************************************
// 版权信息：
// 版权所有 (C) 2024 [黄俊銓]。保留所有权利。
//*****************************************************************************
// 文件名称：regex.h
// 模块功能说明：
// 该模块定义自动机结构，实现正则表达式转NFA、DFA的方法。
//
// 华南师范大学-计算机学院-计算机科学与技术-2班
// 作者：黄俊銓-20222131035
// 完成日期：2024年12月3日
//*****************************************************************************
#pragma once
#include "globals.h"
// 创建并记录NFA节点
NFA* newNFA() {
    NFA* x = new NFA;
    NFAs.push(x);
    return x;
}
// 释放所有NFA节点
void delNFAs() {
    while (!NFAs.empty()) {
        delete NFAs.top();
        NFAs.pop();
    }
}
//创建基本字符NFA，只包含一个字符的NFA图
NFAP basicNFA(const char& c) {
    NFA* S = newNFA();
    NFA* E = newNFA();
    S->isStart = true;
    E->isEnd = true;
    Edge edge;
    edge.c = c;
    edge.next = E;
    S->edges.push_back(edge);
    // 存入全局字符set
    Chars.insert(c);
    return NFAP(S, E);
}
//创建连接运算符的NFA图
NFAP concatNFA(NFAP& x, NFAP& y) {
    // 把nfa1的终止状态与nfa2的起始状态连接起来
    x.end->isEnd = false;
    y.start->isStart = false;
    Edge edge;
    edge.c = EMPTY; // 这里用EMPTY表示空边
    edge.next = y.start;
    x.end->edges.push_back(edge);
    return NFAP(x.start, y.end);
}
//创建选择运算符的NFA图
NFAP unionNFA(NFAP& x, NFAP& y) {
    NFA* S = newNFA();
    NFA* E = newNFA();
    S->isStart = true;
    E->isEnd = true;
    // 把新的初态与nfa1和nfa2的初态连接起来
    Edge edge1;
    edge1.c = EMPTY;
    edge1.next = x.start;
    S->edges.push_back(edge1);
    x.start->isStart = false;    // 初态结束
    Edge edge2;
    edge2.c = EMPTY;
    edge2.next = y.start;
    S->edges.push_back(edge2);
    y.start->isStart = false;    // 初态结束
    // 把nfa1和nfa2的终止状态与新的终止状态连接起来
    x.end->isEnd = false;
    y.end->isEnd = false;
    Edge edge3;
    edge3.c = EMPTY;
    edge3.next = E;
    x.end->edges.push_back(edge3);
    Edge edge4;
    edge4.c = EMPTY;
    edge4.next = E;
    y.end->edges.push_back(edge4);
    return NFAP(S, E);
}
//创建*运算符的NFA图
NFAP repeatNFA(NFAP& x) {
    NFA* S = newNFA();
    NFA* E = newNFA();
    S->isStart = true;
    E->isEnd = true;
    // 把新的初态与nfa1的初态连接起来
    Edge edge1;
    edge1.c = EMPTY;
    edge1.next = x.start;
    S->edges.push_back(edge1);
    x.start->isStart = false;    // 初态结束
    // 把新的初态与新的终止状态连接起来
    Edge edge2;
    edge2.c = EMPTY;
    edge2.next = E;
    S->edges.push_back(edge2);
    // 把nfa1的终止状态与新的终止状态连接起来
    x.end->isEnd = false;
    Edge edge3;
    edge3.c = EMPTY;
    edge3.next = x.start;
    x.end->edges.push_back(edge3);
    Edge edge4;
    edge4.c = EMPTY;
    edge4.next = E;
    x.end->edges.push_back(edge4);
    return NFAP(S, E);
}
//创建？运算符的NFA图
NFAP optionNFA(NFAP& x) {
    NFA* S = newNFA();
    NFA* E = newNFA();
    S->isStart = true;
    E->isEnd = true;
    // 把新的初态与nfa1的初态连接起来
    Edge edge1;
    edge1.c = EMPTY;
    edge1.next = x.start;
    S->edges.push_back(edge1);
    x.start->isStart = false;    // 初态结束
    // 把新的初态与新的终止状态连接起来
    Edge edge2;
    edge2.c = EMPTY;
    edge2.next = E;
    S->edges.push_back(edge2);
    // 把nfa1的终止状态与新的终止状态连接起来
    x.end->isEnd = false;
    Edge edge3;
    edge3.c = EMPTY;
    edge3.next = E;
    x.end->edges.push_back(edge3);
    return NFAP(S, E);
}
//使用DFS算法生成状态转换表
void buildNFAStateTable() {
    stack<NFA*> nfaStack;
    set<NFA*> visited;
    // 初态
    NFA* S = NFAp.start;
    State startState;
    startState.flag = '-'; // -表示初态
    startState.id = S->id;
    StateTable[S->id] = startState;
    InsertOrder.push_back(S->id);
    StartNFA.insert(S->id);
    nfaStack.push(S);
    while (!nfaStack.empty()) {
        NFA* x = nfaStack.top();
        nfaStack.pop();
        visited.insert(x);
        for (const Edge& edge : x->edges) {
            char trChar = edge.c;
            NFA* y = edge.next;
            // 记录状态转换信息
            StateTable[x->id].m[trChar].insert(y->id);
            // 如果下一个状态未被访问，将其加入堆栈
            if (visited.find(y) == visited.end()) {
                nfaStack.push(y);
                // 记录状态信息
                State nextState;
                nextState.id = y->id;
                if (y->isStart) {
                    nextState.flag = "-"; // -表示初态
                    StartNFA.insert(nextState.id);
                }
                if (y->isEnd) {
                    nextState.flag = "+"; // +表示终态
                    EndNFA.insert(nextState.id);
                }
                if (y->isExit)
                    ExitNFA[nextState.id] = y->cd;
                StateTable[y->id] = nextState;
                // 记录插入顺序（排除终态）
                if (!y->isEnd)
                    InsertOrder.push_back(y->id);
            }
        }
    }
    // 顺序表才插入终态
    NFA* endNode = NFAp.end;
    InsertOrder.push_back(endNode->id);
}
//正则表达式转NFA入口
void regex2NFA() {
    // 双栈法，创建两个栈opStack（运算符栈）,nfaStack（nfa图栈）
    stack<char> opStack;
    stack<NFAP> nfaStack;
    // 对表达式进行类似于逆波兰表达式处理
    // 运算符：| @（） ？ +  *
    for (const char& c : OutRegex) {
        switch (c)
        {
        case ' ': // 空格跳过
            break;
        case '(':
            opStack.push(c);
            break;
        case ')':
            while (!opStack.empty() && opStack.top() != '(') {
                // 处理栈顶运算符，构建NFA图，并将结果入栈
                char op = opStack.top();
                opStack.pop();
                NFAP y = nfaStack.top();
                nfaStack.pop();
                NFAP x = nfaStack.top();
                nfaStack.pop();
                if (op == '|' || op == '@')
                    nfaStack.push(op == '|' ? unionNFA(x, y) : concatNFA(x, y));
            }
            if (opStack.empty()) {
                //cout << "括号未闭合，请检查正则表达式！";
                exit(2);
            }
            else
                opStack.pop(); // 弹出(
            break;
        case '|':
        case '@':
            // 处理运算符 | 和 @
            while (!opStack.empty() && (opStack.top() == '|' || opStack.top() == '@')
                && OpPrec[opStack.top()] >= OpPrec[c]) {
                char op = opStack.top();
                opStack.pop();
                NFAP y = nfaStack.top();
                nfaStack.pop();
                NFAP x = nfaStack.top();
                nfaStack.pop();
                // 处理栈顶运算符，构建NFA图，并将结果入栈
                nfaStack.push(op == '|' ? unionNFA(x, y) : concatNFA(x, y));
            }
            opStack.push(c);
            break;
        case '?':
        case '*':
            // 处理闭包运算符 ? + *
            // 从nfaStack弹出NFA，应用相应的闭包操作，并将结果入栈
            if (!nfaStack.empty()) {
                NFAP t = nfaStack.top();
                nfaStack.pop();
                nfaStack.push(c == '?' ? optionNFA(t) : repeatNFA(t));
            }
            else {
                //cout << "正则表达式语法错误：闭包操作没有NFA可用！";
                exit(2);
            }
            break;
        case EXIT:
            if (!nfaStack.empty()) {  // 标记单词编码cd
                NFAP t = nfaStack.top();
                nfaStack.pop();
                t.end->cd = CdCnt++;
                t.end->isExit = true;
                nfaStack.push(t);
            }
            break;
        default:  // 处理字母字符
            nfaStack.push(basicNFA(c));
        }
    }
    // 处理栈中剩余的运算符
    while (!opStack.empty()) {
        char op = opStack.top();
        opStack.pop();
        if (op == '|' || op == '@') {
            // 处理并构建运算符 | 和 .
            if (nfaStack.size() < 2) {
                //cout << "正则表达式语法错误：不足以处理运算符 " << op << "！";
                exit(2);
            }
            NFAP y = nfaStack.top();
            nfaStack.pop();
            NFAP x = nfaStack.top();
            nfaStack.pop();
            nfaStack.push(op == '|' ? unionNFA(x, y) : concatNFA(x, y));
        }
        else {
            //cout << "正则表达式语法错误：未知的运算符 " << op << "！";
            exit(2);
        }
    }
    // 最终的NFA图在nfaStack的顶部
    NFAp = nfaStack.top();
    //cout << "NFA图构建完毕" << endl;
    buildNFAStateTable();
    //cout << "状态转换表构建完毕" << endl;
}
// 判断是否含有初态终态，含有则返回对应字符串
string getDfaFlag(set<int>& stateSet) {
    for (auto& x : stateSet)
        if (StartNFA.find(x) != StartNFA.end())
            return "-";
        else if (EndNFA.find(x) != EndNFA.end() || ExitNFA.find(x) != ExitNFA.end())
            return "+";
    return "";
}
// 计算给定状态的ε-闭包
set<int> emptyClosure(int id) {
    set<int> eResult{ id };
    stack<int> stack;
    stack.push(id);  //将输入的状态id压入栈中
    while (!stack.empty()) {
        int current = stack.top();  //取出栈顶元素
        stack.pop();
        //获取当前状态的ε-转换集合
        set<int> eClosure = StateTable[current].m[EMPTY];
        //遍历ε-转换集合中的每一个状态
        for (const auto& t : eClosure) {
            //如果结果集合中不包含该状态
            if (eResult.find(t) == eResult.end()) {
                eResult.insert(t);  //将该状态加入结果集合
                stack.push(t);  //将该状态压入栈中以便后续处理
            }
        }
    }
    return eResult;
}
//计算给定状态和字符的闭包
set<int> charClosure(int id, char ch) {
    set<int> result;
    set<int> processed;
    stack<int> stack;
    stack.push(id);
    while (!stack.empty()) {
        int current = stack.top();
        stack.pop();
        //如果当前状态已经被处理过，则跳过
        if (processed.find(current) != processed.end())
            continue;
        processed.insert(current);
        set<int> otherClosure = StateTable[current].m[ch];
        for (const auto& o : otherClosure) {
            set<int> tmp = emptyClosure(o);
            result.insert(tmp.begin(), tmp.end());
            stack.push(o);
        }
    }
    return result;
}
void markDFAcd() {
    for (auto& x : DfaList)
        for (auto& y : x.states)
            if (ExitNFA.find(y) != ExitNFA.end())
                if (x.cd > 0) exit(3);  // 单状态多出口
                else x.cd = ExitNFA[y];
}
void NFA2DFA() {
    NFA* startNFA = NFAp.start; // 获得NFA图的起始位置
    int startId = startNFA->id;   // 获得起始编号
    DFA startDFA;
    startDFA.states = emptyClosure(startId); // 初始闭包
    startDFA.flag = getDfaFlag(startDFA.states); // 判断初态终态
    queue<set<int>> newState;
    DfaSetMap[startDFA.states] = DfaCnt;
    if (startDFA.flag == "+")
        DfaEndState.insert(DfaCnt++);
    else
        DfaNotEndState.insert(DfaCnt++);
    // 对每个字符进行遍历
    for (const char& ch : Chars) {
        set<int> chClosure;
        for (const int& id : startDFA.states) {
            set<int> tmp = charClosure(id, ch);
            chClosure.insert(tmp.begin(), tmp.end());
        }
        if (chClosure.empty())  // 如果这个闭包是空集没必要继续下去了
            continue;
        int presize = DfaStateSet.size();
        DfaStateSet.insert(chClosure);
        int lastsize = DfaStateSet.size();
        // 不管一不一样都是该节点这个字符的状态
        startDFA.trans[ch] = chClosure;
        // 如果大小不一样，证明是新状态
        if (lastsize > presize) {
            DfaSetMap[chClosure] = DfaCnt;
            newState.push(chClosure);
            if (getDfaFlag(chClosure) == "+")
                DfaEndState.insert(DfaCnt++);
            else
                DfaNotEndState.insert(DfaCnt++);
        }
    }
    DfaList.push_back(startDFA);
    // 对后面的新状态进行不停遍历
    while (!newState.empty()) {
        // 拿出一个新状态
        set<int> ns = newState.front();
        newState.pop();
        DFA x;
        x.states = ns;  // 该节点状态集合
        x.flag = getDfaFlag(ns);
        for (const char& ch : Chars) {
            set<int> chClosure;
            for (const int& id : ns) {
                set<int> tmp = charClosure(id, ch);
                chClosure.insert(tmp.begin(), tmp.end());
            }
            if (chClosure.empty()) continue; // 如果这个闭包是空集没必要继续下去了
            int presize = DfaStateSet.size();
            DfaStateSet.insert(chClosure);
            int lastsize = DfaStateSet.size();
            // 不管一不一样都是该节点这个字符的状态
            x.trans[ch] = chClosure;
            // 如果大小不一样，证明是新状态
            if (lastsize > presize) {
                DfaSetMap[chClosure] = DfaCnt;
                newState.push(chClosure);
                if (getDfaFlag(chClosure) == "+")
                    DfaEndState.insert(DfaCnt++);
                else
                    DfaNotEndState.insert(DfaCnt++);
            }
        }
        DfaList.push_back(x);
    }
    // 统一编号
    for (int i = 0; i < DfaList.size(); ++i)
        DfaList[i].id = i;
    markDFAcd();  // 标记DFA的cd
}
/*---------------------------------DFA最小化--------------------------------------*/
// 判断是否含有初态终态，含有则返回对应字符串
string getMinDfaFlag(const set<int>& stateSet) {
    if (stateSet.find(0) != stateSet.end())
        return "-";
    for (const int& element : DfaEndState)
        if (stateSet.find(element) != stateSet.end())
            return "+";  // 可能会有多个终态同时包含，我们只要一个
    return "";
}
// 根据字符 ch 将状态集合 node 分成两个子集合
void splitSet(int i, char ch) {
    set<int> result;
    auto& node = DivideVec[i];
    int s = -2, cs;
    for (const auto& state : node) {
        if (DfaList[state].trans.find(ch) == DfaList[state].trans.end())
            cs = -1; // 空集
        else {
            // 根据字符 ch 找到下一个状态
            int next_state = DfaSetMap[DfaList[state].trans[ch]];
            cs = Dfa2MDfa[next_state];    // 这个状态的下标是多少
        }
        if (s == -2) s = cs;   // 初始下标
        else if (cs != s)   // 如果下标不同，就是有问题，需要分出来
            result.insert(state);
    }
    // 删除要删除的元素
    for (int state : result)
        node.erase(state);
    // 都遍历完了，如果result不是空，证明有新的，加入vector中
    if (!result.empty()) {
        DivideVec.push_back(result);
        // 同时更新下标
        for (const auto& a : result)
            Dfa2MDfa[a] = DivideVec.size() - 1;
    }
}
void divideState() {  // 划分初始状态为MaxCd+1个集合(cd=0划分为初态，其余划为MaxCd个独立的终态)
    map<int, set<int>> m;
    for (const auto& x : DfaList)
        m[x.cd].insert(x.id);
    int id = 0;
    for (const auto& p : m) {
        DivideVec.push_back(p.second);
        for (const auto& x : p.second)
            Dfa2MDfa[x] = id;
        ++id;
    }
}
// 标记最小DFA的cd
void markMinDFAcd() {
    for (const auto& p : Dfa2MDfa)
        MDfaList[p.second].cd = DfaList[p.first].cd;
}
// 最小化DFA
void minimizeDFA() {
    divideState();
    // 当flag为1时，一直循环
    int continueFlag = 1;
    while (continueFlag) {
        continueFlag = 0;
        int size1 = DivideVec.size();
        for (int i = 0; i < size1; i++) // 逐个字符尝试分割状态集合
            for (char ch : Chars)
                splitSet(i, ch);
        int size2 = DivideVec.size();
        if (size2 > size1) continueFlag = 1;
    }
    for (MDfaCnt = 0; MDfaCnt < DivideVec.size(); MDfaCnt++) {
        const auto& v = DivideVec[MDfaCnt];
        MDFA x;
        x.flag = getMinDfaFlag(v);
        x.id = MDfaCnt;
        if (x.flag == "-") MDfaStart = x.id;
        // 逐个字符
        for (const char& ch : Chars) {
            if (v.empty()) continue;
            int i = *(v.begin()); // 拿一个出来
            if (DfaList[i].trans.find(ch) == DfaList[i].trans.end()) continue;
            int next_state = DfaSetMap[DfaList[i].trans[ch]];
            int thisNum = Dfa2MDfa[next_state];    // 这个状态下标
            x.trans[ch] = thisNum;
        }
        MDfaList.push_back(x);
    }
    markMinDFAcd();
}
// 生成NFA表格
void genNFA() {
    vector<string> headers{ "状态", "ID", "ε" };  // 设置表头
    int colCnt = headers.size();
    map<char, int> charCol{ {EMPTY, colCnt - 1} };  // 字符和第X列存起来对应
    for (const auto& c : Chars) {
        if (C2S.find(c) != C2S.end())
            headers.push_back(trim(C2S[c]));
        else
            headers.push_back(c == ANY ? "ANY" : STR(c));
        charCol[c] = colCnt++;
    }
    int rowCnt = StateTable.size() + 1;
    NFATable.resize(rowCnt, vector<string>(colCnt));
    NFATable[0] = headers;
    int row = 1;
    for (const auto& id : InsertOrder) {
        const State& node = StateTable[id];
        NFATable[row][0] = node.flag;  // Flag 列
        NFATable[row][1] = to_string(node.id);  // ID 列
        for (const auto& entry : node.m)
            NFATable[row][charCol[entry.first]] = set2string(entry.second);
        ++row;
    }
}
// 生成DFA表格
void genDFA() {
    vector<string> headers{ "状态", "ID", "NFA状态集合" };
    int colCnt = headers.size();
    map<char, int> charCol;
    for (const auto& c : Chars) {
        if (C2S.find(c) != C2S.end())
            headers.push_back(trim(C2S[c]));
        else
            headers.push_back(c == ANY ? "ANY" : STR(c));
        charCol[c] = colCnt++;
    }
    int rowCnt = DfaList.size() + 1;
    DFATable.resize(rowCnt, vector<string>(colCnt));
    DFATable[0] = headers;
    int row = 1;
    for (const auto& x : DfaList) {
        DFATable[row][0] = x.flag;  // Flag 列
        DFATable[row][1] = to_string(x.id);  // 编号
        DFATable[row][2] = "{" + set2string(x.states) + "}";  // 状态集合
        for (const auto& entry : x.trans)  // 状态转换
            DFATable[row][charCol[entry.first]] = to_string(DfaSetMap[entry.second]);
        ++row;
    }
}
// 生成最小DFA表格
void genMinDFA() {
    vector<string> headers{ "状态", "ID", "单词编码", "单词内容" };
    int colCnt = headers.size();
    map<char, int> charCol;
    for (const auto& c : Chars) {
        if (C2S.find(c) != C2S.end())
            headers.push_back(trim(C2S[c]));
        else
            headers.push_back(c == ANY ? "ANY" : STR(c));
        charCol[c] = colCnt++;
    }
    int rowCnt = MDfaList.size() + 1;
    MinDFATable.resize(rowCnt, vector<string>(colCnt));
    MinDFATable[0] = headers;
    int row = 1;
    for (auto& x : MDfaList) {
        MinDFATable[row][0] = x.flag;
        MinDFATable[row][1] = to_string(x.id);
        MinDFATable[row][2] = x.cd > 0 ? to_string(x.cd) : "";
        MinDFATable[row][3] = x.cd > 0 ? Cd2Token[x.cd] : "";
        for (const auto& entry : x.trans)
            MinDFATable[row][charCol[entry.first]] = to_string(entry.second);
        ++row;
    }
}