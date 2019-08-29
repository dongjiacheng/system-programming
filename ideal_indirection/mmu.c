/**
 * Ideal Indirection Lab
 * CS 241 - Spring 2017
 */
#include "kernel.h"
#include "mmu.h"
#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

MMU *MMU_create() {
  MMU *mmu = calloc(1, sizeof(MMU));
  mmu->tlb = TLB_create();
  mmu->curr_pid = 0;
  return mmu;
}

void* getbit(int start, int end,void* A){
uintptr_t temp = (uintptr_t)A;
temp = temp << start;
temp = temp >> (end+start);
return (void*)temp;
}

void* add(void* A, size_t B){
void* temp = (void*)((char*)A + B);
return temp;
}

void* minus(void* A, size_t B){
void* temp = (void*)((char*)A - B);
return temp;
}


void *MMU_get_physical_address(MMU *mmu, void *virtual_address, size_t pid) {
  assert(pid < MAX_PROCESS_ID);
 
  if(mmu->curr_pid != pid)
  {
    TLB_flush(&mmu->tlb);
    mmu->curr_pid = pid;
  }

  

  size_t offset = (size_t)getbit(49, 0, virtual_address);
  void* clearoffset = getbit(13, 15, virtual_address);
  printf("clearoffset is :%p\n", clearoffset);

  void* te1 = getbit(13, 24+15 ,virtual_address);
 // printf("te1 is :%p\n", te1);
  size_t VPN1 = (size_t)te1;
  printf("VPN1 is :%zu\n", VPN1);
  void* te2 = getbit(12+13, 12+15 , virtual_address);
 // printf("te2 is :%p\n", te2);
  size_t VPN2 = (size_t)te2;
  printf("VPN2 is :%zu\n", VPN2);
  void* te3 = getbit(24+13, 0+15, virtual_address);
 // printf("te3 is :%p\n", te3);
  size_t VPN3 = (size_t)te3;
  printf("VPN3 is :%zu\n", VPN3);

  //need to remove offset and misc have ont done
  void* result = TLB_get_physical_address(&mmu->tlb,clearoffset);
  if(result == NULL)
  {
  MMU_tlb_miss(mmu, virtual_address, pid);
  
  PageTable* pointTobaseTaBle = mmu->base_pts[pid];
  PageTable* pointToPage2 = PageTable_get_entry(pointTobaseTaBle,VPN1);
  
  if(pointToPage2 == NULL)
  {
    MMU_raise_page_fault(mmu, virtual_address, pid);
    MMU_raise_page_fault(mmu, virtual_address, pid);
    MMU_raise_page_fault(mmu, virtual_address, pid);
    result = (void*)ask_kernel_for_frame();
    //void* pPHY = minus(result, offset);
    void* pPHY = result;

    PageTable* pt3 = PageTable_create();
    PageTable_set_entry(pt3, VPN3, (void*)pPHY);
    PageTable* pt2 = PageTable_create();
    PageTable_set_entry(pt2, VPN2, (void*)pt3);
    PageTable_set_entry(pointTobaseTaBle, VPN1, (void*)pt2);
   
    TLB_add_physical_address(&mmu->tlb, clearoffset, result);
    
    result = (void*)((char*)result + offset);
    return result;
  }
  

  
  PageTable* pointToPage3 = PageTable_get_entry(pointToPage2,VPN2);
   
   if(pointToPage3 == NULL)
  {
    MMU_raise_page_fault(mmu, virtual_address, pid);
    MMU_raise_page_fault(mmu, virtual_address, pid);

    result = (void*)ask_kernel_for_frame();
    //void* pPHY = minus(result, offset);
    void* pPHY = result;

    PageTable* pt3 = PageTable_create();
    PageTable_set_entry(pt3, VPN3, (void*)pPHY);
    PageTable_set_entry(pointToPage2, VPN2, (void*)pt3);
    
    TLB_add_physical_address(&mmu->tlb, clearoffset, result);
    
    result = (void*)((char*)result + offset);
    return result;
  }
  
  PageTable* pointToPHY = PageTable_get_entry(pointToPage3,VPN3);
   if(pointToPHY == NULL)
  {
    MMU_raise_page_fault(mmu, virtual_address, pid);

    result = (void*)ask_kernel_for_frame();
    //void* pPHY = minus(result, offset);
    void* pPHY = result;

    PageTable_set_entry(pointToPage3, VPN3, (void*)pPHY);
    TLB_add_physical_address(&mmu->tlb, clearoffset, result);
    
    result = (void*)((char*)result + offset);
    return result;
  }
  
  

  TLB_add_physical_address(&mmu->tlb, clearoffset, pointToPHY);
  
  result = (void*)((char*)pointToPHY + offset);
  return result;
  }
  else
  {
    result = (void*)((char*)result + offset);
    return result;
  }
  // TODO: implement this function!
  
}

void MMU_tlb_miss(MMU *mmu, void *address, size_t pid) {
  assert(pid < MAX_PROCESS_ID);
  mmu->num_tlb_misses++;
  printf("Process [%lu] tried to access [%p] and it couldn't find it in the "
         "TLB so the MMU has to check the PAGE TABLES\n",
         pid, address);
}

void MMU_raise_page_fault(MMU *mmu, void *address, size_t pid) {
  assert(pid < MAX_PROCESS_ID);
  mmu->num_page_faults++;
  printf(
      "Process [%lu] tried to access [%p] and the MMU got an invalid entry\n",
      pid, address);
}

void MMU_add_process(MMU *mmu, size_t pid) {
  assert(pid < MAX_PROCESS_ID);
  mmu->base_pts[pid] = PageTable_create();
}

void MMU_free_process_tables(MMU *mmu, size_t pid) {
  assert(pid < MAX_PROCESS_ID);
  PageTable *base_pt = mmu->base_pts[pid];
  Pagetable_delete_tree(base_pt);
}

void MMU_delete(MMU *mmu) {
  for (size_t i = 0; i < MAX_PROCESS_ID; i++) {
    MMU_free_process_tables(mmu, i);
  }
 // puts("table problem");
  TLB_delete(mmu->tlb);
 // puts("TLB problem");
  free(mmu);
}
