#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <pthread.h>
#include <semaphore.h>
#include <iomanip>
#include "configuration.h"
#include "MetaData.h"
#include "PCB.h"
#include "Timer.h"

sem_t mutex;

void CaseA(MDElement element, Timer timer, bool logToMonitor, bool logToFile, std::ofstream &fout);
void CaseP(MDElement element, Timer timer, bool logToMonitor, bool logToFile, std::ofstream &fout);
void CaseM(MDElement element, Timer timer, bool logToMonitor, bool logToFile, std::ofstream &fout, int sysMemory, int currentMemory);
void CaseI(MDElement element, Timer timer, bool logToMonitor, bool logToFile, std::ofstream &fout, int curHardDrive, int curProj);
void CaseO(MDElement element, Timer timer, bool logToMonitor, bool logToFile, std::ofstream &fout, int curHardDrive, int curProj);

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

    int currentHardDrive = 0;
    int currentProj = 0;
    int currentMemory = 0;

    std::ofstream fout;
    fout.open(confData.GetOutputFile());

    timer.StartTimer();

    pcb.setProcessState(Ready);

    timer.EndTimer();

    if(logToMonitor)
        std::cout << std::fixed << std::setprecision(6) << timer.GetDuration() << " - Simulator program starting" << std::endl;
    if(logToFile)
        fout << std::fixed << std::setprecision(6) << timer.GetDuration() << " - Simulator program starting" << std::endl;

    for(int i = 0; i < metaData.GetData().size(); i++)
    {
        for(int j = 0; j < metaData.GetData()[i].operations.size(); j++)
        {
            switch(metaData.GetData()[i].operations[j].code)
            {
                case 'A':
                    pcb.setProcessState(Running);
                    CaseA(metaData.GetData()[i].operations[j], timer, logToMonitor, logToFile, fout);
                    pcb.setProcessState(Ready);
                    break;
                case 'P':
                    pcb.setProcessState(Running);
                    CaseP(metaData.GetData()[i].operations[j], timer, logToMonitor, logToFile, fout);
                    pcb.setProcessState(Ready);
                    break;
                case 'M':
                    pcb.setProcessState(Running);
                    CaseM(metaData.GetData()[i].operations[j], timer, logToMonitor, logToFile, fout,
                          confData.GetSysMemory(), currentMemory);
                    pcb.setProcessState(Ready);
                    break;
                case 'I':
                    pcb.setProcessState(Running);
                    pcb.setProcessState(Waiting);
                    CaseI(metaData.GetData()[i].operations[j], timer, logToMonitor, logToFile, fout,
                          currentHardDrive,
                          currentProj);
                    pcb.setProcessState(Ready);
                    break;
                case 'O':
                    pcb.setProcessState(Running);
                    pcb.setProcessState(Waiting);
                    CaseO(metaData.GetData()[i].operations[j], timer, logToMonitor, logToFile, fout,
                          currentHardDrive,
                          currentProj);
                    pcb.setProcessState(Ready);
                    break;
                default:
                    break;
            }

            if(metaData.GetData()[i].operations[j].descriptor == "projector")
                currentProj++;
            if(currentProj >= confData.GetProjectorQuantity())
                currentProj = 0;

            if(metaData.GetData()[i].operations[j].descriptor == "hard drive")
                currentHardDrive++;
            if(currentHardDrive >= confData.GetHardDriveQuantity())
                currentHardDrive = 0;

            if(metaData.GetData()[i].operations[j].descriptor == "allocate")
                currentMemory += confData.GetBlockSize();
            if(currentMemory >= confData.GetSysMemory())
                currentMemory = 0;
        }
    }

    timer.EndTimer();

    if(logToMonitor)
        std::cout << timer.GetDuration() << " - Simulator program ending" << std::endl;
    if(logToFile)
        fout << timer.GetDuration() << " - Simulator program ending" << std::endl;

    fout.close();

    pcb.setProcessState(Terminated);

    return 0;
}

