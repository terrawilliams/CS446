//
// Created by terra on 2/7/19.
//

#include <fstream>
#include <iostream>
#include <regex>
#include "MetaData.h"
#include "configuration.h"

MetaData::MetaData()
{
    mSysStartFound = false;
    mSysEndFound = false;
    mEndFound = false;
    mStartFound = false;
}

MetaData::~MetaData()
{
    mData.clear();
}

std::vector<MDElement> MetaData::GetData()
{
    return mSortedData;
}

void MetaData::ReadMetaData(Configuration confData)
{
    std::string filename = confData.GetFilePath();
    std::ifstream metaFile;
    metaFile.open(filename);

    if(!metaFile.is_open())
    {
        std::cout << "Error: Invalid metadata file name" << std::endl;
        exit(EXIT_FAILURE);
    }

    if(metaFile.peek() == std::ifstream::traits_type::eof())
    {
        std::cout << "Error: Empty metadata file" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::regex start("Start Program Meta-Data Code:");
    std::regex end("End Program Meta-Data Code.");

    std::regex general(" *[S|A|P|I|O|M] *\\{ *\\w+ *\\w* *\\} *[0-9]+ *[;|.][\n|\r]*");

    std::string metaLine;

    int index = 0;

    while(getline(metaFile, metaLine))
    {
        if(std::regex_match(metaLine, start))
        {
            mStartFound = true;
        }
        else if(std::regex_match(metaLine, end))
        {
            mEndFound = true;
        }
        else
        {
            while(!metaLine.empty())
            {
                std::smatch match;

                if(std::regex_search(metaLine, match, general))
                {
                    if(!match.prefix().str().empty())
                    {
                        std::cout << "Error: unexpected input in metadata file: " << match.prefix().str() << std::endl;
                        exit(EXIT_FAILURE);
                    }

                    AddToVector(match.str(), confData, &index);

                    metaLine = match.suffix().str();
                }
                else
                {
                    std::cout << "Error: unexpected input in metadata file: " << metaLine << std::endl;
                    exit(EXIT_FAILURE);
                }

            }

        }

    }

    metaFile.close();

    int totalTime = 0;

    for(int i = 0; i < mData.size(); i++)
    {
        for(int j = 0; j < mData[i].operations.size(); j++)
        {
            totalTime += mData[i].operations[j].totalTime;
        }

        mData[i].timeRemaining = totalTime;
        totalTime = 0;
    }

    Sort(confData.GetScheduleCode(), confData.GetQuantumNum());
}

void MetaData::ErrorCheck()
{
    if(!mStartFound)
    {
        std::cout << "Error: start message not found in metadata file" << std::endl;
        exit(EXIT_FAILURE);
    }
    if(!mEndFound)
    {
        std::cout << "Error: end message not found in metadata file" << std::endl;
        exit(EXIT_FAILURE);
    }

    if(!mSysStartFound)
    {
        std::cout << "Error: operating system start message not found in metadata file" << std::endl;
        exit(EXIT_FAILURE);
    }
    if(!mSysEndFound)
    {
        std::cout << "Error: operating system end message not found in metadata file" << std::endl;
        exit(EXIT_FAILURE);
    }
}

void MetaData::AddToVector(std::string newElement, Configuration confData, int* index)
{
    newElement.erase(remove(newElement.begin(), newElement.end(), ' '), newElement.end());

    std::regex sBegin("S\\{begin\\}[0-9]+;[\n|\r]*");
    std::regex sFinish("S\\{finish\\}[0-9]+\\.[\n|\r]*");
    std::regex aBegin("A\\{begin\\}[0-9]+;[\n|\r]*");
    std::regex aFinish("A\\{finish\\}[0-9]+;[\n|\r]*");

    std::regex run("P\\{run\\}[0-9]+;[\n|\r]*");
    std::regex iHardDrive("I\\{harddrive\\}[0-9]+;[\n|\r]*");
    std::regex keyboard("I\\{keyboard\\}[0-9]+;[\n|\r]*");
    std::regex scanner("I\\{scanner\\}[0-9]+;[\n|\r]*");
    std::regex oHardDrive("O\\{harddrive\\}[0-9]+;[\n|\r]*");
    std::regex monitor("O\\{monitor\\}[0-9]+;[\n|\r]*");
    std::regex projector("O\\{projector\\}[0-9]+;[\n|\r]*");
    std::regex block("M\\{block\\}[0-9]+;[\n|\r]*");
    std::regex allocate("M\\{allocate\\}[0-9]+;[\n|\r]*");

    if(std::regex_match(newElement, sBegin))
    {
        mSysStartFound = true;
    }
    else if(std::regex_match(newElement, sFinish))
    {
        mSysEndFound = true;
    }
    else if(std::regex_match(newElement, aBegin))
    {
        Process newProcess;
        mData.push_back(newProcess);

        MDElement appBegin('A', "begin", 0, 0, *index + 1);
        mData[*index].operations.push_back(appBegin);
    }
    else if(std::regex_match(newElement, aFinish))
    {
        MDElement appFinish('A', "finish", 0, 0, *index + 1);
        mData[*index].operations.push_back(appFinish);
        (*index)++;
    }
    else if(std::regex_match(newElement, run))
    {
        MDElement pRun('P', "run", ExtractInt(newElement), confData.GetProcessorCycleTime(), *index + 1);
        mData[*index].operations.push_back(pRun);
        mData[*index].numTasks++;
    }
    else if(std::regex_match(newElement, iHardDrive))
    {
        MDElement pRun('I', "hard drive", ExtractInt(newElement), confData.GetHardDriveCycleTime(), *index + 1);
        mData[*index].operations.push_back(pRun);
        mData[*index].numTasks++;
        mData[*index].numInputOutput++;
    }
    else if(std::regex_match(newElement, keyboard))
    {
        MDElement pRun('I', "keyboard", ExtractInt(newElement), confData.GetKeyboardCycleTime(), *index + 1);
        mData[*index].operations.push_back(pRun);
        mData[*index].numTasks++;
        mData[*index].numInputOutput++;
    }
    else if(std::regex_match(newElement, scanner))
    {
        MDElement pRun('I', "scanner", ExtractInt(newElement), confData.GetScannerCycleTime(), *index + 1);
        mData[*index].operations.push_back(pRun);
        mData[*index].numTasks++;
        mData[*index].numInputOutput++;
    }
    else if(std::regex_match(newElement, oHardDrive))
    {
        MDElement pRun('O', "hard drive", ExtractInt(newElement), confData.GetHardDriveCycleTime(), *index + 1);
        mData[*index].operations.push_back(pRun);
        mData[*index].numTasks++;
        mData[*index].numInputOutput++;
    }
    else if(std::regex_match(newElement, monitor))
    {
        MDElement pRun('O', "monitor", ExtractInt(newElement), confData.GetMonitorCycleTime(), *index + 1);
        mData[*index].operations.push_back(pRun);
        mData[*index].numTasks++;
        mData[*index].numInputOutput++;
    }
    else if(std::regex_match(newElement, projector))
    {
        MDElement pRun('O', "projector", ExtractInt(newElement), confData.GetProjectorCycleTime(), *index + 1);
        mData[*index].operations.push_back(pRun);
        mData[*index].numTasks++;
        mData[*index].numInputOutput++;
    }
    else if(std::regex_match(newElement, block))
    {
        MDElement pRun('M', "block", ExtractInt(newElement), confData.GetMemoryCycleTime(), *index + 1);
        mData[*index].operations.push_back(pRun);
        mData[*index].numTasks++;
    }
    else if(std::regex_match(newElement, allocate))
    {
        MDElement pRun('M', "allocate", ExtractInt(newElement), confData.GetMemoryCycleTime(), *index + 1);
        mData[*index].operations.push_back(pRun);
        mData[*index].numTasks++;
    }
    else
    {
        std::cout << "Error: invalid entry in metadata file: " << newElement << std::endl;
        exit(EXIT_FAILURE);
    }
}

int MetaData::ExtractInt(std::string str)
{
    std::stringstream ss;

    ss << str;

    char temp;
    int found;

    temp = ss.get();

    while(temp != '}')
    {
        temp = ss.get();
    }

    ss >> found;

    return found;
}

void MetaData::Sort(std::string schedule, int quantumNum)
{
    int runTimeLeft = quantumNum;
    MDElement tempElement;
    Process tempProcess;
    int i = 0;
    bool finished = false;

    if(schedule == "RR")
    {
        while(!finished)
        {
            runTimeLeft = quantumNum;

            while(runTimeLeft > 0)
            {
                if(!mData[i].operations.empty())
                {
                    if(mData[i].operations[0].totalTime <= runTimeLeft)
                    {
                        runTimeLeft -= mData[i].operations[0].totalTime;
                        tempElement = mData[i].operations[0];
                        mSortedData.push_back(tempElement);
                        mData[i].operations.erase(mData[i].operations.begin());
                    }
                    else
                    {
                        tempElement = mData[i].operations[0];
                        mSortedData.push_back(tempElement);
                        mData[i].operations[0].totalTime -= runTimeLeft;
                        runTimeLeft = 0;
                    }
                }
                else
                    runTimeLeft = 0;
            }

            i++;
            if(i >= mData.size())
                i = 0;

            finished = true;

            for(int j = 0; j < mData.size(); j++)
            {
                if(!mData[j].operations.empty())
                    finished = false;
            }
        }
    }
    else if(schedule == "STR")
    {
        std::vector<Process> sortedProcesses;

        int index = 0;
        int shortestTime;

        for(int j = 0; j < mData.size(); j++)
        {
            shortestTime = mData[0].timeRemaining;
            index = 0;

            for(int k = 1; k < mData.size(); k++)
            {
                if(mData[k].timeRemaining < shortestTime)
                {
                    index = k;
                    shortestTime = mData[k].timeRemaining;
                }
            }
            sortedProcesses.push_back(mData[index]);
            mData[index].timeRemaining = INT32_MAX;
        }

        for(int j = 0; j < sortedProcesses.size(); j++)
        {
            while(!sortedProcesses[j].operations.empty())
            {
                runTimeLeft = quantumNum;

                if(sortedProcesses[j].operations[0].totalTime <= runTimeLeft)
                {
                    runTimeLeft -= sortedProcesses[j].operations[0].totalTime;
                    tempElement = sortedProcesses[j].operations[0];
                    mSortedData.push_back(tempElement);
                    sortedProcesses[j].operations.erase(sortedProcesses[j].operations.begin());
                }
                else
                {
                    tempElement = sortedProcesses[j].operations[0];
                    mSortedData.push_back(tempElement);
                    sortedProcesses[j].operations[0].totalTime -= runTimeLeft;
                    runTimeLeft = 0;
                }
            }
        }
    }
    else
    {
        std::cout << "Error: invalid scheduling code" << std::endl;
        exit(EXIT_FAILURE);
    }

    /*for(int j = 0; j < mSortedData.size(); j++)
    {
        std::cout << mSortedData[j].code;
    }*/
    std::cout << std::endl;
}