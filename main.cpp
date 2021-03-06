    #include "network.h"
    #include <armadillo>
    #include <algorithm>
    #include <ctime>
    #include <ctype.h>
    #include "ctime"
    #include <stdlib.h>
    #include <string.h>
    #include <unistd.h>
    #include <sys/select.h>
    #include <termios.h>
    using namespace arma;
    using namespace std;

    struct termios orig_termios;

void reset_terminal_mode()
{
    tcsetattr(0, TCSANOW, &orig_termios);
}

void set_conio_terminal_mode()
{
    struct termios new_termios;

    /* take two copies - one for now, one for later */
    tcgetattr(0, &orig_termios);
    memcpy(&new_termios, &orig_termios, sizeof(new_termios));

    /* register cleanup handler, and set the new terminal mode */
    atexit(reset_terminal_mode);
    cfmakeraw(&new_termios);
    tcsetattr(0, TCSANOW, &new_termios);
}

int kbhit()
{
    struct timeval tv = { 0L, 0L };
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(0, &fds);
    return select(1, &fds, NULL, NULL, &tv);
}
int getch()
{
    int r;
    unsigned char c;
    if ((r = read(0, &c, sizeof(c))) < 0) {
        return r;
    } else {
        return c;
    }
}

void Mnist(int origin, int h, int accuracy,string prefijo, int sizeTrai )
{
    vector<int> hidden; hidden.push_back(h);
    //Network * my_net = new Network(3,4,8,3);
    //g++ -std=c++11 network.h network.cpp neuron.h neuron.cpp layer.h layer.cpp main.cpp -O2 -I /home/amamani/unsa/eddy/armadillo-7.950.0/include -DARMA_DONT_USE_WRAPPER -lopenblas -llapack
    Network * my_net = new Network(3,origin,hidden,origin);
    my_net->printVector("imprimiendo pesos", my_net->getVectorOrders());
    vector< vector<double >> inputs, outputs, IN;
    int Es=sizeTrai;
    //int Es=60000;
    //my_net->loadDataNumbers("/home/mica/Desktop/TopIA/mnistdataset/mnist_train_100.csv", Es, IN, outputs);
    my_net->loadDataNumbers("/home/mica/Desktop/TopIA/mnistdataset/mnist_train.csv", Es, IN, outputs);
    //my_net->loadDataNumbers("../../mnist_train.csv", Es, IN, outputs);
    //my_net->printMat("emtrada",IN);
    vector<double> FinalErrors;
    int times=0;
    bool flag =true;
    double sum;
    double accTraining;
    srand (time(NULL));
    struct timespec start, finish;
    double elapsed;
    clock_gettime(CLOCK_MONOTONIC, &start);
    //set_conio_terminal_mode();
    srand(time(NULL));
    while((flag==true) && (times <100) && (accTraining<accuracy) )//&& !kbhit() )
    {
       // reset_terminal_mode();
       cout<<"###########################"<< times <<"#################################"""<<endl;
       //fflush(stdout);
        FinalErrors.clear();
        int era=0;
        double delta=1000;
        accTraining=0;
        int stk= 0;
            for(int i=0 ;i<Es; i++)
            {
                double t=0.00001;
                my_net->init(IN[i],IN[i], t);
                //cout<<"entrada:  "<< i << "   ****  era ***  "<<era<<endl;
                my_net->forward();
                delta=my_net->sumSquareError();
                if(delta>0.000001)
                    my_net->backpropagation();
                //my_net->forward();
                if(my_net->isCorrect()) accTraining++;
                FinalErrors.push_back(delta);
                era++;
                stk++;
            }
        //}
        sum=0;
        for(int qw =0; qw<FinalErrors.size();qw++)
        {
            sum+=FinalErrors[qw];
        }
        sum = sum / FinalErrors.size();
        if(sum < 0.00000001)
            flag=false;
       cout<<"*********acumulado MENOR AL FLAG **** \n "<<sum<<endl;
       // setvbuf(stdout, (char *)NULL, _IOLBF, 0);
      // reset_terminal_mode();
        //cout<<"num de correctos entrenamiento    "<<accTraining<<endl;
        times++;
   }
    my_net->saveWeithgs(prefijo);
   // (void)getch();
    clock_gettime(CLOCK_MONOTONIC, &finish);
    elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
    cout<<"tiempo de ejecucion "<<elapsed/60<<endl;
    cout<<"Acuraccy Trainig"<<accTraining<<endl;
    cout<<"*********acumulado MENOR AL FLAG **** \n "<<sum<<endl;
     // *******Guardando Neuronas Entrenadas *************//
    mat * newData =  new mat(Es,h+1,fill::zeros); //h+1 se considera el bias desde el inicio
    for(int i=0 ;i<Es; i++)
    {
        double t=0.00001;
        my_net->init(IN[i],IN[i], t);
        my_net->fd2Save(prefijo+"input");
    }

}

