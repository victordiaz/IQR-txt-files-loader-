#include "iqrvj.hpp"
#include <unistd.h>
#include <sstream>
#include <iomanip>
#include <Common/Helper/iqrUtils.h>

#include <Qt/qfile.h>
#include <Qt/qdir.h>
#include <Qt/qtextstream.h>
#include <Qt/qglobal.h>
#include <Qt/qtimer.h>



MAKE_MODULE_DLL_INTERFACE(iqrcommon::iqrvj, "iqrvj Module")


iqrcommon::iqrvj::iqrvj():ClsThreadModule() {
    tid = 0;

    clsStringParameterDirectory = addStringParameter("Directory", /* _strName */
                                  "Directory",  /* _strLabel */
                                  "../data/iqrvj/spain/", /* _iValue */
                                  true, /* _iMinimum */
                                  true, /* _iMaximum */
                                  "Directory" /* _strDescription */,
                                  "Directory" /* _strCategory */);


    output1StateVariable = addOutputToGroup("vj", "Vj");
    output1StateVariable->setNrCellsHorizontal(40);
    output1StateVariable->setNrCellsVertical(30);


    inputStateVariablePtr = addInputFromGroup("frameNumber", "Frame Number");



}

iqrcommon::iqrvj::~iqrvj() {
#ifdef DEBUG_MODULEDBACCESS
    cout << "iqrvj::~iqrvj()" << endl;
#endif
}

void iqrcommon::iqrvj::init() {
#ifdef DEBUG_MODULEDBACCESS
    cout << "iqrvj::init()" << endl;
#endif

    selFrame = 0;

    strDirectory = clsStringParameterDirectory->getValueAsString();
    cout<<"Get data from the files in the directory:" << strDirectory << endl;
    if (strDirectory == "") {
        throw ModuleError(string("Module \"") + label() + "\": Invalid directory");
    }

    iNrCells = output1StateVariable->getStateArray().getWidth() * output1StateVariable->getStateArray().getDepth();
    cout<<"number of cells "<<iNrCells<<endl;





    cout << "current directory: " << QDir::currentPath().toStdString() << endl;
    cout << " directory: " << clsStringParameterDirectory->getValueAsString() << endl;


    
    QString myDirRoute = clsStringParameterDirectory->getValueAsString().c_str(); 
    cout << " QQQQQQQQQQQQQQQ: " << myDirRoute.toStdString() << endl;

    QDir myDir(myDirRoute); 
    QStringList dirList = myDir.entryList(QDir::Filters(QDir::Files));

    //load files
    foreach(QString q, dirList) {
        QVector< QVector<int> > singleFrame;

        cout << "file: " << q.toStdString() << endl;

        QFile file(myDirRoute + q);
        cout << "qq " << file.fileName().toStdString() << endl;

        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

        QTextStream in(&file);
        QString line = in.readLine();


        //parse lines
        //int line_count = 0;
        while (!line.isNull()) {
            //process_line(line);
            QVector<int> singleLine;
            line = in.readLine();
            //cout << "line: " << line.toStdString() << endl;

            QStringList list = line.split(",");


            for (int i = 0; i < list.size() - 1; i++) {
                singleLine.append(list[i].toInt());
                //cout << "" <<  list[i].toInt();
            }

            //cout << "" << endl;

            singleFrame.append(singleLine);
        }

        frames.append(singleFrame);

    }

    numFrames = frames.size() - 1;



}

void iqrcommon::iqrvj::update() {

//    cerr << "iqrvj::update()" << endl;


    //cout<<"pitch: "<<pitch<<" enveloppe: " <<enveloppe<<" onset: "<<onset<<endl;

    qmutexThread->lock();



    StateArray &clsStateArrayInput1 = inputStateVariablePtr->getTarget()->getStateArray();
    StateArray &clsStateArrayOut1 = output1StateVariable->getStateArray();

    //cout << clsStateArrayInput1[0][0] << endl;

    //for (unsigned int i = 0; i < clsStateArrayOut1.getWidth(); i++){
    //clsStateArrayOut1[0][i] = pitch;
    //}



    //leer
    //for(int i = 0; i < frames.size(); i++) {
    float inputVal1 = (float)clsStateArrayInput1[0][0];


    //cout << "qq" << inputVal1 << endl;

    (ceil(inputVal1) - inputVal1) >= .5 ? selFrame = floor(inputVal1 * numFrames) : selFrame = ceil(inputVal1 * numFrames);

    //cout << "qq " << ceil(inputVal1) - inputVal1 << endl;
    //cout << "qq " << floor(inputVal1) * numFrames << endl;
    //cout << "qq " << ceil(inputVal1) * numFrames << endl;

    //cout << "inputVal1 " << inputVal1 << endl;
    //cout << "numFrames " << numFrames << endl;
    //cout << "selFrame " << selFrame << endl;



    QVector< QVector< int > > q = frames[selFrame];

    for (int j = 0; j < q.size(); j++) {
        QVector< int > w = q[j];

        //cout << "pwqipoqwipoqwi" << w.size() << endl;
        for (int k = 0; k < w.size(); k++) {
            clsStateArrayOut1[0][k + j * output1StateVariable->getNrCellsHorizontal()] = w[k];
            //cout << "" << w[k];

        }
        //cout << "" << endl;
    }
    //cout << "" << endl;
    //}





    qmutexThread->unlock();
    usleep(50);

}

void iqrcommon::iqrvj::cleanup() {
    //qs->close();
}
