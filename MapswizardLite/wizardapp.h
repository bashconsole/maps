/****************************************************************************
**  wizardapp.h
**  
**   Copyright (c) 2001 Impuls
**
**  Autor: A.N.Zubovsky (azu@imp.lg.ua)
**
*****************************************************************************/

#define Uses_TApplication
#define Uses_TDeskTop
#define Uses_TNSCollection
#define Uses_TButton
#include <tv.h>

/*class TApplication;
class TDeskTop;
class TNSCollection;
class TButton;*/

#include "wizardlg.h"


class TWizardApp : public TApplication
{

public:
	TWizardApp();
	~TWizardApp();

    virtual void handleEvent(TEvent& event);	
    
    void addPage(TWizardDialog*);
    void removePage(TWizardDialog*);
      
    void next();
    void prev();
    virtual void finish();
    void cancel();
    void* getCurPage();
    void showPage(short);
    short getPageCount();
      
    TNSCollection *collection;
    TRect bounds;
    short curindex;
};

