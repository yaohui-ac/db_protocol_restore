package exporter

import (
	"fmt"
	"sniffer-agent/model"

	pg "github.com/go-pg/pg"
)

func GetPostgreConnection() *pg.DB {
	return pg.Connect(
		&pg.Options{
			Addr:     ":5432",
			User:     "postgres",
			Password: "postgres",
			Database: "mysql_protocol_restore",
		})
}

type postgreExporter struct {
}

func NewPostgreExporter() *postgreExporter {
	return &postgreExporter{}
}
func (c *postgreExporter) Export(qp model.QueryPiece) (err error) {
	fmt.Println(*qp.String())
	//todo
	return
}
