# 拉取最新代码
apt install supervisor -y

# 安装依赖
pip3 install -r requirements.txt -i https://pypi.tuna.tsinghua.edu.cn/simple

pip3 install flask

# 判断是否已有supervisor配置文件，如果没有则创建
if [ ! -f "/etc/supervisor/conf.d/project.conf" ]; then
    # 没有则复制supervisor_project.conf到/etc/supervisor/conf.d/project.conf
    cp ./supervisor_project.conf /etc/supervisor/conf.d/project.conf
else 
    # 有则删除/etc/supervisor/conf.d/project.conf
    rm /etc/supervisor/conf.d/project.conf
    # 复制supervisor_project.conf到/etc/supervisor/conf.d/project.conf
    cp ./supervisor_project.conf /etc/supervisor/conf.d/project.conf
fi

unlink /var/run/supervisor.sock

# 启动supervisor
/usr/bin/supervisord -c /etc/supervisor/supervisord.conf