from txzmq import ZmqREQConnection
from twisted.internet import reactor, defer


class TimeoutError(Exception):
    def __init__(self, value = "Time Out"):
        self.value = value
    def __str__(self):
        return repr(self.value)

def timeout(secs=1):
    """
    Decorator to add timeout to Deferred calls
    """
    def wrap(func):
        @defer.inlineCallbacks
        def _timeout(*args, **kwargs):
            rawD = func(*args, **kwargs)
            if not isinstance(rawD, defer.Deferred):
                defer.returnValue(rawD)

            timeoutD = defer.Deferred()
            timesUp = reactor.callLater(secs, timeoutD.callback, None)

            try:
                rawResult, timeoutResult = yield defer.DeferredList([rawD, timeoutD], fireOnOneCallback=True, fireOnOneErrback=True, consumeErrors=True)
            except defer.FirstError, e:
                #Only rawD should raise an exception
                assert e.index == 0
                timesUp.cancel()
                e.subFailure.raiseException()
            else:
                #Timeout
                if timeoutD.called:
                    rawD.cancel()
                    #rawD.errback( TimeoutError("%s secs have expired" % secs) )
                    raise TimeoutError("%s secs have expired" % secs)

            #No timeout
            timesUp.cancel()
            defer.returnValue(rawResult)
        return _timeout
    return wrap

class REQTimeoutConnection( ZmqREQConnection):

    def __init__(self, *args, **kwargs):
        ZmqREQConnection.__init__(self, *args, **kwargs)

    @timeout(2)
    def sendMsg(self, *messageParts):

        d = ZmqREQConnection.sendMsg( self, *messageParts )
        return d

