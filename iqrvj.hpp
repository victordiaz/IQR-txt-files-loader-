#ifndef iqrvj_HPP
#define iqrvj_HPP

#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <sys/time.h>
#include <bitset>
#include <vector>

//#include <qsocketdevice.h> 
//#include <qhostaddress.h> 

#include <Common/Item/threadModule.hpp>
#include <QtCore/qlist.h> 
#include <QtCore/QVector> 


using namespace std;

namespace iqrcommon {

    class iqrvj : public ClsThreadModule {
    public:
	iqrvj();
	~iqrvj();

	void init();
	void update();
	void cleanup();
	
    private:
	ClsStringParameter* clsStringParameterDirectory;
	ClsStateVariable* output1StateVariable; 
	StateVariablePtr* inputStateVariablePtr;


	string strDirectory;
	int iNrCells; 
	
	int selFrame; 
	int numFrames;  

	
	QVector < QVector< QVector<int> > > frames; 
    };
}
#endif
