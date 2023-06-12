package capture

import (
	"bytes"
	"fmt"
	"net"
	"reflect"
	"sniffer/util"
	"strconv"
	"strings"
	"unsafe"

	"github.com/pingcap/tidb/util/hack"
)

func getLocalIPAddr() (ipAddr string, err error) {
	addrs, err := net.InterfaceAddrs()
	if err != nil {
		return
	}

	for _, addr := range addrs {
		addrStr := addr.String()
		if strings.Contains(addrStr, "127.0.0.1") ||
			strings.Contains(addrStr, "::1") ||
			strings.Contains(addrStr, "/64") {
			continue
		}

		addrStr = strings.TrimRight(addrStr, "1234567890")
		addrStr = strings.TrimRight(addrStr, "/")
		if len(addrStr) < 1 {
			continue
		}

		ipAddr = addrStr
		return
	}

	err = fmt.Errorf("no valid ip address found")
	return
}
func String(b []byte) (s string) {
	if len(b) == 0 {
		return ""
	}
	pbytes := (*reflect.SliceHeader)(unsafe.Pointer(&b))
	pstring := (*reflect.StringHeader)(unsafe.Pointer(&s))
	pstring.Data = pbytes.Data
	pstring.Len = pbytes.Len
	return
}
func spliceSessionKey(clientIP, serverIP *string, clientPort, serverPort int) *string {
	var buffer = bytes.NewBuffer(make([]byte, 0, 24))
	buffer.WriteString(*clientIP)
	buffer.WriteString(":")
	buffer.WriteString(strconv.Itoa(clientPort) + "-")
	buffer.WriteString(*serverIP)
	buffer.WriteString(":")
	buffer.WriteString(strconv.Itoa(serverPort))
	sessionKey := hack.String(buffer.Bytes())
	util.Log_Debug(sessionKey)
	return &sessionKey
}
