#pragma once


#include  <IInternalModule.h>


#include "Log.h"
#include <mutex>
#include <queue>
#include <thread>
#include <fstream>
	struct LogKeeperInitDesc
	{
		int FileFlushInternalMs = 250;
	};
	class LogKeeper : public Landmark::Systems::IInternalModule<LogKeeper>
	{
		friend class Application;

		const LogKeeperInitDesc RunSettings;
		std::mutex LoggingMutex;
		std::queue<std::string> LogsToPush;
		std::thread LoggingThread;
		std::ofstream logFile;
		bool ShouldShutdown = false;
		Log logger = Log("LogKeeper");
		
		void ThreadEntry();
	public:
		LogKeeper(const LogKeeperInitDesc& desc);

		void Push(const std::string& origin, const std::string& Log);
	};

