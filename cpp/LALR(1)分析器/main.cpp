//*****************************************************************************
// 版权信息：
// 版权所有 (C) 2024 [黄俊銓]。保留所有权利。
//*****************************************************************************
// 文件名称：main.cpp
// 模块功能说明：
// 该模块提供对外执行接口，接收BNF文法路径，打印输出结果文件。
//
// 华南师范大学-计算机学院-计算机科学与技术-2班
// 作者：黄俊銓-20222131035
// 完成日期：2024年12月3日
//*****************************************************************************
#include "output.h"
int main(int argc, char* argv[]) {
	if (argc < 2) {  // 没有文件路径参数传入，只进行编译
		cout << "Usage: " << argv[0] << " <filename>\n";
		exit(0);
	}
	string path = argv[1];  // 文法文件路径
	ifstream file(path);  // 打开文件
	if (!file.is_open()) {
		cout << "Error opening file\n";
		exit(0);
	}
	string line;
	while (getline(file, line)) {
		if (line.empty()) continue;  // 跳过空行
		lines.push_back(line);
	}
	file.close();  // 关闭文件
	Run();  // 文法分析
	Print();  // 打印结果
	Export(argc == 3 ? argv[2] : ".");
	return 0;
}