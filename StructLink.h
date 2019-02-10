

#ifndef _APP_STRUCTLINK_H
#define _APP_STRUCTLINK_H

#include "Memory.h"

#ifndef  NULL_MM
#define    NULL_MM    (void*)(0)
#endif

typedef struct{
  void* Next;
  unsigned char DataAddr[];
}STRUCT_ST;
typedef struct{
  STRUCT_ST* TaskHeader;
  STRUCT_ST* TaskEnd;
  unsigned short TaskNum;
}TASK_LINK_ST;

void COR_AddStructToLink(void* struct_add,void* task_link);
boolean COR_DeleteStructFromLinkLst(void* struct_lst,void* struct_del,void* task_link);
boolean COR_OverStructFromLinkLst(void* struct_lst,void* struct_del,void* struct_new,void* task_link);
boolean COR_DeleteStructFromLink(void* struct_del,void* task_link);
boolean COR_OverStructFromLink(void* struct_del,void* struct_new,void* task_link);
void COR_DeleteAllStruct(void* task_link);
boolean COR_MoveStruct(void* struct_mov ,void* tasklink_sr,void* tasklink_dst);
boolean COR_MoveStructLst(void* struct_lst,void* struct_mov ,void* tasklink_sr,void* tasklink_dst);


#endif