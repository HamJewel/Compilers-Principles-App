from pyecharts import options as Opt
from pyecharts.charts import Tree
from pandas import DataFrame as DF
from graphviz import Digraph
from ui.globals import *
import subprocess
import re


class Node:
    def __init__(self, data, header):
        self.state = data[0]
        self.id = data[1]
        self.cd = data[2]
        self.token = 'START' if self.state == '-' else data[3]
        data, header = data[4:], header[4:]
        self.next = [(data[i], header[i] if header[i] != r'\n' else r'\\n')
                     for i in range(len(data)) if data[i] != '']

    def __str__(self):
        return self.id if self.token == '' else f'{self.id}\n{self.token}'


def renderTree(data, font_size=14, title=None):
    tree = Tree().add(
        series_name='',
        data=data,
        orient='TB',
        initial_tree_depth=-1,  # 展开所有节点
        symbol='none',
        edge_fork_position='50%',  # 调整分叉位置
        label_opts=Opt.LabelOpts(
            position='top',
            vertical_align='middle',
            horizontal_align='center',
            font_size=font_size,
            formatter='{b}',
        )
    )
    tree.set_global_opts(title_opts=Opt.TitleOpts(title=title))
    return tree


class Output1:
    @staticmethod
    def execute():
        output = subprocess.check_output([REGEXER, REGEX, EXPORT_DIR])
        ses.lines = decode(output).strip('\n').split('\n')  # 按行分割输出结果
        ses.Lexer = read(LEXC)
        subprocess.run([COMPILER, LEXC, '-o', LEXER])

    @staticmethod
    def process():
        tables = [[], [], []]
        i = -1
        for line in ses.lines:
            if re.match('^---.*---$', line):
                i += 1
                continue
            tables[i].append(line.split('\t'))

        ses.NFA = DF(tables[0][1:], columns=tables[0][0])
        ses.DFA = DF(tables[1][1:], columns=tables[1][0])
        ses.MDFA = DF(tables[2][1:], columns=tables[2][0])

    @staticmethod
    def renderMDFA():
        graph = Digraph()
        graph.graph_attr['rankdir'] = 'TB'
        header = ses.MDFA.columns
        nodes = [Node(data, header) for data in ses.MDFA.values]
        # 添加节点
        for node in nodes:
            graph.node(node.id, str(node),
                       shape='doublecircle' if node.state == '+' else 'circle')  # (节点名称，节点标签)
        # 添加边
        for node in nodes:
            for n in node.next:
                graph.edge(node.id, n[0], n[1])  # (起点，终点，边标签)
        ses.MDFAG = graph

    def run(self, text):
        save_text(text, REGEX)
        self.execute()
        self.process()
        self.renderMDFA()


class Output2:
    @staticmethod
    def execute():
        subprocess.run([LEXER, CODE, LEX])

    @staticmethod
    def process():
        lines = read(LEX).strip('\n').split('\n')
        data = [line.split('\t') for line in lines]
        ses.Lex = DF(data, columns=['单词编码', '单词内容']).fillna('')

    def run(self, text):
        save_text(text, CODE)
        self.execute()
        self.process()


class Output3:
    END = '---END---'  # 行结束标志
    k = 0

    @staticmethod
    def execute():
        output = subprocess.check_output([LALRER, BNF, EXPORT_DIR])
        ses.lines = decode(output).strip('\n').split('\n')  # 按行分割输出结果

    def getFSet(self, key):
        data = []
        while ses.lines[self.k] != self.END:
            tmp = ses.lines[self.k].split('\t')
            x, y = tmp[0], tmp[1] if len(tmp) > 1 else ''
            data.append([x, y.replace(' ', ', ')])
            self.k += 1
        ses[key] = DF(data, columns=['X', f'{key}(X)'])

    def getItemSet(self, key):
        data = []
        x = ''
        while ses.lines[self.k] != self.END:
            if not re.match(r'--Item\[(.*?)]--', ses.lines[self.k]):
                x += ses.lines[self.k].replace('->', '→', 1) + '\n'
            elif x != '':
                data.append(x.strip())
                x = ''
            self.k += 1
        data.append(x.strip())
        ses[key] = data

    def getEdges(self, key):
        data = []
        while ses.lines[self.k] != self.END:
            x = ses.lines[self.k].split('\t')
            data.append(x)
            self.k += 1
        ses[key] = data

    def getTable(self, key):
        data = []
        while ses.lines[self.k] != self.END:
            x = ses.lines[self.k].split('\t')
            if len(x) == 3:
                data.append(x)
            self.k += 1
        ses[key] = data

    @staticmethod
    def mergeTerms():  # 合并LR(1)项目中的终结符，压缩项目显示长度
        data = []
        for v in ses.LRIS:
            d = {}
            grams = v.split('\n')
            for gram in grams:
                i = gram.rfind(', ')
                core, term = gram[:i], gram[i + 2:]
                if core not in d:
                    d[core] = []
                d[core].append(term)
            x = ''
            for r, t in d.items():
                t.sort()
                x += f"{r}, {'/'.join(t)}\n"
            data.append(x.strip())
        ses.LRIS = data

    @staticmethod
    def toSparse(n, key):
        m = {}
        for _, v, _ in ses[key]:
            m[v] = m.get(v, len(m) + 1)
        header = ['编号'] + list(m.keys())
        data = DF(None, index=range(n), columns=header)
        data['编号'] = range(data.shape[0])
        for x, v, y in ses[key]:
            data.iat[int(x), m[v]] = y
        return data.fillna('')

    def process(self):
        func = [self.getFSet, self.getFSet, self.getItemSet, self.getEdges,
                self.getItemSet, self.getEdges, self.getTable]
        keys = ['First', 'Follow', 'LRIS', 'LRDFA', 'LAIS', 'LADFA', 'DTB']
        self.k = 1
        for f, m in zip(func, keys):
            f(m)
            self.k += 2
        self.mergeTerms()

        data = read(GRAMS).strip().split('\n')
        ses.grams = [x.replace(' ', ' → ', 1) for x in data]
        data = [x.split(' ') for x in ses.grams]
        cols = max(len(x) for x in data)
        for i in range(len(data)):
            data[i] = [i] + data[i] + [''] * (cols - len(data[i]))
        header = ['编号/下标', '0', ''] + list(map(str, (range(1, cols - 1))))
        ses.Grams = DF(data, columns=header)
        ses.LRDFA = self.toSparse(len(ses.LRIS), 'LRDFA')
        ses.LADFA = self.toSparse(len(ses.LAIS), 'LADFA')
        ses.LRDFA.insert(1, '状态', ses.LRIS)
        ses.LADFA.insert(1, '状态', ses.LAIS)
        ses.STB = self.toSparse(len(ses.LAIS), 'DTB')
        ses.DTB = DF(ses.DTB, columns=['状态', '接收', '动作'])

    def run(self, text):
        save_text(text, BNF)
        self.execute()
        self.process()


