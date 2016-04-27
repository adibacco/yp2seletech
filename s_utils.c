/******************** (C) COPYRIGHT 2014 Seletech srl *************************/
/* File Name          : s_Utils.c                                             */
/* Author             : Paolo Torretta                                        */
/* Version            : v1.0.0                                                */
/* Date               : 10/12/2014                                            */
/* Description        : This firmware includes all the functions related to   */
/*                      the ethernet.                                         */
/******************************************************************************/

#include <stdio.h>

/******************************************************************************/
/* functions                                                                  */
/******************************************************************************/

void delay(unsigned nloop)
{
	unsigned counter=0;
	while(counter<nloop)
		counter=counter+1;
}

