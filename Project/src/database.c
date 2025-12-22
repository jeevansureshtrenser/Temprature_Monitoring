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

int ucCheck_and_update_node(CommonDatabase *astCommonDatabase);
int wstRetrive_data_node(PARAMETER_TYPE param_t, CommonDatabase *astCommonDatabase);
/**********************************************************************
* Function name     : create_node
* Description       : to create new node to data table
* Arguments         : CommonDatabase *astCommonDatabase - hold the value of sensor
* Return type       : dataBase_t - return newly created node
*************************************************************************/
dataBase_t* create_node(CommonDatabase *astCommonDatabase)
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
* Function name     : check_and_update_node
* Description       : to check if node is there and update it
* Arguments         : CommonDatabase *astCommonDatabase - hold the value of sensor
* Return type       : uint8_t - return succes or error value
*************************************************************************/
int ucCheck_and_update_node(CommonDatabase *astCommonDatabase)
{
    unsigned char ucDataNodeFound = DEF_CLEAR;
    if(astCommonDatabase == NULL)
    {
        return ERRINVALID;
    }
    else
    {
        /* No process*/
    }

    if(database_node_t == NULL)
    {
        database_node_t = create_node(astCommonDatabase);
        if(database_node_t == NULL)
        {
            return ERRCREATN;
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
            dataBase_t* new_node = create_node(astCommonDatabase);
            if(new_node == NULL)
            {
                return ERRCREATN;
            }
            prev_node->next = new_node;
        }
        else
        {
            /* No process*/
        }
    }
    return NO_ERR;
}

/**********************************************************************
* Function name     : check_and_update_node
* Description       : to check if node is there and update it
* Arguments         : CommonDatabase *astCommonDatabase - hold the value of sensor
* Return type       : uint8_t - return succes or error value
*************************************************************************/
int wstRetrive_data_node(PARAMETER_TYPE param_t, CommonDatabase *astCommonDatabase)
{
    if(astCommonDatabase == NULL)
    {
        return ERRINVALID;
    }
    dataBase_t* temp_node = database_node_t;
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

    return NO_ERR;
}

/*end of file*/