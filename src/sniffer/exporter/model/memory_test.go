package model

import (
	"encoding/json"
	"fmt"
	"strconv"
	"testing"
	"time"
)

func generate_sql_text(i int) string {
	mot := (i % 7) + 1
	switch mot {
	case 1:
		return "select * from gen_table1;"
	case 2:
		return "insert into gen_table2 values(1,2,3);"
	case 3:
		return "update gen3 set a=114514;"
	case 4:
		return "delete from gen4 where a=114;"
	case 5:
		return "truncate table gen5;"
	case 6:
		return "drop table if exists gen6"
	case 7:
		return "use db1;"
	default:
		panic("invalid i")
	}
}
func TestMemory(t *testing.T) {
	sql_list := make([]string, 0)
	for i := 1; i <= 26; i++ {
		sql_detail := SqlDetail{
			UserName:        "test_0525_" + strconv.Itoa(i),
			QueryTime:       uint64(time.Now().Unix()),
			SourceIp:        "127.0.0." + strconv.Itoa(i),
			TargetIp:        "0.0.0.0",
			DatabaseNameStr: "db_" + strconv.Itoa(i%5),
			SourcePort:      uint16(1230 + i),
			TargetPort:      2345,
			ExecTime:        10,
			SqlText:         generate_sql_text(i),
		}
		blist, err := json.Marshal(sql_detail)
		if err != nil {
			fmt.Printf("%+v\n", err)
		}
		sql_list = append(sql_list, string(blist))

	}
	fmt.Println(">>>>>>>>>>>>>>>>>>>>>>>>>")
	for _, sql_str := range sql_list {
		HandleDBString(sql_str)
	}
}
