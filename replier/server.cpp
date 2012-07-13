#include "server.h"



void GetCommand::execute(Server* const server, const std::string& argument)
{
    std::cout << "GetCommand" << std::endl;

    std::string data = "abc;sdfsdf;sdfsdf;abcd sdf sdf sdf";

    std::string message = boost::lexical_cast<std::string>( server->getID() ) + "#" + data;
    server->getSocket()->send(message);
}


void AckCommand::execute(Server* const server, const std::string& argument)
{

    std::cout << "ACK command" << std::endl;

    try
    {
        int ack_id = boost::lexical_cast<int>( argument );

        std::string message = "OK";
        server->getSocket()->send(message);

        server->nextID();

    }
    catch( boost::bad_lexical_cast const& )
    {
        std::cout << "Wrong ACK ID" << std::endl;
    }
}

void Server::work()
{
    while (true)
    {
        std::string command  = socket_->recvAsString();
        std::string argument = command.substr(3);
        command = command.substr(0, 3);

        auto itr = commands_map_.find(command);
        if ( itr != commands_map_.end() )
        {
            (*itr).second->execute( this,  argument );
        }
        else
            std::cout << "not found" << std::endl;
    }
}


Server::Server() : message_id_(0), context_( zmq::context_t(1) )
{
    commands_map_.insert( std::make_pair<std::string, Command* >("GET", new GetCommand()) );
    commands_map_.insert( std::make_pair<std::string, Command* >("ACK", new AckCommand()) );
    
    reset();
}

void Server::reset()
{
    std::cout << "Starting server" << std::endl;

    socket_.reset( new zmq::Socket( context_, ZMQ_REP) );
    socket_->bind(SK_ADDR_SRV);

    int linger = 0;
    socket_->setsockopt(ZMQ_LINGER, &linger, sizeof(linger));
}
 
   

