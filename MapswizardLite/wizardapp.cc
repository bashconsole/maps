/****************************************************************************
**  wizardapp.cc
**  
**   Copyright (c) 2001 Impuls
**
**  Autor: A.N.Zubovsky (azu@imp.lg.ua)
**
*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "wizardapp.h"

TWizardApp::TWizardApp() :
	TProgInit( &TWizardApp::initStatusLine,
		       &TWizardApp::initMenuBar,
		       &TWizardApp::initDeskTop
		     )
{
    collection = new TNSCollection(10, 1);
    curindex = -1;
    
    TEvent event;
    event.what = evCommand;
    event.message.command = cmNextPage;
    putEvent(event);
}

void TWizardApp::handleEvent(TEvent& event)
{   
    if( event.what== evCommand )
        switch (event.message.command)
        {
            case  cmNextPage:
	          clearEvent(event);    
	          next();
            break;
	    case  cmPrevPage:
	          clearEvent(event);
	          prev();
            break;
	    case  cmFinishPage:
	          clearEvent(event);
	          finish();
            break;
	    default:
	            TApplication::handleEvent(event);
	}    
    else
        TApplication::handleEvent(event);
}


TWizardApp::~TWizardApp()
{
    collection->removeAll();
}
void TWizardApp::addPage(TWizardDialog *d)
{
    TWizardDialog *page;

    if( getPageCount() > 0){
	page = (TWizardDialog *)collection->at(getPageCount()-1);
	page->setNextButton();
	d->setPrevButton();
    }
    d->setFinishButton();
    
    collection->insert(d);
}
void TWizardApp::removePage(TWizardDialog *d)
{
    collection->remove(d);
}
void TWizardApp::next()
{
    showPage(1);
}
void TWizardApp::prev()
{
    showPage(-1);
}
void TWizardApp::finish()
{
    TEvent event;
    event.what = evCommand;
    event.message.command = cmQuit;
    putEvent(event);
}
void TWizardApp::cancel()
{

}
void* TWizardApp::getCurPage()
{
    return collection->at(curindex);
}
void TWizardApp::showPage(short index)
{
      curindex += index;
      TWizardDialog* d = (TWizardDialog*)collection->at(curindex);
      if(index < 0)
      	    d->setFocus(PREVBTN);
      else
         if(d->nextButton != NULL)
            d->setFocus(NEXTBTN);
         else
            d->setFocus(FINISHBTN); 	
      	
      deskTop->execView(d);
}
short TWizardApp::getPageCount()
{
    return collection->getCount();
}









