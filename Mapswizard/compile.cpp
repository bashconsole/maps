#include "param.h"

#if 0

������ ��������� �������

return: KEY_WORD  - �������� ����� ��� �������
        KEY_STR   - �������� ����� � ��������
        KEY_NUM   - �����
        KEY_END   - ����� �����

������ ���������� ���������� ������� � �����������,
������� ����� ���� ��� ������������� (#), ��� �
�������������� (/* ... */)

��� KEY_WORD � KEY_STR ��������� ���������� � ������ lex,
������ ��� KEY_STR ��� ��������� � �������������� �������

��� KEY_NUM ��������� � �������� ���� ���������� � num

� ������ ����� ���� ������ ������� ������ �� ����
��������� � ��������� �����������

#endif

void error(char *s)
{
  printf("%d: %s\n",nzap,s);
  fclose(input_file);
  exit(0);
}
#if 0
int isdigit(char buf)
{
    int i,k0;

    k0=0;
    for(i=0; i!=10; ++i)
	if((buf-0x30) == i)
	    return(0);
	else
	    k0++;
    if(k0 == 10)
        return(1);
    return(0);
}
#endif
/*
 ������� ��� ����������������� �����
 � ������ ������ ���������� ��������� �� ������
 � ��������� �����������
*/

int hex(int c)
{
  if(isdigit(c))
    return(c-'0');
  if(c >= 'a' && c <= 'f')
    return(c-'a'+10);
  if(!(c >= 'A' && c <= 'F'))
    error("�������� ������ ������������������ �����");
  return(c-'A'+10);
}

void unget_sym(int c)
{
  flag_unget=1;
  c_unget=c;
}
/*
 ������� 1 � ������ ������� �������, ������� ��������:

 1) ������
 2) ������ ���������
 3) ������� ������
 4) ������ ������������� ����������� #
 5) ������ �������������� ����������� /
 6) ����� ��������� �����

 (� ������ 4-6 ������ ������������ ������� � �������� ����)

 � ��������� ������ ������������ 0
*/

int pusto(int c)
{
  switch(c) {

  case '#': case '/': case -1:
    unget_sym(c);

  case ' ': case '\n': case '\t': case '\r':
    return(1);
  }
  return(0);
}

int read_sym(void)
{
  int c;

  if(flag_unget) {
    flag_unget=0;
    return(c_unget);
  }
  if(flag_input) {

lab:
    if(fgets(pole,LENSTR,input_file) == NULL)
      return(-1);
    ind=0;
    flag_input=0;
  }
  switch(c=pole[ind++]) {

  case 0:
    goto lab;

  case '\n':
    ++nzap;
  }
  return(c&0xFF);
}

int scan(void)
{
  int   i, c;
  char *s;

/* ������������ ������ ������� */

cycle:
  while((c=read_sym()) == ' ' || c == '\t' || c == '\n'|| c == '\r')
    ;

  switch(c) {

  case -1:
    return(KEY_END);

  case '#': /*  ������������ ����������� */
/*    while((c=read_sym()) != '\r' && c != -1) */
    while((c=read_sym()) != '\n' && c != -1)
      ;
    if(c == -1)
      return(KEY_END);
    goto cycle;

  case '/': /* ������������� ����������� */
    if(read_sym() != '*')
      error("�������� ������ �������������� �����������");
    do {
      while((c=read_sym()) != '*')
        if(c == -1)
          error("������������� ������������� �����������");
      if((c=read_sym()) == -1)
        error("������������� ������������� �����������");
    } while(c != '/');
    goto cycle;
  }

/* ��������� ������� ������ ������� ��� ������ ��������� �� ������ */

  err_ind=ind;

/* �� ������ �� �������� ���������? */

  if(isdigit(c)) {
    num=c-'0';
    c=read_sym();

/* �� ����������������� �� �����? */

    if(num == 0)
      if(c == 'x' || c == 'X') {
        while(!pusto(c=read_sym()))
          num=(num<<4)+hex(c);
        return(KEY_NUM);
      }

/* ������ ����������� ����� */

    while(!pusto(c)) {
      if(!isdigit(c))
        error("������ � ������ ����������� �����");
      num=num*10+c-'0';
      c=read_sym();
    }
    return(KEY_NUM);
  }

  s=lex;

/* �� ������ �� ������� �������? */

  if(c == '_' || isalpha(c)) {
    *s++ = c;
    while(!pusto(c=read_sym()))
      if(c == '_' || isalpha(c) || isdigit(c))
        *s++ = c;
      else
        error("������ � ������ ��������� �����");
    *s=0;
    for(i=0; (s=word1[i].name) != NULL; ++i)
      if(strcmp(lex,s) == 0) {
        num=word1[i].code;
        return(KEY_WORD);
      }
    error("����������� �������� �����");
  }

/* �� ������ �� ������� � ��������? */

  if(c != '\"')
    error("�������������� ������");
  if((c=read_sym()) == '\"')
    error("������ ���������� ������");
  do {
    if(c == '\n' || c == -1)
      error("������������� ���������� ������");
    *s++ = c;
  } while((c=read_sym()) != '\"');
  if(!pusto(read_sym()))
    error("�������������� ������");
  *s=0;
  if(flag_file == 0) {
    for(i=0; (s=word2[i].name) != NULL; ++i)
      if(strcmp(lex,s) == 0) {
        num=word2[i].code;
        return(KEY_STR);
      }
    error("����������� ���������� ������");
  }
  flag_file=0;
  return(KEY_STR);
}

void test_end(int code)
{
  if(code == KEY_END)
    error("����������� ����� �����");
}

void test_num(int code)
{
  test_end(code);
  if(code != KEY_NUM)
    error("��������� �����");
}

void dubl(void)
{
  error("��������� ������� ���������");
}

int a_port[] = {0x220, 0x228, 0x2a0, 0x2a8, 0x320, 0x328, 0x3a0, 0x3a8, 0};
int uroven[] = {3, 4, 5, 7, 10, 11, 12, 15, 0};

void subMAGISTRAL(void)
{
  int i, code;

  if(cnt_magistral)
    error("��������� ������� ������");
  if((code=scan()) == KEY_WORD && num == ENDSECTION_m) {
    a_m[0].address_port = 0x3a0;
    a_m[0].num_interrupt = 5;
    a_m[1].address_port = 0;
    a_m[1].num_interrupt = 0;
    a_m[2].address_port = 0;
    a_m[2].num_interrupt = 0;
    return;
  }
  do {
    test_end(code);
    if(code != KEY_WORD || num != ADDRESS_m)
      error("��������� address");
    if(cnt_magistral == MAX_MAGISTRAL)
      error("������� ����� �����������");
    if(scan() != KEY_NUM)
      error("��������� �����");
    if(num) {
      for(i=0; a_port[i]; ++i)
        if(a_port[i] == num)
          goto ok1;
      error("������������ ����� ����� ����������� ���������� ���-31");
    }

ok1:
    a_m[cnt_magistral].address_port=num;
    if(scan() != KEY_NUM)
      error("��������� �����");
    if(num) {
      for(i=0; uroven[i]; ++i)
        if(uroven[i] == num)
          goto ok2;
      error("������������ ����� ������ ���������� ��� ����������� ���������� ���-31");
    }

ok2:
    a_m[cnt_magistral].num_interrupt=num;
    ++cnt_magistral;
  } while((code=scan()) != KEY_WORD || num != ENDSECTION_m);
}

void subFILE(void)
{
  int code;

  if(flag_BootPath)
    error("��������� ������� ������");
  test_end(code=scan());
  if(code != KEY_WORD)
    error("�������������� ������");
  switch(num) {

  case BOOTPATH_m:
    flag_file=1;
    if(scan() != KEY_STR)
      error("��������� ��� ��������");
    strcpy(BootPath,lex);
    if(scan() != KEY_WORD || num != ENDSECTION_m)
      error("��������� EndSection");

  case ENDSECTION_m:
    flag_BootPath=1;
  }
}

void subPARAM(void)
{
  int *adr=NULL;
  int code;

  if(flag_param)
    error("��������� ������� ������");
  flag_param=1;
  if((code=scan()) == KEY_WORD && num == ENDSECTION_m) {
    timer = 0;
    dynamic = 0;
    priority = 0;
    timing = 0;
    return;
  }
  for(;;) {
    test_end(code);
    if(code != KEY_WORD)
      error("�������������� ������");
    switch(num) {

    default:
      error("�������������� ������");

    case ENDSECTION_m:
      return;

    case TIMER_m:
      if(flag_timer)
        dubl();
      flag_timer=1;
      adr = &timer;
      break;

    case DYNAMIC_m:
      if(flag_dynamic)
        dubl();
      flag_dynamic=1;
      adr = &dynamic;
      break;

    case PRIORITY_m:
      if(flag_priority)
        dubl();
      flag_priority=1;
      adr = &priority;
      break;

    case TIMING_m:
      if(flag_timing)
        dubl();
      flag_timing=1;
      if(scan() == KEY_STR)
        switch(num) {

        case NOSYNHRO__m:
        case SRC__m:
        case DEST__m:
          timing=num;
          code=scan();
          continue;
        }
      error("�������������� ������");
    }
    if(scan() == KEY_STR)
      switch(num) {

      case YES__m:
      case NO__m:
        *adr=num;
        code=scan();
        continue;
      }
    error("�������������� ������");
  }
}

void subKVOTA(void)
{
  int code, i;

  if(flag_kvota)
    error("��������� ������� ������");
  flag_kvota=1;
  if((code=scan()) == KEY_WORD && num == ENDSECTION_m) {
    kwtime_t0 = 10;
    kwtime_t = 10;
    kwtime_n0 = 5;
    kwtime_n1 = 5;
    kwtime_n2 = 5;
    kwmes_tinp = 37;
    kwmes_tout = 37;
    kwmes_tinpn = 5;
    kwmes_toutn = 5;
    speed = 1000;
    return;
  }
  for(;;) {
    test_end(code);
    if(code != KEY_WORD)
      error("�������������� ������");
    switch(num) {

    default:
      error("�������������� ������");

    case ENDSECTION_m:
      return;

    case KWTIME_m:
      if(flag_kwtime)
        dubl();
      flag_kwtime=1;
      for(i=0; i<5; ++i) {
        test_num(code=scan());
        switch(i) {

        case 0:
          kwtime_t0=num;
          continue;

        case 1:
          kwtime_t=num;
          continue;

        case 2:
          kwtime_n0=num;
          continue;

        case 3:
          kwtime_n1=num;
          continue;

        default:
          kwtime_n2=num;
        }
      }
      break;

    case SPEED_m:
      if(flag_speed)
        dubl();
      flag_speed=1;
      test_num(code=scan());
      speed=num;
      break;

    case KWMES_m:
      if(flag_kwmes)
        dubl();
      flag_kwmes=1;
      for(i=0; i<4; ++i) {
        test_num(code=scan());
        switch(i) {

        case 0:
          kwmes_tinp=num;
          continue;

        case 1:
          kwmes_tout=num;
          continue;

        case 2:
          kwmes_tinpn=num;
          continue;

        default:
          kwmes_toutn=num;
        }
      }
    }
    code=scan();
  }
}

void subABONENT(void)
{
  int code;

  if(flag_abonent)
    error("��������� ������� ������");
  flag_abonent=1;
  if((code=scan()) == KEY_WORD && num == ENDSECTION_m) {
    vmax_address	= 16;
    vmy_address = 0;
    return;
  }
  for(;;) {
    test_end(code);
    if(code != KEY_WORD)
      error("�������������� ������");
    switch(num) {

    default:
      error("�������������� ������");

    case ENDSECTION_m:
      return;

    case MAX_ADDRESS_m:
      if(flag_max_address)
        dubl();
      flag_max_address=1;
      test_num(code=scan());
      if(num < 0 || num > 62)
        error("������������ �������� ���������");
      vmax_address=num;
      break;

    case MY_ADDRESS_m:
      if(flag_my_address)
        dubl();
      flag_my_address=1;
      test_num(code=scan());
      if(num < 0 || num > vmax_address)
        error("������������ �������� ���������");
      vmy_address=num;
    }
    code=scan();
  }
}

void subARBITR(void)
{
  int code;

  if(flag_arbitr)
    error("��������� ������� ������");
  flag_arbitr=1;
  if((code=scan()) == KEY_WORD && num == ENDSECTION_m) {
    arbitr[0] = 2;
    arbitr[1] = 2;
    arbitr[2] = 2;
    return;
  }
  for(;;) {
    test_end(code);
    if(code != KEY_WORD)
      error("�������������� ������");
    switch(num) {

    default:
      error("�������������� ������");

    case ENDSECTION_m:
      return;

    case ARBITR_m:
      if(count_arbitr == MAX_ARBITR)
        error("������� ����� ����������");
      test_end(code=scan());
      if(code != KEY_STR)
        error("�������������� ������");
      switch(num) {

      default:
        error("�������������� ������");

      case NOARBITR__m:
      case EDARBITR__m:
      case REZARBITRMAG__m:
      case REZARBITRNET__m:
        arbitr[count_arbitr]=num;
        ++count_arbitr;
      }
    }
    code=scan();
  }
}

void compile(void)
{
  int i;
  int code;

  test_end(code=scan());
  do {
    if(code != KEY_WORD && num != SECTION_m)
      error("��������� Section");
    test_end(code=scan());
    if(code != KEY_STR)
      error("��������� ���������� ������");
    switch(num) {

    default:
      error("����������� ������");

    case FILE__m:
      subFILE();
      break;

    case MAGISTRAL__m:
      subMAGISTRAL();
      break;

    case ABONENT__m:
      subABONENT();
      break;

    case ARBITR__m:
      subARBITR();
      for(i=0; i!=3; i++) {
        switch(arbitr[i]) {
	case 0:
        case NOARBITR__m:
          arbitr[i] = 0;
          break;
        case EDARBITR__m:
      	  arbitr[i] = 1;
          break;
        case REZARBITRMAG__m:
      	  arbitr[i] = 2;
          break;
        case REZARBITRNET__m:
      	  arbitr[i] = 3;
          break;
	}
      }
      break;

    case PARAM__m:
      subPARAM();
      if(timer == YES__m)
      	timer = 1;
      else
      	timer = 0;
      if(dynamic == YES__m)
        dynamic = 1;
      else
        dynamic = 0;
      if(priority == YES__m)
        priority = 1;
      else
        priority = 0;
      if(timing == NOSYNHRO__m)
        timing = 0;
      if(timing == SRC__m)
        timing = 1;
      if(timing == DEST__m)
        timing = 2;
      break;

    case KVOTA__m:
      subKVOTA();
    }
  } while((code=scan()) != KEY_END);
}
