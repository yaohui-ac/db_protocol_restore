create table if not exists sql_record (
    id serial PRIMARY KEY,
    query_time timestamp with time zone not null,
    source_ip character varying(20) not null,
    sql_text text not null,
    target_ip character varying(20) not null,
    user_name character varying(20) not null,
    source_port int not null,
    target_port int not null,
    database_name_str character varying(20),
    table_name_str character varying(20),
    exec_time int not null,
    query_type int not null
);