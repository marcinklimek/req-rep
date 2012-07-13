#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <map>
#include <boost/thread.hpp>
#include <boost/lexical_cast.hpp>
#include "zmq2a.h"

#define SK_ADDR_SRV "tcp://*:5556"

class Server;

class Command
{
public:
    
    virtual void execute(Server* const server, const std::string& argument) = 0;
};


class GetCommand : public Command
{
    void execute(Server* const server, const std::string& argument);
};

class AckCommand : public Command
{
    void execute(Server* const server, const std::string& argument);
private:
    int ack_id;
};



class Server
{
private:
    int message_id_;
    
    zmq::context_t                      context_;
    std::unique_ptr<zmq::Socket>        socket_;
    std::map<std::string, Command* >    commands_map_;
    
   
    void reset();
    
public:
    
    Server();
    
    void work();
    
    zmq::Socket* const getSocket() { return socket_.get(); } const
    
    int getID()   { return message_id_;}
    void nextID() { ++message_id_; };
   
};


#endif // SERVER_H
