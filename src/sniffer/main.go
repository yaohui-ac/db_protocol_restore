package main

import (
	"flag"
	"os"

	"sniffer/capture"
	"sniffer/exporter"
	sd "sniffer/session-dealer"
	"sniffer/session-dealer/mysql"
)

func main() {
	flag.Parse()
	prepareEnv()
	mainServer()
}

func mainServer() {
	ept := exporter.NewExporter()
	networkCard := capture.NewNetworkCard()
	for queryPiece := range networkCard.Listen() {

		err := ept.Export(queryPiece)
		if err != nil {

		}
		queryPiece.Recovery()
	}
	os.Exit(1)
}

func prepareEnv() {
	sd.CheckParams()
	mysql.PrepareEnv()
	capture.ShowLocalIP()
}
