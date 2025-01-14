from ui.output import *


_, _, start, txt = input_module(1, '正则表达式')

if start:
    st.toast('分析中...', icon='⏳')
    output1.run(txt)
    st.toast('词法分析器已生成！', icon='✅')
    st.toast(':rainbow[分析完成！]', icon='🎉')

st.subheader('输出')
tabs = st.tabs(['NFA', 'DFA', '最小DFA', '最小DFA图', '词法分析源程序'])
tabs[3].segmented_control('方向', ['上下⬇️', '左右➡️'], key='rankdir', default='上下⬇️')

for tab, key in zip(tabs, ['NFA', 'DFA', 'MDFA', 'MDFAG', 'Lexer']):
    if ses.get(key, None) is None:
        continue
    if key == 'Lexer':
        tab.code(ses[key], language='c', line_numbers=True)
    elif key == 'MDFAG':
        ses[key].graph_attr['rankdir'] = 'TB' if ses.rankdir == '上下⬇️' else 'LR'
        tab.graphviz_chart(ses[key])
    else:
        tab.dataframe(ses[key], hide_index=True)
