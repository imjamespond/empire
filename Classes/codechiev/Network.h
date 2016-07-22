#pragma once

#include "cocos2d.h"
#include "network/WebSocket.h"
#include <string>

namespace codechiev
{
	class Network : public cocos2d::network::WebSocket::Delegate
	{
	public:
        typedef std::function<void()> on_open_func;
        
		Network();
		~Network();

		virtual void onOpen(cocos2d::network::WebSocket* ws)override;
		virtual void onMessage(cocos2d::network::WebSocket* ws, const cocos2d::network::WebSocket::Data& data)override;
		virtual void onClose(cocos2d::network::WebSocket* ws)override;
		virtual void onError(cocos2d::network::WebSocket* ws, const cocos2d::network::WebSocket::ErrorCode& error)override;

		void connect();
        void connect(const on_open_func&);
		void send(const std::string&);
        void close();

		inline bool isConnected() 
		{
			if(wssocket_) return wssocket_->getReadyState()==cocos2d::network::WebSocket::State::OPEN;
			else return false;
		}
	private:
		cocos2d::network::WebSocket* wssocket_;
        on_open_func onOpen_;
	};
}


