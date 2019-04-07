//
// Created by terra on 2/5/19.
//

#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <sstream>
#include "configuration.h"

Configuration::Configuration()
{
    mVersion = 0.0;
    mMonitor = 0;
    mProjector = 0;
    mMemory = 0;
    mKeyboard = 0;
    mHardDrive = 0;
    mScanner = 0;
    mProcessor = 0;
    mSysMemory = 0;
    mFilePath = "empty";
    mOutputFile = "empty";
    mLogToFile = false;
    mLogToMonitor = false;
    mStartFound = false;
    mEndFound = false;
}

Configuration::~Configuration()
{

}

float Configuration::GetVersion()
{
    return mVersion;
}

int Configuration::GetMonitorCycleTime()
{
    return mMonitor;
}

int Configuration::GetProcessorCycleTime()
{
    return mProcessor;
}

int Configuration::GetScannerCycleTime()
{
    return mScanner;
}

int Configuration::GetHardDriveCycleTime()
{
    return mHardDrive;
}

int Configuration::GetKeyboardCycleTime()
{
    return mKeyboard;
}

int Configuration::GetMemoryCycleTime()
{
    return mMemory;
}

int Configuration::GetProjectorCycleTime()
{
    return mProjector;
}

int Configuration::GetSysMemory()
{
    return mSysMemory;
}

int Configuration::GetBlockSize()
{
    return mBlockSize;
}

int Configuration::GetProjectorQuantity()
{
    return mProjQuantity;
}

int Configuration::GetHardDriveQuantity()
{
    return mHardDriveQuantitiy;
}

std::string Configuration::GetFilePath()
{
    return mFilePath;
}

bool Configuration::GetLogToFile()
{
    return mLogToFile;
}

bool Configuration::GetLogToMonitor()
{
    return mLogToMonitor;
}

std::string Configuration::GetOutputFile()
{
    return mOutputFile;
}

