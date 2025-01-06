from ui.output import *


start, txt = input_module(2, '源程序代码')

if start:
    st.toast('分析中...', icon='⏳')
    output2.run(txt)
    st.toast(':rainbow[分析完成！]', icon='🎉')

st.subheader('输出')
tab = st.tabs(['Lex文件'])[0]

if ses.get('Lex', None) is not None:
    tab.dataframe(ses.Lex, hide_index=True, use_container_width=True)
