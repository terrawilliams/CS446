//
// Created by terra on 3/4/19.
//

#include "PCB.h"

PCB::PCB()
{

}

PCB::~PCB()
{

}

int PCB::getProcessState()
{
    return processState;
}

void PCB::setProcessState(PCBStates newState)
{
    switch(newState)
    {
        case New:
            processState = 0;
            break;
        case Ready:
            processState = 1;
            break;
        case Running:
            processState = 2;
            break;
        case Waiting:
            processState = 3;
            break;
        case Terminated:
            processState = 4;
            break;
        default:
            processState = -1;
            break;
    }
}
