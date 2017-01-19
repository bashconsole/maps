#include "param.h"

struct PARAM31 param_ksw;

int handle;
 
int status;

struct BUF_HOST_ARBITR buffer_ans;

unsigned char num_mag;


char *errno_message[] = {"ENFILE
 - ������� ��������� ������ �����������","EPROTONOSUPPORT
 -
 ������ � ����� ���� �� ������ �� �� 
��������������",
"ENOBUFS
 -
 ����������� �������� ������ ��� ������� � 
�������� �������","EBADF
 -
 ������������ ������������� ����� ������� � �������� 
������� ����",
"EINVAL
 - ������ ��� ��� ��������� �����������"};

char ssr[80];    
int  exitpar;
unsigned char przag_ksw;

char buf_my[LEN];
char name[LENNAME*3];
char code[9];
char endcod[5];
unsigned long bincode;
long kscode;
FILE *fcom;
FILE *stdprt;   /* �������� �� ���� ��������� */
char filecom[] = "ksv31cbt.hex";
unsigned long *zad = NULL;
unsigned long header;
int len;
//int nzap;
int nmag;
int ncode;
char flag_er;
char flagadr;
char flagcode;
FILE * fconf;
char filenconf[] = "confmaps";
char *fcnf[3];
int coderr;     /* ��� ���������� */
long firstcode; /* ������� ������� */
unsigned long addr0;      /* ����� ������� */
int wordcode[2];
short int *wordaddr;
char *charaddr;
unsigned long longcode;
int wcode;
int addrparam; /* ����� ���������� ���31 ���������� 32 ������ �� */
int lenparam; /* ����� ���������� ���33 ���������� 33 ������ �� */
char *parm;
int size_mas[] = {568,710,852,994,1136}; /* ������� ������� ���-31 */
int nparam = 0;
int conf_ptr = 0;
int err_maps = 0;

FILE *input_file;
long num;

char flag_unget;
char flag_input = 1;
int  ind;
int  err_ind;
int  nzap = 1;
int  c_unget;

int timer = NO__m;
int dynamic = NO__m;
int priority = NO__m;
int timing = NOSYNHRO__m;

char flag_timer;
char flag_dynamic;
char flag_priority;
char flag_timing;
char flag_param;

char flag_file;
char BootPath[LENSTR+1];
char lex[LENSTR+1];
char flag_BootPath;

char config_filename[256];

char flag_kvota;
int kwtime_t0 = 10;
int kwtime_t = 5;
int kwtime_n0 = 1;	
int kwtime_n1 = 1;
int kwtime_n2 = 1;
int kwmes_tinp = 37;
int kwmes_tout = 5;
int kwmes_tinpn = 37;
int kwmes_toutn = 5;
int speed = 1000;
char flag_kwtime;
char flag_kwmes;
char flag_speed;

char flag_abonent;
char flag_max_address;
char flag_my_address;
int  vmax_address = 62;
int  vmy_address = 14;

char flag_arbitr;
int arbitr[MAX_ARBITR];
int count_arbitr;

int cnt_magistral;
struct address_magistral a_m[MAX_MAGISTRAL];
int ap[8] = {544, 552, 672, 680, 800, 808, 928, 936};

char pole[LENSTR+1];
struct key_word word1[] =
{
"Section",     SECTION_m,
"BootPath",    BOOTPATH_m,
"address",     ADDRESS_m,
"max_address", MAX_ADDRESS_m,
"my_address",  MY_ADDRESS_m,
"arbiter",     ARBITR_m,
"timer",       TIMER_m,
"dynamic",     DYNAMIC_m,
"priority",    PRIORITY_m,
"timing",      TIMING_m,
"kwtime",      KWTIME_m,
"speed",       SPEED_m,
"kwmes",       KWMES_m,
"EndSection",  ENDSECTION_m,
NULL
};

struct key_word word2[] =
{
"����",                        FILE__m,
"����������",                  MAGISTRAL__m,
"�������",                     ABONENT__m,
"������",                      ARBITR__m,
"�� ������",                   NOARBITR__m,
"������������ ������",         EDARBITR__m,
"��������� ������ ����������", REZARBITRMAG__m,
"��������� ������ ����",       REZARBITRNET__m,
"���������",                   PARAM__m,
"��",                          YES__m,
"���",                         NO__m,
"�� ����������������",         NOSYNHRO__m,
"��������",                    SRC__m,
"��������",                    DEST__m,
"�����",                       KVOTA__m,
NULL
};



