package session_dealer

import "sniffer/model"

type ConnSession interface {
	ReceiveTCPPacket(*model.TCPPacket)
	Close()
}
