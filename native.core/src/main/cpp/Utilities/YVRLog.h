#pragma once

#include <string>

#define YDebug(...) YVRLog::GetInstance()->Debug(__VA_ARGS__);
#define YInfo(...) YVRLog::GetInstance()->Info(__VA_ARGS__);
#define YWarn(...) YVRLog::GetInstance()->Warn(__VA_ARGS__);
#define YError(...) YVRLog::GetInstance()->Error(__VA_ARGS__);

#define YStdDebug(...) YVRLog::GetInstance()->StdDebug(__VA_ARGS__);
#define YStdInfo(...) YVRLog::GetInstance()->StdInfo(__VA_ARGS__);
#define YStdWarn(...) YVRLog::GetInstance()->StdWarn(__VA_ARGS__);
#define YStdError(...) YVRLog::GetInstance()->StdError(__VA_ARGS__);

typedef void (*OutputLogViaUnityHandler)(const char* logMsg);

class YVRLog
{
public:
    static YVRLog* GetInstance();

    YVRLog();
    ~YVRLog();

    void Debug(const char* format, ...);
    void Info(const char* format, ...);
    void Warn(const char* format, ...);
    void Error(const char* format, ...);

    void StdDebug(const char* format, ...);
    void StdInfo(const char* format, ...);
    void StdWarn(const char* format, ...);
    void StdError(const char* format, ...);

    void SetDebugOutputHandler(OutputLogViaUnityHandler debugHandler);
    void SetInfoOutputHandler(OutputLogViaUnityHandler infoHandler);
    void SetWarnOutputHandler(OutputLogViaUnityHandler warnHandler);
    void SetErrorOutputHandler(OutputLogViaUnityHandler errorHandler);

private:
    static YVRLog* instance;

    OutputLogViaUnityHandler debugHandler;
    OutputLogViaUnityHandler infoHandler;
    OutputLogViaUnityHandler warnHandler;
    OutputLogViaUnityHandler errorHandler;

    char tag[64]; // The max character length for tag is 64
};

std::string StrFormat(const char* fmt, ...);
