package exporter

import (
	"sniffer/model"
	"sniffer/util"
)

type cliExporter struct {
}

func NewCliExporter() *cliExporter {
	return &cliExporter{}
}

func (c *cliExporter) Export(qp model.QueryPiece) (err error) {
	util.Log_Info(*qp.String())
	return
}
