package consts

import "time"

const (
	Default_log_level        = "warn"
	Default_export_type      = "postgre" //postgre cli
	Default_service_type     = "mysql"
	Default_listen_interface = "eth0"
	Default_listen_port      = 3306
	Default_capture_rate     = 1.0
	Default_sql_buffer_len   = 10
	Default_sql_buffer_timer = 1 * time.Minute
)

///==========================================
const (
	Is_strict_mode    = false
	Admin_User_Name   = "yaohui"
	Admin_Passwd      = "yaohui_123"
	MaxMySQLPacketLen = 128 * 1024
)
