#include "pch.h"
#include "LogKeeper.h"
#include "EditorConsole.h"
#include <Debug/Editor.h>
LogKeeper::LogKeeper(const LogKeeperInitDesc& desc) : RunSettings(desc)
{
	logFile = std::ofstream("log.txt", std::ios::out | std::ios::trunc);
	if (!logFile.is_open()) throw std::runtime_error("LogFile Failed to open!");


	Editor::GetMainToolGroup().GetOrPushGroup("Windows").PushObject<EditorConsole>();

	LoggingThread = std::thread(&LogKeeper::ThreadEntry,this);
	
}

void LogKeeper::Push(const std::string& origin, const std::string& Log)
{
	std::string l = "[" + origin + "] " + Log;
	std::unique_lock lock(LoggingMutex);
	LogsToPush.push(l);
}

void LogKeeper::ThreadEntry()
{
	logger.Print(std::string("Init. Flushing every " + std::to_string(RunSettings.FileFlushInternalMs) + "ms"));
	//std::cerr << "Init. Flushing every " << RunSettings.FileFlushInternalMs << "ms" <<std::endl;

	std::queue<std::string> LogsCopy;
	do
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(RunSettings.FileFlushInternalMs));

		std::unique_lock<std::mutex> lock(LoggingMutex);
		if (LogsToPush.empty()) continue;


		LogsToPush.swap(LogsCopy);

		lock.unlock();

		while (!LogsCopy.empty())
		{
			logFile << LogsCopy.front() << "\n";
			LogsCopy.pop();
		}

		logFile.flush();

	} while (!ShouldShutdown);
}
