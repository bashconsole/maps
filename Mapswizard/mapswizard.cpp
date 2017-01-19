/****************************************************************************
**  mapswizard.cpp
**  
**   Copyright (c) 2001 Impuls
**
**  Autor: A.N.Zubovsky (azu@imp.lg.ua)
**
*****************************************************************************/

#define def_generate_filename                  "confmaps"
#define def_generate_scriptfilename            "init.sh"
#define def_help_pathname                      "./help"
#define def_help_filename                      "mwhelp_ru.html"
#define def_picture_filename                   "maps.jpg"
#define def_picture_format                     "JPEG"

#define def_page1_title                        "Выбор пути к файлам для загрузки КСв-31"
#define def_channel_str                        "Магистраль"
#define def_enabled_str                        "Доступна"
#define def_port_addr_str                      "Адрес порта"
#define def_interrupt_str                      "Уровень прерывания"
#define def_dir_browser_str                    "Выбор директории"
#define def_page2_title                        "Конфигурирование магистралей сети МАПС"
#define def_max_address_title                  "Макс адрес абонента в сети"
#define def_my_address_title                   "Адрес ПЭВМ в сети"
#define def_page3_abonent_section_title        "Как абонента"
#define def_page3_arbiter_section_title        "Режим функционирования"
#define def_page3_channel_title                "Магистраль"
#define def_page3_title                        "Абоненты и режимы функционирования КСв-31"
#define def_page4_title                        "Конфигурирование параметров функционирования КСв-31"
#define def_page4_parameters_box_title         "Параметры"
#define def_page4_kwtime_box_title             "Квоты параметров функционирования"
#define def_page4_timer_str                    "Источник точного времени"
#define def_page4_dynamic_str                  "Динам. подкл. абонентов"
#define def_page4_priority_str                 "Приоритетность ПЭВМ"
#define def_page4_timing_str                   "Синхронизация"
#define def_page4_t0_str                       "Время цикла опроса (дес.мс)"
#define def_page4_t_str                        "Время ожидания ответа (мс)"
#define def_page4_n0_str                       "Квота выд. приорит. абон."
#define def_page4_n1_str                       "Квота выд. при исчерп. цикла"
#define def_page4_n2_str                       "Квота выд. не приорит. абон."
#define def_page5_title                        "Параметры приема/выдачи сообщения"
#define def_page5_kwmes_box_title              "Квоты"
#define def_page5_tinp_str                     "Приема без подтверждения"
#define def_page5_tout_str                     "Приема с подтверждением"
#define def_page5_tinpn_str                    "Выдачи без подтверждения"
#define def_page5_toutn_str                    "Выдачи с подтверждением"
#define def_page5_module_box_title             "Определение параметров модуля"
#define def_page5_filename_box_title           "Имя файла конфигурации"
#define def_page6_title                        "Генерация файла конфигурирования МАПС"
#define def_page6_box_title                    "Введенные параметры"

#define NULL 0

#include <qwidget.h>
#include <qhbox.h>
#include <qvbox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qvalidator.h>
#include <qapplication.h>
#include <qfileinfo.h>
#include <qdir.h>
#include <qgroupbox.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qlayout.h>
#include <qspinbox.h>
#include <qstring.h>
#include <qpushbutton.h>
#include <qpixmap.h>
#include <qmessagebox.h>
#include <qfont.h>
#include <stdio.h>
#include <qtextview.h>
#include <qtextbrowser.h>

#include "mapswizard.h"
#include "createfile.h"
#include "param.h"


const int addr_port_count = 8;
const int addr_int_count  = 8;

const char *addr_port[] = {
   QT_TRANSLATE_NOOP("MapsWizard","0x220"),
   QT_TRANSLATE_NOOP("MapsWizard","0x228"),
   QT_TRANSLATE_NOOP("MapsWizard","0x2a0"),
   QT_TRANSLATE_NOOP("MapsWizard","0x2a8"),
   QT_TRANSLATE_NOOP("MapsWizard","0x320"),
   QT_TRANSLATE_NOOP("MapsWizard","0x328"),
   QT_TRANSLATE_NOOP("MapsWizard","0x3a0"),
   QT_TRANSLATE_NOOP("MapsWizard","0x3a8")
};

