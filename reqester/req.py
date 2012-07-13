#!/usr/bin/python

from txzmq import ZmqFactory
from txzmq import ZmqEndpoint, ZmqEndpointType

from twisted.internet import reactor, defer

from util import REQTimeoutConnection

class Processor:

    def __init__(self):

        endpoint = "tcp://localhost:5556"

        self.zf = ZmqFactory()
        self.e = ZmqEndpoint( ZmqEndpointType.connect, endpoint)
        self.s = None

        self.restart()

    @defer.inlineCallbacks
    def work(self, arg=None):

        try:

            while True:
                print 'get'
                message = yield self.s.sendMsg( 'GET' )

                print 'got message:', message

                msg_id = message[0].split('#')[0]

                print 'processing', msg_id

                reply = yield self.s.sendMsg('ACK' + msg_id )
                print 'reply:', reply

        except Exception as ex:
            print 
            print 'Exception: ', ex

        self.restart()


    def restart(self):

        print 'New restart'

        if self.s is not None:
            self.s.shutdown()

        self.s = REQTimeoutConnection(self.zf, self.e)

        reactor.callWhenRunning( self.work )


p = Processor()
reactor.run()
