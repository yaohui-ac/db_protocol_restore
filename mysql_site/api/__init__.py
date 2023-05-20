import os
import importlib
from core.app import app

# 遍历所有当前目录下的模块，找到所有的蓝图对象，注册蓝图对象
# 蓝图对象存储在子模块下名为bp的变量中
# 例如：api/v1/__init__.py 中的 bp

# 获取当前目录下的所有模块
modules = os.listdir(os.path.dirname(__file__))
# 遍历所有模块
for module in modules:
    # 忽略__init__.py文件
    if module == '__init__.py':
        continue

    # 忽略.py文件
    if module[-3:] == '.py':
        # 导入模块
        module = importlib.import_module('api.' + module[:-3])
        # 判断模块是否有bp变量
        if not hasattr(module, 'bp'):
            continue
        # 注册蓝图
        app.register_blueprint(module.bp)
    # 判断是否为模块
    if not os.path.isdir(os.path.join(os.path.dirname(__file__), module)):
        continue
    # 导入模块
    module = importlib.import_module('api.' + module)
    # 判断模块是否有bp变量
    if not hasattr(module, 'bp'):
        continue
    # 注册蓝图
    app.register_blueprint(module.bp)