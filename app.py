from ui.globals import *
# 保持键值
for k in KEYS:
    if k in ses:
        ses[k] = ses[k]
if LIB_DIR is None:
    LIB_DIR = '.'

st.set_page_config('编译原理项目', page_icon=f'{LIB_DIR}/assets/logo.svg', layout='wide')
st.logo(f'{LIB_DIR}/assets/logo.svg', size='large')
pages = {
    '🅰️词法分析🛠️': [
        st.Page(f'{LIB_DIR}/ui/pages/1_regex_page.py', title='正则表达式分析', icon='1️⃣'),
        st.Page(f'{LIB_DIR}/ui/pages/2_lexer_page.py', title='源程序分析', icon='2️⃣')
    ],
    '🅱️语法分析🔗': [
        st.Page(f'{LIB_DIR}/ui/pages/3_gram_page.py', title='BNF文法分析', icon='1️⃣'),
        st.Page(f'{LIB_DIR}/ui/pages/4_tree_page.py', title='语法树定义', icon='2️⃣'),
        st.Page(f'{LIB_DIR}/ui/pages/5_parser_page.py', title='源程序分析', icon='3️⃣')
    ],
}
pg = st.navigation(pages)  # 导航栏
with st.sidebar.expander('个人信息', icon='👤'):
    for k, v in INFO.items():
        st.caption(f'**{k}**')
        st.caption(v)
pg.run()
