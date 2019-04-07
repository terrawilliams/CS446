#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <pthread.h>
#include <iomanip>
#include "configuration.h"
#include "MetaData.h"
#include "PCB.h"
#include "Timer.h"

void CaseS(MDElement element, Timer timer, bool logToMonitor, bool logToFile, std::ofstream &fout);
void CaseA(MDElement element, Timer timer, int* numProcess, bool logToMonitor, bool logToFile, std::ofstream &fout);
void CaseP(MDElement element, Timer timer, int numProcess, bool logToMonitor, bool logToFile, std::ofstream &fout);
void CaseM(MDElement element, Timer timer, int numProcess, bool logToMonitor, bool logToFile, std::ofstream &fout, int sysMemory);
void CaseI(MDElement element, Timer timer, int numProcess, bool logToMonitor, bool logToFile, std::ofstream &fout);
void CaseO(MDElement element, Timer timer, int numProcess, bool logToMonitor, bool logToFile, std::ofstream &fout);

void *Wait(void *time);

int main(int argc, char **argv)
{
    PCB pcb;
    pcb.setProcessState(New);

    Configuration confData;
    std::string confFileName = argv[1];

    confData.ReadFile(confFileName);
    confData.ErrorCheck();

    bool logToMonitor = confData.GetLogToMonitor();
    bool logToFile = confData.GetLogToFile();

    MetaData metaData;
    metaData.ReadMetaData(confData);
    metaData.ErrorCheck();

    Timer timer;
    int numProcess = 1;

    std::ofstream fout;
    fout.open(confData.GetOutputFile());

    timer.StartTimer();

    pcb.setProcessState(Ready);

    for(int i = 0; i < metaData.GetData().size(); i++)
    {
        switch(metaData.GetData()[i].code)
        {
            case 'S':
                pcb.setProcessState(Running);
                CaseS(metaData.GetData()[i], timer, logToMonitor, logToFile, fout);
                pcb.setProcessState(Ready);
                break;
            case 'A':
                pcb.setProcessState(Running);
                CaseA(metaData.GetData()[i], timer, &numProcess, logToMonitor, logToFile, fout);
                pcb.setProcessState(Ready);
                break;
            case 'P':
                pcb.setProcessState(Running);
                CaseP(metaData.GetData()[i], timer, numProcess, logToMonitor, logToFile, fout);
                pcb.setProcessState(Ready);
                break;
            case 'M':
                pcb.setProcessState(Running);
                CaseM(metaData.GetData()[i], timer, numProcess, logToMonitor, logToFile, fout, confData.GetSysMemory());
                pcb.setProcessState(Ready);
                break;
            case 'I':
                pcb.setProcessState(Running);
                pcb.setProcessState(Waiting);
                CaseI(metaData.GetData()[i], timer, numProcess, logToMonitor, logToFile, fout);
                pcb.setProcessState(Ready);
                break;
            case 'O':
                pcb.setProcessState(Running);
                pcb.setProcessState(Waiting);
                CaseO(metaData.GetData()[i], timer, numProcess, logToMonitor, logToFile, fout);
                pcb.setProcessState(Ready);
                break;
            default:
                break;
        }
    }

    fout.close();

    pcb.setProcessState(Terminated);

    return 0;
}

void CaseS(MDElement element, Timer timer, bool logToMonitor, bool logToFile, std::ofstream &fout)
{
    timer.EndTimer();

    if(element.descriptor == "begin")
    {
        if(logToMonitor)
            std::cout << std::fixed << std::setprecision(6) << timer.GetDuration() << " - Simulator program starting" << std::endl;
        if(logToFile)
            fout << std::fixed << std::setprecision(6) << timer.GetDuration() << " - Simulator program starting" << std::endl;
    }
    else
    {
        timer.EndTimer();
        if(logToMonitor)
            std::cout << timer.GetDuration() << " - Simulator program ending" << std::endl;
        if(logToFile)
            fout << timer.GetDuration() << " - Simulator program ending" << std::endl;
    }
}

