//
// Created by terra on 3/4/19.
//

#ifndef PROJECT1_PCB_H
#define PROJECT1_PCB_H

#include <string>
#include "PCBStates.h"


class PCB
{
public:
    PCB();
    ~PCB();

    int getProcessState();
    void setProcessState(PCBStates newState);

private:
    int processState; //0 = Start, 1 = Ready, 2 = Running, 3 = Waiting, 4 = Exit
};


#endif //PROJECT1_PCB_H
