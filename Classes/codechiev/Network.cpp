#include "Network.h"
#include "BasisUtil.h"
#include "MsgHandler.h"
#include "../layer/NotificationLayer.h"

USING_NS_CC;
using namespace codechiev;

Network::Network():wssocket_(nullptr)
{
}


Network::~Network()
{
	if (wssocket_)
    {
        wssocket_->close();
        CC_SAFE_DELETE(wssocket_);
    }
}

void
Network::connect()
{
	wssocket_ = new network::WebSocket();
	if (!wssocket_->init(*this, "ws://192.168.2.1:9001/websocket"))//139.224.63.101:9001
	{
		CC_SAFE_DELETE(wssocket_);
	}
}

void
Network::connect(const on_open_func& onOpen)
{
    onOpen_ = onOpen;
    connect();
}

// Delegate methods
void
Network::onOpen(network::WebSocket* ws)
{
	log("Websocket (%p) opened", ws);
	if (ws == wssocket_)
	{
		if(onOpen_)
        {
            onOpen_();
        }
        base::Singleton<BaseDebug>::get()->add(StringUtils::format("onOpen"));
	}
}

void 
Network::onMessage(cocos2d::network::WebSocket* ws, const cocos2d::network::WebSocket::Data& data)
{
	if (ws == wssocket_)
	{
        cocos2d:log("onMessage:%s", data.bytes);
		codechiev::MsgHandler msgHandler(data.bytes);
	}
}
void 
Network::onClose(cocos2d::network::WebSocket* ws)
{
	if (ws == wssocket_ && gNotificationLayer)
	{
        gNotificationLayer->autoReconnect();
        
        auto debug = base::Singleton<BaseDebug>::get();
		if(debug)debug->add(StringUtils::format("onClose"));
	}
}
void 
Network::onError(cocos2d::network::WebSocket* ws, const cocos2d::network::WebSocket::ErrorCode& error)
{
	if (ws == wssocket_)
	{
		log("onError.");
	}
}

void 
Network::send(const std::string& msg)
{
	if (wssocket_ && wssocket_->getReadyState()==network::WebSocket::State::OPEN)
	{
		wssocket_->send(msg);
	}
}

void
Network::close()
{
    if (wssocket_)
    {
        wssocket_->close();
    }
}
