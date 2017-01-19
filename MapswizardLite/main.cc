/****************************************************************************
**  main.cpp
**  
**   Copyright (c) 2001 Impuls
**
**  Autor: A.N.Zubovsky (azu@imp.lg.ua)
**
*****************************************************************************/

#include "mapsconf.h"
#include "param.h"
#include "setupdefaults.h"

#include <stdio.h>
#include <stdlib.h>

#define Uses_TApplication
#define Uses_TDeskTop
#define Uses_TNSCollection
#define Uses_TButton
#include <tv.h>

#include "global.h"
#include "wizardapp.h"
#include "dirdlg.h"

// tv.h will pull in the headers needed for TApplication and its
// base classes. Add a #define Uses_Txxx statement (before the
// #include <tv.h> line) for each Turbo Vision class used in
// your program. Explicit or implied duplications are harmless:
// for example, #define Uses_TProgram would be redundant here,
// but harmless.
TApplication *app;

class TMapsWizardApp : public TWizardApp
{

public:
	TMapsWizardApp();
	~TMapsWizardApp();

};

TMapsWizardApp::TMapsWizardApp() :
	TProgInit( &TMapsWizardApp::initStatusLine,
		       &TMapsWizardApp::initMenuBar,
		       &TMapsWizardApp::initDeskTop
		     )
{
    app = this; 
    
    TRect r(1,5,80,23);
    TWizardDialog *d0 = new TChDirWizardDlg(0, r, _("Дерево"));
    addPage(d0);
    TWizardDialog *d1 = new TWizardDialog(r, _("TEST1"));
    addPage(d1);
    TWizardDialog *d2 = new TWizardDialog(r, _("TEST2"));
    addPage(d2);
    TWizardDialog *d3 = new TWizardDialog(r, _("TEST3"));
    addPage(d3);
    TWizardDialog *d4 = new TWizardDialog(r, _("TEST4"));
    addPage(d4);
    TWizardDialog *d5 = new TWizardDialog(r, _("TEST5"));
    addPage(d5);

    //disableCommand(cmNextPage);
}

TMapsWizardApp::~TMapsWizardApp()
{



}


int main()
{
	TMapsWizardApp wizardApp;
	wizardApp.run();
	return 0;
}



