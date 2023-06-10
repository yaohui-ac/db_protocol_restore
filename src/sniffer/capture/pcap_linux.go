//go:build linux
// +build linux

package capture

import (
	"fmt"
	"sniffer/util"
	"time"

	"github.com/google/gopacket"
	"github.com/google/gopacket/layers"
	"github.com/google/gopacket/pcap"
	"github.com/google/gopacket/pcapgo"
	"golang.org/x/net/bpf"
)

func initEthernetHandlerFromPcap() (pcapgoHandler *pcapgo.EthernetHandle) {
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
	util.Log_Debug("net package: %v\n", pcapgoHandler.LocalAddr())
	return
}

func dealEachTCPIPPacket(dealTCPIPPacket func(tcpIPPkt *TCPIPPair)) {
	handler := initEthernetHandlerFromPcap()
	defer func() {
		util.Log_Info("pcap handle close")
		handler.Close()
	}()

	for {

		var ci gopacket.CaptureInfo
		data, ci, err := handler.ZeroCopyReadPacketData()

		if err == pcap.NextErrorTimeoutExpired {

			continue
		}
		if err != nil {
			util.Log_Error("err:%v", err.Error())
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
			util.Log_Error("no ip layer found in package")
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