void CaseA(MDElement element, Timer timer, bool logToMonitor, bool logToFile, std::ofstream &fout)
{
    timer.EndTimer();

    if(element.descriptor == "begin")
    {
        if(logToMonitor)
        {
            std::cout << timer.GetDuration() << " - OS: preparing process " << element.numProcess << std::endl;
            std::cout << timer.GetDuration() << " - OS: starting process " << element.numProcess << std::endl;
        }

        if(logToFile)
        {
            fout << timer.GetDuration() << " - OS: preparing process " << element.numProcess << std::endl;
            fout << timer.GetDuration() << " - OS: starting process " << element.numProcess << std::endl;
        }
    }
    else
    {
        if(logToMonitor)
            std::cout << timer.GetDuration() << " - OS: removing process " << element.numProcess << std::endl;
        if(logToFile)
            fout << timer.GetDuration() << " - OS: removing process " << element.numProcess << std::endl;

        (element.numProcess)++;
    }
}
void CaseP(MDElement element, Timer timer, bool logToMonitor, bool logToFile, std::ofstream &fout)
{
    timer.EndTimer();

    if(logToMonitor)
        std::cout << timer.GetDuration() << " - Process " << element.numProcess << ": start processing action" << std::endl;
    if(logToFile)
        fout << timer.GetDuration() << " - Process " << element.numProcess << ": start processing action" << std::endl;

    timer.Wait(element.totalTime / (double) 1000);

    timer.EndTimer();

    if(logToMonitor)
        std::cout << timer.GetDuration() << " - Process " << element.numProcess << ": end processing action" << std::endl;
    if(logToFile)
        fout << timer.GetDuration() << " - Process " << element.numProcess << ": end processing action" << std::endl;
}

void CaseM(MDElement element, Timer timer, bool logToMonitor, bool logToFile, std::ofstream &fout, int sysMemory, int currentMemory)
{
    timer.EndTimer();

    if(element.descriptor == "block")
    {
        if(logToMonitor)
            std::cout << timer.GetDuration() << " - Process " << element.numProcess << ": start memory blocking" << std::endl;
        if(logToFile)
            fout << timer.GetDuration() << " - Process " << element.numProcess << ": start memory blocking" << std::endl;

        timer.Wait(element.totalTime / (double) 1000);

        timer.EndTimer();


        if(logToMonitor)
            std::cout << timer.GetDuration() << " - Process " << element.numProcess << ": end memory blocking" << std::endl;
        if(logToFile)
            fout << timer.GetDuration() << " - Process " << element.numProcess << ": end memory blocking" << std::endl;
    }
    else
    {
        if(logToMonitor)
            std::cout << timer.GetDuration() << " - Process " << element.numProcess << ": allocating memory" << std::endl;
        if(logToFile)
            fout << timer.GetDuration() << " - Process " << element.numProcess << ": allocating memory" << std::endl;

        timer.Wait(element.totalTime / (double) 1000);

        timer.EndTimer();

        if(logToMonitor)
            std::cout << timer.GetDuration() << " - Process " << element.numProcess << ": memory allocated at 0x" << std::setfill('0') << std::setw(8) <<std::hex << currentMemory << std::dec << std::endl;
        if(logToFile)
            fout << timer.GetDuration() << " - Process " << element.numProcess << ": memory allocated at 0x" << std::setfill('0') << std::setw(8) << std::hex << currentMemory << std::endl;
    }
}

