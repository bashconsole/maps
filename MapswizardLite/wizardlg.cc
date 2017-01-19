#include <math.h>

#define Uses_TDialog
#define Uses_TButton
#define Uses_TEvent

#define BUTTONLEN 14

#include <tv.h>

#include "global.h"
#include "wizardlg.h"

TWizardDialog::TWizardDialog( const TRect& bounds, const char *aTitle, short place):
               TDialog( bounds, aTitle ),TWindowInit( &TWizardDialog::initFrame )
{
     growMode = 0;
     //flags = wfMove | wfClose;
     options |= ofCentered;
     prevButton = 0;
     nextButton = 0;
     finishButton = 0;
}
TWizardDialog::~TWizardDialog()
{

}
void TWizardDialog::handleEvent(TEvent& event)
{   
    if( event.what== evCommand )
        switch (event.message.command)
        {
            case  cmCancel:
	                TDialog::handleEvent(event);
                         event.what = evCommand;
                         event.message.command = cmQuit;
			putEvent(event);
            break;
	    case  cmPrevPage:
	    case  cmNextPage:
	    case  cmFinishPage:
	                TEvent dialogevent;
	                dialogevent.what = evCommand;
                         dialogevent.message.command = cmCancel;
	                TDialog::handleEvent(dialogevent);
			putEvent(event);
            break;
	    default:
	            TDialog::handleEvent(event);
	}    
    else
        TDialog::handleEvent(event);
}
void TWizardDialog::setPrevButton()
{
    if(prevButton != 0){
       remove(prevButton);
       prevButton = 0;
    }

    TRect b = getBounds();
    TRect r;
    
    r.a.x = (int)((b.b.x - b.a.x)/4) - BUTTONLEN/2;
    r.b.x = (int)((b.b.x - b.a.x)/4) + BUTTONLEN/2;
    r.a.y = (b.b.y - b.a.y) - 3;
    r.b.y = (b.b.y - b.a.y) - 1;
    prevButton = new TButton(r, "Prev", cmPrevPage, bfNormal);
    insert(prevButton);
}
void TWizardDialog::setNextButton()
{
    TRect b = getBounds();
    TRect r;
    
    if(finishButton != 0){
       remove(finishButton);
       finishButton = 0;
    }

    r.a.x = (int)((b.b.x - b.a.x)/4) + (int)((b.b.x - b.a.x)/2) - BUTTONLEN/2;
    r.b.x = (int)((b.b.x - b.a.x)/4) + (int)((b.b.x - b.a.x)/2) + BUTTONLEN/2;
    r.a.y = (b.b.y - b.a.y) - 3;
    r.b.y = (b.b.y - b.a.y) - 1;
    nextButton = new TButton(r, "Next", cmNextPage, bfNormal);
    insert(nextButton);
}
void TWizardDialog::setFinishButton()
{
    TRect b = getBounds();
    TRect r;
    
    if(nextButton != 0){
       remove(nextButton);
       nextButton = 0;
    }

    r.a.x = (int)((b.b.x - b.a.x)/4) + (int)((b.b.x - b.a.x)/2) - BUTTONLEN/2;
    r.b.x = (int)((b.b.x - b.a.x)/4) + (int)((b.b.x - b.a.x)/2) + BUTTONLEN/2;
    r.a.y = (b.b.y - b.a.y) - 3;
    r.b.y = (b.b.y - b.a.y) - 1;
    finishButton = new TButton(r, "Finish", cmFinishPage, bfNormal);
    insert( finishButton );
}

void TWizardDialog::setFocus( short button )
{

      switch( button ){

      	case  PREVBTN:
      	     if( (prevButton)&&
      	     ((
      	        ((prevButton->state & (sfVisible + sfDisabled)) == sfVisible) &&
      	              (prevButton->options & ofSelectable)
      	     )) )
      	        prevButton->select();
      	break;
         case NEXTBTN:
              if( (nextButton)&&
      	     ((
      	        ((nextButton->state &(sfVisible + sfDisabled)) == sfVisible) &&
      	              (nextButton->options & ofSelectable)
      	     )) )
      	        nextButton->select();
         break;
         case FINISHBTN:
              if( (finishButton)&&
      	     ((
      	        ((finishButton->state &(sfVisible + sfDisabled)) == sfVisible) &&
      	              (finishButton->options & ofSelectable)
      	     )) )
      	        finishButton->select();
         break;

      }




}







