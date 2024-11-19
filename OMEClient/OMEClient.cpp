#include <quickfix/Application.h>
#include <quickfix/SessionSettings.h>
#include <quickfix/FileStore.h>
#include <quickfix/FileLog.h>
#include <quickfix/SocketInitiator.h>
#include "Application.h"

#include <chrono>
#include <thread>

int main(int argc, char ** argv) {
	  try
	  {
		FIX::SessionSettings settings( "OMEClient.cfg");

		FixClient application;
		FIX::FileStoreFactory storeFactory( settings );
		FIX::SocketInitiator initiator( application, storeFactory, settings );

		initiator.start();
		application.run();
		initiator.stop();

		return 0;
	  }
	  catch ( std::exception & e )
	  {
		std::cout << e.what();
		return 1;
	  }
}

//export LD_LIBRARY_PATH=`pwd`
//quickfix/src/C++/.libs

/*find |egrep FIXT11.xml
2053  cd spec
2054  cd ~/eclipse-workspaceOME/ordermatch
2055  cd ..
2056  ls -l
2057  ln -sf ~/eclipse-workspaceOME/quickfix/spec spec
*/
