/****************************************************************************
**  main.cpp
**  
**   Copyright (c) 2001 Impuls
**
**  Autor: A.N.Zubovsky (azu@imp.lg.ua)
**
*****************************************************************************/


#ifdef WITH_GUI
#include "mapswizard.h"
#include <qapplication.h>
#endif

#include "mapsconf.h"
#include "param.h"
#include "setupdefaults.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    input_file = NULL;
    if(argc > 1){
       strcpy(config_filename, argv[1]);
       input_file = fopen(config_filename, "r");
    }
    else{
       strcpy(config_filename, "confmaps");
       input_file = fopen(config_filename, "r");
    }
    
    if(input_file != NULL)
       compile();
    else   
       setupDefaults();      
#ifdef WITH_GUI
    if( getenv("DISPLAY") == NULL){
       printf("************* no display! ****************\n");
#endif    
        return mapsconf();
#ifdef WITH_GUI	
    }else{
    	QApplication a(argc,argv);
    	MapsWizard wizard;
    	wizard.setCaption("MAPS Net Configurator");
    	return wizard.exec();
    }	
#endif    
}
