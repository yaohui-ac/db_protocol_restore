package util

import (
	"log"
	"os"
	"runtime"
	"sniffer/consts"
)

func init() {
	log.SetFlags(log.Ltime | log.Ldate)
	if consts.Default_log_location != "stdout" {
		logFile, _ := os.OpenFile(consts.Default_log_location, os.O_RDWR|os.O_CREATE|os.O_APPEND, 0766)
		log.SetOutput(logFile)
	}

}
func get_caller_func_name() string {
	pc, _, _, _ := runtime.Caller(2)
	name := runtime.FuncForPC(pc).Name()
	return name
}
func Log_Debug(format string, a ...any) {
	switch consts.Default_log_level {
	case "info", "error", "debug":
		log.Printf("["+get_caller_func_name()+"] [DEBUG] "+format+"\n", a...)
	default:
		return
	}
}
func Log_Info(format string, a ...any) {
	switch consts.Default_log_level {
	case "info", "error":
		log.Printf("["+get_caller_func_name()+"] [INFO] "+format+"\n", a...)
	default:
		return
	}

}

func Log_Error(format string, a ...any) {
	switch consts.Default_log_level {
	case "error":
		log.Printf("["+get_caller_func_name()+"] [ERROR] "+format+"\n", a...)
	default:
		return
	}
}
