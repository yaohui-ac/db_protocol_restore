package exporter

import (
	"fmt"
	md "sniffer/exporter/model"
	"sniffer/model"
)

type postgreExporter struct {
}

func NewPostgreExporter() *postgreExporter {
	return &postgreExporter{}
}
func (c *postgreExporter) Export(qp model.QueryPiece) (err error) {
	fmt.Println(*qp.String())
	md.GetPostgreConnection()
	return
}
