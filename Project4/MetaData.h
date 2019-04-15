//
// Created by terra on 2/7/19.
//

#ifndef PROJECT1_METADATA_H
#define PROJECT1_METADATA_H


#include <string>
#include <vector>
#include "configuration.h"

struct MDElement
{
    MDElement(char newCode, std::string newDescriptor, int newNumCycles, int newCycleTime, int newNumProcess)
    {
        code = newCode;
        descriptor = newDescriptor;
        numCycles = newNumCycles;
        totalTime = newCycleTime * newNumCycles;
        numProcess = newNumProcess;
    }

    char code;
    std::string descriptor;
    int numCycles;
    int totalTime;
    int numProcess;
};

struct Process
{
    Process()
    {
        numProcesses = 0;
    }

    int numProcesses;
    std::vector<MDElement> operations;
};

class MetaData
{
    public:
        MetaData();
        ~MetaData();

        std::vector<Process> GetData();

        void ReadMetaData(Configuration confData);
        void ErrorCheck();

    private:
        std::vector<Process> mData;
        bool mStartFound;
        bool mEndFound;
        bool mSysStartFound;
        bool mSysEndFound;

        void AddToVector(std::string newElement, Configuration confData, int* index);
        int ExtractInt(std::string str);
        void Sort();
};


#endif //PROJECT1_METADATA_H