const char *interrup[] = {
   QT_TRANSLATE_NOOP("MapsWizard","3"),
   QT_TRANSLATE_NOOP("MapsWizard","4"),
   QT_TRANSLATE_NOOP("MapsWizard","5"),
   QT_TRANSLATE_NOOP("MapsWizard","7"),
   QT_TRANSLATE_NOOP("MapsWizard","10"),
   QT_TRANSLATE_NOOP("MapsWizard","11"),
   QT_TRANSLATE_NOOP("MapsWizard","12"),
   QT_TRANSLATE_NOOP("MapsWizard","15")
};

const int arbiters_count = 3;

const char *arbiters[] = {
   QT_TRANSLATE_NOOP("MapsWizard","не арбитр"),
   QT_TRANSLATE_NOOP("MapsWizard","единственный арбитр"),
   QT_TRANSLATE_NOOP("MapsWizard","резервный арбитр магистрали")
   //QT_TRANSLATE_NOOP("MapsWizard","резервный арбитр сети")
};

const char *yes_no[] = {
   "нет",
   "да"
};

const int timings_count = 3;

const char *timings[] = {
   QT_TRANSLATE_NOOP("MapsWizard","не синхронизируется"),
   QT_TRANSLATE_NOOP("MapsWizard","источник"),
   QT_TRANSLATE_NOOP("MapsWizard","приемник")
};

const int speeds_count = 6;

const char *speeds[] = {
   QT_TRANSLATE_NOOP("MapsWizard","1000"),
   QT_TRANSLATE_NOOP("MapsWizard","500"),
   QT_TRANSLATE_NOOP("MapsWizard","250"),
   QT_TRANSLATE_NOOP("MapsWizard","125"),
   QT_TRANSLATE_NOOP("MapsWizard","62"),
   QT_TRANSLATE_NOOP("MapsWizard","5")
};

MapsWizard::MapsWizard( QWidget *parent, const char *name )
    : QWizard( parent, name, TRUE )
{
   setMaximumSize( 558, 451 );

   font = new QFont("times", 10, QFont::Normal, FALSE);
   boldfont = new QFont("times", 12, QFont::Bold, FALSE);
   smallfont = new QFont("times", 8, QFont::Normal, FALSE);
   if( font == NULL ){
      font = new QFont("courier", 10, QFont::Normal, FALSE);
      boldfont = new QFont("courier", 12, QFont::Bold, FALSE);
      smallfont = new QFont("courier", 8, QFont::Normal, FALSE);

   }
      	
   setFont(*boldfont);

   QString filename(def_picture_filename);
   image = new QPixmap(filename, def_picture_format);

   setupPage0();
   setupPage1();
   setupPage2();
   setupPage3();
   setupPage4();
   setupPage5();

}

void MapsWizard::setupPage0()
{
   QHBox* Page = createPage(0);

   Page->setMinimumSize( 546, 367 );

   QVBox *page = new QVBox( Page );
   page->setFont(*font);
         
   dw = new DirectoryView( page, 0, TRUE );

   dw->addColumn( tr("Name") );
   dw->addColumn( tr("Type") );
   dw->setTreeStepSize( 10 );

   const QFileInfoList* roots = QDir::drives();
   QListIterator<QFileInfo> i(*roots);
   QFileInfo* fi;
   while ( (fi = *i) ) {
	++i;
	Directory * root = new Directory( dw, fi->filePath() );
	if ( roots->count() <= 1 )
	    root->setOpen( TRUE ); // be interesting
   }

   dw->resize( 400, 400 );
   dw->setCaption( QString::fromLocal8Bit( tr(def_dir_browser_str) ) );
   dw->setAllColumnsShowFocus( TRUE );

   bootpath = new QLineEdit( page );
   bootpath->setMaxLength( 255 );

   connect( bootpath, SIGNAL( textChanged( const QString & ) ), this, SLOT( bootPathChanged( const QString & ) ) );
   connect( dw, SIGNAL( folderSelected( const QString & )), bootpath, SLOT( setText( const QString & )));

   bootpath->setText(BootPath);

   addPage( Page, QString::fromLocal8Bit( tr(def_page1_title) ) );
   
   setNextEnabled( Page, FALSE );
   setHelpEnabled( Page, TRUE );
}

