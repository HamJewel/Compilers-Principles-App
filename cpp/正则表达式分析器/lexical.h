#pragma once
#include "globals.h"
void genHeader() {
    LexCode +=
        "#include <stdio.h>\n"
        "#include <stdlib.h>\n"
        "#include <string.h>\n"
        "#include <stdbool.h>\n"
        "#define MaxCd " + to_string(CdCnt) + "\n"
        "int n, m;\n"
        "bool flag;\n"
        "char c, str[128];\n"
        "int curr[MaxCd], next[MaxCd];\n"
        "FILE* input, * output;\n";
}
void genState() {
    LexCode +=
        "struct State { int cd, pr; }\n"
        "S[] = {";
    int k = 0;
    for (auto& x : MDfaList) {
        if (k++ % 5 == 0) LexCode += "\n\t";
        LexCode += "{" + to_string(x.cd) + ", " + to_string(CdPrec[x.cd]) + "}, ";
    }
    LexCode.erase(LexCode.end() - 2, LexCode.end());
    LexCode += "\n};\n";
}
void genTools() {
    LexCode +=
        "void append() {\n"
        "\tsize_t len = strlen(str);\n"
        "\tstr[len] = c, str[len + 1] = '\\0';\n"
        "}\n"
        "int get_cd() {\n"
        "\tint cd = 0, pr = -1;\n"
        "\tfor (int i = 0; i < n; ++i)\n"
        "\t\tif (S[curr[i]].pr > pr)\n"
        "\t\t\tcd = S[curr[i]].cd, pr = S[curr[i]].pr;\n"
        "\treturn cd;\n"
        "}\n"
        "bool has_end(int id) {\n"
        "\tfor (int i = 0; i < n; ++i)\n"
        "\t\tif (curr[i] != id && S[curr[i]].cd > 0)\n"
        "\t\t\treturn true;\n"
        "\treturn false;\n"
        "}\n";
}
void genIsCharSet() {
    for (const auto& p : CharSet) {
        auto v = sortCharSet(p.second);
        string code0, code1;
        LexCode +=
            "bool is_" + p.first + "() {\n"
            "\treturn";
        for (int i = 0; i < v[0].size(); i += 2)
            code0 += " (c >= \'" + escape(v[0][i]) + "\' && c <= \'" + escape(v[0][i + 1]) + "\') ||";
        if (!code0.empty()) code0.erase(code0.end() - 3, code0.end());
        if (!v[1].empty()) {  // 单字符
            for (const auto& c : v[1])
                code1 += "|| c == \'" + escape(c) + "\' ";
            code1.pop_back();
        }
        if (code0.empty()) code1.erase(code1.begin(), code1.begin() + 2);
        else if (!code1.empty()) code0 += "\n\t\t";
        LexCode += code0, LexCode += code1;
        LexCode +=
            ";\n"
            "}\n";
    }
}
void genCores() {
    LexCode +=
        "void new_state() {\n"
        "\tstr[0] = '\\0';\n"
        "\tn = 1, curr[0] = " + to_string(MDfaStart) + ";\n"
        "}\n"
        "void end_state() {\n"
        "\tint cd = get_cd();\n"
        "\tswitch (cd) {\n"
        "\tcase 0: return;\n";
    for (auto& x : MainCd[1])
        LexCode += "\tcase " + to_string(x) + ": break;\n";
    for (auto& x : MainCd[0])
        LexCode += "\tcase " + to_string(x) + ": fprintf(output, \"%d\\t%s\\n\", cd, str); break;\n";
    LexCode +=
        "\tdefault: fprintf(output, \"%d\\n\", cd);\n"
        "\t}\n"
        "\tungetc(c, input);\n"
        "\tn = 0;\n"
        "}\n"
        "bool next_state(int id) {\n"
        "\tint k = m, x = -1;\n"
        "\tswitch (id) {\n";
    for (auto& x : MDfaList) {
        int k = 0;
        string anyCode, setCode, charCode;
        for (const auto& p : x.trans) {
            if (p.second == -1) continue;
            string m = to_string(p.second);
            if (p.first == ANY) {
                if (x.id == p.second) anyCode += "\t\tif (has_end(id)) return false;\n";
                anyCode += "\t\tnext[m++] = " + m + ";\n";
            }
            else if (CharSetSym.find(p.first) != CharSetSym.end())
                setCode += "\t\tif (is_" + C2S[p.first] + "()) next[m++] = " + m + ";\n";
            else {
                if (charCode.empty()) charCode += "\t\tswitch (c) {\n";
                string n = C2S.find(p.first) != C2S.end() ? trim(C2S[p.first]) : STR(p.first);
                charCode += "\t\tcase '" + n + "': next[m++] = " + m + "; break;\n";
            }
        }
        if (!anyCode.empty() || !setCode.empty() || !charCode.empty()) {
            if (!charCode.empty()) charCode += "\t\t}\n";
            LexCode += "\tcase " + to_string(x.id) + ":\n";
            LexCode += anyCode, LexCode += setCode, LexCode += charCode;
            LexCode += "\t\tbreak;\n";
        }
    }
    LexCode +=
        "\t}\n"
        "\treturn k != m;\n"
        "}\n";
}
void genMain() {
    LexCode +=
        "void coding() {\n"
        "\twhile (c = fgetc(input)) {\n"
        "\t\tif (n == 0) new_state();\n"
        "\t\tm = 0, flag = false;\n"
        "\t\tfor (int i = 0; i < n; ++i)\n"
        "\t\t\tflag |= next_state(curr[i]);\n"
        "\t\tflag ? append() : end_state();\n"
        "\t\tn = m;\n"
        "\t\tfor (int i = 0; i < n; ++i)\n"
        "\t\t\tcurr[i] = next[i];\n"
        "\t\tif (c == EOF) break;\n"
        "\t}\n"
        "\tfprintf(output, \"0\");\n"
        "}\n"
        "int main(int argc, char* argv[]) {\n"
        "\tif (argc < 3) exit(1);\n"
        "\tinput = fopen(argv[1], \"r\");\n"
        "\tif (input == NULL) {\n"
        "\t\tprintf(\"Failed to open input file\");\n"
        "\t\texit(1);\n"
        "\t}\n"
        "\toutput = fopen(argv[2], \"w\");\n"
        "\tif (output == NULL) {\n"
        "\t\tprintf(\"Failed to open output file\");\n"
        "\t\tfclose(input);\n"
        "\t\texit(1);\n"
        "\t}\n"
        "\tcoding();\n"
        "\tfclose(input);\n"
        "\tfclose(output);\n"
        "\treturn 0;\n"
        "}";
}
// 生成词法分析源程序
void genLexer() {
    genHeader(), genState(), genTools();
    genIsCharSet(), genCores(), genMain();
}
