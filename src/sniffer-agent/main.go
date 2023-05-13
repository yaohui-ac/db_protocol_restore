package main

import (
	"flag"
	"fmt"
	"os"

	"sniffer-agent/capture"
	"sniffer-agent/communicator"
	"sniffer-agent/exporter"
	sd "sniffer-agent/session-dealer"
	"sniffer-agent/session-dealer/mysql"

	log "github.com/golang/glog"
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
	fmt.Println("begin listen")
	log.Info("begin listen")
	for queryPiece := range networkCard.Listen() {
		err := ept.Export(queryPiece)
		if err != nil {
			log.Error(err.Error())
		}
		queryPiece.Recovery()
	}

	log.Errorf("cannot get network package from %s", capture.DeviceName)
	os.Exit(1)
}

func prepareEnv() {
	initLog()
	sd.CheckParams()
	mysql.PrepareEnv()
	capture.ShowLocalIP()
}
