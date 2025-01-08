//*****************************************************************************
// ��Ȩ��Ϣ��
// ��Ȩ���� (C) 2024 [�ƿ��]����������Ȩ����
//*****************************************************************************
// �ļ����ƣ�regex.h
// ģ�鹦��˵����
// ��ģ�鶨���Զ����ṹ��ʵ��������ʽתNFA��DFA�ķ�����
//
// ����ʦ����ѧ-�����ѧԺ-�������ѧ�뼼��-2��
// ���ߣ��ƿ��-20222131035
// ������ڣ�2024��12��3��
//*****************************************************************************
#pragma once
#include "globals.h"
// ��������¼NFA�ڵ�
NFA* newNFA() {
    NFA* x = new NFA;
    NFAs.push(x);
    return x;
}
// �ͷ�����NFA�ڵ�
void delNFAs() {
    while (!NFAs.empty()) {
        delete NFAs.top();
        NFAs.pop();
    }
}
//���������ַ�NFA��ֻ����һ���ַ���NFAͼ
NFAP basicNFA(const char& c) {
    NFA* S = newNFA();
    NFA* E = newNFA();
    S->isStart = true;
    E->isEnd = true;
    Edge edge;
    edge.c = c;
    edge.next = E;
    S->edges.push_back(edge);
    // ����ȫ���ַ�set
    Chars.insert(c);
    return NFAP(S, E);
}
//���������������NFAͼ
NFAP concatNFA(NFAP& x, NFAP& y) {
    // ��nfa1����ֹ״̬��nfa2����ʼ״̬��������
    x.end->isEnd = false;
    y.start->isStart = false;
    Edge edge;
    edge.c = EMPTY; // ������EMPTY��ʾ�ձ�
    edge.next = y.start;
    x.end->edges.push_back(edge);
    return NFAP(x.start, y.end);
}
//����ѡ���������NFAͼ
NFAP unionNFA(NFAP& x, NFAP& y) {
    NFA* S = newNFA();
    NFA* E = newNFA();
    S->isStart = true;
    E->isEnd = true;
    // ���µĳ�̬��nfa1��nfa2�ĳ�̬��������
    Edge edge1;
    edge1.c = EMPTY;
    edge1.next = x.start;
    S->edges.push_back(edge1);
    x.start->isStart = false;    // ��̬����
    Edge edge2;
    edge2.c = EMPTY;
    edge2.next = y.start;
    S->edges.push_back(edge2);
    y.start->isStart = false;    // ��̬����
    // ��nfa1��nfa2����ֹ״̬���µ���ֹ״̬��������
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
//����*�������NFAͼ
NFAP repeatNFA(NFAP& x) {
    NFA* S = newNFA();
    NFA* E = newNFA();
    S->isStart = true;
    E->isEnd = true;
    // ���µĳ�̬��nfa1�ĳ�̬��������
    Edge edge1;
    edge1.c = EMPTY;
    edge1.next = x.start;
    S->edges.push_back(edge1);
    x.start->isStart = false;    // ��̬����
    // ���µĳ�̬���µ���ֹ״̬��������
    Edge edge2;
    edge2.c = EMPTY;
    edge2.next = E;
    S->edges.push_back(edge2);
    // ��nfa1����ֹ״̬���µ���ֹ״̬��������
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
//�������������NFAͼ
NFAP optionNFA(NFAP& x) {
    NFA* S = newNFA();
    NFA* E = newNFA();
    S->isStart = true;
    E->isEnd = true;
    // ���µĳ�̬��nfa1�ĳ�̬��������
    Edge edge1;
    edge1.c = EMPTY;
    edge1.next = x.start;
    S->edges.push_back(edge1);
    x.start->isStart = false;    // ��̬����
    // ���µĳ�̬���µ���ֹ״̬��������
    Edge edge2;
    edge2.c = EMPTY;
    edge2.next = E;
    S->edges.push_back(edge2);
    // ��nfa1����ֹ״̬���µ���ֹ״̬��������
    x.end->isEnd = false;
    Edge edge3;
    edge3.c = EMPTY;
    edge3.next = E;
    x.end->edges.push_back(edge3);
    return NFAP(S, E);
}
//ʹ��DFS�㷨����״̬ת����
void buildNFAStateTable() {
    stack<NFA*> nfaStack;
    set<NFA*> visited;
    // ��̬
    NFA* S = NFAp.start;
    State startState;
    startState.flag = '-'; // -��ʾ��̬
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
            // ��¼״̬ת����Ϣ
            StateTable[x->id].m[trChar].insert(y->id);
            // �����һ��״̬δ�����ʣ���������ջ
            if (visited.find(y) == visited.end()) {
                nfaStack.push(y);
                // ��¼״̬��Ϣ
                State nextState;
                nextState.id = y->id;
                if (y->isStart) {
                    nextState.flag = "-"; // -��ʾ��̬
                    StartNFA.insert(nextState.id);
                }
                if (y->isEnd) {
                    nextState.flag = "+"; // +��ʾ��̬
                    EndNFA.insert(nextState.id);
                }
                if (y->isExit)
                    ExitNFA[nextState.id] = y->cd;
                StateTable[y->id] = nextState;
                // ��¼����˳���ų���̬��
                if (!y->isEnd)
                    InsertOrder.push_back(y->id);
            }
        }
    }
    // ˳���Ų�����̬
    NFA* endNode = NFAp.end;
    InsertOrder.push_back(endNode->id);
}
//������ʽתNFA���
void regex2NFA() {
    // ˫ջ������������ջopStack�������ջ��,nfaStack��nfaͼջ��
    stack<char> opStack;
    stack<NFAP> nfaStack;
    // �Ա��ʽ�����������沨�����ʽ����
    // �������| @���� �� +  *
    for (const char& c : OutRegex) {
        switch (c)
        {
        case ' ': // �ո�����
            break;
        case '(':
            opStack.push(c);
            break;
        case ')':
            while (!opStack.empty() && opStack.top() != '(') {
                // ����ջ�������������NFAͼ�����������ջ
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
                //cout << "����δ�պϣ�����������ʽ��";
                exit(2);
            }
            else
                opStack.pop(); // ����(
            break;
        case '|':
        case '@':
            // ��������� | �� @
            while (!opStack.empty() && (opStack.top() == '|' || opStack.top() == '@')
                && OpPrec[opStack.top()] >= OpPrec[c]) {
                char op = opStack.top();
                opStack.pop();
                NFAP y = nfaStack.top();
                nfaStack.pop();
                NFAP x = nfaStack.top();
                nfaStack.pop();
                // ����ջ�������������NFAͼ�����������ջ
                nfaStack.push(op == '|' ? unionNFA(x, y) : concatNFA(x, y));
            }
            opStack.push(c);
            break;
        case '?':
        case '*':
            // ����հ������ ? + *
            // ��nfaStack����NFA��Ӧ����Ӧ�ıհ����������������ջ
            if (!nfaStack.empty()) {
                NFAP t = nfaStack.top();
                nfaStack.pop();
                nfaStack.push(c == '?' ? optionNFA(t) : repeatNFA(t));
            }
            else {
                //cout << "������ʽ�﷨���󣺱հ�����û��NFA���ã�";
                exit(2);
            }
            break;
        case EXIT:
            if (!nfaStack.empty()) {  // ��ǵ��ʱ���cd
                NFAP t = nfaStack.top();
                nfaStack.pop();
                t.end->cd = CdCnt++;
                t.end->isExit = true;
                nfaStack.push(t);
            }
            break;
        default:  // ������ĸ�ַ�
            nfaStack.push(basicNFA(c));
        }
    }
    // ����ջ��ʣ��������
    while (!opStack.empty()) {
        char op = opStack.top();
        opStack.pop();
        if (op == '|' || op == '@') {
            // ������������� | �� .
            if (nfaStack.size() < 2) {
                //cout << "������ʽ�﷨���󣺲����Դ�������� " << op << "��";
                exit(2);
            }
            NFAP y = nfaStack.top();
            nfaStack.pop();
            NFAP x = nfaStack.top();
            nfaStack.pop();
            nfaStack.push(op == '|' ? unionNFA(x, y) : concatNFA(x, y));
        }
        else {
            //cout << "������ʽ�﷨����δ֪������� " << op << "��";
            exit(2);
        }
    }
    // ���յ�NFAͼ��nfaStack�Ķ���
    NFAp = nfaStack.top();
    //cout << "NFAͼ�������" << endl;
    buildNFAStateTable();
    //cout << "״̬ת���������" << endl;
}
// �ж��Ƿ��г�̬��̬�������򷵻ض�Ӧ�ַ���
string getDfaFlag(set<int>& stateSet) {
    for (auto& x : stateSet)
        if (StartNFA.find(x) != StartNFA.end())
            return "-";
        else if (EndNFA.find(x) != EndNFA.end() || ExitNFA.find(x) != ExitNFA.end())
            return "+";
    return "";
}
// �������״̬�Ħ�-�հ�
set<int> emptyClosure(int id) {
    set<int> eResult{ id };
    stack<int> stack;
    stack.push(id);  //�������״̬idѹ��ջ��
    while (!stack.empty()) {
        int current = stack.top();  //ȡ��ջ��Ԫ��
        stack.pop();
        //��ȡ��ǰ״̬�Ħ�-ת������
        set<int> eClosure = StateTable[current].m[EMPTY];
        //������-ת�������е�ÿһ��״̬
        for (const auto& t : eClosure) {
            //�����������в�������״̬
            if (eResult.find(t) == eResult.end()) {
                eResult.insert(t);  //����״̬����������
                stack.push(t);  //����״̬ѹ��ջ���Ա��������
            }
        }
    }
    return eResult;
}
//�������״̬���ַ��ıհ�
set<int> charClosure(int id, char ch) {
    set<int> result;
    set<int> processed;
    stack<int> stack;
    stack.push(id);
    while (!stack.empty()) {
        int current = stack.top();
        stack.pop();
        //�����ǰ״̬�Ѿ����������������
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
                if (x.cd > 0) exit(3);  // ��״̬�����
                else x.cd = ExitNFA[y];
}
void NFA2DFA() {
    NFA* startNFA = NFAp.start; // ���NFAͼ����ʼλ��
    int startId = startNFA->id;   // �����ʼ���
    DFA startDFA;
    startDFA.states = emptyClosure(startId); // ��ʼ�հ�
    startDFA.flag = getDfaFlag(startDFA.states); // �жϳ�̬��̬
    queue<set<int>> newState;
    DfaSetMap[startDFA.states] = DfaCnt;
    if (startDFA.flag == "+")
        DfaEndState.insert(DfaCnt++);
    else
        DfaNotEndState.insert(DfaCnt++);
    // ��ÿ���ַ����б���
    for (const char& ch : Chars) {
        set<int> chClosure;
        for (const int& id : startDFA.states) {
            set<int> tmp = charClosure(id, ch);
            chClosure.insert(tmp.begin(), tmp.end());
        }
        if (chClosure.empty())  // �������հ��ǿռ�û��Ҫ������ȥ��
            continue;
        int presize = DfaStateSet.size();
        DfaStateSet.insert(chClosure);
        int lastsize = DfaStateSet.size();
        // ����һ��һ�����Ǹýڵ�����ַ���״̬
        startDFA.trans[ch] = chClosure;
        // �����С��һ����֤������״̬
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
    // �Ժ������״̬���в�ͣ����
    while (!newState.empty()) {
        // �ó�һ����״̬
        set<int> ns = newState.front();
        newState.pop();
        DFA x;
        x.states = ns;  // �ýڵ�״̬����
        x.flag = getDfaFlag(ns);
        for (const char& ch : Chars) {
            set<int> chClosure;
            for (const int& id : ns) {
                set<int> tmp = charClosure(id, ch);
                chClosure.insert(tmp.begin(), tmp.end());
            }
            if (chClosure.empty()) continue; // �������հ��ǿռ�û��Ҫ������ȥ��
            int presize = DfaStateSet.size();
            DfaStateSet.insert(chClosure);
            int lastsize = DfaStateSet.size();
            // ����һ��һ�����Ǹýڵ�����ַ���״̬
            x.trans[ch] = chClosure;
            // �����С��һ����֤������״̬
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
    // ͳһ���
    for (int i = 0; i < DfaList.size(); ++i)
        DfaList[i].id = i;
    markDFAcd();  // ���DFA��cd
}
/*---------------------------------DFA��С��--------------------------------------*/
// �ж��Ƿ��г�̬��̬�������򷵻ض�Ӧ�ַ���
string getMinDfaFlag(const set<int>& stateSet) {
    if (stateSet.find(0) != stateSet.end())
        return "-";
    for (const int& element : DfaEndState)
        if (stateSet.find(element) != stateSet.end())
            return "+";  // ���ܻ��ж����̬ͬʱ����������ֻҪһ��
    return "";
}
// �����ַ� ch ��״̬���� node �ֳ������Ӽ���
void splitSet(int i, char ch) {
    set<int> result;
    auto& node = DivideVec[i];
    int s = -2, cs;
    for (const auto& state : node) {
        if (DfaList[state].trans.find(ch) == DfaList[state].trans.end())
            cs = -1; // �ռ�
        else {
            // �����ַ� ch �ҵ���һ��״̬
            int next_state = DfaSetMap[DfaList[state].trans[ch]];
            cs = Dfa2MDfa[next_state];    // ���״̬���±��Ƕ���
        }
        if (s == -2) s = cs;   // ��ʼ�±�
        else if (cs != s)   // ����±겻ͬ�����������⣬��Ҫ�ֳ���
            result.insert(state);
    }
    // ɾ��Ҫɾ����Ԫ��
    for (int state : result)
        node.erase(state);
    // ���������ˣ����result���ǿգ�֤�����µģ�����vector��
    if (!result.empty()) {
        DivideVec.push_back(result);
        // ͬʱ�����±�
        for (const auto& a : result)
            Dfa2MDfa[a] = DivideVec.size() - 1;
    }
}
void divideState() {  // ���ֳ�ʼ״̬ΪMaxCd+1������(cd=0����Ϊ��̬�����໮ΪMaxCd����������̬)
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
// �����СDFA��cd
void markMinDFAcd() {
    for (const auto& p : Dfa2MDfa)
        MDfaList[p.second].cd = DfaList[p.first].cd;
}
// ��С��DFA
void minimizeDFA() {
    divideState();
    // ��flagΪ1ʱ��һֱѭ��
    int continueFlag = 1;
    while (continueFlag) {
        continueFlag = 0;
        int size1 = DivideVec.size();
        for (int i = 0; i < size1; i++) // ����ַ����Էָ�״̬����
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
        // ����ַ�
        for (const char& ch : Chars) {
            if (v.empty()) continue;
            int i = *(v.begin()); // ��һ������
            if (DfaList[i].trans.find(ch) == DfaList[i].trans.end()) continue;
            int next_state = DfaSetMap[DfaList[i].trans[ch]];
            int thisNum = Dfa2MDfa[next_state];    // ���״̬�±�
            x.trans[ch] = thisNum;
        }
        MDfaList.push_back(x);
    }
    markMinDFAcd();
}
// ����NFA���
void genNFA() {
    vector<string> headers{ "״̬", "ID", "��" };  // ���ñ�ͷ
    int colCnt = headers.size();
    map<char, int> charCol{ {EMPTY, colCnt - 1} };  // �ַ��͵�X�д�������Ӧ
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
        NFATable[row][0] = node.flag;  // Flag ��
        NFATable[row][1] = to_string(node.id);  // ID ��
        for (const auto& entry : node.m)
            NFATable[row][charCol[entry.first]] = set2string(entry.second);
        ++row;
    }
}
// ����DFA���
void genDFA() {
    vector<string> headers{ "״̬", "ID", "NFA״̬����" };
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
        DFATable[row][0] = x.flag;  // Flag ��
        DFATable[row][1] = to_string(x.id);  // ���
        DFATable[row][2] = "{" + set2string(x.states) + "}";  // ״̬����
        for (const auto& entry : x.trans)  // ״̬ת��
            DFATable[row][charCol[entry.first]] = to_string(DfaSetMap[entry.second]);
        ++row;
    }
}
// ������СDFA���
void genMinDFA() {
    vector<string> headers{ "״̬", "ID", "���ʱ���", "��������" };
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