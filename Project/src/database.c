/************************************************************
* File name     : database.c
* Description   : database file for project
* Author        : Jeevan Suresh
* License       : Copyright (c) 2021 Trenser 
                    All Rights Reserved
**************************************************************/
/**********************Include Files**************************/
#include <stdio.h>
#include <stdlib.h>
#include "../include/database.h"

/*********************Global Variable Declaration*************/

dataBase_t *database_node_t = NULL;

ERROR_TYPE CheckAndUpdateDataNode(CommonDatabase *astCommonDatabase);
ERROR_TYPE RetriveDataNode(PARAMETER_TYPE param_t, CommonDatabase *astCommonDatabase);
/**********************************************************************
* Function name     : CreateNewDataNode
* Description       : to create new node to data table
* Arguments         : CommonDatabase *astCommonDatabase - hold the value of sensor
* Return type       : dataBase_t - return newly created node
*************************************************************************/
dataBase_t* CreateNewDataNode(CommonDatabase *astCommonDatabase)
{
    dataBase_t *database_newnode_t = (dataBase_t*)malloc(sizeof(dataBase_t));
    if(database_newnode_t == NULL)
    {
        return NULL;
    }
    else
    {
        database_newnode_t->wstCommonDatabase.param_t   = astCommonDatabase->param_t;
        database_newnode_t->wstCommonDatabase.iReadVal  = astCommonDatabase->iReadVal;
        database_newnode_t->next = NULL;
    }
    return database_newnode_t;
}

/**********************************************************************
* Function name     : CheckAndUpdateDataNode
* Description       : to check if node is there and update it
* Arguments         : CommonDatabase *astCommonDatabase - hold the value of sensor
* Return type       : uint8_t - return succes or error value
*************************************************************************/
ERROR_TYPE CheckAndUpdateDataNode(CommonDatabase *astCommonDatabase)
{
    ERROR_TYPE errVal = NO_ERR;
    unsigned char ucDataNodeFound = DEF_CLEAR;
    if(astCommonDatabase == NULL)
    {
        return ERRINVALID;
    }
    else
    {
        /* No process*/
    }
    pthread_mutex_lock(&g_pthreadlock);
    if(database_node_t == NULL)
    {
        database_node_t = CreateNewDataNode(astCommonDatabase);
        if(database_node_t == NULL)
        {
            errVal = ERRCREATN;
        }
        else
        {
            /*No Process*/
        }
    }
    else
    {
        dataBase_t* temp_node = database_node_t;
        dataBase_t* prev_node = NULL;
        while(temp_node != NULL)
        {
            if(temp_node->wstCommonDatabase.param_t == astCommonDatabase->param_t)
            {
                ucDataNodeFound = 1;
                temp_node->wstCommonDatabase.iReadVal = astCommonDatabase->iReadVal;
                break;
            }
            prev_node = temp_node;
            temp_node = temp_node->next;
        }

        if(ucDataNodeFound == DEF_CLEAR)
        {
            dataBase_t* new_node = CreateNewDataNode(astCommonDatabase);
            if(new_node == NULL)
            {
                errVal = ERRCREATN;
            }
            prev_node->next = new_node;
        }
        else
        {
            /* No process*/
        }
    }
    pthread_mutex_unlock(&g_pthreadlock);
    return errVal;
}

/**********************************************************************
* Function name     : RetriveDataNode
* Description       : to check if node is there and update it
* Arguments         : CommonDatabase *astCommonDatabase - hold the value of sensor
* Return type       : uint8_t - return succes or error value
*************************************************************************/
ERROR_TYPE RetriveDataNode(PARAMETER_TYPE param_t, CommonDatabase *astCommonDatabase)
{
    if(astCommonDatabase == NULL)
    {
        return ERRINVALID;
    }
    else
    {
        /* No Process*/
    }
    dataBase_t* temp_node = database_node_t;
    pthread_mutex_lock(&g_pthreadlock);
    while(temp_node != NULL)
    {
        if(temp_node->wstCommonDatabase.param_t == param_t)
        {
            astCommonDatabase->iReadVal = temp_node->wstCommonDatabase.iReadVal;
            astCommonDatabase->param_t = temp_node->wstCommonDatabase.param_t;
            break;
        }
        else
        {
            temp_node = temp_node->next;
        }
    }
    pthread_mutex_unlock(&g_pthreadlock);
    if(temp_node == NULL)
    {
        return ERR_NOT_FOUND;
    }

    return NO_ERR;
}

/*end of file*/