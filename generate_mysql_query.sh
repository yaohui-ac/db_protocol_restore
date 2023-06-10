#!/bin/bash
mysql_host='rm-bp13ld0589xu122erko.mysql.rds.aliyuncs.com'
mysql_port=3306
user_name='yaohui'
passwd='yaohui_123'
function execute_create_table() {

    mysql -h$mysql_host -P$mysql_port -u$user_name -p$passwd test_db_01 -e "select 1"
    sleep 1
    mysql -h$mysql_host -P$mysql_port -u$user_name -p$passwd test_db_01 -e "use test_db_01;show tables;create table if not exists test_table1(id int, name text);"
    sleep 1
    mysql -h$mysql_host -P$mysql_port -u$user_name -p$passwd test_db_01 -e "use test_db_01;show tables;create table if not exists test_table2(id int, name text);"
    sleep 1
}

function execute_insert_data() {
    mysql -h$mysql_host -P$mysql_port -u$user_name -p$passwd test_db_01 -e "select 1"
    sleep 1
    insert_cmd_1="insert into test_db_01.test_table1(id, name) values"
    insert_cmd_2="insert into test_db_01.test_table2(id, name) values"
    for i in {1..30}
    do
        insert_cmd_tmp_1=$insert_cmd_1"("$i", 'kfcvivo50')"
        insert_cmd_tmp_2=$insert_cmd_2"("$i", '1145141919810')"
        mysql -h$mysql_host -P$mysql_port -u$user_name -p$passwd test_db_01 -e "$insert_cmd_tmp_1"
        sleep 1
        mysql -h$mysql_host -P$mysql_port -u$user_name -p$passwd test_db_01 -e "$insert_cmd_tmp_2"
        sleep 1
    done

}
function execute_update_data() {
    mysql -h$mysql_host -P$mysql_port -u$user_name -p$passwd test_db_01 -e "select 1"
    sleep 1

    for i in {1..10}
    do
        update_cmd_1="update test_table1 set name='ikun' where id=$i"
        update_cmd_2="update test_table2 set name='yaohui' where id=$i"
        mysql -h$mysql_host -P$mysql_port -u$user_name -p$passwd test_db_01 -e "$update_cmd_1"
        sleep 1
        mysql -h$mysql_host -P$mysql_port -u$user_name -p$passwd test_db_01 -e "$update_cmd_2"
        sleep 1
    done
}
function execute_select_data() {
    mysql -h$mysql_host -P$mysql_port -u$user_name -p$passwd test_db_01 -e "select 1"
    sleep 1

    for i in {1..10}
    do
        select_cmd_1="select * from test_table1 where id=$i"
        select_cmd_2="select id from test_table1 where id=$i"
        mysql -h$mysql_host -P$mysql_port -u$user_name -p$passwd test_db_01 -e "$select_cmd_1"
        sleep 1
        mysql -h$mysql_host -P$mysql_port -u$user_name -p$passwd test_db_01 -e "$select_cmd_1"
        sleep 1
    done
}
function execute_delete_data() {
    mysql -h$mysql_host -P$mysql_port -u$user_name -p$passwd test_db_01 -e "select 1"
    sleep 1

    for i in {1..10}
    do
        select_cmd_1="delete from test_table1 where id=$i"
        select_cmd_2="delete from test_table1 where id=$i"
        mysql -h$mysql_host -P$mysql_port -u$user_name -p$passwd test_db_01 -e "$select_cmd_1"
        sleep 1
        mysql -h$mysql_host -P$mysql_port -u$user_name -p$passwd test_db_01 -e "$select_cmd_1"
        sleep 1
    done
}

function execute_truncate_table() {
    mysql -h$mysql_host -P$mysql_port -u$user_name -p$passwd test_db_01 -e "select 1"
    sleep 1
    mysql -h$mysql_host -P$mysql_port -u$user_name -p$passwd test_db_01 -e "truncate table test_table1"
    sleep 1
    mysql -h$mysql_host -P$mysql_port -u$user_name -p$passwd test_db_01 -e "truncate table test_table2"
    sleep 1
}
function execute_drop_table() {
    mysql -h$mysql_host -P$mysql_port -u$user_name -p$passwd test_db_01 -e "select 1"
    sleep 1
    mysql -h$mysql_host -P$mysql_port -u$user_name -p$passwd test_db_01 -e "drop table test_table1"
    sleep 1
    mysql -h$mysql_host -P$mysql_port -u$user_name -p$passwd test_db_01 -e "drop table test_table2"
    sleep 1

}


while true
    do
    execute_create_table
    echo "create table done!"

    execute_insert_data
    echo "insert data done!"

    execute_update_data
    echo "update data done!"

    execute_delete_data
    echo "delete data done!"

    execute_truncate_table
    echo "truncate table done!"

    execute_drop_table
    echo "drop table done!"
    
    sleep 2
    done
