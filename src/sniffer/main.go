package main

import (
	"flag"
	"fmt"
	"os"

	"sniffer/capture"
	"sniffer/communicator"
	"sniffer/exporter"
	sd "sniffer/session-dealer"
	"sniffer/session-dealer/mysql"
)

var (
	logLevel string
)

func init() {
	flag.StringVar(&logLevel, "log_level", "warn", "log level. Default is info")
}

func initLog() {
}

func main() {
	flag.Parse()
	prepareEnv()

	go communicator.Server()
	mainServer()
}

func mainServer() {
	ept := exporter.NewExporter()
	networkCard := capture.NewNetworkCard()
	fmt.Println("begin running")
	for queryPiece := range networkCard.Listen() {
		fmt.Println("666")
		err := ept.Export(queryPiece)
		if err != nil {

		}
		queryPiece.Recovery()
	}

	os.Exit(1)
}

func prepareEnv() {
	initLog()
	sd.CheckParams()
	mysql.PrepareEnv()
	capture.ShowLocalIP()
}