class Output4:
    _map = {'[': '[]', ']': '[]', '(': '()', ')': '()', '}': '{}', '{': '{}'}

    def map(self, s):
        return self._map.get(s, s)

    @staticmethod
    def process(text):
        ses.lines = [line.split(' ') for line in text.split('\n')]
        lines = []
        for i in range(len(ses.lines)):
            if ses.lines[i][0] == '':
                continue
            data = [int(ses.lines[i][0])]
            for j in range(1, len(ses.lines[i])):
                data.append(tuple(map(int, ses.lines[i][j].split('-'))))
            lines.append(data)
        ses.lines = lines[:min(len(ses.Grams), len(ses.lines))]

    def render(self):
        trees = []
        for i in range(len(ses.lines)):
            gram, line = ses.Grams.iloc[i].drop(1), ses.lines[i]
            nodes = {line[0]: {'name': self.map(gram[line[0]]), 'children': []}}
            for x, y in line[1:]:
                if x not in nodes:
                    nodes[x] = {'name': self.map(gram[x]), 'children': []}
                if y not in nodes:
                    nodes[y] = {'name': self.map(gram[y]), 'children': []}
                nodes[x]['children'].append(nodes[y])
            trees.append(renderTree([nodes[line[0]]]))
        ses.Trees = trees

    def run(self, text):
        save_text(text, TREE)
        self.process(text)
        self.render()


class Output5:
    k = 0

    @staticmethod
    def execute():
        output = subprocess.check_output([PARSER, GRAMS, TABLE, MAP, LEX, TREE])
        ses.lines = decode(output).strip('\n').split('\n')  # 按行分割输出结果

    def generate(self, state):
        data = []
        while self.k < len(ses.lines):
            depth = ses.lines[self.k].count(' ')
            if depth == state:
                data.append(ses.lines[self.k].strip())
                self.k += 1
            elif depth > state:
                data.append(self.generate(depth))
            else:
                return data
        return data

    def trim(self, data):
        nodes = []
        for node in data:
            if isinstance(node, str):
                nodes.append({'name': node, 'children': []})
            else:
                nodes[-1]['children'].extend(self.trim(node))
        return nodes

    def render(self):
        data = self.generate(0)
        nodes = self.trim(data)
        ses.TreeMap = renderTree(nodes, 12)

    def process(self):
        n = ses.lines.index('---Tree---')
        ses.Steps = [[i // 4 + 1, ses.lines[i + 1][5:], ses.lines[i + 2][5:-1].replace(' ', ' → ', 1),
                      ses.lines[i + 3]] for i in range(1, n, 4)]
        ses.Steps = DF(ses.Steps, columns=['步骤', '分析栈', '输入', '动作'])
        ses.lines = ses.lines[n + 1:]
        self.k = 0
        rows, cols = len(ses.lines), []
        for line in ses.lines:
            cols.append(line.count(' ') + 1)
        ses.TreeList = DF(None, index=range(rows), columns=range(max(cols))).fillna('')
        for i in range(rows):
            ses.TreeList.iat[i, cols[i] - 1] = ses.lines[i].strip()

    def run(self, text):
        save_text(text, CODE)
        self.execute()
        self.process()
        self.render()


class Output6:
    @staticmethod
    def execute():
        output = subprocess.check_output([INTERMER, LEX])
        ses.lines = decode(output).strip('\n').split('\n')  # 按行分割输出结果

    @staticmethod
    def process():
        data = [line.split('\t') for line in ses.lines]
        ses.Interm = DF(data, columns=['编号/四元组', '运算符', '对象A', '对象B', '目标'])

    def run(self):
        self.execute()
        self.process()


output1 = Output1()  # 全局变量
output2 = Output2()  # 全局变量
output3 = Output3()  # 全局变量
output4 = Output4()  # 全局变量
output5 = Output5()  # 全局变量
output6 = Output6()  # 全局变量
