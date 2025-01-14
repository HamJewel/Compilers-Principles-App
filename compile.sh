sources=("正则表达式分析器" "LALR(1)分析器" "语法分析器" "中间代码分析器")
for X in "${sources[@]}"; do
    g++ -I ./cpp/"$X" ./cpp/"$X"/main.cpp -o ./bin/"$X"
done