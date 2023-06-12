package consts

import "time"

const (
	Default_log_level        = "info"    // info debug error
	Default_export_type      = "postgre" //postgre cli
	Default_service_type     = "mysql"
	Default_listen_interface = "eth0"
	Default_listen_port      = 3306
	Default_tcp_capture_rate = 1.0
	Default_sql_capture_rate = 1.0
	Default_sql_buffer_len   = 10
	Default_sql_buffer_timer = 1 * time.Minute
	Default_log_location     = "stdout"
)
