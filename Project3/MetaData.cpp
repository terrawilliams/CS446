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
    return mData;
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

                    AddToVector(match.str(), confData);

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

void MetaData::AddToVector(std::string newElement, Configuration confData)
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
        MDElement sysBegin('S', "begin", 0, 0);
        mData.push_back(sysBegin);
    }
    else if(std::regex_match(newElement, sFinish))
    {
        mSysEndFound = true;
        MDElement sysFinish('S', "finish", 0, 0);
        mData.push_back(sysFinish);
    }
    else if(std::regex_match(newElement, aBegin))
    {
        MDElement appBegin('A', "begin", 0, 0);
        mData.push_back(appBegin);
    }
    else if(std::regex_match(newElement, aFinish))
    {
        MDElement appFinish('A', "finish", 0, 0);
        mData.push_back(appFinish);
    }
    else if(std::regex_match(newElement, run))
    {
        MDElement pRun('P', "run", ExtractInt(newElement), confData.GetProcessorCycleTime());
        mData.push_back(pRun);
    }
    else if(std::regex_match(newElement, iHardDrive))
    {
        MDElement pRun('I', "hard drive", ExtractInt(newElement), confData.GetHardDriveCycleTime());
        mData.push_back(pRun);
    }
    else if(std::regex_match(newElement, keyboard))
    {
        MDElement pRun('I', "keyboard", ExtractInt(newElement), confData.GetKeyboardCycleTime());
        mData.push_back(pRun);
    }
    else if(std::regex_match(newElement, scanner))
    {
        MDElement pRun('I', "scanner", ExtractInt(newElement), confData.GetScannerCycleTime());
        mData.push_back(pRun);
    }
    else if(std::regex_match(newElement, oHardDrive))
    {
        MDElement pRun('O', "hard drive", ExtractInt(newElement), confData.GetHardDriveCycleTime());
        mData.push_back(pRun);
    }
    else if(std::regex_match(newElement, monitor))
    {
        MDElement pRun('O', "monitor", ExtractInt(newElement), confData.GetMonitorCycleTime());
        mData.push_back(pRun);
    }
    else if(std::regex_match(newElement, projector))
    {
        MDElement pRun('O', "projector", ExtractInt(newElement), confData.GetProjectorCycleTime());
        mData.push_back(pRun);
    }
    else if(std::regex_match(newElement, block))
    {
        MDElement pRun('M', "block", ExtractInt(newElement), confData.GetMemoryCycleTime());
        mData.push_back(pRun);
    }
    else if(std::regex_match(newElement, allocate))
    {
        MDElement pRun('M', "allocate", ExtractInt(newElement), confData.GetMemoryCycleTime());
        mData.push_back(pRun);
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
