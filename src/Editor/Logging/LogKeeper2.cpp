#include "LogKeeper2.h"

std::vector<AppModule::EngineCallInject> LogKeeper2::GetInjections()
{
    
        AppModule::EngineCallInject inject_start("LogKeeper Start");
        inject_start.call_point = AppModule::EngineCallPoints::START;
        inject_start.priority = INT64_MAX;
        inject_start.call = std::bind(&LogKeeper2::Init, this);

        AppModule::EngineCallInject inject_end("LogKeeper End");
        inject_end.call_point = AppModule::EngineCallPoints::END;
        inject_end.priority = INT64_MIN;
        inject_end.call = std::bind(&LogKeeper2::Shutdown, this);
    return {inject_start,inject_end};
    
}

void LogKeeper2::SetFlushInterval(milliseconds ms)
{
    std::unique_lock<std::mutex> lock(flush_interval_mutex);
    flush_interval = ms;
}

void LogKeeper2::PushLog(const std::string &origin, const std::string &log)
{
    uint32_t space_required = origin.length() + log.length() + 4; // plus both [, space and newline
    std::string f_push = '[' + origin + ']' + ' ' + log + '\n';
    std::unique_lock lock(log_buffer_mutex);
    const auto existing_size = log_buffer.size();
    if (log_buffer.capacity() - existing_size < space_required)
        log_buffer.resize(existing_size + space_required,0);
    std::copy(f_push.data(), f_push.data() + f_push.size(), log_buffer.data()+existing_size);
}

LogKeeper2::LogKeeper2()
{
    LASSERT(!instance, "LogKeeper instance already exists");
    instance = this;
    log_file = std::ofstream("log.txt", std::ios::out);
    if (!log_file.is_open())
    {
        logger.Critical("Failed to open log file for writing");
        LASSERT(false, "idiot");
        return;
    }
    logger.Debug("Logging Init");
    flushing_thread = std::thread(&LogKeeper2::flush_thread_entry, this);
}

void LogKeeper2::Init()
{
   
}

void LogKeeper2::Shutdown()
{
    logger.Debug("Logging closing");

    should_shutdown.store(true);
    flushing_thread.join();
    log_file.close();
    logger.Debug("Logging closed");
}

void LogKeeper2::flush_thread_entry()
{
    while (true)
    {
        //std::cerr << "Loop\n";
        {
            std::unique_lock buffer_lock(log_buffer_mutex);
            bool buffer_empty = log_buffer.empty();
            if (should_shutdown.load() && buffer_empty)
                break;
            if (!buffer_empty)
            {
                log_file.write(reinterpret_cast<char *>(log_buffer.data()), log_buffer.size());
                log_file.flush();
                log_buffer.clear();
            }

        }

        std::unique_lock<std::mutex> lock(flush_interval_mutex);
        milliseconds sleep_for = flush_interval;
        std::this_thread::sleep_for(flush_interval);
    }
    log_file.close();
    //std::cerr << "Thread Closed\n";
}
