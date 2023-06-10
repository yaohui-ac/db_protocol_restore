package model

import (
	"fmt"
	"sniffer/consts"
	"time"
)

type SqlDetail struct {
	UserName        string  `json:"user"`
	SqlText         string  `json:"sql"`
	QueryTime       uint64  `json:"bt"`
	SourceIp        string  `json:"cip"`
	TargetIp        string  `json:"sip"`
	SourcePort      uint16  `json:"cport"`
	TargetPort      uint16  `json:"sport"`
	DatabaseNameStr string  `json:"db"`
	ExecTime        float64 `json:"cms"`
	TableNameStr    string  `json:"-"`
	QueryType       int8    `json:"-"`
}

var sql_buffer_len = 1024

var cur_point = 0
var to_db_point = 1 - cur_point
var cur_buffer_count = 0
var SqlBuffer [][]*SqlDetail //二维数组
var buffer_timer *time.Timer

func init() {
	/*初始化缓存*/
	SqlBuffer = make([][]*SqlDetail, 2)
	SqlBuffer[0] = make([]*SqlDetail, sql_buffer_len)
	SqlBuffer[1] = make([]*SqlDetail, sql_buffer_len)
	sql_buffer_len = consts.Default_sql_buffer_len
	cur_point = 0
	cur_buffer_count = 0
	to_db_point = 1 - cur_point
	buffer_timer = time.NewTimer(consts.Default_sql_buffer_timer)
}

func IsBufferFull() bool {
	return cur_buffer_count == sql_buffer_len
}

func IsBufferTimerExpired() bool {
	select {
	case <-buffer_timer.C:
		return true
	default:
		return false
	}
}

func ResetBufferTick() {
	buffer_timer.Reset(consts.Default_sql_buffer_timer)
}

func ResetBuffer() {
	cur_point = 1 - cur_point
	to_db_point = 1 - cur_point
	cur_buffer_count = 0
	for i := 0; i < len(SqlBuffer[cur_point]); i++ {
		// fmt.Print(i, " ")
		SqlBuffer[cur_point][i] = nil //置空
	}

	ResetBufferTick()
	// fmt.Println("Reset Over")
}

func InsertToBuffer(s *SqlDetail) {
	// fmt.Println("insert idx==>", cur_point, ":", cur_buffer_count)
	SqlBuffer[cur_point][cur_buffer_count] = s
	cur_buffer_count += 1
}
func GetInsertToDBBuffer() []*SqlDetail {
	return SqlBuffer[to_db_point]
}
func ShowCurBufferStatus() {
	fmt.Printf("Buffer %+v in use, Buffer len %+v, Have Use %+v\n", cur_point, sql_buffer_len, cur_buffer_count)
}
