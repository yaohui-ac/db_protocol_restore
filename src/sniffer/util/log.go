package util

import (
	"log"
	"os"
	"runtime"
	"sniffer/consts"
	"strconv"
)

func init() {
	log.SetFlags(log.Ltime | log.Ldate)
	//log.SetFlags(log.Ltime | log.Ldate | log.Llongfile)

	if consts.Default_log_location != "stdout" {
		logFile, _ := os.OpenFile(consts.Default_log_location, os.O_RDWR|os.O_CREATE|os.O_APPEND, 0766)
		log.SetOutput(logFile)
	}

}
func get_caller_func_name_and_line() string {
	pc, _, line, _ := runtime.Caller(2)
	name := runtime.FuncForPC(pc).Name()
	return name + " " + strconv.Itoa(line)
}
func Log_Debug(format string, a ...any) {
	switch consts.Default_log_level {
	case "debug":
		log.Printf("["+get_caller_func_name_and_line()+"] \033[1;34;43m[DEBUG]\033[0m "+format+"\n", a...)
	default:
		return
	}
}
func Log_Info(format string, a ...any) {
	switch consts.Default_log_level {
	case "debug", "info":
		log.Printf("["+get_caller_func_name_and_line()+"] \033[1;34;42m[INFO]\033[0m "+format+"\n", a...)
	default:
		return
	}

}

func Log_Error(format string, a ...any) {
	switch consts.Default_log_level {
	case "debug", "info", "error":
		log.Printf("["+get_caller_func_name_and_line()+"] \033[1;34;41m[ERROR]\033[0m"+format+"\n", a...)
	default:
		return
	}
}
