/* gcc -o example example.c dwmyChart.c */
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h> //sleep
#include <stdlib.h> //randr

void printChart(int* chartArray, char printSamples, int partition);

int main()
{
    printf("\n");

    char buffer [80];
    // Aktualny czas
    time_t rawTime=time(NULL);
    struct tm * utcTime=gmtime(&rawTime);
    struct tm * localTime=localtime(&rawTime);
    /*
    printf("rawTime: %ld\n",rawTime);
    strftime (buffer,80,"utcTime: %F %T",utcTime);
    printf("%s (%ld)\n",buffer,mktime(utcTime));
    strftime (buffer,80,"localTime: %F %T",localTime);
    printf("%s (%ld)\n",buffer,mktime(localTime));
    */
   
    //wykresy - zbieranie danych
    int dChart[288];
    int wChart[288];
    int mChart[288];
    int yChart[288];
    time_t dChartLastTime=rawTime;
    time_t wChartLastTime=rawTime;
    time_t mChartLastTime=rawTime;
    time_t yChartLastTime=rawTime;
    int dChartSum=0;
    int dChartAvg=0;
    int dChartCount=0;
    int sample=0;

    for (int i=0; i<288 ; i++) { //zerowanie tablic; memset nie radzi sobie z int
        dChart[i]=-5000;
        wChart[i]=-6000;
        mChart[i]=-5000;
        yChart[i]=-5000;
    }

    //na razie próbki będą losowane
    srand(rawTime); //ustawimy ziarno dla generatora
    sample=rand(); //losujemy

    //próbki dla wykresu dziennego
    for(int i=0 ; i <456 ; i++) {  
        //na razie próbki będą losowane
        sample=rand()%5001; //losujemy z zakresu 0-5000

        for (int d=0 ; d<287 ; d++) { //zawsze zapisujemy na końcu tablicy dlatego przed zapisem przesówamy elementy w lewo
            dChart[d]=dChart[d+1];
        }
        dChart[287]=sample;
        dChartCount++;
        dChartSum+=sample;
        dChartAvg=dChartSum/dChartCount;

        //próbki dla wykresu tygodniowego; co 41 próbek wykresu dziennego
        if (dChartCount >=7) {
            for (int w=0 ; w<287 ; w++) { //zawsze zapisujemy na końcu tablicy dlatego przed zapisem przesówamy elementy w lewo
                wChart[w]=wChart[w+1];
            }
            wChart[287]=dChartAvg;
            dChartCount=0;
            dChartSum=0;
            dChartAvg=0;
        }
    }

    printChart(dChart,1,0);
    //printChart(wChart,0,1);

    printf("\n");
    return 0;
}

void printChart(int* chartArray, char printSamples, int partition) {
    //przeskalowany w pionie i poziomie o 2 wykres dzienny z obciętą częścią ułamkową
    int graphBarPrev=0;
    int graphBarNext=0;
    int graphBarAvg=0;
    for (int row=25; row >= 0 ; row--) { //rysowanie wykresu wierszami, zaczynamy od góry
        for (int samples=0; samples < 288 ; samples=samples+2) { //iteracja po próbkach; ...samples+2 - zwężamy wykres uśredniając 2 pozycje
            graphBarPrev=chartArray[samples]/100/2; //obcinamy cześć ułamkową i skalujemy wykres dzieląc przez 2;
            graphBarNext=chartArray[samples+1]/100/2; //obcinamy cześć ułamkową i skalujemy wykres dzieląc przez 2;
            graphBarAvg=(graphBarPrev+graphBarNext)/2;
            if ( graphBarAvg >= row) { //jeżeli odczytane uśrednione dane/próbki są większe niż przetwarzany wiersz to drukuj znak wykresu
                if (partition != 0 && samples%21 == 0) {
                    printf("%c",'#');
                } else {
                    printf("%c",'|');
                }
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }
    printf("\n");
    
    //wydruk próbek
    if (printSamples != 0) {
        printf("[");
        for (int i=0; i < 288 ; i++) {
            printf("%d, ",chartArray[i]/100);
        }
        printf("]");
        printf("\n");
    }
}
