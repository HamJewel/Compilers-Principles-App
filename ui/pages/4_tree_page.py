from ui.output import *
from streamlit_echarts import st_pyecharts

_, _, start, txt = input_module(4, '语法树结构', '定义语法树', '🧩')

if start:
    st.toast('定义中...', icon='⏳')
    output4.run(txt)
    st.toast(':rainbow[定义完成！]', icon='🎉')

st.subheader('输出')
tab1, tab2 = st.tabs(['文法产生式', '语法树结构'])

if ses.get('Grams', None) is not None:
    tab1.dataframe(ses.Grams, hide_index=True, use_container_width=True)
if ses.get('Trees', None) is not None:
    n = 3
    for i, tree in enumerate(ses.Trees):
        if i % n == 0:
            cols = tab2.columns(n, border=True)
        with cols[i % n]:
            st.text(f'[{i}] {ses.grams[i]}')
            st_pyecharts(tree, key=f'tree{i}')

