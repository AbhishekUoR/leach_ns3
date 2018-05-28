//
// Program description: Create a UDP or TCP packet flow across a
// WiFi link.  Allow user to select the transport protocol.
//
// node 0 --------> node 1 <-------------- node 2
// 10.1.1.1 		10.1.1.2           10.1.1.3
//          
//

#include "ns3/olsr-helper.h" 
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/applications-module.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/netanim-module.h"
#include "ns3/mobility-module.h"
#include "ns3/config-store-module.h"
#include "ns3/ipv4-static-routing-helper.h"
#include "ns3/wifi-module.h"
#include "ns3/flow-monitor-helper.h"

using namespace std;
using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("assignment");

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
  std::string chaMode ("DsssRate1Mbps");

  double distance = 100;  // m
  int packetSize = 1500;
  bool verbose = false;
  uint16_t port = 5009;

  CommandLine cmd;
  cmd.AddValue ("transport", "TypeId for socket factory", protocol);
  cmd.AddValue ("phyMode", "Wifi Phy mode", phyMode);
  cmd.AddValue ("distance", "distance (m)", distance);
  cmd.AddValue ("packetSize", "packetSize (bit)", packetSize);
  cmd.AddValue ("verbose", "turn on all WifiNetDevice log components", verbose);
  cmd.Parse (argc,argv);

  cerr << distance << endl; 
  // Nodes
  NodeContainer nodes;
  nodes.Create (3);

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
  wifiChannel.AddPropagationLoss ("ns3::LogDistancePropagationLossModel");//set the channel
  Config::SetDefault ("ns3::LogDistancePropagationLossModel::ReferenceLoss", DoubleValue (40.046));
  wifiPhy.SetChannel (wifiChannel.Create ());

  // Add a non-QoS upper mac, and disable rate control
  NqosWifiMacHelper wifiMac = NqosWifiMacHelper::Default ();
  wifi.SetStandard (WIFI_PHY_STANDARD_80211g);


  wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager",
                                "DataMode",StringValue (phyMode),
                                "ControlMode",StringValue (chaMode));
  // Set it to adhoc mode
  wifiMac.SetType ("ns3::AdhocWifiMac");
  NetDeviceContainer devices = wifi.Install (wifiPhy, wifiMac, nodes);



  OlsrHelper olsr;
  Ipv4StaticRoutingHelper staticRouting;

  Ipv4ListRoutingHelper list;
  bool enableRouting = true;

  if (enableRouting)
    {
      //list.Add (staticRouting, 0);
      list.Add (olsr, 10);
    }

  InternetStackHelper internet;

  if (enableRouting)
    {
      internet.SetRoutingHelper (list);  // has effect on the next Install ()
    }
  internet.Install (nodes);
  // Internet stack
  //InternetStackHelper stack;
  //stack.Install (nodes);

  Ipv4AddressHelper address;//address for node 0
  address.SetBase ("10.1.1.0", "255.255.255.0");
/*
  Ipv4AddressHelper address1;//address for node 1
  address1.SetBase ("10.1.1.0", "255.255.255.0", "0.0.0.2");
  Ipv4AddressHelper address2;//address for node 2
  address2.SetBase ("10.1.1.0", "255.255.255.0", "0.0.0.3");
*/

  Ipv4InterfaceContainer interfaces;
  interfaces = address.Assign (devices);
/*
  Ipv4StaticRoutingHelper ipv4RoutingHelper;
  // Create static routes from A to C
  Ptr<Ipv4StaticRouting> staticRoutingA = ipv4RoutingHelper.GetStaticRouting (ipv4A);
  // The ifIndex for this outbound route is 1; the first p2p link added
  staticRoutingA->AddHostRouteTo (Ipv4Address ("10.1.1.2"), Ipv4Address ("10.1.1.1"), 1);
  Ptr<Ipv4StaticRouting> staticRoutingB = ipv4RoutingHelper.GetStaticRouting (ipv4B);
  // The ifIndex we want on node B is 2; 0 corresponds to loopback, and 1 to the first point to point link
  staticRoutingB->AddHostRouteTo (Ipv4Address ("10.1.1.2"), Ipv4Address ("10.1.1.3"), 2);
*/


  // Applications
  InetSocketAddress destinationAddress = InetSocketAddress (Ipv4Address ("10.1.1.1"), port); //set middle node for sending traffic
  InetSocketAddress destinationAddress1= InetSocketAddress (Ipv4Address ("10.1.1.3"), port);
  InetSocketAddress sinkAddress = InetSocketAddress (Ipv4Address::GetAny (), port); 

  OnOffHelper onOff (protocol, destinationAddress1);
  onOff.SetConstantRate (DataRate ("3Mb/s"));
  onOff.SetAttribute("PacketSize", UintegerValue(packetSize));
  PacketSinkHelper sink (protocol, sinkAddress);
  OnOffHelper onOff1 (protocol, destinationAddress); 
 
  ApplicationContainer serverApps = onOff.Install (nodes.Get (0));
  ApplicationContainer clientApps = sink.Install (nodes.Get (1));
  ApplicationContainer serverApps2 = onOff1.Install (nodes.Get (2));
 
  serverApps.Start (Seconds (1.0));//node 0 starting traffic
  serverApps2.Start (Seconds (1.0));

  serverApps.Stop (Seconds (50.0));
  serverApps2.Stop (Seconds (50.0));

  clientApps.Start (Seconds (1.0));
  clientApps.Stop (Seconds (51.0));

  // Add some visualization and output data capabilities
  MobilityHelper mobility;
  mobility.SetPositionAllocator ("ns3::GridPositionAllocator", 
                                 "DeltaX", DoubleValue (distance));
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (nodes);
/*
  Ptr<UniformRandomVariable> uvSrc = CreateObject<UniformRandomVariable> ();
  uvSrc->SetAttribute ("Min", DoubleValue (0));
  uvSrc->SetAttribute ("Max", DoubleValue (3 / 2 - 1));
  Ptr<UniformRandomVariable> uvDest = CreateObject<UniformRandomVariable> ();
  uvDest->SetAttribute ("Min", DoubleValue (3 / 2));
  uvDest->SetAttribute ("Max", DoubleValue (3));
*/


  Config::ConnectWithoutContext ("/NodeList/1/ApplicationList/0/$ns3::PacketSink/Rx", MakeCallback (&PacketSinkTraceSink));

  // Output config store to txt format
  Config::SetDefault ("ns3::ConfigStore::Filename", StringValue ("assignment.txt"));
  Config::SetDefault ("ns3::ConfigStore::FileFormat", StringValue ("RawText"));
  Config::SetDefault ("ns3::ConfigStore::Mode", StringValue ("Save"));
  ConfigStore outputConfig;
  outputConfig.ConfigureDefaults ();
  outputConfig.ConfigureAttributes ();

  bool enableFlowMonitor = true;
  // Flow Monitor
  FlowMonitorHelper flowmonHelper;
  if (enableFlowMonitor)
    {
      flowmonHelper.InstallAll ();
    }
  Simulator::Stop (Seconds (52.0));
  // Tracing
  wifiPhy.EnablePcap ("assignment", devices);

  Simulator::Run ();
  if (enableFlowMonitor)
    {
      flowmonHelper.SerializeToXmlFile ("assignment_v1.120.flowmon", true, true);
    }
  Simulator::Destroy ();

  return 0;
}