void MapsWizard::setupPage1()
{
   QHBox* Page = createPage(1);

   QVBox *page = new QVBox( Page );
   page->setFont(*font);
   
   QGroupBox *groupbox[3];
   QVBoxLayout *vbox[3];
   QHBoxLayout *row[3];
   QLabel *label;
   
   int i, j, n = 2;

   QString str = QString::fromLocal8Bit( tr(def_channel_str) );

   for(i = 0; i < 3; i++){
      groupbox[i] = new QGroupBox( 0, Horizontal, tr("%1 %2").arg(str).arg(i+1), page );
      groupbox[i]->setMargin( 10 );
      
      vbox[i] = new QVBoxLayout(groupbox[i]->layout());
      
      //if (i > 0) n = 3;
      
      for(j = 0; j < 3; j++){
         row[j] = new QHBoxLayout(vbox[i]);
         row[j]->setMargin( 5 );
      }
            
      label = new QLabel( QString::fromLocal8Bit( tr(def_port_addr_str) ), groupbox[i] );
      row[0]->addWidget(label);
      address[i] = new QComboBox(FALSE, groupbox[i]);
      row[0]->addWidget(address[i]);


      addr_enabled[i] = new QCheckBox(QString::fromLocal8Bit( tr(def_enabled_str) ), groupbox[i]);
      row[2]->addWidget( addr_enabled[i] );
      addr_enabled[i]->setChecked(TRUE);

      if(a_m[i].address_port == 0)
          addr_flags[i] = 0;
      else{
          addr_flags[i] = 1;
          for(j=0;j<8;j++){
              if(ap[j] == a_m[i].address_port) break;
              addr_flags[i] <<= 1;
          }
      }

      label = new QLabel( QString::fromLocal8Bit( tr(def_interrupt_str) ), groupbox[i] );
      row[1]->addWidget(label);
      interrupt[i] = new QComboBox(FALSE, groupbox[i]);
      row[1]->addWidget(interrupt[i]);

      if(a_m[i].num_interrupt == 0)
         int_flags[i] = 0;
      else{
         int_flags[i] = 1;
         for(j=0;j<8;j++){
            if(atoi(interrup[j]) == a_m[i].num_interrupt) break;
            int_flags[i] <<= 1;
         }
      }

//      if( i > 0 ){
//	 addr_enabled[i - 1] = new QCheckBox(QString::fromLocal8Bit( tr(def_enabled_str) ), groupbox[i]);
//         row[2]->addWidget( addr_enabled[i - 1] );
//	 addr_enabled[i - 1]->setChecked(TRUE);
//      }
	
   }

   updateAddrComboBoxes();
   updateIntComboBoxes();

   connect( address[0], SIGNAL( activated( int ) ), this, SLOT( address1Changed( int ) ) );
   connect( address[1], SIGNAL( activated( int ) ), this, SLOT( address2Changed( int ) ) );
   connect( address[2], SIGNAL( activated( int ) ), this, SLOT( address3Changed( int ) ) );
   connect( interrupt[0], SIGNAL( activated( int ) ), this, SLOT( int1Changed( int ) ) );
   connect( interrupt[1], SIGNAL( activated( int ) ), this, SLOT( int2Changed( int ) ) );
   connect( interrupt[2], SIGNAL( activated( int ) ), this, SLOT( int3Changed( int ) ) );
   connect( addr_enabled[0], SIGNAL( toggled( bool ) ), this, SLOT( address1Enabled( bool ) ) );
   connect( addr_enabled[1], SIGNAL( toggled( bool ) ), this, SLOT( address2Enabled( bool ) ) );
   connect( addr_enabled[2], SIGNAL( toggled( bool ) ), this, SLOT( address3Enabled( bool ) ) );

   addPage( Page, QString::fromLocal8Bit( tr(def_page2_title) ) );

   setNextEnabled( Page, TRUE );
   setHelpEnabled( Page, TRUE );
}

