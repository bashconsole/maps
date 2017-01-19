/****************************************************************************
**  createfile.cpp
**  
**   Copyright (c) 2001 Impuls
**
**  Autor: A.N.Zubovsky (azu@imp.lg.ua)
**
*****************************************************************************/

#define def_section1_title "Файл" 
#define def_section2_title "Магистрали" 
#define def_section3_title "Абонент"
#define def_section4_title "Арбитр"
#define def_section5_title "Параметры"
#define def_section6_title "Квоты"

#define def_section1_comments "# Конфигурирование сети МАПС, выполняется программой mapsconfig\n\
#\n\
# Секция файл содержит путь к каталогу с файлами для загрузки КСв-31,\n\
# если путь не задан загрузка из текущего каталога\n\
# Пример не заданной секции:\n\
#       Section \"Файл\"\n\
#       EndSection"

#define def_section2_comments "# Секция конфигурирования магистралей сети МАПС\n\
#       address        <address_port>   <interrupt>\n\
#\n\
#       address_port - задает адрес порта подключения устройства КСв-31\n\
#                      необходимо указывать для каждой магистрали (0x220,\n\
#                      0x228,0x2a0,0x2a8,0x320,0x328,0x3a0,0x3a8), не может\n\
#                      быть опущен, 0 - отсутсвует данная магистраль.\n\
#       interrupt - задает номер уровня прерывания для подключения устройства\n\
#                      КСв-31 (3, 4, 5, 7, 10, 11, 12, 15), не может быть опущен,\n\
#                      0 - отсутсвует данная магистраль.\n\
#                      (address 0 0) - нет выхода на данную магистраль."

#define def_section3_comments "#  Секция конфигурирования абонентов сети МАПС\n\
#\n\
#	max_address - задает маскимальный адрес подключения абонента в сеть,\n\
#	              не может превышать 62 (по умолчанию 62).\n\
#\n\
#	my_address - задает адрес подключения данной ПЭВМ в сеть (0-62),\n\
#	             не может превышать max_address, не может быть опущен."

#define def_section4_comments "#  Секция конфигурирования режима функционирования КСв-31 как арбитра\n\
#  в сети МАПС\n\
#\n\
#	Для каждой магистрали может быть задан только один из вариантов,\n\
#	не может быть опущен:\n\
#	 - не арбитр;\n\
#	 - единственный арбитр;\n\
#	 - резервный арбитр;\n\
#"

#define def_section5_comments "#  Секция конфигурирования параметров функционирования КСв-31\n\
#\n\
#	priority - приоритетность ПЭВМ как абонента: приоритетный - \"да\",\n\
#		   неприоритетный - \"нет\" (по умолчанию \"нет\").\n\
#"

#define def_section6_comments "#  Секция конфигурирования параметров функционирования КСв-31\n\
#\n\
#	kwtime	<t0> <t> <n0> <n1> <n2>\n\
#\n\
#	t0 - время цикла опроса сообщений  (по умолчанию 10)\n\
#\n\
#	t - время ожидания ответа  (по умолчанию 10)\n\
#\n\
#	n0 - квота выдачи сообщения для приоритетных абонентов\n\
#	     (по умолчанию 5)\n\
#	n1 - квота выдачи сообщения для приоритетных абонентов\n\
#	     при исчерпаниии длительности цикла  (по умолчанию 37)\n\
#	n2 - квота выдачи сообщения для неприоритетных абонентов\n\
#	     (по умолчанию 5)\n\
#\n\
#  Параметры приема/выдачи сообщений\n\
#\n\
#	kwmes <tinp> <tout> <tinpn> <toutn>\n\
#\n\
#	tinp - квота приема сообщений без подтверждения (по умолчанию 37)\n\
#\n\
#	tout - квота приема сообщений с подтверждением  (по умолчанию 37)\n\
#\n\
#	tinpn - квота выдачи сообщений без подтверждения  (по умолчанию 5)\n\
#\n\
#	toutn - квота выдачи сообщений с подтверждением (по умолчанию 5)"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "createfile.h"

void beginSection(FILE *file, char *title){ fprintf(file,"Section \"%s\"\n",title); }
void endSection(FILE *file){ fprintf(file,"EndSection\n"); }

int createfile(TMapsCfgData &data)
{
   int i;
   
   FILE *file = fopen(data.savefilename,"w");

   if(file == NULL) return 0;
   
   //section 1
   
   fprintf(file,"%s\n", def_section1_comments);
   beginSection(file, def_section1_title);
   fprintf(file,"%10s BootPath    \"%s\"\n", "", data.path);
   endSection(file);
   
   //section 2
   
   fprintf(file,"%s\n", def_section2_comments);
   beginSection(file, def_section2_title);
   for(i = 0; i < 3; i++)
      fprintf(file,"%10s address %10s %6s\n", "",data.channel[i].address_port, data.channel[i].interrupt);
   endSection(file);
   
   //section 3
   
   
   fprintf(file,"%s\n", def_section3_comments);
   beginSection(file, def_section3_title);
   fprintf(file,"%10s max_address %10s\n", "", data.max_address);
   fprintf(file,"%10s my_address  %10s\n", "", data.my_address);
   endSection(file);
   
   
   //section 4
   
   fprintf(file,"%s\n", def_section4_comments);
   beginSection(file, def_section4_title);
   for(i = 0; i < 3; i++)
     fprintf(file,"%10s arbiter    \"%s\"\n", "", data.arbiter[i]);
   endSection(file);

   
   fprintf(file,"%s\n", def_section5_comments);
   beginSection(file, def_section5_title);
   fprintf(file,"%10s priority \"%s\"\n", "", data.priority);
   endSection(file);
   
   
   fprintf(file,"%s\n", def_section6_comments);
   beginSection(file, def_section6_title);
   fprintf(file,"%10s kwtime    %4s %3s %3s %3s %3s\n", "", data.t0, data.t, data.n0, data.n1, data.n2);
   fprintf(file,"%10s kwmes     %4s %3s %3s %3s\n", "", data.tinp, data.tout, data.tinpn, data.toutn);
   endSection(file);

   fclose(file);

   file = fopen(data.scriptfilename, "w");

   fprintf(file,"#!/bin/sh\n");

   bool f = false;

   for(int i=0;i<3;i++)
      if(strcmp(data.channel[i].address_port, "0") != 0)
          if(f)
            fprintf(file, "insmod /usr/src/linux/drivers/net/ksw31/ksw31%d.o  io=%s  irq=%s device=\"ksv31%d\"\n",
                i+1, data.channel[i].address_port, data.channel[i].interrupt, i+1);
          else
            fprintf(file, "insmod /usr/src/linux/drivers/net/ksw31/ksw31%d.o  io=%s  irq=%s\n", i+1, data.channel[i].address_port, data.channel[i].interrupt);
      else
          f = true;

    fprintf(file, "insmod /usr/src/linux/net/maps/af_maps.o SK=%s  BUF=%s\n", data.maxpoints, data.maxbuf);
    fprintf(file, "./initmaps %s\n", data.savefilename);

   fclose(file);
   chmod(data.scriptfilename, 00744);

   return 1;
}
