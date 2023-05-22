package model

import (
	"context"
	"encoding/json"
	"fmt"
	"strings"
	"time"

	pg "github.com/go-pg/pg/v10"
	sqlparser "github.com/krasun/gosqlparser"
)

const (
	WriteToPgCount = 1000
)

type db_sql_detail struct {
	Id              int64     `pg:"id,pk"`
	UserName        string    `pg:"user_name,notnull"`
	SqlText         string    `pg:"sql_text,notnull"`
	QueryTime       time.Time `pg:"query_time,notnull"`
	SourceIp        string    `pg:"source_ip,notnull"`
	TargetIp        string    `pg:"target_ip,notnull"`
	SourcePort      int8      `pg:"source_port,notnull"`
	TargetPort      int8      `pg:"target_port,notnull"`
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
		panic(err)
	}

}
func GetDBDetailQueryType(sql_text string) int8 {
	words := strings.Fields(sql_text)
	query_type_str := strings.ToLower(words[0])
	switch query_type_str {
	case "select":
		return 1
	case "insert":
		return 2
	case "update":
		return 3
	case "delete":
		return 4
	case "truncate":
		return 5
	case "drop":
		return 6
	default:
		return 7
	}

}
func GetSqlTableName(sql_text string) string {
	// todo:
	query, err := sqlparser.Parse(sql_text)
	if err != nil {
		fmt.Printf("%+v", err)
		return ""
	}
	json_str, err := json.Marshal(query)
	if err != nil {
		fmt.Printf("unexpected error: %s", err)
		return ""
	}
	return string(json_str)
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
	db_detail.QueryTime = time.Unix(int64(s.QueryTime), 0)
	db_detail.QueryType = GetDBDetailQueryType(db_detail.SqlText)
	return &db_detail
}
func FlashToDB(buffer []*SqlDetail) {
	conn := GetPostgreConnection()
	TestDBRunning(conn)
	flash_db_list := make([]db_sql_detail, 0)
	for i := 0; i <= len(buffer); i++ {
		if buffer[i] == nil {
			continue
		}
		db_tail := BufferDetailToDBDetail(buffer[i])
		flash_db_list = append(flash_db_list, *db_tail)
	}
	return
}
