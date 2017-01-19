#include "param.h"

#if 0

Разбор очередной лексемы

return: KEY_WORD  - Ключевое слово без кавычек
        KEY_STR   - Ключевое слово в кавычках
        KEY_NUM   - Число
        KEY_END   - Конец файла

Сканер игнорирует незначащие символы и комментарии,
которые могут быть как однострочными (#), так и
многострочными (/* ... */)

Для KEY_WORD и KEY_STR результат помещается в строку lex,
причем для KEY_STR без начальной и заключительной кавычки

Для KEY_NUM результат в двоичном виде помещается в num

В случае каких либо ошибок функция выдает об этом
сообщение и программа завершается

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
 Вернуть код шестнадцатиричной цифры
 В случае ошибок печатается сообщение об ошибке
 и программа завершается
*/

int hex(int c)
{
  if(isdigit(c))
    return(c-'0');
  if(c >= 'a' && c <= 'f')
    return(c-'a'+10);
  if(!(c >= 'A' && c <= 'F'))
    error("Ошибочна запись шестнадцатиричного числа");
  return(c-'A'+10);
}

void unget_sym(int c)
{
  flag_unget=1;
  c_unget=c;
}
/*
 Вернуть 1 в случае пустого символа, которым является:

 1) Пробел
 2) Символ табуляции
 3) Перевод строки
 4) Начало однострочного комментария #
 5) Начало многострочного комментария /
 6) Конец исходного файла

 (В случае 4-6 символ возвращается обратно в исходный файл)

 В противном случае возвращается 0
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

/* Игнорировать пустые символы */

cycle:
  while((c=read_sym()) == ' ' || c == '\t' || c == '\n'|| c == '\r')
    ;

  switch(c) {

  case -1:
    return(KEY_END);

  case '#': /*  Однострочный комментарий */
/*    while((c=read_sym()) != '\r' && c != -1) */
    while((c=read_sym()) != '\n' && c != -1)
      ;
    if(c == -1)
      return(KEY_END);
    goto cycle;

  case '/': /* Многострочный комментарий */
    if(read_sym() != '*')
      error("Ошибочна запись многострочного комментария");
    do {
      while((c=read_sym()) != '*')
        if(c == -1)
          error("Незаконченный многострочный комментарий");
      if((c=read_sym()) == -1)
        error("Незаконченный многострочный комментарий");
    } while(c != '/');
    goto cycle;
  }

/* Сохранить позицию начала лексемы для выдачи сообщения об ошибке */

  err_ind=ind;

/* Не начало ли числовой константы? */

  if(isdigit(c)) {
    num=c-'0';
    c=read_sym();

/* Не шестнадцатиричное ли число? */

    if(num == 0)
      if(c == 'x' || c == 'X') {
        while(!pusto(c=read_sym()))
          num=(num<<4)+hex(c);
        return(KEY_NUM);
      }

/* Сборка десятичного числа */

    while(!pusto(c)) {
      if(!isdigit(c))
        error("Ошибка в записи десятичного числа");
      num=num*10+c-'0';
      c=read_sym();
    }
    return(KEY_NUM);
  }

  s=lex;

/* Не начало ли обычной лексемы? */

  if(c == '_' || isalpha(c)) {
    *s++ = c;
    while(!pusto(c=read_sym()))
      if(c == '_' || isalpha(c) || isdigit(c))
        *s++ = c;
      else
        error("Ошибка в записи ключевого слова");
    *s=0;
    for(i=0; (s=word1[i].name) != NULL; ++i)
      if(strcmp(lex,s) == 0) {
        num=word1[i].code;
        return(KEY_WORD);
      }
    error("Неизвестное ключевое слово");
  }

/* Не начало ли лексемы в кавычках? */

  if(c != '\"')
    error("Синтаксическая ошибка");
  if((c=read_sym()) == '\"')
    error("Пустая символьная строка");
  do {
    if(c == '\n' || c == -1)
      error("Незаконченная символьная строка");
    *s++ = c;
  } while((c=read_sym()) != '\"');
  if(!pusto(read_sym()))
    error("Синтаксическая ошибка");
  *s=0;
  if(flag_file == 0) {
    for(i=0; (s=word2[i].name) != NULL; ++i)
      if(strcmp(lex,s) == 0) {
        num=word2[i].code;
        return(KEY_STR);
      }
    error("Неизвестная символьная строка");
  }
  flag_file=0;
  return(KEY_STR);
}

