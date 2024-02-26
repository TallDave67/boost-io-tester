#pragma once

#include <string>

class CBoostIoTesterConfig;

class CBoostIoTesterAsio
{
public:
    void get_weather(CBoostIoTesterConfig* pConfig, const std::string& lat, const std::string& lon);
};