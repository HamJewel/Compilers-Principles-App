from streamlit import session_state as ses
import streamlit as st
import os

LIB_DIR = '.'
EXPORT_DIR = './data/output'
REGEX = f'{EXPORT_DIR}/Regex.txt'
MAP = f'{EXPORT_DIR}/Map.txt'
CODE = f'{EXPORT_DIR}/Code.txt'
BNF = f'{EXPORT_DIR}/BNF.txt'
GRAMS = f'{EXPORT_DIR}/Grams.txt'
TABLE = f'{EXPORT_DIR}/Table.txt'
TREE = f'{EXPORT_DIR}/Tree.txt'
LEX = f'{EXPORT_DIR}/Lex.lex'
LEXC = f'{EXPORT_DIR}/Lexer.c'
COMPILER = 'g++'
REGEXER = './bin/正则表达式分析器'
LEXER = './bin/词法分析器'
LALRER = './bin/LALR(1)分析器'
PARSER = './bin/语法分析器'
INTERMER = './bin/中间代码分析器'

os.makedirs(EXPORT_DIR, exist_ok=True)
RULE1 = [r'1️⃣ 如果与正则表达式符号冲突(+、|、(、)、*、?、~)，用\进行转义(如\+)',
         r'2️⃣ 字符集合无需前缀_，如果要表示实际减号需要转义(即\-)',
         '3️⃣ 大类编码正则式前缀为1个_，对整个正则式编1个号',
         '4️⃣ 子类编码正则式前缀为2个_，按|分开独立编号各个单词',
         '5️⃣ 若正则式不需要编码(如注释)，则前缀为1个_加1个!，即_!',
         '6️⃣ ~表示任意字符，空格和制表符会被丢弃处理，输入无效']
RULE2 = ['1️⃣ 每个文法Token之间要用空格分隔(->前后可以没有空格)',
         '2️⃣ 输入的文法规则应确保有统一的归约出口，即无需再扩充开始文法']
RULE3 = ['1️⃣ 第i行的输入是第i个文法规则的语法树结构',
         '2️⃣ 输入的数字代表文法规则中Token的下标，可在输出中对应查找',
         '3️⃣ 每行的第一个数字表示作为根节点的Token下标',
         '4️⃣ 之后空格分隔，输入若干个数字对x-y，多个数字对间也用空格分隔',
         '5️⃣ 数字对x-y表示下标x的Token是下标y的父节点']
INFO = {'✍️姓名': '黄俊銓', '🏫班级': '2022计科2', '🆔学号': '20222131035', '💻内核编程语言': 'C/C++',
        '🎨UI设计框架': 'Streamlit'}
KEYS = [
    'txt1', 'lc1', 'NFA', 'DFA', 'MDFA', 'MDFAG', 'Lexer', 'rankdir',
    'txt2', 'lc2', 'Lex',
    'txt3', 'lc3', 'grams', 'Grams', 'First', 'Follow', 'LRDFA', 'LADFA', 'STB', 'DTB',
    'txt4', 'lc4', 'Trees',
    'txt5', 'lc5', 'interm', 'TreeList', 'TreeMap', 'Steps', 'K', 'Interm'
]


def decode(data):
    try:
        text = data.decode('utf-8')
    except UnicodeDecodeError:
        text = data.decode('gbk')
    return text.replace('\r', '')  # 去除\r换行符(避免错误读入)


def read(path):
    with open(path, 'rb') as f:
        data = f.read()
    return decode(data)


def save_text(text, path):
    with open(path, 'w', encoding='utf-8') as f:
        f.write(text)


@st.dialog('保存文本框内容为文本文件')
def save_web_text(text):
    col1, col2 = st.columns([2, 1], vertical_alignment='bottom')
    name = col1.text_input('输入文件名', value='.txt').strip()
    col2.download_button('下载', icon='📥', type='primary', use_container_width=True, file_name=name, data=text)


def input_module(page, content, label='开始分析', icon='🧑‍💻'):
    st.subheader('输入')
    col1, col2 = st.columns([2, 1])
    fu = col2.file_uploader('📤上传文件')
    load = col2.button('加载文件', icon='🗃️', disabled=not fu, use_container_width=True)
    save = col2.button('保存文件', icon='💾', use_container_width=True)
    start = col2.button(label, icon=icon, type='primary', use_container_width=True)
    if load:
        ses[f'txt{page}'] = decode(fu.read())
    txt = col1.text_area(f'⌨️输入{content}', key=f'txt{page}', height=200).strip()
    if save:
        save_web_text(txt)
    col3 = col1.columns(2)
    if col3[0].checkbox('🔡忽略大小写', key=f'lc{page}'):
        txt = txt.lower()
    st.divider()
    return col1, col3[1], start, txt
