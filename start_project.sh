#!/bin/bash

backend_name='mysql_site'
sniffer_name='sniffer'

base_dir=`pwd`
backend_dir=$base_dir"/"$backend_name
sniffer_dir=$base_dir"/src/"$sniffer_name

# 启动mongoDB
# mongod --dbpath /var/lib/mongo --logpath /var/log/mongodb/mongod.log --fork

#启动postgresql
systemctl stop postgresql
systemctl start postgresql

if [ $? -ne 0 ]; then
    echo "database start failed!"
else
    echo "success! postgresql have been started"    
fi

# 启动后端服务器
cd $backend_dir
flask --app main run & # 后台运行

if [ $? -ne 0 ]; then
    echo "flask start failed!"
else
    echo "success! backend have been started"    
fi

#启动抓包
cd $sniffer_dir
go env -w GO111MODULE=on && go build && ./sniffer & # 后台运行

if [ $? -ne 0 ]; then
    echo "sniffer start failed!"
else
    echo "success! sniffer have been started"   
fi