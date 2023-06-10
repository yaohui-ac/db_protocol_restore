package util

import (
	"log"
	"os"
	"sniffer/consts"
)

func init() {
	log.SetFlags(log.Lshortfile | log.Ltime | log.Ldate)
	if consts.Default_log_location != "stdout" {
		logFile, _ := os.OpenFile(consts.Default_log_location, os.O_RDWR|os.O_CREATE|os.O_APPEND, 0766)
		log.SetOutput(logFile)
	}

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
