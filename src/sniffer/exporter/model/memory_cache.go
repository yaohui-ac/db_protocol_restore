package model

import (
	"encoding/json"
	"sniffer/util"
	"strings"
)

func CleanSqlText(str string) string {
	if strings.HasPrefix(str, "\u0000\u0001") {
		str = strings.TrimPrefix(str, "\u0000\u0001")
	}
	return str
}
func HandleDBString(str string) {
	var sql_detail SqlDetail
	err := json.Unmarshal([]byte(str), &sql_detail)
	if err != nil {
		util.Log_Error("%+v\n", err)
		return
	}
	if IsBufferFull() || IsBufferTimerExpired() {
		ResetBuffer()
		FlashToDB(GetInsertToDBBuffer())
	}

	sql_detail.SqlText = CleanSqlText(sql_detail.SqlText)
	//ShowCurBufferStatus()
	InsertToBuffer(&sql_detail)
}