void test_end(int code)
{
  if(code == KEY_END)
    error("Неожиданный конец файла");
}

void test_num(int code)
{
  test_end(code);
  if(code != KEY_NUM)
    error("Ожидалось число");
}

void dubl(void)
{
  error("Повторное задание параметра");
}

int a_port[] = {0x220, 0x228, 0x2a0, 0x2a8, 0x320, 0x328, 0x3a0, 0x3a8, 0};
int uroven[] = {3, 4, 5, 7, 10, 11, 12, 15, 0};

void subMAGISTRAL(void)
{
  int i, code;

  if(cnt_magistral)
    error("Повторное задание секции");
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
      error("Ожидалось address");
    if(cnt_magistral == MAX_MAGISTRAL)
      error("Слишком много магистралей");
    if(scan() != KEY_NUM)
      error("Ожидалось число");
    if(num) {
      for(i=0; a_port[i]; ++i)
        if(a_port[i] == num)
          goto ok1;
      error("Недопустимый адрес порта подключения устройства КСв-31");
    }

ok1:
    a_m[cnt_magistral].address_port=num;
    if(scan() != KEY_NUM)
      error("Ожидалось число");
    if(num) {
      for(i=0; uroven[i]; ++i)
        if(uroven[i] == num)
          goto ok2;
      error("Недопустимый номер уровня прерывания для подключения устройства КСв-31");
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
    error("Повторное задание секции");
  test_end(code=scan());
  if(code != KEY_WORD)
    error("Синтаксическая ошибка");
  switch(num) {

  case BOOTPATH_m:
    flag_file=1;
    if(scan() != KEY_STR)
      error("Ожидалось имя каталога");
    strcpy(BootPath,lex);
    if(scan() != KEY_WORD || num != ENDSECTION_m)
      error("Ожидалось EndSection");

  case ENDSECTION_m:
    flag_BootPath=1;
  }
}

void subPARAM(void)
{
  int *adr=NULL;
  int code;

  if(flag_param)
    error("Повторное задание секции");
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
      error("Синтаксическая ошибка");
    switch(num) {

    default:
      error("Синтаксическая ошибка");

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
      error("Синтаксическая ошибка");
    }
    if(scan() == KEY_STR)
      switch(num) {

      case YES__m:
      case NO__m:
        *adr=num;
        code=scan();
        continue;
      }
    error("Синтаксическая ошибка");
  }
}

void subKVOTA(void)
{
  int code, i;

  if(flag_kvota)
    error("Повторное задание секции");
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
      error("Синтаксическая ошибка");
    switch(num) {

    default:
      error("Синтаксическая ошибка");

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
    error("Повторное задание секции");
  flag_abonent=1;
  if((code=scan()) == KEY_WORD && num == ENDSECTION_m) {
    vmax_address	= 16;
    vmy_address = 0;
    return;
  }
  for(;;) {
    test_end(code);
    if(code != KEY_WORD)
      error("Синтаксическая ошибка");
    switch(num) {

    default:
      error("Синтаксическая ошибка");

    case ENDSECTION_m:
      return;

    case MAX_ADDRESS_m:
      if(flag_max_address)
        dubl();
      flag_max_address=1;
      test_num(code=scan());
      if(num < 0 || num > 62)
        error("Недопустимое значение параметра");
      vmax_address=num;
      break;

    case MY_ADDRESS_m:
      if(flag_my_address)
        dubl();
      flag_my_address=1;
      test_num(code=scan());
      if(num < 0 || num > vmax_address)
        error("Недопустимое значение параметра");
      vmy_address=num;
    }
    code=scan();
  }
}

void subARBITR(void)
{
  int code;

  if(flag_arbitr)
    error("Повторное задание секции");
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
      error("Синтаксическая ошибка");
    switch(num) {

    default:
      error("Синтаксическая ошибка");

    case ENDSECTION_m:
      return;

    case ARBITR_m:
      if(count_arbitr == MAX_ARBITR)
        error("Слишком много параметров");
      test_end(code=scan());
      if(code != KEY_STR)
        error("Синтаксическая ошибка");
      switch(num) {

      default:
        error("Синтаксическая ошибка");

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
      error("Ожидалось Section");
    test_end(code=scan());
    if(code != KEY_STR)
      error("Ожидалась символьная строка");
    switch(num) {

    default:
      error("Неизвестная секция");

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