void Configuration::ReadFile(std::string filename)
{
    std::regex extension("(.*)(.conf)");

    if(!std::regex_match(filename, extension))
    {
        std::cout << "Error: invalid extension for configuration file" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::ifstream confFile;

    confFile.open(filename);

    if(!confFile.is_open())
    {
        std::cout << "Error: Invalid configuration file name" << std::endl;
        exit(EXIT_FAILURE);
    }

    if(confFile.peek() == std::ifstream::traits_type::eof())
    {
        std::cout << "Error: Empty configuration file" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::string confLine;

    std::regex start(" *Start +Simulator +Configuration +File *");
    std::regex version(" *Version */ *Phase *: +[0-9]+.[0-9]+");
    std::regex fileName(" *File +Path *: +(.*)");
    std::regex fileExtension(" *File +Path *: +(.*).mdf *");
    std::regex monitor(" *Monitor +display +time +\\{ *msec *\\} *: +[0-9]+ *");
    std::regex processor(" *Processor +cycle +time +\\{ *msec *\\} *: +[0-9]+ *");
    std::regex scanner(" *Scanner +cycle +time +\\{ *msec *\\} *: +[0-9]+ *");
    std::regex hardDrive("Hard drive cycle time \\{msec\\}: [0-9]+");
    std::regex keyboard(" *Keyboard +cycle +time +\\{ *msec *\\} *: +[0-9]+ *");
    std::regex memory(" *Memory +cycle +time +\\{ *msec *\\} *: +[0-9]+ *");
    std::regex projector(" *Projector +cycle +time +\\{ *msec *\\} *: +[0-9]+ *");
    std::regex sysMemory(" *System +memory +\\{ *\\w+ *\\} *: +[0-9]+ *");
    std::regex blockSize(" *Memory +block +size +\\{ *\\w+ *\\} *: +[0-9]+ *");
    std::regex projQuantity (" *Projector +quantity *: +[0-9]+ *");
    std::regex hardDriveQuantity(" *Hard +drive +quantity *: +[0-9]+ *");
    std::regex log(" *Log *: +Log +to +(.*)");
    std::regex filePath(" *Log +File +Path *: +(.*)");
    std::regex end(" *End +Simulator +Configuration +File *");

    while(getline(confFile, confLine))
    {
        if(std::regex_match(confLine, start))
        {
            mStartFound = true;
        }
        else if(std::regex_match(confLine, version))
        {
            mVersion = ExtractFloat(confLine);
        }
        else if(std::regex_match(confLine, fileName))
        {
            if(!std::regex_match(confLine, fileExtension))
            {
                std::cout << "Error: invalid metadata file extension" << std::endl;
                exit(EXIT_FAILURE);
            }

            std::stringstream ss;

            ss << confLine;

            std::string temp;

            ss >> temp;
            ss >> temp;

            ss >> mFilePath;
        }
        else if(std::regex_match(confLine, monitor))
        {
            mMonitor = ExtractInt(confLine);
        }
        else if(std::regex_match(confLine, processor))
        {
            mProcessor = ExtractInt(confLine);
        }
        else if(std::regex_match(confLine, scanner))
        {
            mScanner = ExtractInt(confLine);
        }
        else if(std::regex_match(confLine, hardDrive))
        {
            mHardDrive = ExtractInt(confLine);
        }
        else if(std::regex_match(confLine, keyboard))
        {
            mKeyboard = ExtractInt(confLine);
        }
        else if(std::regex_match(confLine, memory))
        {
            mMemory = ExtractInt(confLine);
        }
        else if(std::regex_match(confLine, projector))
        {
            mProjector = ExtractInt(confLine);
        }
        else if(std::regex_match(confLine, sysMemory))
        {
            mSysMemory = ExtractInt(confLine);

            std::stringstream ss;

            ss << confLine;

            std::string temp;
            std::string units;

            ss >> temp;
            ss >> temp;

            ss >> units;

            std::regex kbytes("\\{kbytes\\}:");
            std::regex mbytes("\\{Mbytes\\}:");
            std::regex gbytes("\\{Gbytes\\}:");

            if(std::regex_match(units, kbytes))
            {

            }
            else if(std::regex_match(units, mbytes))
            {
                mSysMemory *= 1000;
            }
            else if(std::regex_match(units, gbytes))
            {
                mSysMemory *= 1000000;
            }
            else
            {
                std::cout << "Error: invalid units: " << confLine << std::endl;
                exit(EXIT_FAILURE);
            }
        }
        else if(std::regex_match(confLine, blockSize))
        {
            mBlockSize = ExtractInt(confLine);

            std::regex kBytes (".*\\{kbytes\\}.*");
            std::regex mBytes (".*\\{Mbytes\\}.*");
            std::regex gBytes (".*\\{Gbytes\\}.*");

            if(std::regex_match(confLine, kBytes))
            {

            }
            else if(std::regex_match(confLine, mBytes))
            {
                mBlockSize *= 1000;
            }
            else if(std::regex_match(confLine, gBytes))
            {
                mBlockSize *= 1000000;
            }
            else
            {
                std::cout << "Error: invalid units: " << confLine << std::endl;
                exit(EXIT_FAILURE);
            }
        }
        else if(std::regex_match(confLine, projQuantity))
        {
            mProjQuantity = ExtractInt(confLine);
        }
        else if(std::regex_match(confLine, hardDriveQuantity))
        {
            mHardDriveQuantitiy = ExtractInt(confLine);
        }
        else if(std::regex_match(confLine, log))
        {
            std::stringstream ss;

            ss << confLine;

            std::string temp;
            std::string logTo;

            ss >> temp;
            ss >> temp;
            ss >> temp;

            ss >> logTo;

            if(logTo == "Both")
            {
                mLogToMonitor = true;
                mLogToFile = true;
            }
            else if(logTo == "Monitor")
                mLogToMonitor = true;
            else if(logTo == "File")
                mLogToFile = true;
        }
        else if(std::regex_match(confLine, filePath))
        {
            std::stringstream ss;

            ss << confLine;

            std::string temp;

            ss >> temp;
            ss >> temp;
            ss >> temp;

            ss >> mOutputFile;
        }
        else if(std::regex_match(confLine, end))
        {
            mEndFound = true;
        }
        else
        {
            std::cout << "Error: unexpected line or incorrect spelling in line \"" << confLine << "\"" << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    confFile.close();
}

void Configuration::ErrorCheck()
{
    if(mMonitor <= 0)
    {
        std::cout << "Error: zero or negative value for monitor display time" << std::endl;
        exit(EXIT_FAILURE);
    }
    if(mProcessor <= 0)
    {
        std::cout << "Error: zero or negative value for processor cycle time" << std::endl;
        exit(EXIT_FAILURE);
    }
    if(mScanner <= 0)
    {
        std::cout << "Error: zero or negative value for scanner cycle time" << std::endl;
        exit(EXIT_FAILURE);
    }
    if(mHardDrive <= 0)
    {
        std::cout << "Error: zero or negative value for hard drive cycle time" << std::endl;
        exit(EXIT_FAILURE);
    }
    if(mKeyboard <= 0)
    {
        std::cout << "Error: zero or negative value for keyboard cycle time" << std::endl;
        exit(EXIT_FAILURE);
    }
    if(mMemory <= 0)
    {
        std::cout << "Error: zero or negative value for memory cycle time" << std::endl;
        exit(EXIT_FAILURE);
    }
    if(mProjector <= 0)
    {
        std::cout << "Error: zero or negative value for projector cycle time" << std::endl;
        exit(EXIT_FAILURE);
    }

    if(mSysMemory <= 0)
    {
        std::cout << "Error: zero or negative value for system memory" << std::endl;
        exit(EXIT_FAILURE);
    }

    if(mBlockSize <= 0)
    {
        std::cout << "Error: zero or negative value for memory block size" << std::endl;
        exit(EXIT_FAILURE);
    }

    if(!mStartFound)
    {
        std::cout << "Error: missing \"Start Simulator Configuration File\" line in config file" << std::endl;
        exit(EXIT_FAILURE);
    }
    if(!mEndFound)
    {
        std::cout << "Error: missing \"End Simulator Configuration File\" line in config file" << std::endl;
        exit(EXIT_FAILURE);
    }

    if(!mLogToMonitor and !mLogToFile)
    {
        std::cout << "Error: Log location not specified in configuration file" << std::endl;
        exit(EXIT_FAILURE);
    }
}

int Configuration::ExtractInt(std::string str)
{
    std::stringstream ss;

    ss << str;

    std::string temp;
    int found;

    while(!ss.eof())
    {
        ss >> temp;
        if (std::stringstream(temp) >> found)
            return found;
    }

    return -1;
}

float Configuration::ExtractFloat(std::string str)
{
    std::stringstream ss;

    ss << str;

    std::string temp;
    float found;

    while(!ss.eof())
    {
        ss >> temp;
        if(std::stringstream(temp) >> found)
            return found;
    }

    return -1;
}
