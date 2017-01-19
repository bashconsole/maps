

#include <tv.h>

const ushort
    cdNormal     = 0x0000, // Option to use dialog immediately
    cdNoLoadDir  = 0x0001, // Option to init the dialog to store on a stream
    cdHelpButton = 0x0002; // Put a help button in the dialog

class TEvent;
class TInputLine;
class TDirListBox;
class TButton;

#include "wizardlg.h"


class TChDirWizardDlg : public TWizardDialog
{

public:

    TChDirWizardDlg(ushort, TRect&, const char*);

    friend class TDirListBox;

    virtual uint32 dataSize();
    virtual void getData( void *rec );
    virtual void handleEvent( TEvent& );
    virtual void setData( void *rec );
    virtual Boolean valid( ushort );
    virtual void shutDown();

private:

    void setUpDialog();

    TInputLine *dirInput;
    TDirListBox *dirList;
    TButton *okButton;
    TButton *chDirButton;

};





