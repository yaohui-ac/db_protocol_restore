package util

import (
	"log"
	"sniffer/consts"
)

func init() {
	log.SetFlags(log.Llongfile | log.Lmicroseconds | log.Ldate)

}
func Log_Debug(format string, a ...any) {
	switch consts.Default_log_level {
	case "info", "error", "debug":
		log.Printf("[DEBUG] "+format+"\n", a...)
	default:
		return
	}
}
func Log_Info(format string, a ...any) {
	switch consts.Default_log_level {
	case "info", "error":
		log.Printf("[INFO] "+format+"\n", a...)
	default:
		return
	}

}

func Log_Error(format string, a ...any) {
	switch consts.Default_log_level {
	case "error":
		log.Printf("[ERROR] "+format+"\n", a...)
	default:
		return
	}
}
