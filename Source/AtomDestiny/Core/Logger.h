#pragma once

#include <Runtime/Core/Public/Logging/LogMacros.h>

#define LOG_INFO(Format, ...) UE_LOG(LogTemp, Log, Format, ##__VA_ARGS__)
#define LOG_WARNING(Format, ...) UE_LOG(LogTemp, Warning, Format, ##__VA_ARGS__)
#define LOG_ERROR(Format, ...) UE_LOG(LogTemp, Error, Format, ##__VA_ARGS__)
#define LOG_FATAL(Format, ...) UE_LOG(LogTemp, Fatal, Format, ##__VA_ARGS__)

#define ENSURE_WARNING(CheckStatement, Format, ...) \
        if (!(CheckStatement)) \
                LOG_WARNING(Format, ##__VA_ARGS__);

#define ENSURE_ERROR(CheckStatement, Format, ...) \
        if (!(CheckStatement)) \
                LOG_ERROR(Format, ##__VA_ARGS__);

#define ENSURE_FATAL(CheckStatement, Format, ...) \
        if (!(CheckStatement)) \
                LOG_FATAL(Format, ##__VA_ARGS__);
