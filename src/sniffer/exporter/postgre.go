package exporter

import (
	md "sniffer/exporter/model"
	"sniffer/model"
	"sniffer/util"
)

type postgreExporter struct {
}

func NewPostgreExporter() *postgreExporter {
	return &postgreExporter{}
}
func (c *postgreExporter) Export(qp model.QueryPiece) (err error) {
	util.Log_Info(*qp.String())
	md.HandleDBString(*qp.String())
	return
}
