/****************************************************************************
** mapswizard.h
**
** Copyright (C) Impuls
**
** 
** Autor: A.N.Zubovsky (azu@imp.lg.ua)
**
*****************************************************************************/

#ifndef WIZARD_H
#define WIZARD_H

#include <qwizard.h>

#include "dirview.h"

class QWidget;
class QHBox;
class QLineEdit;
class QLabel;
class QComboBox;
class QCheckBox;
class QSpinBox;
class QPushButton;
class QString;
class QPixmap;
class QTextView;

class MapsWizard : public QWizard
{
   Q_OBJECT

public:
   MapsWizard( QWidget *parent = 0, const char *name = 0 );

   QHBox * createPage(int);
   void showPage(QWidget* page);
protected:
   void setupPage0();
   void setupPage1();
   void setupPage2();
   void setupPage3();
   void setupPage4();
   void setupPage5();

   QHBox *WPage[6];
   QLineEdit *bootpath, *savefile;
   QComboBox *address[3], *interrupt[3], *arbiter[3],
             *parameters[4];
   QCheckBox *addr_enabled[3];
   unsigned int addr_flags[3], int_flags[3];
   QSpinBox *max_address, *my_address, *kwtime[5],*kwmes[4], *module[2];
	    
   QString *filename;
   QPixmap *image;
   QFont *font, *boldfont, *smallfont;
   DirectoryView *dw;
   QLabel *finaltext[19];
   QTextView *textview;

   int pageindex;

   void updateAddrComboBoxes();
   void updateIntComboBoxes();
   void changeAddrSelected(int, int);
   void changeIntSelected(int, int);
   void changeArbiterSelected(int, int);
   void toggledAddressCheckBox(bool, int);
   
   void generate(const char*);
   
protected slots:
   void bootPathChanged( const QString & );
   void address1Changed( int );
   void address2Changed( int );
   void address3Changed( int );
   void arbiter1Changed( int );
   void arbiter2Changed( int );
   void arbiter3Changed( int );
   void int1Changed( int );
   void int2Changed( int );
   void int3Changed( int );
   void address1Enabled( bool );
   void address2Enabled( bool );
   void address3Enabled( bool );
   void max_addressChanged( int );
   virtual void accept();
   virtual void help();

};

#endif
