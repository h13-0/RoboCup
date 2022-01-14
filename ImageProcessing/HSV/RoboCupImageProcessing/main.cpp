#include <iostream>
#include <string>
#include <gflags/gflags.h> 
#include "MainWorkingFlow.hpp"
#include "Configs.hpp"

#define GLOG_NO_ABBREVIATED_SEVERITIES 
#include <glog/logging.h>

//DEFINE_string(configs, "/home/h13/.vs/RoboCupImageProcessing/7f42b696-d5b4-46ba-9e2b-6277ebe4641b/src/configs.yml", "YAML config file path.");
DEFINE_string(configs, "./configs.yml", "YAML config file path.");
DEFINE_string(generate, "", "Generate default config file path.");
DEFINE_string(mode, "MainWorkingFlow",
    R"(Working mode,
       Available Mode : 
       - MainWorkingFlow  #default.
       )");

int main(int argc, char* argv[])
{
    using namespace std;
    using namespace RoboCup;

    google::SetUsageMessage("type --help for help.");
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    if (FLAGS_generate.length() > 0)
    {
        //TODO: generate default config file.
        return 0;
    }

    Configs config;
    try
    {
        config = Configs(FLAGS_configs);
    }
    catch (...)
    {
        LOG(FATAL) << "Open config file: " + FLAGS_configs +" error!";
        LOG(FATAL) << "Aborted.";
        return -1;
    }
    
    if (!FLAGS_mode.compare("MainWorkingFlow"))
    {
        RoboCup::MainWorkingFlow app(config);
        int ret = app.Run();
        LOG(INFO) << "Application exit with return code: " << ret;
    }
    else {

        return 0;
    }
}
