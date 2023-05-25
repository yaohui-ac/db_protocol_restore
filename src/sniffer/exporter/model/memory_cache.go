package model

import (
	"encoding/json"
	"fmt"
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
		fmt.Printf("%+v\n", err)
		return
	}
	if IsBufferFull() || IsBufferTimerExpired() {
		ResetBuffer()
		//---数据库存储
		FlashToDB(GetBuffer())
	}
	sql_detail.SqlText = CleanSqlText(sql_detail.SqlText)
	InsertToBuffer(&sql_detail)
}