void CaseI(MDElement element, Timer timer, bool logToMonitor, bool logToFile, std::ofstream &fout, int curHardDrive, int curProj)
{
    timer.EndTimer();

    if(element.descriptor == "projector" or element.descriptor == "hard drive")
    {
        if(logToMonitor)
        {
            std::cout << timer.GetDuration() << " - Process " << element.numProcess << ": start " << element.descriptor
                      << " input on ";
            if(element.descriptor == "projector")
                std::cout << "PROJ " << curProj << std::endl;
            else
                std::cout << "HDD " << curHardDrive << std::endl;
        }
        if(logToFile)
        {
            fout << timer.GetDuration() << " - Process " << element.numProcess << ": start " << element.descriptor
                 << " input on ";
            if(element.descriptor == "projector")
                fout << "PROJ " << curProj << std::endl;
            else
                fout << "HDD " << curHardDrive << std::endl;
        }

        pthread_t iThread;

        //create semaphore
        sem_init(&mutex, 0, 1);
        sem_wait(&mutex);

        //critical section
        pthread_create(&iThread, NULL, Wait, (void *) element.totalTime);

        //release semaphore
        sem_post(&mutex);

        pthread_join(iThread, NULL);

        timer.EndTimer();

        if(logToMonitor)
            std::cout << timer.GetDuration() << " - Process " << element.numProcess << ": end " << element.descriptor
                      << " input" << std::endl;
        if(logToFile)
            fout << timer.GetDuration() << " - Process " << element.numProcess << ": end " << element.descriptor << " input"
                 << std::endl;
    }
    else
    {
        if(logToMonitor)
            std::cout << timer.GetDuration() << " - Process " << element.numProcess << ": start " << element.descriptor
                      << " input" << std::endl;
        if(logToFile)
            fout << timer.GetDuration() << " - Process " << element.numProcess << ": start " << element.descriptor << " input"
                 << std::endl;

        pthread_t iThread;

        //create semaphore
        sem_init(&mutex, 0, 1);
        sem_wait(&mutex);

        //critical section
        pthread_create(&iThread, NULL, Wait, (void *) element.totalTime);

        //release semaphore
        sem_post(&mutex);

        pthread_join(iThread, NULL);

        timer.EndTimer();

        if(logToMonitor)
            std::cout << timer.GetDuration() << " - Process " << element.numProcess << ": end " << element.descriptor
                      << " input" << std::endl;
        if(logToFile)
            fout << timer.GetDuration() << " - Process " << element.numProcess << ": end " << element.descriptor << " input"
                 << std::endl;
    }
}

void CaseO(MDElement element, Timer timer, bool logToMonitor, bool logToFile, std::ofstream &fout, int curHardDrive, int curProj)
{
    timer.EndTimer();

    if(logToMonitor)
    {
        std::cout << timer.GetDuration() << " - Process " << element.numProcess << ": start " << element.descriptor << " ouput";

        if(element.descriptor == "projector")
            std::cout << " on PROJ " << curProj;
        else if(element.descriptor == "hard drive")
            std::cout << " on HDD " << curHardDrive;

        std::cout << std::endl;
    }
    if(logToFile)
    {
        fout << timer.GetDuration() << " - Process " << element.numProcess << ": start " << element.descriptor << " ouput";

        if(element.descriptor == "projector")
            fout << " on PROJ " << curProj;
        else if(element.descriptor == "hard drive")
            fout << " on HDD " << curHardDrive;

        fout << std::endl;
    }

    pthread_t oThread;

    //create semaphore
    sem_init(&mutex, 0, 1);
    sem_wait(&mutex);

    pthread_create(&oThread, NULL, Wait, (void*) element.totalTime);
    pthread_join(oThread, NULL);

    //release semaphore
    sem_post(&mutex);

    timer.EndTimer();

    if(logToMonitor)
        std::cout << timer.GetDuration() << " - Process " << element.numProcess << ": end " << element.descriptor << " ouput" << std::endl;
    if(logToFile)
        fout << timer.GetDuration() << " - Process " << element.numProcess << ": end " << element.descriptor << " ouput" << std::endl;
}

void *Wait(void *time)
{
    Timer timer;

    long tid;
    tid = (long) time;

    timer.Wait((double) tid / 1000);
}