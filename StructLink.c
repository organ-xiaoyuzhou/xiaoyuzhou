

#include PLATFORM_HEADER

#include "StructLink.h"

void COR_AddStructToLink(void* struct_add,void* task_link)
{
  ((TASK_LINK_ST*)task_link)->TaskNum++;
  ((STRUCT_ST*)struct_add)->Next = NULL_MM;
  if(((TASK_LINK_ST*)task_link)->TaskHeader==NULL_MM)
  {((TASK_LINK_ST*)task_link)->TaskHeader = struct_add;}
  else
  {((TASK_LINK_ST*)task_link)->TaskEnd->Next = struct_add;}
  ((TASK_LINK_ST*)task_link)->TaskEnd = struct_add;
}
boolean COR_DeleteStructFromLinkLst(void* struct_lst,void* struct_del,void* task_link)
{

  if(struct_del==(void*)((TASK_LINK_ST*)task_link)->TaskHeader)
  {((TASK_LINK_ST*)task_link)->TaskHeader = ((TASK_LINK_ST*)task_link)->TaskHeader->Next;}
  if(struct_del==((TASK_LINK_ST*)task_link)->TaskEnd)((TASK_LINK_ST*)task_link)->TaskEnd = struct_lst;
  if(struct_lst!=NULL_MM)((STRUCT_ST*)struct_lst)->Next = ((STRUCT_ST*)struct_del)->Next;
  if(((TASK_LINK_ST*)task_link)->TaskNum)((TASK_LINK_ST*)task_link)->TaskNum--;
  COR_MemoryFree((void*)struct_del);
  return TRUE;
}
boolean COR_OverStructFromLinkLst(void* struct_lst,void* struct_del,void* struct_new,void* task_link)
{
  if(struct_lst!=NULL_MM)((STRUCT_ST*)struct_lst)->Next = struct_new;
  ((STRUCT_ST*)struct_new)->Next = ((STRUCT_ST*)struct_del)->Next;
  if(struct_del==(void*)((TASK_LINK_ST*)task_link)->TaskHeader)
  {((TASK_LINK_ST*)task_link)->TaskHeader = (STRUCT_ST*)struct_new;}
  if(struct_del==((TASK_LINK_ST*)task_link)->TaskEnd)
  {((TASK_LINK_ST*)task_link)->TaskEnd = struct_new;}
  COR_MemoryFree((void*)struct_del);
  return TRUE;
}
boolean COR_DeleteStructFromLink(void* struct_del,void* task_link)
{
  STRUCT_ST* struct_temp;
  STRUCT_ST* struct_lst;
  struct_temp = ((TASK_LINK_ST*)task_link)->TaskHeader;
  struct_lst = NULL_MM;
  while(struct_temp != NULL_MM)
  {
    if(struct_temp==struct_del)
    {
      if(struct_del==((TASK_LINK_ST*)task_link)->TaskHeader)
      {((TASK_LINK_ST*)task_link)->TaskHeader = ((TASK_LINK_ST*)task_link)->TaskHeader->Next;}
      if(struct_del==((TASK_LINK_ST*)task_link)->TaskEnd)((TASK_LINK_ST*)task_link)->TaskEnd = struct_lst;
      if(struct_lst!=NULL_MM)struct_lst->Next = ((STRUCT_ST*)struct_del)->Next;
      if(((TASK_LINK_ST*)task_link)->TaskNum)((TASK_LINK_ST*)task_link)->TaskNum--;
      COR_MemoryFree((void*)struct_del);
      return TRUE;
    }
    struct_lst = struct_temp;
    struct_temp = struct_lst->Next;
  }
  return FALSE;
}
boolean COR_OverStructFromLink(void* struct_del,void* struct_new,void* task_link)
{
  STRUCT_ST* struct_temp;
  STRUCT_ST* struct_lst;
  struct_temp = ((TASK_LINK_ST*)task_link)->TaskHeader;
  struct_lst = NULL_MM;
  while(struct_temp != NULL_MM)
  {
    if(struct_temp==struct_del)
    {
      if(struct_lst!=NULL_MM)((STRUCT_ST*)struct_lst)->Next = struct_new;
      ((STRUCT_ST*)struct_new)->Next = ((STRUCT_ST*)struct_del)->Next;
      if(struct_del==(void*)((TASK_LINK_ST*)task_link)->TaskHeader)
      {((TASK_LINK_ST*)task_link)->TaskHeader = (STRUCT_ST*)struct_new;}
      if(struct_del==((TASK_LINK_ST*)task_link)->TaskEnd)
      {((TASK_LINK_ST*)task_link)->TaskEnd = struct_new;}
      COR_MemoryFree((void*)struct_del);
      return TRUE;
    }
    struct_lst = struct_temp;
    struct_temp = struct_lst->Next;
  }
  return FALSE;
}
void COR_DeleteAllStruct(void* task_link)
{
  STRUCT_ST* struct_temp;
  STRUCT_ST* struct_del;
  struct_temp = ((TASK_LINK_ST*)task_link)->TaskHeader;
  while(struct_temp != NULL_MM)
  {
    struct_del = struct_temp;
    struct_temp = struct_temp->Next;
    COR_MemoryFree((void*)struct_del);
  }
  ((TASK_LINK_ST*)task_link)->TaskHeader = NULL_MM;
  ((TASK_LINK_ST*)task_link)->TaskEnd = NULL_MM;
  ((TASK_LINK_ST*)task_link)->TaskNum = 0;
  return;
}
boolean COR_MoveStruct(void* struct_mov ,void* tasklink_sr,void* tasklink_dst)
{
  STRUCT_ST* struct_temp;
  STRUCT_ST* struct_lst;
  struct_temp = ((TASK_LINK_ST*)tasklink_sr)->TaskHeader;
  struct_lst = NULL_MM;
  while(struct_temp != NULL_MM)
  {
    if(struct_temp==struct_mov)
    {
      if(struct_mov==((TASK_LINK_ST*)tasklink_sr)->TaskHeader)
      {((TASK_LINK_ST*)tasklink_sr)->TaskHeader = ((TASK_LINK_ST*)tasklink_sr)->TaskHeader->Next;}
      if(struct_mov==((TASK_LINK_ST*)tasklink_sr)->TaskEnd)((TASK_LINK_ST*)tasklink_sr)->TaskEnd = struct_lst;
      if(struct_lst!=NULL_MM)((STRUCT_ST*)struct_lst)->Next = ((STRUCT_ST*)struct_mov)->Next;
      if(((TASK_LINK_ST*)tasklink_sr)->TaskNum)((TASK_LINK_ST*)tasklink_sr)->TaskNum--;
      
      ((TASK_LINK_ST*)tasklink_dst)->TaskNum++;
      ((STRUCT_ST*)struct_mov)->Next = NULL_MM;
      if(((TASK_LINK_ST*)tasklink_dst)->TaskHeader==NULL_MM)
      {((TASK_LINK_ST*)tasklink_dst)->TaskHeader = struct_mov;}
      else
      {((TASK_LINK_ST*)tasklink_dst)->TaskEnd->Next = struct_mov;}
      ((TASK_LINK_ST*)tasklink_dst)->TaskEnd = struct_mov;
      return TRUE;
    }
    struct_lst = struct_temp;
    struct_temp = struct_lst->Next;
  }
  return FALSE;
}
boolean COR_MoveStructLst(void* struct_lst,void* struct_mov ,void* tasklink_sr,void* tasklink_dst)
{
  if(struct_mov==((TASK_LINK_ST*)tasklink_sr)->TaskHeader)
  {((TASK_LINK_ST*)tasklink_sr)->TaskHeader = ((TASK_LINK_ST*)tasklink_sr)->TaskHeader->Next;}
  if(struct_mov==((TASK_LINK_ST*)tasklink_sr)->TaskEnd)((TASK_LINK_ST*)tasklink_sr)->TaskEnd = struct_lst;
  if(struct_lst!=NULL_MM)((STRUCT_ST*)struct_lst)->Next = ((STRUCT_ST*)struct_mov)->Next;
  if(((TASK_LINK_ST*)tasklink_sr)->TaskNum)((TASK_LINK_ST*)tasklink_sr)->TaskNum--;
  
  ((TASK_LINK_ST*)tasklink_dst)->TaskNum++;
  ((STRUCT_ST*)struct_mov)->Next = NULL_MM;
  if(((TASK_LINK_ST*)tasklink_dst)->TaskHeader==NULL_MM)
  {((TASK_LINK_ST*)tasklink_dst)->TaskHeader = struct_mov;}
  else
  {((TASK_LINK_ST*)tasklink_dst)->TaskEnd->Next = struct_mov;}
  ((TASK_LINK_ST*)tasklink_dst)->TaskEnd = struct_mov;
  return TRUE;
}
