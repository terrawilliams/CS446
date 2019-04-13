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
    MDElement(char newCode, std::string newDescriptor, int newNumCycles, int newCycleTime)
    {
        code = newCode;
        descriptor = newDescriptor;
        numCycles = newNumCycles;
        totalTime = newCycleTime * newNumCycles;
    }

    char code;
    std::string descriptor;
    int numCycles;
    int totalTime;
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
        std::vector<MDElement> mData;
        bool mStartFound;
        bool mEndFound;
        bool mSysStartFound;
        bool mSysEndFound;

        void AddToVector(std::string newElement, Configuration confData);
        int ExtractInt(std::string str);
};


#endif //PROJECT1_METADATA_H
