from streamlit import session_state as ses
import streamlit as st
import os

LIB_DIR = None
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
TCC = './bin/tcc/tcc.exe'
REGEXER = './bin/正则表达式分析器.exe'
LEXER = './bin/词法分析器.exe'
LALRER = './bin/LALR(1)分析器.exe'
PARSER = './bin/语法分析器.exe'


os.makedirs(EXPORT_DIR, exist_ok=True)
RULES = r"""☆ 如果与正则表达式符号冲突(+、|、(、)、*、?、~)，用\进行转义(如\+)
☆ 字符集合无需前缀_，如果要表示实际减号需要转义(即\-)
☆ 大类编码正则式加1个前缀_，对整个正则式编1个号
☆ 子类编码正则式加2个前缀_，按|分开独立编号各个单词
☆ 若正则式不需要编码(如注释)，则前缀为1个_加1个!，即_!
☆ ~表示任意字符，空格和制表符会被丢弃处理，输入无效"""
INFO = {'✍️姓名': '黄俊銓', '🏫班级': '2022计科2', '🆔学号': '20222131035', '💻内核编程语言': 'C/C++', '🎨UI设计框架': 'Streamlit'}
KEYS = [
    'txt1', 'lc1', 'NFA', 'DFA', 'MDFA', 'MDFAG', 'Lexer', 'rankdir',
    'txt2', 'lc2', 'Lex',
    'txt3', 'lc3', 'Grams', 'First', 'Follow', 'LRDFA', 'LADFA', 'STB', 'DTB',
    'txt4', 'lc4', 'grams', 'Trees',
    'txt5', 'lc5', 'TreeList', 'TreeMap', 'Steps', 'K'
]


def read(path):
    try:
        with open(path, 'r', encoding='utf-8') as f:
            text = f.read()
    except UnicodeDecodeError:
        with open(path, 'r', encoding='gbk') as f:
            text = f.read()
    return text


def decode(data):
    try:
        text = data.decode('utf-8')
    except UnicodeDecodeError:
        text = data.decode('gbk')
    return text


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
        ses[f'txt{page}'] = decode(fu.read()).replace('\r', '')  # 去除\r换行符(避免错误读入)
    txt = col1.text_area(f'⌨️输入{content}', key=f'txt{page}', height=200).strip()
    if save:
        save_web_text(txt)
    if col1.checkbox('🔡忽略大小写', key=f'lc{page}'):
        txt = txt.lower()
    st.divider()
    return start, txt