void MapsWizard::setupPage2()
{
   QHBox* Page = createPage(2);

   QVBox *page = new QVBox( Page );
   page->setFont(*font);

   QString groupboxtitle[2];

   groupboxtitle[0] = QString::fromLocal8Bit( tr(def_page3_abonent_section_title) );
   groupboxtitle[1] = QString::fromLocal8Bit( tr(def_page3_arbiter_section_title) );

   int i;

   QGroupBox *groupbox[2];

   for( i = 0; i < 2; i++){
       groupbox[i] = new QGroupBox( 0, Horizontal, groupboxtitle[i], page );
       groupbox[i]->setMargin( 10 );
   }

   QVBoxLayout *vbox[2];

   for ( i = 0; i < 2; i++)
       vbox[i] = new QVBoxLayout(groupbox[i]->layout());

   //first Group Box

   QHBoxLayout *row1[2];

   for( i = 0; i < 2; i++){
       row1[i] = new QHBoxLayout(vbox[0]);
       row1[i]->setMargin( 5 );
   }

   QLabel *label;

   label = new QLabel( QString::fromLocal8Bit( tr(def_max_address_title) ), groupbox[0] );
   row1[0]->addWidget(label);
   max_address = new QSpinBox( 0, 62, 1,  groupbox[0] );
   max_address->setValue(vmax_address);
   row1[0]->addWidget(this->max_address);
   
   label = new QLabel( QString::fromLocal8Bit( tr(def_my_address_title) ), groupbox[0] );
   row1[1]->addWidget(label);
   my_address = new QSpinBox( 0, 62, 1, groupbox[0] );
   my_address->setValue(vmy_address);
   row1[1]->addWidget(my_address);
   
   //second GroupBox

   QHBoxLayout *row;

   QString str = QString::fromLocal8Bit( tr(def_page3_channel_title) );

   for (int i = 0; i < 3;i++){
      row = new QHBoxLayout(vbox[1]);
      row->setMargin( 5 );

      label = new QLabel(tr("%1 %2").arg(str).arg(i+1), groupbox[1]);
      row->addWidget(label);
      
      arbiter[i] = new QComboBox(FALSE, groupbox[1]);
      row->addWidget(arbiter[i]);

      for( int j = 0; j < arbiters_count; j++)
	 arbiter[i]->insertItem( QString::fromLocal8Bit(arbiters[j]) );
       arbiter[i]->setCurrentItem(arbitr[i]);
   }

   connect( max_address, SIGNAL( valueChanged( int ) ), this, SLOT( max_addressChanged( int ) ) );
   connect( arbiter[0], SIGNAL( activated( int ) ), this, SLOT( arbiter1Changed( int ) ) );
   connect( arbiter[1], SIGNAL( activated( int ) ), this, SLOT( arbiter2Changed( int ) ) );
   connect( arbiter[2], SIGNAL( activated( int ) ), this, SLOT( arbiter3Changed( int ) ) );


   addPage( Page, QString::fromLocal8Bit( tr(def_page3_title) ) );
   
   setNextEnabled( Page, TRUE );
   setHelpEnabled( Page, TRUE );

   for(i=0;i<3;i++)
      if(a_m[i].address_port == 0)
         addr_enabled[i]->setChecked(FALSE);
}

void MapsWizard::setupPage3()
{
   QHBox* Page = createPage(3);

   QVBox *page = new QVBox( Page );
   page->setFont(*font);

   int i, j;

   QString groupboxtitle[2];

   groupboxtitle[0] = QString::fromLocal8Bit( tr(def_page4_parameters_box_title) );
   groupboxtitle[1] = QString::fromLocal8Bit( tr(def_page4_kwtime_box_title) );

   QGroupBox *groupbox[2];

   for( i = 0; i < 2; i++){
       groupbox[i] = new QGroupBox( 0, Horizontal, groupboxtitle[i], page );
       groupbox[i]->setMargin( 10 );
   }

   QVBoxLayout *vbox[2];

   for ( i = 0; i < 2; i++)
       vbox[i] = new QVBoxLayout(groupbox[i]->layout());

    //first Group Box
   QHBoxLayout *row1[4];

   for( i = 0; i < 1; i++){
       row1[i] = new QHBoxLayout(vbox[0]);
       row1[i]->setMargin( 5 );
   }

   QLabel *label;

   QString title1[4];
//   title1[0] = QString::fromLocal8Bit( tr(def_page4_timer_str) );
//   title1[1] = QString::fromLocal8Bit( tr(def_page4_dynamic_str) );
   title1[0] = QString::fromLocal8Bit( tr(def_page4_priority_str) );
//   title1[3] = QString::fromLocal8Bit( tr(def_page4_timing_str) );

   label = new QLabel( title1[0], groupbox[0] );
   row1[0]->addWidget(label);
   parameters[0] = new QComboBox( FALSE,  groupbox[0] );
   row1[0]->addWidget(parameters[0]);
   for (j = 0; j < 2; j++)
       parameters[0]->insertItem(QString::fromLocal8Bit( yes_no[j] ));
   parameters[0]->setCurrentItem(priority);


   //second GroupBox

   QString title2[5];
   title2[0] = QString::fromLocal8Bit( tr(def_page4_t0_str) );
   title2[1] = QString::fromLocal8Bit( tr(def_page4_t_str) );
   title2[2] = QString::fromLocal8Bit( tr(def_page4_n0_str) );
   title2[3] = QString::fromLocal8Bit( tr(def_page4_n1_str) );
   title2[4] = QString::fromLocal8Bit( tr(def_page4_n2_str) );

   QHBoxLayout *row2[5];

   for( i = 0; i < 5; i++){
       row2[i] = new QHBoxLayout(vbox[1]);
       row2[i]->setMargin( 5 );
       label = new QLabel( title2[i], groupbox[1] );
       row2[i]->addWidget(label);
       kwtime[i] = new QSpinBox(0, 1000, 1, groupbox[1]);
       row2[i]->addWidget(kwtime[i]);
   }

   kwtime[0]->setValue(kwtime_t0);
   kwtime[1]->setValue(kwtime_t);
   kwtime[2]->setValue(kwtime_n0);
   kwtime[3]->setValue(kwtime_n1);
   kwtime[4]->setValue(kwtime_n2);

   addPage( Page, QString::fromLocal8Bit( tr(def_page4_title) ) );

   setNextEnabled( Page, TRUE );
   setHelpEnabled( Page, TRUE );
}