void innerMnist(string fileOpen,int origin, int h, int accuracy,string prefijo, int sizeTrai)
{
    vector<int> hidden; hidden.push_back(h);
    //g++ -std=c++11 network.h network.cpp neuron.h neuron.cpp layer.h layer.cpp main.cpp -O2 -I /home/amamani/unsa/eddy/armadillo-7.950.0/include -DARMA_DONT_USE_WRAPPER -lopenblas -llapack
    Network * my_net = new Network(3,origin,hidden,origin);
    my_net->printVector("imprimiendo pesos", my_net->getVectorOrders());
    vector< vector<double >> inputs, outputs, IN;
    int Es=sizeTrai;
    my_net->loadDataNumbersLastLayer(fileOpen, Es, IN);
    //my_net->printMat("emtrada",IN);
    vector<double> FinalErrors;
    int times=0;
    bool flag =true;
    double sum;
    double accTraining;
    srand (time(NULL));
    struct timespec start, finish;
    double elapsed;
    clock_gettime(CLOCK_MONOTONIC, &start);
    //set_conio_terminal_mode();
    srand(time(NULL));
    while((flag==true) && (times <200) && (accTraining<accuracy) )//&& !kbhit() )
    {
       // reset_terminal_mode();
       cout<<"###########################"<< times <<"#################################"""<<endl;
       //fflush(stdout);
        FinalErrors.clear();
        int era=0;
        double delta=1000;
        accTraining=0;
        int stk= 0;
        for(int i=0 ;i<1; i++)
        {
            double t=0.00001;
            my_net->init(IN[i],IN[i], t);
            //cout<<"entrada:  "<< i << "   ****  era ***  "<<era<<endl;
            my_net->forward();
            delta=my_net->sumSquareError();
            if(delta>0.000001)
                my_net->backpropagation();
            //my_net->forward();
            if(my_net->isCorrect()) accTraining++;
            FinalErrors.push_back(delta);
            era++;
            stk++;
        }
        sum=0;
        for(int qw =0; qw<FinalErrors.size();qw++)
        {
            sum+=FinalErrors[qw];
        }
        sum = sum / FinalErrors.size();
        if(sum < 0.00000001)
            flag=false;
        cout<<"*********acumulado MENOR AL FLAG **** \n "<<sum<<endl;
        // setvbuf(stdout, (char *)NULL, _IOLBF, 0);
        //reset_terminal_mode();
        //cout<<"num de correctos entrenamiento    "<<accTraining<<endl;
        times++;
    }
    my_net->saveWeithgs(prefijo); //return;
    //(void)getch();
    clock_gettime(CLOCK_MONOTONIC, &finish);
    elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
    cout<<"tiempo de ejecucion "<<elapsed/60<<endl;
    cout<<"Acuraccy Trainig"<<accTraining<<endl;
    cout<<"*********acumulado MENOR AL FLAG **** \n "<<sum<<endl;
    // *******Guardando Neuronas Entrenadas *************//
    mat * newData =  new mat(Es,h+1,fill::zeros); //h+1 se considera el bias desde el inicio
    for(int i=0 ;i<Es; i++)
    {
        double t=0.00001;
        my_net->init(IN[i],IN[i], t);
        my_net->fd2Save(prefijo+"input");
    }
}

