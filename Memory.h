#ifndef _APP_MEMORY_H
#define _APP_MEMORY_H

#ifndef NULL_MM
#define NULL_MM    (void*)(0)
#endif

typedef unsigned long MEMORYTYPE;
typedef unsigned short MEMORYSIZE_TYPE;

#if (MEMORYBOLCK_MAXSIZE>0x0001ffff)
#err "MEMORYBOLCK_MAXSIZE can not big than 0x0001ffff, because : half of size of (MEMORYTYPE/2) * MEMORYUNIT "
#endif

#define MEMORYBOLCK_MAXSIZE     0xffff
#define MEMORYUNIT              4
#define MEMORYUNIT_BITSIZE      0x02
#define MEMORYFLAG_USED         0x80000000
#define MEMORY_SIZE_MASK_LOCAL  0x0000ffff
#define MEMORY_SIZE_MASK_LAST   0x7fff0000
#define MEMORY_LAST_SIZE_SHIFT  16

#define  ATOMIC_MEMORY(x)   ATOMIC(x)

void COR_MemoryInit(void);
unsigned char* COR_MemoryAlloc(unsigned short msize);
void COR_MemoryFree(void* addr);
unsigned short COR_GetFreeBlocks(void);
void COR_ManageFuseMemo(void);
unsigned short COR_GetMemoryFreeSize(void);
unsigned short COR_GetFirstFreeBlockSize(void);
unsigned short COR_MemoCheck(void);

void COR_MemoryCpy(unsigned char* dest,const unsigned char* source,unsigned short len);
void COR_MemorySet(unsigned char* dest,unsigned char dataT,unsigned short len);
boolean COR_MemoryCmp(const unsigned char* dest,const unsigned char* source,unsigned short len);
boolean COR_MemoryCmpData(const unsigned char* dest,unsigned char datatemp,unsigned short len);

#endif   
