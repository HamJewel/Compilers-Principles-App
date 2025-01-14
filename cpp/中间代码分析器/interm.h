#pragma once
#include "globals.h"
set<string> types{ "int", "float", "double", "void" };
int lenQ(Quad* x) {
	int n = 0;
	while (x != nullptr) {
		if (Hids.find(x->op) == Hids.end())
			++n;
		x = x->next;
	}
	return n;
}
Quad* newQuad(string op, string x, string y, string t) {
	Quad* q = new Quad{ op, x, y, t };
	Qd.push_back(q);
	return q;
}
void delQuads() {
	for (auto& x : Qd)
		delete x;
}
string newTmp() {
	int k = 0;
	for (auto& x : Tmps) {
		if (k != x) break;
		++k;
	}
	Tmps.insert(k);
	return "Τ" + to_string(k);
}
void logTmp(Quad* q) {
	Rels.push_back(q->t);
}
void delTmps() {
	for (auto& t : Rels)
		if (t.find("Τ") == 0)
			Tmps.erase(stoi(t.substr(2)));
	Rels.clear();
}
void printQuads() {
	int id = 0;
	Quad* q = Qp[0].start;
	while (q != nullptr) {
		if (Hids.find(q->op) != Hids.end()) {
			q = q->next;
			continue;
		}
		string x = q->x;
		if (q->op.find("跳转") != -1)
			x = to_string(stoi(x) + id);
		cout << id++ << '\t' << q->op << '\t' << x << '\t' << q->y << '\t' << q->t << '\n';
		q = q->next;
	}
}
void trim() {
	Tb = vector<string>(Ta.end() - Gn, Ta.end());
	Ta.erase(Ta.end() - Gn, Ta.end());
}
void program() {
	Ta.push_back("");
	Quad* q = newQuad("EOF", "", "", "");
	QP& x = Qp.back();
	x.end->next = q, x.end = q;
}
void definition_list() {
	Ta.push_back("");
	if (Gn == 1) return;
	QP y = Qp.back(); Qp.pop_back();
	QP& x = Qp.back();
	x.end->next = y.start, x.end = y.end;
}
void definition() {
	Ta.push_back("");
}
void variable_definition() {
	Ta.push_back("");
	string kind = Gn == 3 ? "变量" : "指针";
	Quad* q = newQuad("定义", Tb[0], kind, Tb[1]);
	Qp.push_back(QP{ q, q });
}
void type_indicator() {
	Ta.push_back(Tb[0]);
}
void function_definition() {
	Ta.push_back("");
	Quad* q = newQuad("定义", Tb[0], "函数", Tb[1]);
	QP y = Qp.back(); Qp.pop_back();
	QP& x = Qp.back();
	x.end->next = q, x.end = q;
	x.end->next = y.start, x.end = y.end;
}
void parameters() {
	Ta.push_back("");
	if (Gy.r[0] != "void") return;
	Quad* q = newQuad("跳过", "", "", "");
	Qp.push_back(QP{ q, q });
}
void parameter_list() {
	Ta.push_back("");
	if (Gn == 1) return;
	QP y = Qp.back(); Qp.pop_back();
	QP& x = Qp.back();
	x.end->next = y.start, x.end = y.end;
}
void parameter() {
	Ta.push_back("");
	string kind = Gn == 2 ? "变量" : "指针";
	Quad* q = newQuad("参数", Tb[0], kind, Tb[1]);
	Qp.push_back(QP{ q, q });
}
void compound_stmt() {
	Ta.push_back("");
	QP y = Qp.back(); Qp.pop_back();
	QP& x = Qp.back();
	x.end->next = y.start, x.end = y.end;
}
void local_definitions() {
	Ta.push_back("");
	if (Gn == 1) {
		Quad* q = newQuad("跳过", "", "", "");
		Qp.push_back(QP{ q, q });
		return;
	}
	QP y = Qp.back(); Qp.pop_back();
	QP& x = Qp.back();
	x.end->next = y.start, x.end = y.end;
}
void statement_list() {
	Ta.push_back("");
	if (Gn == 1) {
		Quad* q = newQuad("跳过", "", "", "");
		Qp.push_back(QP{ q, q });
		return;
	}
	QP y = Qp.back(); Qp.pop_back();
	QP& x = Qp.back();
	x.end->next = y.start, x.end = y.end;
}
void statement() {
	Ta.push_back("");
}
void expression_stmt() {
	Ta.push_back("");
	if (Gn == 1) {
		Quad* q = newQuad("跳过", "", "", "");
		Qp.push_back(QP{ q, q });
		return;
	}
	QP& x = Qp.back();
	logTmp(x.end);
	delTmps();
}
void condition_stmt() {
	Ta.push_back("");
	if (Gn == 5) {
		QP y = Qp.back(); Qp.pop_back();
		QP& x = Qp.back();
		logTmp(x.end), logTmp(y.end);
		int n = lenQ(y.start);
		Quad* q = newQuad("取反跳转", to_string(n + 1), "", x.end->t);
		x.end->next = q, x.end = q;
		x.end->next = y.start, x.end = y.end;
		delTmps();
		return;
	}
	QP z = Qp.back(); Qp.pop_back();
	QP y = Qp.back(); Qp.pop_back();
	QP& x = Qp.back();
	logTmp(x.end), logTmp(y.end), logTmp(z.end);
	int n = lenQ(y.start), m = lenQ(z.start);
	Quad* q1 = newQuad("取反跳转", to_string(n + 2), "", x.end->t);
	Quad* q2 = newQuad("无条件跳转", to_string(m + 1), "", "");
	x.end->next = q1, x.end = q1;
	x.end->next = y.start, x.end = y.end;
	x.end->next = q2, x.end = q2;
	x.end->next = z.start, x.end = z.end;
	delTmps();
}
void while_stmt() {
	Ta.push_back("");
	QP y = Qp.back(); Qp.pop_back();
	QP& x = Qp.back();
	logTmp(x.end), logTmp(y.end);
	int n = lenQ(x.start), m = lenQ(y.start);
	Quad* q1 = newQuad("取反跳转", to_string(m + 2), "", x.end->t);
	Quad* q2 = newQuad("无条件跳转", to_string(-n - m - 1), "", "");
	x.end->next = q1, x.end = q1;
	x.end->next = q2, x.end = q2;
	delTmps();
}
void dowhile_stmt() {
	Ta.push_back("");
	QP y = Qp.back(); Qp.pop_back();
	QP& x = Qp.back();
	logTmp(x.end), logTmp(y.end);
	int n = lenQ(x.start), m = lenQ(y.start);
	Quad* q = newQuad("条件跳转", to_string(-n - m), "", y.end->t);
	x.end->next = y.start, x.end = y.end;
	x.end->next = q, x.end = q;
	delTmps();
}
void return_stmt() {
	Ta.push_back("");
	Quad* q = newQuad("返回", "", "", "");
	if (Gn == 2) {
		Qp.push_back(QP{ q, q });
		return;
	}
	QP& x = Qp.back();
	logTmp(x.end);
	q->t = x.end->t;
	x.end->next = q, x.end = q;
	delTmps();
}
void expression() {
	Ta.push_back("");
	if (Gn == 1) return;
	QP x = Qp.back(); Qp.pop_back();
	QP y = Qp.back(); Qp.pop_back();
	Quad* q = newQuad("=", x.end->t, "", y.end->t);
	logTmp(x.end);
	x.end->next = y.start, x.end = y.end;
	x.end->next = q, x.end = q;
	Qp.push_back(x);
	delTmps();
}
void variable() {
	Ta.push_back("");
	Quad* q = newQuad("引用", Tb[0], "", Tb[0]);
	if (Gn == 1) {
		Qp.push_back({ q, q });
		return;
	}
	QP& x = Qp.back();
	q->y = x.end->t, q->t += '[' + q->y + ']';
	x.end->next = q, x.end = q;
}
void simple_expression() {
	Ta.push_back("");
	if (Gn == 1) return;
	QP y = Qp.back(); Qp.pop_back();
	QP& x = Qp.back();
	logTmp(x.end), logTmp(y.end);
	Quad* q = newQuad(Tb[1], x.end->t, y.end->t, newTmp());
	x.end->next = y.start, x.end = y.end;
	x.end->next = q, x.end = q;
	delTmps();
}
void relop() {
	Ta.push_back(Tb[0]);
}
void additive_expression() {
	Ta.push_back("");
	if (Gn == 1) return;
	QP y = Qp.back(); Qp.pop_back();
	QP& x = Qp.back();
	logTmp(x.end), logTmp(y.end);
	Quad* q = newQuad(Tb[1], x.end->t, y.end->t, newTmp());
	x.end->next = y.start, x.end = y.end;
	x.end->next = q, x.end = q;
	delTmps();
}
void addop() {
	Ta.push_back(Tb[0]);
}
void term() {
	Ta.push_back("");
	if (Gn == 1) return;
	QP y = Qp.back(); Qp.pop_back();
	QP& x = Qp.back();
	logTmp(x.end), logTmp(y.end);
	Quad* q = newQuad(Tb[1], x.end->t, y.end->t, newTmp());
	x.end->next = y.start, x.end = y.end;
	x.end->next = q, x.end = q;
	delTmps();
}
void mulop() {
	Ta.push_back(Tb[0]);
}
void factor() {
	Ta.push_back("");
	if (Gy.r[0] == "NUM") {
		Quad* q = newQuad("引用", Tb[0], "", Tb[0]);
		Qp.push_back(QP{ q, q });
	}
}
void call() {
	Ta.push_back("");
	string s;
	QP& x = Qp.back();
	Quad* p = x.start;
	while (p != nullptr) {
		if (p->op == "参数") {
			s += p->t + ", ";
			logTmp(p);
		}
		p = p->next;
	}
	if (!s.empty()) s.erase(s.end() - 2, s.end());
	Quad* q = newQuad("调用", Tb[0], s, Tb[0]);
	q->t += '(' + q->y + ')';
	x.end->next = q, x.end = q;
	delTmps();
}
void arguments() {
	Ta.push_back("");
	if (Gy.r[0] != EMPTY) return;
	Quad* x = newQuad("跳过", "", "", "");
	Qp.push_back(QP{ x, x });
}
void argument_list() {
	Ta.push_back("");
	if (Gn == 1) {
		QP& x = Qp.back();
		Quad* q = newQuad("参数", "", "", x.end->t);
		x.end->next = q, x.end = q;
		return;
	}
	QP y = Qp.back(); Qp.pop_back();
	QP& x = Qp.back();
	Quad* q = newQuad("参数", "", "", y.end->t);
	x.end->next = y.start, x.end = y.end;
	x.end->next = q, x.end = q;
}
void genToken() {
	trim();
	if (Gy.l == "program") program();
	else if (Gy.l == "definition-list") definition_list();
	else if (Gy.l == "definition") definition();
	else if (Gy.l == "variable-definition") variable_definition();
	else if (Gy.l == "type-indicator") type_indicator();
	else if (Gy.l == "function-definition") function_definition();
	else if (Gy.l == "parameters") parameters();
	else if (Gy.l == "parameter-list") parameter_list();
	else if (Gy.l == "parameter") parameter();
	else if (Gy.l == "compound-stmt") compound_stmt();
	else if (Gy.l == "local-definitions") local_definitions();
	else if (Gy.l == "statement-list") statement_list();
	else if (Gy.l == "statement") statement();
	else if (Gy.l == "expression-stmt") expression_stmt();
	else if (Gy.l == "condition-stmt") condition_stmt();
	else if (Gy.l == "while-stmt") while_stmt();
	else if (Gy.l == "dowhile-stmt") dowhile_stmt();
	else if (Gy.l == "return-stmt") return_stmt();
	else if (Gy.l == "expression") expression();
	else if (Gy.l == "variable") variable();
	else if (Gy.l == "simple-expression") simple_expression();
	else if (Gy.l == "relop") relop();
	else if (Gy.l == "additive-expression") additive_expression();
	else if (Gy.l == "addop") addop();
	else if (Gy.l == "term") term();
	else if (Gy.l == "mulop") mulop();
	else if (Gy.l == "factor") factor();
	else if (Gy.l == "call") call();
	else if (Gy.l == "arguments") arguments();
	else if (Gy.l == "argument-list") argument_list();
	else {
		cout << "未知文法左部\n";
		exit(1);
	}
}