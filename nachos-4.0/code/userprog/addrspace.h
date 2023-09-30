// addrspace.h 
//	Data structures to keep track of executing user programs 
//	(address spaces).
//
//	For now, we don't keep any information about address spaces.
//	The user level CPU state is saved and restored in the thread
//	executing the user program (see thread.h).
//
// Copyright (c) 1992-1996 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#ifndef ADDRSPACE_H
#define ADDRSPACE_H

#include "copyright.h"
#include "filesys.h"
#include <string.h>

#include "noff.h" //N3

#define UserStackSize		1024 	// increase this as necessary!

//N3: Specify victim
enum VictimType {
    Random, 
	  LRU, 
    LFU
};

class AddrSpace {
  public:
    AddrSpace();			// Create an address space.
    ~AddrSpace();			// De-allocate an address space

    void Execute(char *fileName);	// Run the the program
					// stored in the file "executable"

    void SaveState();			// Save/restore address space-specific
    void RestoreState();		// info on a context switch 

    TranslationEntry *pageTable;	// N3: Change to public
    // Assume linear page table translation for now!

    //N3: Since noffH and executable have to be used later, to copy data into mainMemory
    NoffHeader noffH;
    OpenFile *executable;


  private:
    unsigned int numPages;		// Number of pages in the virtual 
					// address space

    bool Load(char *fileName);		// Load the program into memory
					// return false if not found

    void InitRegisters();		// Initialize user-level CPU registers,
					// before jumping to user code
    int TranalateVir2Phys(int);
};

// N3
class FrameInfoEntry{
  public:
    bool valid; //valid to use or not
    bool lock; 
    AddrSpace* addrspace;  // which process is using this page
    unsigned int vpn; // virtual page number
    //which virtual page of the process is stored in this page

    unsigned int latestTick;
    unsigned int usageCount;
};

class MemoryManager{
  public:
    VictimType vicType;
    MemoryManager(VictimType v);
    int TransAddr(AddrSpace* space, int virAddr);
    bool AcquirePage(AddrSpace* space, int vpn);
    bool ReleasePage(AddrSpace* space, int vpn);
    void PageFaultHandler(int faultPageNum);

    int ChooseVictim();
};

#endif // ADDRSPACE_H