void CaseA(MDElement element, Timer timer, int* numProcess, bool logToMonitor, bool logToFile, std::ofstream &fout)
{
    timer.EndTimer();

    if(element.descriptor == "begin")
    {
        if(logToMonitor)
        {
            std::cout << timer.GetDuration() << " - OS: preparing process " << *numProcess << std::endl;
            std::cout << timer.GetDuration() << " - OS: starting process " << *numProcess << std::endl;
        }

        if(logToFile)
        {
            fout << timer.GetDuration() << " - OS: preparing process " << *numProcess << std::endl;
            fout << timer.GetDuration() << " - OS: starting process " << *numProcess << std::endl;
        }
    }
    else
    {
        if(logToMonitor)
            std::cout << timer.GetDuration() << " - OS: removing process " << *numProcess << std::endl;
        if(logToFile)
            fout << timer.GetDuration() << " - OS: removing process " << *numProcess << std::endl;

        (*numProcess)++;
    }
}
void CaseP(MDElement element, Timer timer, int numProcess, bool logToMonitor, bool logToFile, std::ofstream &fout)
{
    timer.EndTimer();

    if(logToMonitor)
        std::cout << timer.GetDuration() << " - Process " << numProcess << ": start processing action" << std::endl;
    if(logToFile)
        fout << timer.GetDuration() << " - Process " << numProcess << ": start processing action" << std::endl;

    timer.Wait(element.totalTime / (double) 1000);

    timer.EndTimer();

    if(logToMonitor)
        std::cout << timer.GetDuration() << " - Process " << numProcess << ": end processing action" << std::endl;
    if(logToFile)
        fout << timer.GetDuration() << " - Process " << numProcess << ": end processing action" << std::endl;
}

void CaseM(MDElement element, Timer timer, int numProcess, bool logToMonitor, bool logToFile, std::ofstream &fout, int sysMemory)
{
    timer.EndTimer();

    if(element.descriptor == "block")
    {
        if(logToMonitor)
            std::cout << timer.GetDuration() << " - Process " << numProcess << ": start memory blocking" << std::endl;
        if(logToFile)
            fout << timer.GetDuration() << " - Process " << numProcess << ": start memory blocking" << std::endl;

        timer.Wait(element.totalTime / (double) 1000);

        timer.EndTimer();


        if(logToMonitor)
            std::cout << timer.GetDuration() << " - Process " << numProcess << ": end memory blocking" << std::endl;
        if(logToFile)
            fout << timer.GetDuration() << " - Process " << numProcess << ": end memory blocking" << std::endl;
    }
    else
    {
        if(logToMonitor)
            std::cout << timer.GetDuration() << " - Process " << numProcess << ": allocating memory" << std::endl;
        if(logToFile)
            fout << timer.GetDuration() << " - Process " << numProcess << ": allocating memory" << std::endl;

        timer.Wait(element.totalTime / (double) 1000);

        timer.EndTimer();

        int memLocation;

        memLocation = rand() % sysMemory;

        if(logToMonitor)
            std::cout << timer.GetDuration() << " - Process " << numProcess << ": memory allocated at 0x" << std::hex << memLocation << std::dec << std::endl;
        if(logToFile)
            fout << timer.GetDuration() << " - Process " << numProcess << ": memory allocated at 0x" << std::endl;
    }
}

void CaseI(MDElement element, Timer timer, int numProcess, bool logToMonitor, bool logToFile, std::ofstream &fout)
{
    timer.EndTimer();

    if(logToMonitor)
        std::cout << timer.GetDuration() << " - Process " << numProcess << ": start " << element.descriptor << " input" << std::endl;
    if(logToFile)
        fout << timer.GetDuration() << " - Process " << numProcess << ": start " << element.descriptor << " input" << std::endl;

    pthread_t iThread;
    pthread_create(&iThread, NULL, Wait, (void*) element.totalTime);
    pthread_join(iThread, NULL);

    timer.EndTimer();

    if(logToMonitor)
        std::cout << timer.GetDuration() << " - Process " << numProcess << ": end " << element.descriptor << " input" << std::endl;
    if(logToFile)
        fout << timer.GetDuration() << " - Process " << numProcess << ": end " << element.descriptor << " input" << std::endl;
}

void CaseO(MDElement element, Timer timer, int numProcess, bool logToMonitor, bool logToFile, std::ofstream &fout)
{
    timer.EndTimer();

    if(logToMonitor)
        std::cout << timer.GetDuration() << " - Process " << numProcess << ": start " << element.descriptor << " ouput" << std::endl;
    if(logToFile)
        fout << timer.GetDuration() << " - Process " << numProcess << ": start " << element.descriptor << " ouput" << std::endl;

    pthread_t oThread;
    pthread_create(&oThread, NULL, Wait, (void*) element.totalTime);
    pthread_join(oThread, NULL);

    timer.EndTimer();

    if(logToMonitor)
        std::cout << timer.GetDuration() << " - Process " << numProcess << ": end " << element.descriptor << " ouput" << std::endl;
    if(logToFile)
        fout << timer.GetDuration() << " - Process " << numProcess << ": end " << element.descriptor << " ouput" << std::endl;
}

void *Wait(void *time)
{
    Timer timer;

    long tid;
    tid = (long) time;

    timer.Wait((double) tid / 1000);
}