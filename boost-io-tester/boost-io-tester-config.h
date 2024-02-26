#pragma once

#include <string>

class CBoostIoTesterConfig
{
public:
    bool read_config();

public:
    std::string weather_server_appid;
};