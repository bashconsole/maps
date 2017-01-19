#define Uses_TDialog
#include <tv.h>


#if !defined( __TWizardDialog)
#define __TWizardDialog

#define NEXTBTN   0
#define PREVBTN   1
#define FINISHBTN 2


const short cmNextPage   = 100,
             cmPrevPage   = 101,
	     cmFinishPage = 102;

class TButton;
class TDialog;	    	
	
class TWizardDialog:public TDialog
{
public:
      TWizardDialog( const TRect& bounds, const char *aTitle, short place = 0);
      ~TWizardDialog();
      
      TRect bounds;
      
      virtual void handleEvent(TEvent& event);
      
      TButton *nextButton;
      TButton *prevButton;
      TButton *finishButton;
      
      void setPrevButton();
      void setNextButton();
      void setFinishButton();
      void setFocus(short);
};

#endif

