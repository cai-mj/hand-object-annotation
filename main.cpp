#include "widget.h"




int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;

    int stat = 0;
    string seqName;
    string suffix;
    int step;

    if(argc < 2)
    {
        seqName = string("022");
        suffix = string(".mpg");
        step = 10;
    }
    else
    {
        int k = 1;
        while(k < argc)
        {
            if("-sequence" == (string)argv[k]) seqName = (string)argv[++k];
            else if("-suffix" == (string)argv[k]) suffix = (string)argv[++k];
            else if("-step" == (string)argv[k]) step = atoi(argv[++k]);
            else stat = 1;
            k++;
        }
    }

    if(stat)
    {
        cout << "./graspLabel -sequence <string> -suffix <string> -step <int>\n";
        return -1;
    }

    char command[200];
    sprintf(command, "mkdir %s", seqName.c_str());
    system(command);
    VideoCapture cap(seqName+suffix);
    if(!cap.isOpened())
    {
        cout << "ERROR: Cannot open file:" << seqName+suffix << endl;
        exit(1);
    }

    int f = 0; //frame id
    while(waitKey(1)!=27)
    {
        f++;
        cap >> w._src;
        if(f<101) continue;
        if(!w._src.data) break;

        if((f-1) % step != 0) //update and check index
            continue;

//        string filename("00000279.jpg");
        char filename[100];
        sprintf(filename, "%08d", f);
        w._seqname = seqName;
        w.initialize(filename);

        stat = a.exec();
        if(w._isQuit)
        {
            cout << "Quit forcefully!\n";
            break;
        }
    }

}


