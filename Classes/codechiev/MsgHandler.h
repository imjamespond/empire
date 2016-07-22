#pragma once
#include <external/json/document.h>

namespace codechiev
{
	class MsgHandler
	{
	public:
		explicit MsgHandler(const char* );
		~MsgHandler();

		inline bool hasError() { return error_; }
	private:
		rapidjson::Document jsonDoc_;
		bool error_;
	};
}
