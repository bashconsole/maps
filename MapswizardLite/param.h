// Програма начальной инициализации устройства Ксв-31
// Запуск на выполнение в ОС Linux
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <malloc.h>
#include <string.h>
#include <errno.h>

#define LENSTR          512
#define MAX_MAGISTRAL   10
#define MAX_ARBITR      10

#define KEY_WORD        0
#define KEY_STR         1
#define KEY_NUM         2
#define KEY_END         3

#define SECTION_m       0
#define BOOTPATH_m      1
#define ADDRESS_m       2
#define MAX_ADDRESS_m   3
#define MY_ADDRESS_m    4
#define ARBITR_m        5
#define TIMER_m         6
#define DYNAMIC_m       7
#define PRIORITY_m      8
#define TIMING_m        9
#define KWTIME_m        10
#define SPEED_m         11
#define KWMES_m         12
#define ENDSECTION_m    13

#define FILE__m         100
#define MAGISTRAL__m    101
#define ABONENT__m      102
#define ARBITR__m       103
#define NOARBITR__m     104
#define EDARBITR__m     105
#define REZARBITRMAG__m 106
#define REZARBITRNET__m 107
#define PARAM__m        108
#define YES__m          109
#define NO__m           110
#define NOSYNHRO__m     111
#define KVOTA__m        112
#define SRC__m          113
#define DEST__m         114

#define LENBUF 256

#define YES     0
#define NO      1

#define LEN     2048
#define LENCOM  14
#define LENNAME 14
#define LENSTROK 80

#define TICK_IN_DAY 0x1800b0
#define TIMEIN    1000/55
#define TIMEOUT   1000/55
#define TIMERESET 4000/55

#define SIZE_1  5
#define SIZE_2  6
#define SIZE_3  8
#define SIZE_4  14

#define TKSW31  6
#define TBSI23  5

struct key_word {
  char *name;
  int  code;
};

// Описатель абонента в параметрах КСв 31
struct PARAM_AB {
        unsigned int  addr:6,   // Адрес абонента (1 уровня)
                      n_shl:2,  // Номер шлюза (0 - нет выхода)
                                // совпадает с номером магистрали
                      rzab:8;
};

struct ADDR_SH {
        unsigned int adr:6, // адрес на магистрали (1 уровня)
                     shl:2, // номер шлюза выхода на магистраль,
//                             совпадает с номером магистрали
                     rzaddr:8;
};

// Тип загружаемого КСв
struct KSW_MODE {
        unsigned int  pri:1,
//              0=0 - неприоритетный;    =1 - приоритетный.
                      stat:2,
//              =00 - пассивный
//              =01 - инициативний, по умолчанию задано
//              =10 - активизируемый находится в пассивном
//              =10 - активизируемый находится в активном
                      rz:1,
                      kto:1,    // 4 разряд
//                      0 - решение о вхождении принимает КСв
//                      1 - решение о вхождении принимает Host
                      rzksw:3,
                      rzmode:4,
                      mytype:4; // 15-12 разряд
//              =1000 - КСв31
//              =0011 - БСИ-23
};

struct YES_SHL {
        unsigned int  shl1_yes:1,
                      shl2_yes:1,
                      shl3_yes:1,
//              000 - нет шлюзов 1, 2, 3
//              100 - есть шлюз 1
//              010 - есть шлюз 2
//              001 - есть шлюз 3
                      rzshl:5,
                      rzyes:8;
};


// Буфер параметров настройки КСв 31
struct PARAM31 {
        unsigned int maxaddr_ab:8, // Максимальный адрес абонентов в сети
                                   // (2-62)
                     rzparam:8;
        struct ADDR_SH addr_sh;   // Адрес с номером шлюза
        struct KSW_MODE type_ksw; // Тип загружаемого КСв
        struct YES_SHL shl;       // Наличие шлюзов
        short int t0_cic;     // Т0 (..) - время цикла опроса сообщений;
                        // десятки миллисекунд.
        int t_echo;     // t  (..) - время ожидания ответа; миллисекунд.
        int kwotaN0;    // N0-квота выдачи сообщений для приоритетных
                        // абонентов
        int kwotaN1;    // N1-квота выдачи сообщений для приоритетных
                        // абонентов (при исчерпании длительности цикла)
        int kwotaN2;    // N2-квота выдачи сообщений для неприоритетных
                        // абонентов
// 23.06.2000
	int kwinp00;	// Квота для входных сообщений без подтверждения (9)
	int kwinpxx;	// Квота для входных сообщений с подтверждением (10)
	int kwout00;	// Квота для выходных сообщений без подтверждения (11)
	int kwoutxx;	// Квота для выходных сообщений с подтверждением (12)
/*
	int lenspace;   // Размер буферной памяти для приема сообщений
                        // с подтверждением/ответом 4*142=568
*/
};

