package model

import (
	"context"
	"sniffer/util"
	"time"

	pg "github.com/go-pg/pg/v10"
)

const (
	WriteToPgCount = 1000
)

type db_sql_detail struct {
	tableName       struct{}  `pg:"sql_record,alias:mysql_record"`
	Id              int64     `pg:"id,pk"`
	UserName        string    `pg:"user_name,notnull"`
	SqlText         string    `pg:"sql_text,notnull"`
	QueryTime       time.Time `pg:"query_time,notnull"`
	SourceIp        string    `pg:"source_ip,notnull"`
	TargetIp        string    `pg:"target_ip,notnull"`
	SourcePort      uint16    `pg:"source_port,notnull"`
	TargetPort      uint16    `pg:"target_port,notnull"`
	DatabaseNameStr string    `pg:"database_name_str"`
	TableNameStr    string    `pg:"table_name_str"`
	QueryType       int8      `pg:",notnull"`
	ExecTime        float64   `pg:",notnull"`
}

func GetPostgreConnection() *pg.DB {
	return pg.Connect(
		&pg.Options{
			Addr:     ":5432",
			User:     "postgres",
			Password: "postgres",
			Database: "mysql_protocol_restore",
		})
}
func TestDBRunning(db *pg.DB) {
	ctx := context.Background()

	if err := db.Ping(ctx); err != nil {
		util.Log_Error("test ping failed")
		panic(err)
	}

}

func BufferDetailToDBDetail(s *SqlDetail) *db_sql_detail {
	db_detail := db_sql_detail{}
	db_detail.ExecTime = s.ExecTime
	db_detail.UserName = s.UserName
	db_detail.SqlText = s.SqlText
	db_detail.SourceIp = s.SourceIp
	db_detail.TargetIp = s.TargetIp
	db_detail.SourcePort = s.SourcePort
	db_detail.TargetPort = s.TargetPort
	db_detail.DatabaseNameStr = s.DatabaseNameStr
	db_detail.ExecTime = s.ExecTime
	db_detail.QueryTime = time.Unix(int64(s.QueryTime/1000), 0)

	query_type := GetDBDetailQueryType(db_detail.SqlText)
	db_detail.QueryType = query_type.Int8()
	db_detail.TableNameStr = GetTableName(db_detail.SqlText, query_type)
	return &db_detail
}
func FlashToDB(buffer []*SqlDetail) {
	conn := GetPostgreConnection()
	TestDBRunning(conn)
	flash_db_list := make([]db_sql_detail, 0)
	for i := 0; i < len(buffer); i++ {
		if buffer[i] == nil {
			continue
		}
		db_tail := BufferDetailToDBDetail(buffer[i])
		flash_db_list = append(flash_db_list, *db_tail)
	}

	_, err := conn.Model(&flash_db_list).Insert()
	if err != nil {
		util.Log_Error("%+v\n", err)
	}
	util.Log_Info("Flash To PosgreSQL DONE")
	return
}
