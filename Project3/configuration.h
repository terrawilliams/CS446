//
// Created by terra on 2/5/19.
//

#ifndef PROJECT1_CONFIGURATION_H
#define PROJECT1_CONFIGURATION_H


#include <string>

class Configuration
{
public:
    Configuration();
    ~Configuration();

    float GetVersion();

    int GetMonitorCycleTime();

    int GetProcessorCycleTime();

    int GetScannerCycleTime();

    int GetHardDriveCycleTime();

    int GetKeyboardCycleTime();

    int GetMemoryCycleTime();

    int GetProjectorCycleTime();

    int GetSysMemory();

    int GetBlockSize();

    int GetProjectorQuantity();

    int GetHardDriveQuantity();

    std::string GetFilePath();

    bool GetLogToFile();

    bool GetLogToMonitor();

    std::string GetOutputFile();

    void ReadFile(std::string filename);
    void ErrorCheck();


private:
    float mVersion;
    int mMonitor;
    int mProcessor;
    int mScanner;
    int mHardDrive;
    int mKeyboard;
    int mMemory;
    int mProjector;
    int mSysMemory;
    int mBlockSize;
    int mProjQuantity;
    int mHardDriveQuantitiy;
    std::string mFilePath;
    bool mLogToFile;
    bool mLogToMonitor;
    std::string mOutputFile;
    bool mStartFound;
    bool mEndFound;

    int ExtractInt(std::string str);
    float ExtractFloat(std::string str);
};


#endif //PROJECT1_CONFIGURATION_H