void MapsWizard::setupPage4()
{
   QHBox* Page = createPage(4);

   QVBox *page = new QVBox( Page );
   page->setFont(*font);


   QString groupboxtitle[3];

   groupboxtitle[0] = QString::fromLocal8Bit( tr(def_page5_kwmes_box_title) );
   groupboxtitle[1] = QString::fromLocal8Bit( tr(def_page5_module_box_title) );
   groupboxtitle[2] = QString::fromLocal8Bit( tr(def_page5_filename_box_title) );

   int i;

   QGroupBox *groupbox[3];

   for( i = 0; i < 3; i++){
       groupbox[i] = new QGroupBox( 0, Horizontal, groupboxtitle[i], page );
       groupbox[i]->setMargin( 10 );
   }

   QVBoxLayout *vbox[3];

   for ( i = 0; i < 3; i++)
       vbox[i] = new QVBoxLayout(groupbox[i]->layout());

   QHBoxLayout *row1[4];

   for( i = 0; i < 4; i++){
       row1[i] = new QHBoxLayout(vbox[0]);
       row1[i]->setMargin( 5 );
   }

   QLabel *label;

   QString title[4];
   title[0] = QString::fromLocal8Bit( tr(def_page5_tinp_str) );
   title[1] = QString::fromLocal8Bit( tr(def_page5_tout_str) );
   title[2] = QString::fromLocal8Bit( tr(def_page5_tinpn_str) );
   title[3] = QString::fromLocal8Bit( tr(def_page5_toutn_str) );

   for( i = 0; i < 4; i++){
       label = new QLabel( title[i], groupbox[0] );
       row1[i]->addWidget(label);
       kwmes[i] = new QSpinBox(0, 1000, 1, groupbox[0]);
       row1[i]->addWidget(kwmes[i]);
   }

   kwmes[0]->setValue(kwmes_tinp);
   kwmes[1]->setValue(kwmes_tout);
   kwmes[2]->setValue(kwmes_tinpn);
   kwmes[3]->setValue(kwmes_toutn);


   for( i = 0; i < 2; i++){
       row1[i] = new QHBoxLayout(vbox[1]);
       row1[i]->setMargin( 5 );
   }

   title[0] = QString::fromLocal8Bit( tr("Макс. кол-во точек") );
   title[1] = QString::fromLocal8Bit( tr("Макс. кол-во буферов") );

   for( i = 0; i < 2;i++){
       label = new QLabel( title[i], groupbox[1] );
       row1[i]->addWidget(label);
       module[i] = new QSpinBox(1, 1000, 1, groupbox[1]);
       module[i]->setValue(10);
       row1[i]->addWidget(module[i]);
   }

   row1[0] = new QHBoxLayout( vbox[2] );
   savefile = new QLineEdit( groupbox[2] );
   row1[0]->addWidget(savefile);

   savefile->setText(config_filename);

   addPage( Page, QString::fromLocal8Bit( tr(def_page5_title) ) );
   
   setNextEnabled( Page, TRUE );
   setHelpEnabled( Page, TRUE );
}

void MapsWizard::setupPage5()
{
   QHBox* Page = createPage(5);

   QGroupBox *groupbox = new QGroupBox( 0, Horizontal, QString::fromLocal8Bit( tr(def_page6_box_title) ), Page );
   groupbox->setMargin( 10 );
   groupbox->setFont(*font);

   QHBoxLayout *hbox;
   hbox = new QHBoxLayout(groupbox->layout());

   textview = new QTextView(groupbox);
   hbox->addWidget(textview);

   addPage( Page, QString::fromLocal8Bit( tr(def_page6_title) ) );
   
   setFinishEnabled( Page, TRUE );
   setHelpEnabled( Page, TRUE );
}

