package exporter

import (
	"flag"
	"fmt"

	"sniffer-agent/model"
)

var (
	exportType string
)

func init() {
	flag.StringVar(&exportType, "export_type", "cli", "export type. Default is cli, that is command line")
}

type Exporter interface {
	Export(model.QueryPiece) error
}

func NewExporter() Exporter {
	fmt.Printf("Exporter:%s begin listen\n", exportType)
	switch exportType {
	case "cli":
		return NewCliExporter()
	case "kafka":
		return NewKafkaExporter()
	case "postgre":
		return NewPostgreExporter()
	default:
		return NewCliExporter()
	}

}
