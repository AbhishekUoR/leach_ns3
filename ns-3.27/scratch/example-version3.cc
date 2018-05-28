//
// Program description: Create a UDP or TCP packet flow across a
// WiFi link.  Allow user to select the transport protocol.
//
//  node 0 ----------------------- node 1
//  10.1.1.1                       10.1.1.2
//         <----- direction of data flow 
//
 
#include "ns3/core-module.h"
#include "ns3/network-module.h"
//#include "ns3/point-to-point-module.h"
#include "ns3/internet-module.h"
#include "ns3/applications-module.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/netanim-module.h"
#include "ns3/mobility-module.h"
#include "ns3/config-store-module.h"

#include "ns3/wifi-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("Wns3Example");

double receivedPacket = 0;

void
PacketSinkTraceSink (Ptr<const Packet> packet, const Address &from)
{
  std::cout << "Packet received; UID = " << packet->GetUid () << 
               " - Number of received packet = " << ++receivedPacket  << std::endl;

}

int 
main (int argc, char *argv[])
{
  // Defaults and command-line arguments
  std::string protocol = "ns3::UdpSocketFactory"; 
  std::string phyMode ("DsssRate2Mbps"); 
//  std::string chaMode ("DsssRate1Mbps");

  double distance = 100;  // m
  int packetSize = 500;
  bool verbose = false;
  uint16_t port = 5009;

  CommandLine cmd;
  cmd.AddValue ("transport", "TypeId for socket factory", protocol);
  cmd.AddValue ("phyMode", "Wifi Phy mode", phyMode);
  cmd.AddValue ("distance", "distance (m)", distance);
  cmd.AddValue ("packetSize", "packetSize (bit)", packetSize);
  cmd.AddValue ("verbose", "turn on all WifiNetDevice log components", verbose);
  cmd.Parse (argc,argv);

  // Nodes
  NodeContainer nodes;
  nodes.Create (2);

  // Devices
  // The below set of helpers will help us to put together the wifi NICs we want

  // disable fragmentation for frames below 2200 bytes
  Config::SetDefault ("ns3::WifiRemoteStationManager::FragmentationThreshold", StringValue ("2200"));
  // turn off RTS/CTS for frames below 2200 bytes
  Config::SetDefault ("ns3::WifiRemoteStationManager::RtsCtsThreshold", StringValue ("2200"));
  // Fix non-unicast data rate to be the same as that of unicast
  Config::SetDefault ("ns3::WifiRemoteStationManager::NonUnicastMode", 
                      StringValue (phyMode));

  WifiHelper wifi;
  if (verbose)
    {
      wifi.EnableLogComponents ();  // Turn on all Wifi logging
    }

  YansWifiPhyHelper wifiPhy =  YansWifiPhyHelper::Default ();
  // set it to zero; otherwise, gain will be added
  wifiPhy.Set ("RxGain", DoubleValue (-10) ); 
  // ns-3 supports RadioTap and Prism tracing extensions for 802.11b
  wifiPhy.SetPcapDataLinkType (YansWifiPhyHelper::DLT_IEEE802_11_RADIO); 

  YansWifiChannelHelper wifiChannel;
  wifiChannel.SetPropagationDelay ("ns3::ConstantSpeedPropagationDelayModel");
//  wifiChannel.AddPropagationLoss ("ns3::FriisPropagationLossModel");//set the channel
//  wifiChannel.AddPropagationLoss ("ns3::LogDistancePropagationLossModel");//set the channel
  wifiChannel.AddPropagationLoss ("ns3::LogDistancePropagationLossModel");
  Config::SetDefault ("ns3::LogDistancePropagationLossModel::ReferenceLoss", DoubleValue (40.046));
  wifiPhy.SetChannel (wifiChannel.Create ());

  // Add a non-QoS upper mac, and disable rate control
  NqosWifiMacHelper wifiMac = NqosWifiMacHelper::Default ();
  wifi.SetStandard (WIFI_PHY_STANDARD_80211b);
  wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager",
                                "DataMode",StringValue (phyMode),
                                "ControlMode",StringValue (phyMode));
  // Set it to adhoc mode
  wifiMac.SetType ("ns3::AdhocWifiMac");
  NetDeviceContainer devices = wifi.Install (wifiPhy, wifiMac, nodes);


  // Internet stack
  InternetStackHelper stack;
  stack.Install (nodes);

  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");

  Ipv4InterfaceContainer interfaces;
  interfaces = address.Assign (devices);

  // Applications
  InetSocketAddress destinationAddress = InetSocketAddress (Ipv4Address ("10.1.1.1"), port);
  InetSocketAddress sinkAddress = InetSocketAddress (Ipv4Address::GetAny (), port); 

  OnOffHelper onOff (protocol, destinationAddress);
  onOff.SetConstantRate (DataRate ("1Mb/s"));
  onOff.SetAttribute("PacketSize", UintegerValue(packetSize));
  PacketSinkHelper sink (protocol, sinkAddress);
  
  ApplicationContainer serverApps = onOff.Install (nodes.Get (1));
  ApplicationContainer clientApps = sink.Install (nodes.Get (0));
  
  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (11.0));

  clientApps.Start (Seconds (1.0));
  clientApps.Stop (Seconds (12.0));

  // Add some visualization and output data capabilities
  MobilityHelper mobility;
  mobility.SetPositionAllocator ("ns3::GridPositionAllocator", 
                                 "DeltaX", DoubleValue (distance));
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (nodes);

  Config::ConnectWithoutContext ("/NodeList/0/ApplicationList/0/$ns3::PacketSink/Rx", MakeCallback (&PacketSinkTraceSink));

  // Output config store to txt format
  Config::SetDefault ("ns3::ConfigStore::Filename", StringValue ("example-3-wifi-attributes.txt"));
  Config::SetDefault ("ns3::ConfigStore::FileFormat", StringValue ("RawText"));
  Config::SetDefault ("ns3::ConfigStore::Mode", StringValue ("Save"));
  ConfigStore outputConfig;
  outputConfig.ConfigureDefaults ();
  outputConfig.ConfigureAttributes ();

  Simulator::Stop (Seconds (13.0));

  Simulator::Run ();
  Simulator::Destroy ();

  return 0;
}