void MapsWizard::showPage( QWidget* page )
{
   if (page == WPage[0]){
      bootPathChanged(bootpath->text());
      pageindex = 0;
   }
   if (page == WPage[1])
      pageindex = 1;
   else
   if (page == WPage[2])
      pageindex = 2;
   else
   if (page == WPage[3])
      pageindex = 3;
   else
   if (page == WPage[4])
      pageindex = 4;
   else
   if ( page == WPage[5] ) {
         QString s;
         bool f = 0;
         for(int i =0; i < 3; i++)
            if(address[i]->isEnabled())
                if(!f){
                   s += tr(" %1 %2").arg(address[i]->text(address[i]->currentItem()))
                                   .arg(interrupt[i]->text(interrupt[i]->currentItem()));
                   f = !f;
                }
                else
                   s += tr(", %1 %2").arg(address[i]->text(address[i]->currentItem()))
                                    .arg(interrupt[i]->text(interrupt[i]->currentItem()));


	textview->setText(
	  tr("%1\n%2\n").arg(QString::fromLocal8Bit(tr("Путь к каталогу с файлами для загрузки КСв-31:")))
	                       .arg(bootpath->text())
	+ tr("%1 %2\n").arg(QString::fromLocal8Bit(tr("Магистрали:")))
	                      .arg(s)
	+ tr("%1 %2\n").arg(QString::fromLocal8Bit(tr("Максимальный адрес подключения абонента в сеть:")))
	                      .arg(max_address->text())
	+ tr("%1 %2\n").arg(QString::fromLocal8Bit(tr("Адрес подключения данной ПЭВМ в сеть:")))
	                      .arg(my_address->text())
	+ tr("%1 %2, %3, %4\n").arg(QString::fromLocal8Bit(tr("Арбитры:")))
	                      .arg(arbiter[0]->text(arbiter[0]->currentItem()))
	                      .arg(arbiter[1]->text(arbiter[1]->currentItem()))
	                      .arg(arbiter[2]->text(arbiter[2]->currentItem()))
	+ tr("%1 %2\n").arg(QString::fromLocal8Bit(tr("Приоритетность ПЭВМ как абонента:")))
	                      .arg(parameters[0]->text(parameters[0]->currentItem()))
	+ tr("%1 %2\n").arg(QString::fromLocal8Bit(tr("Время цикла опроса сообщения:")))
	                      .arg(kwtime[0]->text())
	+ tr("%1 %2\n").arg(QString::fromLocal8Bit(tr("Время ожидания ответа:")))
	                      .arg(kwtime[1]->text())
	+ tr("%1 %2\n").arg(QString::fromLocal8Bit(tr("Квота выдачи сообщения для приоритетных абонентов:")))
	                      .arg(kwtime[2]->text())
	+ tr("%1 %2\n").arg(QString::fromLocal8Bit(tr("Квота выдачи сообщения для приоритетных абонентов при исчерпании длительного цикла:")))
	                      .arg(kwtime[3]->text())
	+ tr("%1 %2\n").arg(QString::fromLocal8Bit(tr("Квота выдачи сообщения для неприоритетных абонентов:")))
	                      .arg(kwtime[4]->text())
	+ tr("%1 %2\n").arg(QString::fromLocal8Bit(tr("Квота приема сообщения без подтверждения:")))
	                      .arg(kwmes[0]->text())
	+ tr("%1 %2\n").arg(QString::fromLocal8Bit(tr("Квота приема сообщения с подтверждением:")))
	                      .arg(kwmes[1]->text())
	+ tr("%1 %2\n").arg(QString::fromLocal8Bit(tr("Квота выдачи сообщения без подтверждения:")))
	                      .arg(kwmes[2]->text())
	+ tr("%1 %2\n").arg(QString::fromLocal8Bit(tr("Квота выдачи сообщения с подтверждением:")))
                               .arg(kwmes[3]->text())
         + tr("%1 %2\n").arg(QString::fromLocal8Bit(tr("Максимальное количество точек:")))
                               .arg(module[0]->text())
         + tr("%1 %2\n").arg(QString::fromLocal8Bit(tr("Максимальное количество буферов:")))
                               .arg(module[1]->text())
         + tr("%1 %2\n").arg(QString::fromLocal8Bit(tr("Имя файла конфигурации:")))
                               .arg(savefile->text())
         );
       pageindex = 5;
    }
    QWizard::showPage(page);
}

