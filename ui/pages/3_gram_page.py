from ui.output import *


_, _, start, txt = input_module(3, 'BNF文法')

if start:
    st.toast('分析中...', icon='⏳')
    output3.run(txt)
    st.toast(':rainbow[分析完成！]', icon='🎉')

st.subheader('输出')
tabs = st.tabs(['文法产生式', 'First集合', 'Follow集合', 'LR(1)DFA', 'LALR(1)DFA', 'LALR(1)分析表'])

for tab, key in zip(tabs, ['Grams', 'First', 'Follow', 'LRDFA', 'LADFA', 'STB']):
    if ses.get(key, None) is None:
        continue
    if key == 'LRDFA' or key == 'LADFA':
        tab.dataframe(ses[key], hide_index=True, use_container_width=True)
    elif key == 'STB':
        for t, k in zip(tab.tabs(['稀疏表', '稠密表']), ['STB', 'DTB']):
            t.dataframe(ses[k], hide_index=True, use_container_width=True)
    else:
        tab.dataframe(ses[key], hide_index=True, use_container_width=True)
