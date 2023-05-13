/*
项目中请设置数据库用户postgres 密码postgres
安装postgresql且设置密码之后
1. apt-get安装postgresql和postgresql-client
2.修改postgres的密码
3.修改之后可使用如下命令登录
psql postgres://postgres:postgres@localhost:5432/postgres
*/
/*数据库名 mysql_protocol_restore*/
/*
执行如下命令创建库和切换库
CREATE DATABASE mysql_protocol_restore;
\c mysql_protocol_restore 选择数据库
之后复制下列sql语句执行即可
*/
create table mysql_detail
(
    id bigserial primary key, --自增主键
    user_name character varying(255) not null, --用户名
    sql_text text not null, --sql语句
    query_time bigint not null, --查询时间(发起查询时的时间,秒级时间戳)
    source_ip character varying(255) not null, --源ip(发请求客户端的ip) 按照ip展示指的是这个ip
    target_ip character varying(255) not null, --目的ip(服务器ip)
    source_port smallint not null,  --源端口 客户端端口
    target_port smallint not null,  --目的端口 服务器端口(一般是3306)
    database_name_str character varying(255), --sql_text所属的数据库名
    table_name_str character varying(255),    --sql_text所属的表名
    query_type smallint not null, /*
    sql_text的查询类型 
    select=1/insert=2/update=3/delete=4/
    truncate=5/drop=6/system_command=7
    system_command指的是use table这样的查看系统语句 展示数据类型时不展示该类型
    */
    exec_time decimal(10, 4) not null --sql执行用时
);


create table users ( --用户表
    id serial primary key, --自增主键
    user_name character varying(255) not null, --管理系统的用户
    passwd varchar(512) not null, --密码 存储的时(sha512sum计算的摘要)
    create_time timestamp default current_timestamp,
    update_time timestamp default current_timestamp

);
create or replace function update_timestamp() returns trigger as --触发器,update_time
$$
begin
    new.update_time = current_timestamp;
    return new;
end
$$
language plpgsql;

create trigger auto_update_time 
	before update on users
	for each row execute procedure update_timestamp();
