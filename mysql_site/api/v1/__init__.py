import importlib
import os
from flask import (
    Blueprint
)

# 获取当前模块名
current_module_name = os.path.split(os.path.dirname(__file__))[-1]
bp = Blueprint(current_module_name, __name__, url_prefix=f'/{current_module_name}')

# 设置蓝图template_folder参数
# bp.template_folder = 'templates/v1'

# 遍历所有当前目录下的模块，找到所有的蓝图对象，注册蓝图对象
# 蓝图对象存储在子模块下名为bp的变量中
# 例如：api/v1/index.py 中的 bp

# 获取当前目录下的所有模块
modules = os.listdir(__path__[0])
# 遍历所有模块
for module in modules:
    # 忽略__init__.py文件
    if module == '__init__.py':
        continue
    # 判断是否为文件夹
    if not os.path.isdir(os.path.join(__path__[0], module)):
        # 忽略非.py文件
        if module[-3:] != '.py':
            continue
        # 导入模块
        _module = importlib.import_module(f"api.{current_module_name}.{module[:-3]}")
        # 判断模块是否有bp变量
        if not hasattr(_module, 'bp'):
            continue
        bp.register_blueprint(_module.bp, name=f"{module[:-3]}",url_prefix=f'/{module[:-3] if module != "index.py" else ""}', strict_slashes=False)
    else:
        # 导入模块
        _module = importlib.import_module(f"api.{current_module_name}.{module}")
        # 判断模块是否有bp变量
        if not hasattr(_module, 'bp'):
            continue
        # 注册蓝图
        bp.register_blueprint(_module.bp, name=f"{module}",url_prefix=f'/{module}', strict_slashes=False)

__all__ = [
    'bp'
]