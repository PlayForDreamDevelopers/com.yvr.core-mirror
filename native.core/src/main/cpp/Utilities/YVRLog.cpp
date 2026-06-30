#include "./YVRLog.h"

#include <android/log.h>

#include <memory>
#include <stdexcept>

#define LOG_VIA_UNITY(func)                                   \
    va_list args;                                             \
    va_start(args, format);                                   \
    int size_s = std::snprintf(nullptr, 0, format, args) + 1; \
    auto size = static_cast<size_t>(size_s);                  \
    std::unique_ptr<char[]> buf(new char[size]);              \
    std::snprintf(buf.get(), size, format, args);             \
    std::string result(buf.get(), buf.get() + size - 1);      \
    func(result.c_str());                                     \
    va_end(args);

#define STANDARD_LOG_CALL(priority)                    \
    va_list args;                                      \
    va_start(args, format);                            \
    __android_log_vprint(priority, tag, format, args); \
    va_end(args);

#define LOG_CALL(priority, unityHandle) \
    if (unityHandle != nullptr)         \
    {                                   \
        LOG_VIA_UNITY(unityHandle);     \
    }                                   \
    else                                \
    {                                   \
        STANDARD_LOG_CALL(priority);    \
    }

YVRLog *YVRLog::instance = nullptr;

YVRLog *YVRLog::GetInstance()
{
    if (instance == nullptr) instance = new YVRLog();
    return instance;
}

YVRLog::YVRLog() : debugHandler(nullptr), infoHandler(nullptr), warnHandler(nullptr), errorHandler(nullptr)
{
    const char *tag = "Core";
    memset(this->tag, '\0', sizeof(this->tag));
    memcpy(this->tag, tag, sizeof(char) * strlen(tag));
}

void YVRLog::Debug(const char *format, ...) { LOG_CALL(ANDROID_LOG_DEBUG, debugHandler); }
void YVRLog::Info(const char *format, ...) { LOG_CALL(ANDROID_LOG_INFO, infoHandler); }
void YVRLog::Warn(const char *format, ...) { LOG_CALL(ANDROID_LOG_WARN, warnHandler); }
void YVRLog::Error(const char *format, ...) { LOG_CALL(ANDROID_LOG_ERROR, errorHandler); }

void YVRLog::StdInfo(const char *format, ...) { STANDARD_LOG_CALL(ANDROID_LOG_INFO); }
void YVRLog::StdDebug(const char *format, ...) { STANDARD_LOG_CALL(ANDROID_LOG_DEBUG); }
void YVRLog::StdWarn(const char *format, ...) { STANDARD_LOG_CALL(ANDROID_LOG_WARN); }
void YVRLog::StdError(const char *format, ...) { STANDARD_LOG_CALL(ANDROID_LOG_ERROR); }

void YVRLog::SetDebugOutputHandler(OutputLogViaUnityHandler debugHandler) { this->debugHandler = debugHandler; }
void YVRLog::SetInfoOutputHandler(OutputLogViaUnityHandler infoHandler) { this->infoHandler = infoHandler; }
void YVRLog::SetWarnOutputHandler(OutputLogViaUnityHandler warnHandler) { this->warnHandler = warnHandler; }
void YVRLog::SetErrorOutputHandler(OutputLogViaUnityHandler errorHandler) { this->errorHandler = errorHandler; }

std::string StrFormat(const char* fmt, ...)
{
    va_list vl;
    va_start(vl, fmt);
    int size = std::vsnprintf(nullptr, 0, fmt, vl);
    va_end(vl);

    if (size != -1)
    {
        std::unique_ptr<char[]> buffer(new char[size + 1]);

        va_start(vl, fmt);
        size = std::vsnprintf(buffer.get(), size + 1, fmt, vl);
        va_end(vl);
        if (size != -1)
        {
            return std::string(buffer.get(), size);
        }
    }

    throw std::runtime_error("Unexpected vsnprintf failure");
}
