
#include "quickfix/config.h"
#include <quickfix/FileStore.h>
#include <quickfix/FileLog.h>
#include <quickfix/SocketAcceptor.h>
#include <quickfix/SessionSettings.h>

#include "Application.h"
#include <chrono>
#include <thread>

#include <string>
#include <iostream>
#include <fstream>

int main(int argc, char ** argv) {
    try {
        FIX::SessionSettings settings("OMEProgram.cfg");
        FixServer application;
        FIX::FileStoreFactory storeFactory(settings);
        FIX::FileLogFactory logFactory(settings);
        FIX::SocketAcceptor acceptor(application, storeFactory, settings, logFactory);
        acceptor.start();
        std::cout << "Server started..." << std::endl;
        // Keep running the server
        while ( true )
           {
             std::string value;
             std::cin >> value;

             if ( value == "#symbols" ) {

             }
             else if( value == "#quit" ) {
                 break;
             }
             else {
            	 std::cout<<"hell0";
             }

             std::cout << std::endl;
           }
           acceptor.stop();
           return 0;
         }
         catch ( std::exception & e )
         {
           std::cout << e.what() << std::endl;
           return 1;
         }

}

//quickfix/src/C++/.libs
//export LD_LIBRARY_PATH=`pwd`


/*find |egrep FIXT11.xml
2053  cd spec
2054  cd ~/eclipse-workspaceOME/ordermatch
2055  cd ..
2056  ls -l
2057  ln -sf ~/eclipse-workspaceOME/quickfix/spec spec
*/

