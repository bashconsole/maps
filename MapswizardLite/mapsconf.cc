/****************************************************************************
**  mapsconf.cpp
**  
**   Copyright (c) 2001 Impuls
**
**  Autor: A.N.Zubovsky (azu@imp.lg.ua)
**
*****************************************************************************/

#include "mapsconf.h"
#include "createfile.h"
#include "param.h"

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define def_menu_count 18

char *adrport[8] = {"0x220","0x228","0x2a0","0x2a8","0x320","0x328","0x3a0","0x3a8"};
short adrportf[3];
char *inter[8] = {"3", "4", "5", "7", "10", "11", "12", "15"};
short interruptf[3];
char *arbiter[3] = {"не арбитр", "единств. арбитр", "резервн. арбитр"};
char *arbiterr[3] = {"не арбитр", "единственный арбитр", "резервный арбитр магистрали"};
char *y_n[2] = {"нет", "да"};

char *menu[def_menu_count] = {
" 1. Выбор пути к файлам для загрузки КСв-31",
" 2. Конфигурирование магистралей сети МАПС",
" 3. Макс адрес подкл. абонента",
" 4. Адрес подключения",
" 5. Режим функционирования КСв-31",
" 6. Приоритетность ПЭВМ как абонента",
" 7. Время цикла опроса сообщений(дес.мс)",
" 8. Время ожидания ответа(мс)",
" 9. Квота выдачи сообщения для приоритетных абонентов",
"10. Квота выдачи сообщения при исчерпаниии длительности цикла",
"11. Квота выдачи сообщения для неприоритетных абонентов",
"12. Квота приема сообщений без подтверждения",
"13. Квота приема сообщений с подтверждением",
"14. Квота выдачи сообщений без подтверждения",
"15. Квота выдачи сообщений с подтверждением",
"16. Максимальное кол-во точек",
"17. Максимальное кол-во буферов",
"18. Имя файла конфигурации"
};

TMapsCfgData data;


char *getdata(char* s, int i)
{
   sprintf(s, "%d", i);
   return s;
}

void showmenu()
{
    printf(
"\n\n\n\n\n"
"______________________________________________\n"
"      Меню конфигурирования сети МАПС\n"
"______________________________________________\n\n");

for(int i=0; i<def_menu_count;i++)
   switch(i){
       case 0:
             printf("%s\n %10s %s\n", menu[i], "", data.path);
             break;
       case 1:
             printf("%s\n%10s", menu[i], "");
             for(int j = 0; j<3;j++)
                 printf(" [%d. %s  %s] ", j, data.channel[j].address_port, data.channel[j].interrupt);
             printf("\n");
             break;
       case 2:
            printf("%s = %s\n", menu[i], data.max_address);
            break;
       case 3:
            printf("%s = %s\n", menu[i], data.my_address);
            break;
       case 4:
            printf("%s\n%10s", menu[i], "");
            for(int j=0;j<3;j++)
                printf(" [%d. %s] ", j, data.arbiter[j]);
            printf("\n");
            break;
       case 17:
            printf("%s\n %10s %s\n", menu[i], "", data.savefilename);
            break;
       default:
            int j = i - 5;
            char* ptr = (char*)&data.priority;
            ptr += j*5;
            printf("%s = %s\n", menu[i], ptr);
            break;
   }
   printf(" 'q' - Выход\n");
}



void initdata()
{
   int i, j;

   strcpy(data.path, BootPath);
   for(i=0; i<3;i++){
       if(a_m[i].address_port == 0)
           sprintf(data.channel[i].address_port, "%x", a_m[i].address_port);
       else 
           sprintf(data.channel[i].address_port, "0x%x", a_m[i].address_port);
       sprintf(data.channel[i].interrupt, "%d", a_m[i].num_interrupt);

       if(a_m[i].address_port == 0)
           adrportf[i] = 0;
       else{
           adrportf[i] = 1;
           for(j=0; j<8; j++){
              if(a_m[i].address_port == ap[j]) break;
              adrportf[i] <<= 1;
           }
       }

       if(a_m[i].num_interrupt == 0)
          interruptf[i] = 0;
       else{
           interruptf[i] = 1;
           for(j=0; j<8;j++){
              if(a_m[i].num_interrupt == atoi(inter[j])) break;
              interruptf[i] <<= 1;
           }
       }

   }
   sprintf(data.max_address, "%d", vmax_address);
   sprintf(data.my_address, "%d", vmy_address);
   for(i=0;i<3;i++)
      strcpy(data.arbiter[i], arbiter[arbitr[i]]);
   strcpy(data.priority,y_n[priority]);
   sprintf(data.t0, "%d", kwtime_t0);
   sprintf(data.t, "%d", kwtime_t);
   sprintf(data.n0, "%d", kwtime_n0);
   sprintf(data.n1, "%d", kwtime_n1);
   sprintf(data.n2, "%d", kwtime_n2);
   sprintf(data.tinp, "%d", kwmes_tinp);
   sprintf(data.tout, "%d", kwmes_tout);
   sprintf(data.tinpn, "%d", kwmes_tinpn);
   sprintf(data.toutn, "%d", kwmes_toutn);
   strcpy(data.maxpoints, "10");
   strcpy(data.maxbuf, "10");
   sprintf(data.savefilename, "%s", config_filename);
}