struct address_magistral {
  int address_port;
  int num_interrupt;
};

extern int errno;

extern struct key_word word1[];
extern struct key_word word2[];

extern struct PARAM31 param_ksw;
extern int handle;
extern int status;
extern struct BUF_HOST_ARBITR buffer_ans;
extern unsigned char num_mag;

extern char *errno_message[];

extern char buf_my[];
extern char pole[];
extern char name[];
extern char code[];
extern char endcod[];
extern unsigned long bincode;
extern long kscode;
extern FILE *fcom;
extern FILE *stdprt;   /* Указтель на файл протокола */
extern char filecom[];
extern char filenconf[];
extern unsigned long *zad;
extern unsigned long header;
extern int len;
extern int nzap;
extern int nmag;
extern int ncode;
extern char flag_er;
extern char flagadr;
extern char flagcode;
extern FILE *input_file;
extern char *fcnf[];
extern int coderr;     /* код завершения */
extern long firstcode; /* Команда запуска */
extern unsigned long addr0;      /* Адрес запуска */
extern int wordcode[];
extern char *charaddr;
extern short int *wordaddr;
extern unsigned long longcode;
extern int wcode;
extern int addrparam; /* Адрес параметров КСв31 содержимое 32 ячейки РП */
extern int lenparam; /* Длина параметров КСв33 содержимое 33 ячейки РП */
extern char *parm;
extern int size_mas[]; /* Размеры буферов КСВ-31 */
extern int nparam;
extern int conf_ptr;
extern int err_maps;

extern long num;

extern char flag_unget;
extern char flag_input;
extern int  ind;
extern int  err_ind;
extern int  nzap;
extern int  c_unget;

extern int timer;
extern int dynamic;
extern int priority;
extern int timing;

extern char flag_timer;
extern char flag_dynamic;
extern char flag_priority;
extern char flag_timing;
extern char flag_param;

extern char flag_file;
extern char BootPath[];
extern char lex[];
extern char flag_BootPath;

extern char config_filename[];

extern char flag_kvota;
extern int kwtime_t0;
extern int kwtime_t;
extern int kwtime_n0;
extern int kwtime_n1;
extern int kwtime_n2;
extern int kwmes_tinp;
extern int kwmes_tout;
extern int kwmes_tinpn;
extern int kwmes_toutn;
extern int speed;
extern char flag_kwtime;
extern char flag_kwmes;
extern char flag_speed;

extern char flag_abonent;
extern char flag_max_address;
extern char flag_my_address;
extern int  vmax_address;
extern int  vmy_address;

extern char flag_arbitr;
extern int arbitr[];
extern int count_arbitr;

extern int cnt_magistral;
extern struct address_magistral a_m[MAX_MAGISTRAL];
extern int ap[];

extern char ssr[];
extern int  exitpar;
extern unsigned char przag_ksw;

void compile(void);

int begin_boot_MAPS(unsigned long header, unsigned char number_adapter);
int reset_boot_MAPS(unsigned long header, unsigned char number_adapter);
int move_boot_MAPS(unsigned long header,unsigned char number_adapter,
                  unsigned long address,void *buffer,int len);
int end_boot_MAPS(unsigned long header,unsigned char number_adapter,
                 unsigned long comand,int len);

struct BUF_HOST_ARBITR {
        unsigned char address_arbitr;	/* Адрес активного арбитра на */
					/* магистрали */
        unsigned char nmag;             /* Номер магистрали, */
        unsigned char port;             /* Адрес (1 уровеня) КСв как */
        				/* абонента на магистрали */
        unsigned char status;	/* Первый байт состояния */
					/* своего шлюза */
};
