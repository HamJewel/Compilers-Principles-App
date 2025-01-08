//*****************************************************************************
// 版权信息：
// 版权所有 (C) 2024 [黄俊銓]。保留所有权利。
//*****************************************************************************
// 文件名称：main.cpp
// 模块功能说明：
// 该模块定义了调用所需参数和对外功能接口。
//
// 华南师范大学-计算机学院-计算机科学与技术-2班
// 作者：黄俊銓-20222131035
// 完成日期：2024年12月3日
//*****************************************************************************
#include "output.h"
int main(int argc, char* argv[]) {  // 参数：是否区分大小写、正则表达式路径、源程序路径(可选)
	if (argc < 3) {  // 没有文件路径参数传入，只进行编译
		cout << "Usage: " << argv[0] << " <filename>\n";
		exit(0);
	}
	ifstream regexFile(argv[1]);  // 打开正则表达式文件
	if (!regexFile.is_open()) {
		cout << "Error opening regex file\n";
		exit(1);
	}
	stringstream buffer;
	buffer << regexFile.rdbuf();
	InRegex = buffer.str();
	regexFile.close();  // 关闭文件
	Run();
	Print();
	Export(argc == 3 ? argv[2] : ".");
	return 0;
}