int getValInt(const char* title, int min, int max)
{
     char s[256];
     int val = min - 1;
     while(1){
         printf("%s>>", title);
         scanf("%s", s);
         if(strncmp(s, "q", 1) == 0) return -1;
         if(strncmp(s, "m", 1) == 0) return -2;
         val = atoi(s);
         if( (val<min) || (val>max) )
             printf("ошибка: допустимое значение от %d до %d\n", min, max);
         else
             break;
     }
     return val;
}

char* getValStr(const char* title, char* s)
{
    printf("%s>>", title);
    scanf("%s", s);
    return s;
}

void channelConfig()
{
   int channel, param,n, i, count;
   int res, test;
   while(1){
       for(int j = 0; j<3;j++)
            printf("%5s %d. %s  %s\n", "", j, data.channel[j].address_port, data.channel[j].interrupt);
       channel = getValInt("Введите номер канала (0-2) m-меню", 0,2);
       if(channel < 0) return;

       printf("%5s %d. %s  %s\n", "",
               channel, data.channel[channel].address_port, data.channel[channel].interrupt);
       param = getValInt("Введите параметр редактирования (0-адрес, 1-прерывание, 2 - запретить) q-назад, m-меню", 0,2);
       if(param == -1) continue;
       if(param == -2) return;
       switch(param){
          case 0:
                res = 0;
                for(i=0;i<3;i++)
                    res |= adrportf[i];
                test = 1;
                count = 0;
                for(i=0;i<8;i++){
                   if((res & test) == 0){
                      printf("%5s %d. %s\n", "", count, adrport[i]);
                      count++;
                   }
                   test <<= 1;
                }
                n = getValInt("Выберите из списка необходимый адрес",0, count-1);
                if(n == -1) continue;
                if(n == -2) return;
                count = 0;
                n++;
                adrportf[channel] = 1;
                for(i=0; count!=n; i++){
                   if((res & adrportf[channel]) == 0) count++;
                   if(count!=n) adrportf[channel] <<= 1;
                }
                strcpy(data.channel[channel].address_port, adrport[i-1]);
                break;
          case 1:
                res = 0;
                for(i=0;i<3;i++)
                    res |= interruptf[i];
                test = 1;
                count = 0;
                for(i=0;i<8;i++){
                   if((res & test) == 0){
                      printf("%5s %d. %s\n", "", count, inter[i]);
                      count++;
                   }
                   test <<= 1;
                }
                n = getValInt("Выберите из списка прерывание",0, count-1);
                if(n == -1) continue;
                if(n == -2) return;
                count = 0;
                n++;
                interruptf[channel] = 1;
                for(i=0;count!=n;i++){
                   if((res & interruptf[channel]) == 0)  count++;
                   if(count!=n) interruptf[channel] <<= 1;
                }
                strcpy(data.channel[channel].interrupt, inter[i-1]);
                break;
          case 2:
                strcpy(data.channel[channel].address_port, "0");
                strcpy(data.channel[channel].interrupt, "0");
                adrportf[channel] = 0;
                interruptf[channel] = 0;
                break;
       }
   }
}
void abonentConf()
{
    int i, channel, n;

    while(1){
        for(i=0; i<3;i++)
           if(adrportf[i] != 0)
              printf("%10s %d. %s\n", "", i, data.arbiter[i]);
       channel = getValInt("Введите номер канала (m-меню)", 0,2);
       if(channel < 0) return;
       if(adrportf[channel] == 0){
           printf("ошибка: данный канал исключен из конфигурации\n");
           continue;
       }
       printf("%10s %d. %s\n", "", channel, data.arbiter[channel]);
       printf("Выберите необходимый параметр из списка:\n");
       for(i=0;i<3;i++)
          printf("%10s %d. %s\n", "", i, arbiter[i]);
       n = getValInt("Параметр (0-3) m-Меню, q-назад", 0,3);
       if(n == -1) continue;
       if(n == -2) return;
       strcpy(data.arbiter[channel], arbiter[n]);
       if(n == 3)
          for(i=0;i<3;i++){
             if(i == channel) continue;
             if(strcmp(data.arbiter[i], arbiter[3])!=0)
                strcpy(data.arbiter[i], arbiter[3]);
          }
       else
           for(i=0;i<3;i++){
             if(i == channel) continue;
             if(strcmp(data.arbiter[i], arbiter[3])==0)
                strcpy(data.arbiter[i], arbiter[0]);
          }
    }
}