void testingNetwork(int origin, int h,string prefijo)
{
    vector<int> hidden; hidden.push_back(h);
    Network * my_net = new Network(3,origin,hidden,10);
    my_net->printVector("imprimiendo pesos", my_net->getVectorOrders());
    vector< vector<double >> inputs, outputs, IN;
    //int Es=100;
    //  int Es=60000;
    //my_net->loadDataNumbers("/home/mica/Desktop/TopIA/mnistdataset/mnist_train_100.csv", Es, IN, outputs);
    mat A,B;
    A.load(prefijo+"_pesos1.mat", arma_ascii);
    my_net->getVectorLayers()->at(0)->setMat(A);
    B.load(prefijo+"_pesos2.mat", arma_ascii);
    my_net->getVectorLayers()->at(1)->setMat(B);
    int Test =10000;
    vector< vector<double >> I, O, NIT;
    my_net->loadDataNumbers("/home/mica/Desktop/TopIA/mnistdataset/mnist_test.csv", Test, NIT, O);
   // my_net->loadDataNumbers("/home/mica/Desktop/TopIA/mnistdataset/mnist_test_10.csv", Test, NIT, O);
    //my_net->loadDataNumbers("../../mnist_test.csv", Test, NIT, O);
    //my_net->printMat("\n Entrada: \n", NIT);
    //my_net->printMat("\n Salidas: \n", O);
    int total_acierto=0;
    for(int i=0 ;i<Test; i++)
    {
         bool f= my_net->testSet(NIT[i],O[i]);
         if(f)
           total_acierto++;
    }
    cout<<"\n \n Aciertos \n "<<total_acierto<<endl;
}
int main()
{
    srand (time(NULL));
    int ci=784;
    int h1=500;
    int h2=250;
    int h3=100;
    int h4=50;
    int h5=10;
    //testingNetwork(ci,h1);
    vector<int> hidden;
    hidden.push_back(h1);hidden.push_back(h2);hidden.push_back(h3);hidden.push_back(h4);hidden.push_back(h5);
    Network * my_net = new Network(7,ci,hidden,10);
    vector< vector<double >> inputs, outputs, IN;

    //void innerMnist(string fileOpen,int origin, int h, int accuracy,string prefijo, int sizeTrai)
    //void Mnist(int origin, int h, int accuracy,string prefijo, int sizeTrai )
    int tamData=60000;
    cout<<"%%%%%%%%%%% entrando capa 1"<<endl;
    Mnist(ci, h1, 58000,"data/l1",tamData);
    cout<<"%%%%%%%%%%%%%%%%%%%%%%% entrando capa 2"<<endl;
    innerMnist("data/l1input", h1-1, h2, 58000,"data/l2",tamData);
    cout<<"%%%%%%%%%%% entrando capa 3"<<endl;
    innerMnist("data/l2input",h2-1, h3, 58000,"data/l3",tamData);
    cout<<"%%%%%%%%%% entrando capa 4"<<endl;
    innerMnist("data/l3input",h3-1, h4, 58000,"data/l4",tamData);
    cout<<"%%%%%%%%%%% entrando capa 5"<<endl;
    innerMnist("data/l4input",h4-1, h5, 58000,"data/l5",tamData);

    cout<<"********************aprendizaje modulado***************************"<<endl;
    //return 0;

    mat A,B,C,D,E;
    mat lA,lB,lC,lD,lE;
    /*A.load("data/l1_pesos1.mat", arma_ascii);
    B.load("data/l2_pesos1.mat", arma_ascii);
    C.load("data/l3_pesos1.mat", arma_ascii);
    D.load("data/l4_pesos1.mat", arma_ascii);
    E.load("data/l5_pesos1.mat", arma_ascii);*/
    A.load("data/l1_pesos1.mat");
    B.load("data/l2_pesos1.mat");
    C.load("data/l3_pesos1.mat");
    D.load("data/l4_pesos1.mat");
    E.load("data/l5_pesos1.mat");
    my_net->getVectorLayers()->at(0)->setMat(A);
    my_net->getVectorLayers()->at(1)->setMat(B);
    my_net->getVectorLayers()->at(2)->setMat(C);
    my_net->getVectorLayers()->at(3)->setMat(D);
    my_net->getVectorLayers()->at(4)->setMat(E);

    int Test =10000;
    vector< vector<double >> O, NIT;
    my_net->loadDataNumbers("/home/mica/Desktop/TopIA/mnistdataset/mnist_test.csv", Test, NIT, O);
    // my_net->loadDataNumbers("/home/mica/Desktop/TopIA/mnistdataset/mnist_test_10.csv", Test, NIT, O);
    //my_net->loadDataNumbers("../../mnist_test.csv", Test, NIT, O);
    //my_net->printMat("\n Entrada: \n", NIT);
    //my_net->printMat("\n Salidas: \n", O);
    int total_acierto=0;
    for(int i=0 ;i<Test; i++)
    {
         bool f= my_net->testSet(NIT[i],O[i]);
         if(f)
           total_acierto++;
    }
    cout<<"\n \n Aciertos \n "<<total_acierto<<endl;
    return 0;
}
        /*
        Network * my_net = new Network(3,2,2,1); // numcapas, numInput, numHidden, numOutput
         my_net->printVector("imprimiendo pesos", my_net->getVectorOrders());
         vector< vector<double >> imputs;
         vector<double > outputs;

         int Es=4;
         imputs.resize(Es);
         for(int i = 0 ; i< Es ;i++)
             imputs[i].resize(my_net->getNumEntradas()-1);

         imputs[0][0]=1;
         imputs[0][1]=1;
         imputs[1][0]=0;
         imputs[1][1]=1;
         imputs[2][0]=1;
         imputs[2][1]=0;
         imputs[3][0]=0;
         imputs[3][1]=0;

         outputs.push_back(0);
         outputs.push_back(1);
         outputs.push_back(1);
         outputs.push_back(0);


         my_net->printMat("\n Training: \n", imputs);
         my_net->printVector("\n Expected: \n", outputs);
         cout<<"leyo"<<endl;
         vector<double> FinalErrors;

         int times=0;
         bool flag =true;
         double sum=0;
        while(flag)
        {
            cout<<"###########################"<< times <<"#################################"""<<endl;
            for(int i=0 ;i<Es; i++)
             {
                 //int i =2 ;
                 my_net->init2(imputs[i],outputs[i], 0.1);
                 double delta=2;
                 int era=0;
                 cout<<"threashold: "<<my_net->getThreshold()<<endl;
                 my_net->printVector("\n entrada \n ",imputs[i]);
                 cout<<"\n Valor esperado \n" << outputs[i]<<endl;
                 {
                     for(int t =0 ; t<1;t++)
                     {
                         era++;
                         cout<<"entrada:  "<< i << "   ****  era ***  "<<era<<endl;
                         my_net->forward();
                         //cout<<"el error es :"<<delta<<endl;
                         delta=my_net->sumSquareError();
                         //if(delta>my_net->getThreshold())
                         my_net->backpropagation();
                         //my_net->forward();
                         FinalErrors.push_back(delta);
                         cout<<"delta"<<delta;
                     }
                 }
             }
             //cout<<"Final"<<endl;
           //  cout<<"esta entrando.............           "<<FinalErrors.size()<<endl;
             for(int qw =0; qw<FinalErrors.size();qw++)
             {
                // cout<<"  -  "<<FinalErrors[qw]<<endl;
                 sum+=FinalErrors[qw];
             }
             //cout<<"====primer sum"<< sum <<endl;
             sum = sum / FinalErrors.size();
             if(sum < 0.001)
                 flag=false;
             cout<<"*********acumulado**** \n "<<sum<<endl;
             times++;
         }
        cout<<"%%%%%%%%%%%%%%  Probando %%%%%%%%%%%%%%"<<endl;

        for(int i=0 ;i<4; i++)
        {
                //my_net->testSet(I[0],O[0]);
            my_net->init2(imputs[i],outputs[i], 0.01);
            my_net->forward();
            cout<<"\n Espeardo:     "<< outputs[i]<<endl;
        }
    */


    // ************************************* XOR *******************************************

    /* */


    /************ MAtriz Armadillo ************************/

    //QCoreApplication a(argc, argv);
    /*
    mat * A= new mat(1,5,fill::ones);
    *A *= 3;


    mat * C = new mat(1,5,fill::ones);
    *C *= 4;
    mat * D = new mat((*A) % (*C));
    cout<<*(D)<<endl;
    */



    /*

       int main()
    {
        //vector<int> hidden; hidden.push_back(3);
        //vector<int> hidden; hidden.push_back(4);
        //vector<int> hidden; hidden.push_back(6);
        //vector<int> hidden; hidden.push_back(8);
        //vector<int> hidden; hidden.push_back(3);hidden.push_back(5);
        //vector<int> hidden; hidden.push_back(4);hidden.push_back(6);
        //vector<int> hidden; hidden.push_back(6);hidden.push_back(8);
        vector<int> hidden; hidden.push_back(8);hidden.push_back(6);
        //Network * my_net = new Network(3,4,8,3);

        Network * my_net = new Network(4,4,hidden,3);
        my_net->printVector("imprimiendo pesos", my_net->getVectorOrders());
        vector< vector<double >> inputs, outputs, IN;
        int Es=120;
        my_net->loadDataFlowers("irisTraining.txt", Es, inputs, outputs);
        my_net->normalize(IN, inputs);
        //my_net->printMat("Entrada normalizada \n ", IN);
        vector<double> FinalErrors;
        int times=0;
        bool flag =true;
        double sum;
        while((flag==true) && (times <6000))
        {
            //cout<<"###########################"<< times <<"#################################"""<<endl;
            FinalErrors.clear();
            int era=0;
            double delta=1000;
            for(int i=0 ;i<Es; i++)
            {
                double t=0.00001;
                my_net->init(IN[i],outputs[i], t);
                //cout<<"entrada:  "<< i << "   ****  era ***  "<<era<<endl;
                my_net->forward();
                delta=my_net->sumSquareError();
                //cout<<"SumsquareError de la capa:"<<delta<<endl;
                if(delta>0.000001)
                    my_net->backpropagation();
                //my_net->forward();
                FinalErrors.push_back(delta);
                era++;
            }
            sum=0;
            for(int qw =0; qw<FinalErrors.size();qw++)
            {
                // cout<<"  -  "<<FinalErrors[qw]<<endl;
                sum+=FinalErrors[qw];
            }
            //cout<<" solo la sumatoria  "<< sum <<" El tamño del vector"<<FinalErrors.size()<<endl;
            sum = sum / FinalErrors.size();
            if(sum < 0.001)
                flag=false;

            times++;
       }
        cout<<"*********acumulado MENOR AL FLAG **** \n "<<sum<<endl;
        cout<<"%%%%%%%%%%%%%%%%%% TEST %%%%%%%%%%%%%%"<<endl;
        int Test =30;
        vector< vector<double >> I, O, NIT;
        my_net->loadDataFlowers("irisTest.txt", Test, I, O);
        my_net->normalize(NIT, I);
        //my_net->printMat("\n Entrada: \n", NIT);
        //my_net->printMat("\n Salidas: \n", O);
        int total_acierto=0;
        for(int i=0 ;i<Test; i++)
        {
             bool f= my_net->testSet(NIT[i],O[i]);
             if(f)
               total_acierto++;
        }
        cout<<"\n \n Aciertos \n "<<total_acierto<<endl;
    }

    */
