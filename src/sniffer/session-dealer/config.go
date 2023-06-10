package session_dealer

import (
	"flag"
	"sniffer/consts"
)

const (
	ServiceTypeMysql = "mysql"
)

var (
	serviceType string
)

func init() {
	flag.StringVar(&serviceType, "service_type", consts.Default_service_type, "service type. Default is mysql")
}
