#include <pch.h>
#include <Core/AppModule.h>
#include <Time/TimeTypes.h>
#include <Debug/Logging/Log.h>
class LogKeeper2 : public AppModule
{

private:
    static inline LogKeeper2 *instance = nullptr;

    std::thread flushing_thread;
    std::ofstream log_file;
    milliseconds flush_interval = milliseconds(200);
    std::mutex flush_interval_mutex;
    Log logger = Log("Logger");
    std::atomic_bool should_shutdown = false;
    std::vector<int8_t> log_buffer;
    std::mutex log_buffer_mutex;

public:
    std::vector<AppModule::EngineCallInject> GetInjections() override;

    void SetFlushInterval(milliseconds ms);
    void PushLog(const std::string &origin, const std::string &log);
    static LogKeeper2 *GetInstance()
    {
        LASSERT(instance != nullptr, "Instance doesnt exists. Not initialized?");
        return instance;
    }

private:
    void Init();
    void Shutdown();

    void flush_thread_entry();
};