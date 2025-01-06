from ui.output import *
from streamlit_echarts import st_pyecharts


start, txt = input_module(5, '源程序代码')

if start:
    st.toast('分析中...', icon='⏳')
    output2.run(txt)
    st.toast('Lex文件已生成！', icon='✅')
    output5.run(txt)
    st.toast(':rainbow[分析完成！]', icon='🎉')

st.subheader('输出')
tab1, tab2 = st.tabs(['分析过程', '语法树'])
t1, t2 = tab2.tabs(['树状表', '树形图'])
t2.button('点击加载', icon='▶️', type='primary')

if ses.get('Steps', None) is not None:
    tab1.dataframe(ses.Steps, hide_index=True, use_container_width=True)
if ses.get('TreeList', None) is not None:
    t1.dataframe(ses.TreeList)
if ses.get('TreeMap', None) is not None:
    ses.K = 1 - ses.get('K', 0)
    with t2:
        st_pyecharts(ses.TreeMap, key=f'_TreeMap{ses.K}')
