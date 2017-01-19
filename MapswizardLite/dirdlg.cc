#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define Uses_MsgBox
#define Uses_TRect
#define Uses_TInputLine
#define Uses_TLabel
#define Uses_THistory
#define Uses_TScrollBar
#define Uses_TDirListBox
#define Uses_TButton
#define Uses_TEvent
#define Uses_TDirEntry
#define Uses_TDirCollection
//#define Uses_opstream
//#define Uses_ipstream
//#define Uses_TStreamableClass
#include <tv.h>

#include "dirdlg.h"
//#include "wizardlg.h"


TChDirWizardDlg::TChDirWizardDlg(ushort opts, TRect& r, const char* title) :
    TWizardDialog( r, title )
    , TWindowInit( &TChDirWizardDlg::initFrame )
{
    char *tmp;

    dirInput = new TInputLine( TRect( 3, 3, 30, 4 ), FILENAME_MAX );
    insert( dirInput );
    tmp = _("Directory ~n~ame");
    insert( new TLabel( TRect( 2, 2, 3+cstrlen(tmp), 3 ), tmp, dirInput ));
    insert( new THistory( TRect( 30, 3, 33, 4 ), dirInput, cmChangeDir ) );

    TScrollBar *sb = new TScrollBar( TRect( 32, 5, 33, 15 ) );
    insert( sb );
    dirList = new TDirListBox( TRect( 3, 5, 32, 15 ), sb );
    insert( dirList );
    tmp = _("Directory ~t~ree");
    insert( new TLabel( TRect( 2, 4, 3+cstrlen(tmp), 5 ), tmp, dirList ) );

    //okButton = new TButton( TRect( 35, 6, 45, 8 ), _("~O~K"), cmOK, bfDefault );
    //insert( okButton );
    chDirButton = new TButton( TRect( 35, 9, 45, 11 ), _("~C~hdir"), cmChangeDir, bfNormal );
    insert( chDirButton );
    insert( new TButton( TRect( 35, 12, 45, 14 ), _("~R~evert"), cmRevert, bfNormal ) );
   // if( (opts & cdHelpButton) != 0 )
   //     insert( new TButton( TRect( 35, 15, 45, 17 ), _("Help"), cmHelp, bfNormal ) );
    if( (opts & cdNoLoadDir) == 0 )
        setUpDialog();
    selectNext( False );
}

static inline
int changeDir( const char *path )
{
    return chdir( path );
}

uint32 TChDirWizardDlg::dataSize()
{
    return 0;
}

void TChDirWizardDlg::shutDown()
{
    dirList = 0;
    dirInput = 0;
    okButton = 0;
    chDirButton = 0;
    TDialog::shutDown();
}

void TChDirWizardDlg::getData( void * )
{
}

void TChDirWizardDlg::handleEvent( TEvent& event )
{
    TWizardDialog::handleEvent( event );
    switch( event.what )
        {
        case evCommand:
            {
            char curDir[PATH_MAX];
            switch( event.message.command )
                {
                case cmRevert:
                    getCurDir( curDir );
                    break;
                case cmChangeDir:
                    {
                    TDirEntry *p = dirList->list()->at( dirList->focused );
                    strcpy( curDir, p->dir() );
                    if( curDir[strlen(curDir)-1] != DIRSEPARATOR )
                        strcat( curDir, DIRSEPARATOR_ );
                    changeDir(curDir);
                    if(isHexDir())
                        enableCommand(cmNextPage);
                    else
                        disableCommand(cmNextPage);

                    break;
                    }
                case cmDirSelection:
                    chDirButton->makeDefault((Boolean)event.message.infoPtr);
                    return; // Do not use break here !
                default:
                    return;
                }
            dirList->newDirectory( curDir );
            strcpy( dirInput->data, curDir );
            dirInput->drawView();
            dirList->select();
            clearEvent( event );
            }
        default:
            break;
        }
}

void TChDirWizardDlg::setData( void * )
{
}

void TChDirWizardDlg::setUpDialog()
{
    if( dirList != 0 )
        {
	char curDir[PATH_MAX];
        getCurDir( curDir );
        dirList->newDirectory( curDir );
        if( dirInput != 0 )
            {
            strcpy( dirInput->data, curDir );
            dirInput->drawView();
            }
        }
}

Boolean TChDirWizardDlg::valid( ushort command )
{
  if ( command != cmOK )
    return True;
  if( changeDir( dirInput->data ) != 0 )
  {
    messageBox( _("Invalid directory"), mfError | mfOKButton );
    return False;
  }
  return True;
}


Boolean TChDirWizardDlg::isHexDir()
{
     getCurDir( curDir );

}