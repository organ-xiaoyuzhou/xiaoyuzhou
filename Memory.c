
#include PLATFORM_HEADER

#include "Memory.h"

#define MEMORYSIZE      4096

#define MEMOFREETHRED   64
// #define ENTER

static MEMORYTYPE      MemoryBuffer[MEMORYSIZE];
typedef struct{
   unsigned short FreeSize;
   unsigned short UsebleFreeSize;
   unsigned short BlocksUsed;
   unsigned short BlocksTotal;
}MEMOINFO_ST;
static MEMOINFO_ST MemoInfor;

void COR_MemoryInit(void)
{
ATOMIC_MEMORY(
  MemoryBuffer[0] = MEMORYSIZE-1;
  MemoInfor.UsebleFreeSize = MEMORYSIZE-1;
  MemoInfor.FreeSize = MEMORYSIZE-1;
  MemoInfor.BlocksUsed = 0;
  MemoInfor.BlocksTotal = 1;
  )
}

unsigned char* COR_MemoryAlloc(unsigned short msize)
{
  MEMORYTYPE *Addr_Scan,*MemoAddrStar;
  if(msize==0)return NULL_MM;
ATOMIC_MEMORY(
  msize = (msize + MEMORYUNIT - 1)>>MEMORYUNIT_BITSIZE;
  

  MemoAddrStar = &MemoryBuffer[0];
  if(!(*MemoAddrStar & MEMORYFLAG_USED))
  {if(*MemoAddrStar >= MEMOFREETHRED)MemoInfor.UsebleFreeSize -= *MemoAddrStar;}
  Addr_Scan = MemoAddrStar + (*MemoAddrStar&~MEMORYFLAG_USED) + 1;
  while(Addr_Scan < &MemoryBuffer[MEMORYSIZE])
  {
    if(*Addr_Scan & MEMORYFLAG_USED)
    {
      if(!(*MemoAddrStar & MEMORYFLAG_USED))
      {
        if(*MemoAddrStar >= msize)break;
        else 
        {
          if(*MemoAddrStar >= MEMOFREETHRED)MemoInfor.UsebleFreeSize += *MemoAddrStar;
          MemoAddrStar = Addr_Scan;
        }
      }
    }
    else
    {
      if(*Addr_Scan >= MEMOFREETHRED)MemoInfor.UsebleFreeSize -= *Addr_Scan;
      if(!(*MemoAddrStar & MEMORYFLAG_USED))
      {
        MemoInfor.FreeSize++;
        *MemoAddrStar += *Addr_Scan + 1;
        MemoInfor.BlocksTotal--;
      }
      else 
      {
        MemoAddrStar = Addr_Scan;
      }
    }
    Addr_Scan += (*Addr_Scan & (~MEMORYFLAG_USED)) + 1;
  }
  
  if((!(*MemoAddrStar & MEMORYFLAG_USED))&&
     (*MemoAddrStar >= msize))
  {
    if(*MemoAddrStar >= msize + 2)
    {
      MemoInfor.FreeSize -= msize+1;
      Addr_Scan = MemoAddrStar + msize + 1;
      *Addr_Scan = *MemoAddrStar - msize - 1;
      *MemoAddrStar = msize;
      if(*Addr_Scan >= MEMOFREETHRED)MemoInfor.UsebleFreeSize += *Addr_Scan;
      MemoInfor.BlocksTotal++;
    }
    else
    {MemoInfor.FreeSize -= *MemoAddrStar;}
    *MemoAddrStar |= MEMORYFLAG_USED;
    MemoInfor.BlocksUsed++;
  }
  else 
  {
    if(!(*MemoAddrStar & MEMORYFLAG_USED))
    {if(*MemoAddrStar >= MEMOFREETHRED)MemoInfor.UsebleFreeSize += *MemoAddrStar;}
    MemoAddrStar = NULL_MM;
  }
)
  if(MemoAddrStar != NULL_MM)return ((unsigned char*)(MemoAddrStar+1));
  return NULL_MM;
}
void COR_MemoryFree(void* addrt)
{
  MEMORYTYPE *Addr_Scan,*MemoAddrStar;
ATOMIC_MEMORY(
  MemoAddrStar = (MEMORYTYPE*)addrt;
  MemoAddrStar--;
  *MemoAddrStar &= ~MEMORYFLAG_USED;
  MemoInfor.FreeSize += *MemoAddrStar;
  Addr_Scan = MemoAddrStar + *MemoAddrStar + 1;
  while(!(*Addr_Scan & MEMORYFLAG_USED))
  {
    if(Addr_Scan >= &MemoryBuffer[MEMORYSIZE])break;
    if(*Addr_Scan >= MEMOFREETHRED)MemoInfor.UsebleFreeSize -= *Addr_Scan;
    MemoInfor.FreeSize++;
    *MemoAddrStar += *Addr_Scan + 1;
    Addr_Scan += *Addr_Scan + 1;
    MemoInfor.BlocksTotal--;
  }
  if(*MemoAddrStar >= MEMOFREETHRED)MemoInfor.UsebleFreeSize += *MemoAddrStar;
  MemoInfor.BlocksUsed--;
)
  return;
}
unsigned short COR_MemoCheck(void)
{
  MEMORYTYPE *Addr_Scan;
  MEMOINFO_ST memoinfotemp;
  unsigned short rt_f;
//ATOMIC_MEMORY(
  rt_f = 0;
  memoinfotemp.FreeSize = 0;
  memoinfotemp.UsebleFreeSize = 0;
  memoinfotemp.BlocksUsed = 0;
  memoinfotemp.BlocksTotal = 0;
  Addr_Scan = &MemoryBuffer[0];
  while(Addr_Scan < &MemoryBuffer[MEMORYSIZE])
  {
    memoinfotemp.BlocksTotal++;
    if(*Addr_Scan & MEMORYFLAG_USED)
    {
      memoinfotemp.BlocksUsed++;
    }
    else
    {
      memoinfotemp.FreeSize += *Addr_Scan;
      if(*Addr_Scan >= MEMOFREETHRED)memoinfotemp.UsebleFreeSize += *Addr_Scan;
    }
    Addr_Scan +=  (*Addr_Scan&~MEMORYFLAG_USED) + 1;
  }
  if(Addr_Scan != &MemoryBuffer[MEMORYSIZE])
  {rt_f = 0x0001;}
  if(memoinfotemp.FreeSize != MemoInfor.FreeSize)
  {rt_f |= 0x0002;}
  if(memoinfotemp.UsebleFreeSize != MemoInfor.UsebleFreeSize)
  {rt_f |= 0x0004;}
  if(memoinfotemp.BlocksUsed != MemoInfor.BlocksUsed)
  {rt_f |= 0x0008;}
  if(memoinfotemp.BlocksTotal != MemoInfor.BlocksTotal)
  {rt_f |= 0x0010;}
  //)                                                    
  return rt_f;
}
unsigned short COR_GetMemoryFreeSize(void)
{
  return MemoInfor.UsebleFreeSize;
}
unsigned short COR_GetFirstFreeBlockSize(void)
{
  MEMORYTYPE *Addr_Scan;
 // ATOMIC_MEMORY(
  Addr_Scan = &MemoryBuffer[0];
  while(Addr_Scan < &MemoryBuffer[MEMORYSIZE])
  {
    if(!(*Addr_Scan & MEMORYFLAG_USED))
    {
      return (unsigned short)(*Addr_Scan);
    }
    Addr_Scan += (*Addr_Scan & (~MEMORYFLAG_USED)) + 1;
  }
    //)
  return 0xffff;
}
unsigned short COR_GetFreeBlocks(void)
{
  return (MemoInfor.BlocksTotal-MemoInfor.BlocksUsed);
}
void COR_ManageFuseMemo(void)
{
    MEMORYTYPE *Addr_Scan,*MemoAddrStar;
 ATOMIC_MEMORY(
  MemoAddrStar = &MemoryBuffer[0];
  if(!(*MemoAddrStar & MEMORYFLAG_USED))
  {if(*MemoAddrStar >= MEMOFREETHRED)MemoInfor.UsebleFreeSize -= *MemoAddrStar;}
  Addr_Scan = MemoAddrStar + (*MemoAddrStar&~MEMORYFLAG_USED) + 1;
  while(Addr_Scan < &MemoryBuffer[MEMORYSIZE])
  {
    if(*Addr_Scan & MEMORYFLAG_USED)
    {
      if(!(*MemoAddrStar & MEMORYFLAG_USED))
      {
        //if(*MemoAddrStar >= msize)break;
        //else 
        {
          if(*MemoAddrStar >= MEMOFREETHRED)MemoInfor.UsebleFreeSize += *MemoAddrStar;
          MemoAddrStar = Addr_Scan;
        }
      }
    }
    else
    {
      if(*Addr_Scan >= MEMOFREETHRED)MemoInfor.UsebleFreeSize -= *Addr_Scan;
      if(!(*MemoAddrStar & MEMORYFLAG_USED))
      {
        MemoInfor.FreeSize++;
        *MemoAddrStar += *Addr_Scan + 1;
        MemoInfor.BlocksTotal--;
      }
      else 
      {
        MemoAddrStar = Addr_Scan;
      }
    }
    Addr_Scan += (*Addr_Scan & (~MEMORYFLAG_USED)) + 1;
  }
  if(!(*MemoAddrStar & MEMORYFLAG_USED))
  {if(*MemoAddrStar >= MEMOFREETHRED)MemoInfor.UsebleFreeSize += *MemoAddrStar;}
 )
}
void COR_MemoryCpy(unsigned char* dest,const unsigned char* source,unsigned short len)
{
  while(len--)
  {*dest++ = *source++;}
}
void COR_MemorySet(unsigned char* dest,unsigned char dataT,unsigned short len)
{
  while(len--)
  {*dest++ = dataT;}
}
boolean COR_MemoryCmp(const unsigned char* dest,const unsigned char* source,unsigned short len)
{
  while(len--)
  {if(*dest++ != *source++)return FALSE;}
  return TRUE;
}

