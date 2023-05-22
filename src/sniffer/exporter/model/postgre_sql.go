package model

import (
	"context"
	"time"

	pg "github.com/go-pg/pg/v10"
)

const (
	WriteToPgCount = 1000
)

type mysql_detail_table struct {
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
func BufferToDB(buffer []*SqlDetail) {

	return
}