void MapsWizard::bootPathChanged( const QString & path )
{
   if( !bootpath->text().isEmpty()){
      dw->setDir(bootpath->text());
      QDir d( path, "*.hex" );
      if(d.count() != 0)
          nextButton()->setEnabled( TRUE );
      else
          nextButton()->setEnabled( FALSE );
   }
   else{
      nextButton()->setEnabled( FALSE );
   }
   
}

void MapsWizard::address1Changed(int index)
{
   changeAddrSelected(index, 0);
}

void MapsWizard::address2Changed(int index)
{
   changeAddrSelected(index, 1);    
}

void MapsWizard::address3Changed(int index)
{
   changeAddrSelected(index, 2);   
}

void MapsWizard::changeAddrSelected(int index, int i)
{
   
   unsigned int mask, test;
   int n;
   mask = 0;
   for(int j = 0; j < 3; j++)
	 if ( j != i) mask = mask | addr_flags[j];
   
   addr_flags[i] = 0;
   test = 1;
   n = 0;
   while (n <= index){
     if((mask & test) == 0) n++;
     test = test << 1;
   }
   test = test >> 1;
   addr_flags[i] = addr_flags[i] | test;
   
   updateAddrComboBoxes();
   
}

void MapsWizard::updateAddrComboBoxes()
{
   int i,j;
   unsigned int mask, test;
   QString str;
   
   for( i = 0; i < 3;i++){
      mask = 0;
      for(j = 0; j < 3; j++)
	 if ( j != i) mask |= addr_flags[j];
      address[i]->clear();
      test = 1;
      for(j = 0; j < addr_port_count; j++){
	 if(!(mask & test)||(addr_flags[i] & test)){
	    str = QString::fromLocal8Bit(addr_port[j]);
	    address[i]->insertItem(str);
	    if (addr_flags[i] & test)
	       address[i]->setCurrentItem(address[i]->count() - 1);
	 }
         test = test << 1;
      }
   }
}

void MapsWizard::int1Changed(int index)
{
   changeIntSelected(index, 0);
}

void MapsWizard::int2Changed(int index)
{
   changeIntSelected(index, 1);    
}

void MapsWizard::int3Changed(int index)
{
   changeIntSelected(index, 2);   
}

void MapsWizard::changeIntSelected(int index, int i){
   
   unsigned int mask, test;
   int n;
   mask = 0;
   for(int j = 0; j < 3; j++)
	 if ( j != i) mask = mask | int_flags[j];
   
   int_flags[i] = 0;
   test = 1;
   n = 0;
   while (n <= index){
     if((mask & test) == 0) n++;
     test = test << 1;
   }
   test = test >> 1;
   int_flags[i] = int_flags[i] | test;
   
   updateIntComboBoxes();
}

void MapsWizard::updateIntComboBoxes()
{
   int i,j;
   unsigned int mask, test;
   QString str;
   
   for( i = 0; i < 3;i++){
      mask = 0;
      for(j = 0; j < 3; j++)
	 if ( j != i) mask |= int_flags[j];
      interrupt[i]->clear();
      test = 1;
      for(j = 0; j < addr_port_count; j++){
	 if(!(mask & test)||(int_flags[i] & test)){
	    str = QString::fromLocal8Bit(interrup[j]);
	    interrupt[i]->insertItem(str);
	    if (int_flags[i] & test)
	       interrupt[i]->setCurrentItem(interrupt[i]->count() - 1);
	 }
         test = test << 1;
      }
   }
}

void MapsWizard::address1Enabled( bool isChecked)
{
   toggledAddressCheckBox(isChecked, 0);
}

void MapsWizard::address2Enabled( bool isChecked)
{
   toggledAddressCheckBox(isChecked, 1);
}

void MapsWizard::address3Enabled( bool isChecked)
{
   toggledAddressCheckBox(isChecked, 2);
}

void MapsWizard::toggledAddressCheckBox(bool isChecked, int index)
{
   address[index]->setEnabled(isChecked);
   interrupt[index]->setEnabled(isChecked);
   arbiter[index]->setEnabled(isChecked);
   if(isChecked){
      changeAddrSelected(1, index);
      changeIntSelected(1, index);
      changeArbiterSelected(arbiter[0]->currentItem(), 0);
   }
   else{
      addr_flags[index] = 0;
      int_flags[index] = 0;
      updateAddrComboBoxes();
      updateIntComboBoxes();
      arbiter[index]->setCurrentItem(0);
   }
}

