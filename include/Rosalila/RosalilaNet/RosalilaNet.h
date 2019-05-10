#ifndef ROSALILA_NET
#define ROSALILA_NET

/*
#include <SDL2/SDL_net.h>

#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <functional>
#include "../RosalilaUtility/RosalilaUtility.h"

#ifdef WINDOWS
#include <windows.h>
#endif

class RosalilaNet
{
public:
  IPaddress server_ip;
  IPaddress peer_ip;
  UDPpacket *packet_server;
  UDPpacket *packet_peer;
  UDPsocket udp_socket;
  int packet_size;

  //Callbacks
  std::function<void(int response_code, std::string &room_key)> on_create_room;
  std::function<void(int response_code)> on_player2_join;
  std::function<void(int response_code)> on_connect;

  RosalilaNet();
  void init();
  ~RosalilaNet();
  void update();
  void onConnectionComplete(std::vector<std::string> received_messages);
  bool sendToPacket(UDPpacket *packet, std::string message);
  std::string receiveFromPacket(UDPpacket *packet, IPaddress ip_address);

  // User interface
  bool send(string message);
  std::vector<std::string> receive();
  void createRoom(const std::function<void(int response_code, std::string &room_key)> &on_create_room,
                  const std::function<void(int response_code)> &callback);
  void joinRoom(std::string room_key, const std::function<void(int response_code)> &callback);
  void disconnectFromPeer();
};
*/

#endif
