/****************************************************************************
**  createfile.h
**  
**   Copyright (c) 2001 Impuls
**
**  Autor: A.N.Zubovsky (azu@imp.lg.ua)
**
*****************************************************************************/

struct TMapsChannel{
   char address_port[6];
   char interrupt[3];   
};


struct TMapsCfgData{
   char path[256];
   TMapsChannel channel[3];
   char max_address[3];
   char my_address[3];
   char arbiter[3][30];
   char priority[5];
   char t0[5];
   char t[5];
   char n0[5];
   char n1[5];
   char n2[5];
   char tinp[5];
   char tout[5];
   char tinpn[5];
   char toutn[5];
   char maxpoints[5];
   char maxbuf[5];
   char savefilename[256];
   char scriptfilename[256];
};


int createfile(TMapsCfgData &);