void MapsWizard::arbiter1Changed(int index)
{
   changeArbiterSelected(index, 0);
}

void MapsWizard::arbiter2Changed(int index)
{
   changeArbiterSelected(index, 1);
}

void MapsWizard::arbiter3Changed(int index)
{
   changeArbiterSelected(index, 2);
}

void MapsWizard::changeArbiterSelected(int index, int i)
{
 //     int j;
 //     if(index == (arbiter[i]->count() - 1)){
 //         for(j = 0; j < 3; j++)
 //            if(arbiter[j]->isEnabled() && i != j)
 //               arbiter[j]->setCurrentItem(arbiter[j]->count() - 1);
 //     }
 //     else
 //        for(j = 0; j < 3; j++)
 //           if(arbiter[j]->currentItem() == arbiter[i]->count() - 1)
 //              arbiter[j]->setCurrentItem(0);
}


void MapsWizard::max_addressChanged( int value)
{
     if(my_address->value() > value)
        my_address->setValue( value );
     my_address->setMaxValue( value );
}

void MapsWizard::generate(const char* filename)
{

   int i;	
   TMapsCfgData d;

   strcpy(d.path,bootpath->text().local8Bit());
   for(i=0; i<3; i++){
      if( address[i]->isEnabled() ){
           strcpy(d.channel[i].address_port, address[i]->text(address[i]->currentItem()).local8Bit());
           strcpy(d.channel[i].interrupt, interrupt[i]->text(interrupt[i]->currentItem()).local8Bit());
      }
      else{
           strcpy(d.channel[i].address_port, tr("0").local8Bit());
           strcpy(d.channel[i].interrupt, tr("0").local8Bit());
      }
   }
   strcpy(d.max_address, max_address->text());
   strcpy(d.my_address, my_address->text());
   for(i = 0; i < 3; i++)
       strcpy(d.arbiter[i], arbiter[i]->text(arbiter[i]->currentItem()).local8Bit());
   strcpy(d.priority, parameters[0]->text(parameters[0]->currentItem()).local8Bit());
   strcpy(d.t0, kwtime[0]->text());
   strcpy(d.t, kwtime[1]->text());
   strcpy(d.n0, kwtime[2]->text());
   strcpy(d.n1, kwtime[3]->text());
   strcpy(d.n2, kwtime[4]->text());
   strcpy(d.tinp, kwmes[0]->text());
   strcpy(d.tout, kwmes[1]->text());
   strcpy(d.tinpn, kwmes[2]->text());
   strcpy(d.toutn, kwmes[3]->text());
   strcpy(d.maxpoints, module[0]->text());
   strcpy(d.maxbuf, module[1]->text());
   sprintf(d.savefilename, "%s", filename);
   sprintf(d.scriptfilename,"%s", def_generate_scriptfilename);

   if(!createfile(d)){
       QString str;
       str = QString::fromLocal8Bit( tr("Error: Open file\n%1 error.").arg(filename) );
       QMessageBox::information( this, QString::fromLocal8Bit( tr("MAPS Net Configurator") ),
                    str, "Ok", 0, 0, 0, 1 );
   }
}

QHBox* MapsWizard::createPage(int pageindex)
{
   WPage[pageindex] = new QHBox( this );
   WPage[pageindex]->setSpacing(8);

   QLabel *info = new QLabel( WPage[pageindex] );
   info->setPixmap(*image);
   info->setIndent( 8 );
   info->setMaximumWidth( info->sizeHint().width() );

   return WPage[pageindex];

}

void MapsWizard::accept()
{
   char *filename = new char[256];
   strcpy(filename, savefile->text());
   generate(filename);
   delete filename;

   QWizard::accept();
}

void MapsWizard::help()
{
    QDialog *helpdialog = new QDialog(this, "Help", TRUE);
    QTextBrowser *browser = new QTextBrowser( helpdialog );
    browser->mimeSourceFactory()->setFilePath(tr(def_help_pathname));
    browser->mimeSourceFactory()->setExtensionType("html",  "text/html;charset=KOI8-r");
    browser->setSource( tr(def_help_filename) );

    QString str(tr("%1%2").arg("page").arg(pageindex));
    browser->scrollToAnchor( str );

    browser->resize(600, 500);
    helpdialog->exec();
}
