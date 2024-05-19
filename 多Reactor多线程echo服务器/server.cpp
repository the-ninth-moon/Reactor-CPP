#include "src/Server.h"
#include <iostream>
#include "src/Buffer.h"
#include "src/Connection.h"
#include "src/EventLoop.h"
#include "src/Socket.h"

int main() {
  EventLoop *loop = new EventLoop();
  Server *server = new Server(loop);
  //通过onConnect传递函数，server将会传递给conection的回调函数，然后再传递给channel
  server->OnConnect([](Connection *conn) {
    conn->Read();
    if (conn->GetState() == Connection::State::Closed) {
      conn->Close();
      return;
    }
    std::cout << "Message from client " << conn->GetSocket()->GetFd() << ": " << conn->ReadBuffer() << std::endl;
    conn->SetSendBuffer(conn->ReadBuffer());
    conn->Write();
  });

  loop->Loop();
  delete server;
  delete loop;
  return 0;
}
