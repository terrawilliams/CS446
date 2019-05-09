//
// Created by terra on 2/7/19.
//

#ifndef PROJECT1_METADATA_H
#define PROJECT1_METADATA_H


#include <string>
#include <vector>
#include <queue>
#include "configuration.h"

struct MDElement
{
    MDElement()
    {
        code = 'a';
        descriptor = "blank";
        numCycles = 0;
        totalTime = 0;
        numProcess = 0;
    }
    MDElement(char newCode, std::string newDescriptor, int newNumCycles, int newCycleTime, int newNumProcess)
    {
        code = newCode;
        descriptor = newDescriptor;
        numCycles = newNumCycles;
        totalTime = newCycleTime * newNumCycles;
        numProcess = newNumProcess;
    }

    MDElement &operator = (const MDElement &E)
    {
        code = E.code;
        descriptor = E.descriptor;
        numCycles = E.numCycles;
        totalTime = E.totalTime;
        numProcess = E.numProcess;
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
        numTasks = 0;
        numInputOutput = 0;
    }

    Process & operator = (const Process &P)
    {
        numTasks = P.numTasks;
        numInputOutput = P.numInputOutput;

        for(int i = 0; i < P.operations.size(); i++)
        {
            operations.push_back(P.operations[i]);
        }

        return *this;
    }

    int numTasks;
    int numInputOutput;
    int timeRemaining;
    std::vector<MDElement> operations;
};

class MetaData
{
    public:
        MetaData();
        ~MetaData();

        std::vector<MDElement> GetData();

        void ReadMetaData(Configuration confData);
        void ErrorCheck();

    private:
        std::vector<Process> mData;
        std::vector <MDElement> mSortedData;
        bool mStartFound;
        bool mEndFound;
        bool mSysStartFound;
        bool mSysEndFound;

        void AddToVector(std::string newElement, Configuration confData, int* index);
        int ExtractInt(std::string str);
        void Sort(std::string schedule, int quantumNum);
};


#endif //PROJECT1_METADATA_H
