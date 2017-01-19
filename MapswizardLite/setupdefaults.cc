/****************************************************************************
**  setupdefaults.cpp
**  
**   Copyright (c) 2001 Impuls
**
**  Autor: A.N.Zubovsky (azu@imp.lg.ua)
**
*****************************************************************************/

#include "setupdefaults.h"
#include "param.h"

#include <string.h>
#include <unistd.h>

void setupDefaults()
{
   printf("defaults setup...\n");

   char *path = new char[256];
   path = getcwd(path, 255);
   strcpy(BootPath, path);
   delete path;

   int i;

   a_m[0].address_port = ap[4];
   a_m[0].num_interrupt = 10;
   
   a_m[1].address_port = ap[5];
   a_m[1].num_interrupt = 11;

   a_m[2].address_port = ap[1];
   a_m[2].num_interrupt = 5;

   for(i=0;i<3;i++)
      arbitr[i] = 2;

   priority = 0;

   strcpy(config_filename, "confmaps");
}
