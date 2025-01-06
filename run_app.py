import streamlit.web.cli as stcli
from ui import globals
import sys
import os

if __name__ == '__main__':
    globals.LIB_DIR = sys._MEIPASS if getattr(sys, 'frozen', False) else os.path.dirname(__file__)
    print(f'Library Directory: {globals.LIB_DIR}')
    sys.argv = [
        'streamlit',
        'run',
        f'{globals.LIB_DIR}/app.py',
        '--global.developmentMode=false'
    ]
    sys.exit(stcli.main())
# datas = [('D:/miniconda3/envs/Streamlit/Lib/site-packages/pyecharts', './pyecharts')]  附加库路径
# pyinstaller run_app.spec --clean 根据记录信息快速打包
