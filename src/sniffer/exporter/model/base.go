package model

import "time"

type SqlDetail struct {
	UserName        string  `json:"user"`
	SqlText         string  `json:"sql"`
	QueryTime       uint64  `json:"bt"`
	SourceIp        string  `json:"cip"`
	TargetIp        string  `json:"sip"`
	SourcePort      int8    `json:"cport"`
	TargetPort      int8    `json:"sport"`
	DatabaseNameStr string  `json:"db"`
	ExecTime        float64 `json:"cms"`
	TableNameStr    string
	QueryType       int8
}

const buffer_sql_count = 1000

var cur_point = 0
var to_db_point = 1 - cur_point
var cur_buffer_len = 0
var SqlBuffer [][]*SqlDetail //二维数组
var buffer_timer *time.Timer

func init() {
	/*初始化缓存*/
	SqlBuffer = make([][]*SqlDetail, 2)
	SqlBuffer[0] = make([]*SqlDetail, buffer_sql_count)
	SqlBuffer[1] = make([]*SqlDetail, buffer_sql_count)
	cur_point = 0
	cur_buffer_len = 0
	to_db_point = 1 - cur_point
	buffer_timer = time.NewTimer(60 * time.Second)
}

func IsBufferFull() bool {
	return cur_point+1 == len(SqlBuffer)
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
	buffer_timer.Reset(60 * time.Second)
}

func ResetBuffer() {
	cur_point = 1 - cur_point
	to_db_point = 1 - cur_point
	cur_buffer_len = 0
	for i := 0; i < len(SqlBuffer[cur_point]); i++ {
		SqlBuffer[cur_point][i] = nil //置空
	}
	ResetBufferTick()
}

func InsertToBuffer(s *SqlDetail) {
	SqlBuffer[cur_point][cur_buffer_len] = s
	cur_buffer_len += 1
}
func GetBuffer() []*SqlDetail {
	return SqlBuffer[cur_point]
}