int mapsconf()
{
   initdata();
   bool f = true;
   while(f){
      showmenu();
      int n = getValInt("Введите номер пункта для редактирования", 0, def_menu_count);
      switch(n){
      	case -1:
               f = false;
               break;
         case 1:
               char s[256];
               strcpy(data.path, getValStr("Введите путь", s));
               break;
         case 2:
               channelConfig();
               break;
         case 3:
               n = getValInt("Введите максимальный адрес (0-62)", 0, 62);
               if(n<0) break;
               sprintf(data.max_address,"%d", n);
               if(n < atoi(data.my_address))
                   sprintf(data.my_address,"%d", n);
               break;
         case 4:
               sprintf(s,"Введите адрес абонента (0-%d)",  atoi(data.max_address));
               n = getValInt(s, 0, atoi(data.max_address));
               if(n<0) break;
               sprintf(data.my_address,"%d", n);
               break;
         case 5:
               abonentConf();
               break;
         case 6:
               n = getValInt("Введите приоритетность (0-нет/1-да, m-Меню)", 0, 1);
               if(n<0) break;
               if(n==0)
                  sprintf(data.priority,"%s", "нет");
               else
                  sprintf(data.priority,"%s", "да");
               break;
         case 7:
               n = getValInt("Введите время цикла опроса", 0, 1000);
               if(n<0) break;
               sprintf(data.t0,"%d", n);
               break;
         case 8:
               n = getValInt("Введите время ожидания ответа", 0, 1000);
               if(n<0) break;
               sprintf(data.t,"%d", n);
               break;
         case 9:
               n = getValInt("Введите квоту выдачи", 0, 1000);
               if(n<0) break;
               sprintf(data.n0,"%d", n);
               break;
         case 10:
               n = getValInt("Введите квоту выдачи", 0, 1000);
               if(n<0) break;
               sprintf(data.n1,"%d", n);
               break;
         case 11:
               n = getValInt("Введите квоту выдачи", 0, 1000);
               if(n<0) break;
               sprintf(data.n2,"%d", n);
               break;
         case 12:
               n = getValInt("Введите квоту приема", 0, 1000);
               if(n<0) break;
               sprintf(data.tinp,"%d", n);
               break;
         case 13:
               n = getValInt("Введите квоту приема", 0, 1000);
               if(n<0) break;
               sprintf(data.tout,"%d", n);
               break;
         case 14:
               n = getValInt("Введите квоту выдачи", 0, 1000);
               if(n<0) break;
               sprintf(data.tinpn,"%d", n);
               break;
         case 15:
               n = getValInt("Введите квоту выдачи", 0, 1000);
               if(n<0) break;
               sprintf(data.toutn,"%d", n);
               break;
         case 16:
               n = getValInt("Ведите максимальное кол-во точек", 1, 32000);
               if(n<0) break;
               sprintf(data.maxpoints,"%d", n);
               break;
         case 17:
               n = getValInt("Введите максимальное кол-во буферов", 1, 32000);
               if(n<0) break;
               sprintf(data.maxbuf,"%d", n);
               break;
         case 18:
               strcpy(data.savefilename, getValStr("Введите путь с именем файла", s));
               break;
         default:
               break;
      }
   }

   char s[256];
   while(1){
      getValStr("Сгенирировать файлы настройки сети МАПС?(y/n)", s);
      if(strncmp(s, "y",1)==0){
         for(int i=0;i<3;i++)
              strcpy(data.arbiter[i], arbiterr[arbitr[i]]);
         sprintf(data.scriptfilename,"%s/init.sh", data.path);
         if(createfile(data)){
            printf("файлы сгенерированы\n");
         }
         else
            printf("Ошибка генерации файлов\n");
         break;
      }
      else
      if(strncmp(s, "n",1)==0)
         break;
      else
         printf("Ошибка ввода\n");
   }
   return 0;
}




