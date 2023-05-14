package model

import (
	"context"

	pg "github.com/go-pg/pg/v10"
)

const (
	WriteToPgCount = 1000
)

type mysql_detail_table struct {
	tableName         struct{} `pg:"mysql_detail,alias:mysql_detail"`
	Id                int64    `pg:",pk"`
	UserName          string   `pg:",notnull"`
	SqlText           string   `pg:",notnull"`
	query_time        uint64   `pg:",notnull"`
	source_ip         string   `pg:",notnull"`
	target_ip         string   `pg:",notnull"`
	source_port       int8     `pg:",notnull"`
	target_port       int8     `pg:",notnull"`
	database_name_str string
	table_name_str    string
	query_type        int8    `pg:",notnull"`
	exec_time         float64 `pg:",notnull"`
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
