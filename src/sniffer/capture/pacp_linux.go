//go:build linux
// +build linux

package capture

import (
	"fmt"
	"time"

	log "github.com/golang/glog"
	"github.com/google/gopacket"
	"github.com/google/gopacket/layers"
	"github.com/google/gopacket/pcap"
	"github.com/google/gopacket/pcapgo"
	"golang.org/x/net/bpf"
)

func initEthernetHandlerFromPacp() (pcapgoHandler *pcapgo.EthernetHandle) {
	pcapgoHandler, err := pcapgo.NewEthernetHandle(DeviceName)
	if err != nil {
		panic(fmt.Sprintf("cannot open network interface %s <-- %s", DeviceName, err.Error()))
	}

	// set BPFFilter
	pcapBPF, err := pcap.CompileBPFFilter(
		layers.LinkTypeEthernet, 65535, fmt.Sprintf("tcp and (port %d)", snifferPort))
	// pcapBPF, err := pcap.CompileBPFFilter(
	// 	layers.LinkTypeEthernet, 65535, "tcp")
	if err != nil {
		panic(err.Error())
	}
	bpfIns := []bpf.RawInstruction{}
	for _, ins := range pcapBPF {
		bpfIn := bpf.RawInstruction{
			Op: ins.Code,
			Jt: ins.Jt,
			Jf: ins.Jf,
			K:  ins.K,
		}
		bpfIns = append(bpfIns, bpfIn)
	}

	err = pcapgoHandler.SetBPF(bpfIns)
	if err != nil {
		panic(err.Error())
	}

	_ = pcapgoHandler.SetCaptureLength(65536)
	fmt.Printf("net package: %v\n", pcapgoHandler.LocalAddr())
	return
}

func dealEachTCPIPPacket(dealTCPIPPacket func(tcpIPPkt *TCPIPPair)) {
	handler := initEthernetHandlerFromPacp()
	//	fmt.Println("[well begin]")
	defer func() {
		fmt.Println("pcap handle close")
		handler.Close()
	}()

	for {

		var ci gopacket.CaptureInfo
		data, ci, err := handler.ZeroCopyReadPacketData()

		if err == pcap.NextErrorTimeoutExpired {

			continue
		}
		if err != nil {
			log.Error(err.Error())
			fmt.Printf("err:%v", err.Error())
			time.Sleep(time.Second * 3)
			continue
		}

		packet := gopacket.NewPacket(data, layers.LayerTypeEthernet, gopacket.NoCopy)
		m := packet.Metadata()
		m.CaptureInfo = ci

		tcpPkt, ok := packet.TransportLayer().(*layers.TCP)
		if !ok {

			continue
		}

		ipLayer := packet.NetworkLayer()

		if ipLayer == nil {
			fmt.Println("no ip layer found in package")
			log.Error("no ip layer found in package")
			continue
		}

		var srcIP, dstIP string
		switch realIPLayer := ipLayer.(type) {
		case *layers.IPv6:
			{
				srcIP = realIPLayer.SrcIP.String()
				dstIP = realIPLayer.DstIP.String()
			}
		case *layers.IPv4:
			{
				srcIP = realIPLayer.SrcIP.String()
				dstIP = realIPLayer.DstIP.String()
			}
		}
		tcpipPair := &TCPIPPair{
			srcIP:  srcIP,
			dstIP:  dstIP,
			tcpPkt: tcpPkt,
		}
		dealTCPIPPacket(tcpipPair)
	}
}
