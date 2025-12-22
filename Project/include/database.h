/************************************************************
* File name     : database.h
* Description   : header file for polling
* Author        : Jeevan Suresh
* License       : 
**************************************************************/
#ifndef __DATABASE_H__
#define __DATABASE_H__
#include "../include/common.h"

/******************************Local Variable Declaration******************************/
typedef struct dataBase {
    CommonDatabase wstCommonDatabase;         // The data stored in the node (can be any type)
    struct dataBase *next; // Pointer to the next node in the list
} dataBase_t;

extern dataBase_t *database_node_t;
/******************************Function Declaration******************************/
extern int ucCheck_and_update_node(CommonDatabase *astCommonDatabase);
extern int wstRetrive_data_node(PARAMETER_TYPE param_t, CommonDatabase *astCommonDatabase);

#endif /*__DATABASE_H__*/
