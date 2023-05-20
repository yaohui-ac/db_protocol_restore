# 数据库数据统计分析系统

## 部署说明

1. 准备好数据库，pogresql数据库,配置文件在'core/settings.py'中

2. 安装依赖包

   ```shell
   pip install -r requirements.txt
   ```

3. 测试数据在'core/__init__.py'中，可以自行修改, 如不需要测试数据，注释掉即可

4. 运行

   ```shell
   flask --app main run
   ```