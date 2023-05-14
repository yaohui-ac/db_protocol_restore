package capture

import (
	"fmt"
	"math/rand"
	"time"

	sd "sniffer/session-dealer"
)

var (
	localIPAddr *string

	sessionPool = make(map[string]sd.ConnSession)
	// sessionPoolLock sync.Mutex
)

func init() {
	ipAddr, err := getLocalIPAddr()
	if err != nil {
		panic(err)
	}

	localIPAddr = &ipAddr

	rand.Seed(time.Now().UnixNano())
}

func ShowLocalIP() {
	fmt.Printf("[parsed] local ip address:%s\n", *localIPAddr)
}
