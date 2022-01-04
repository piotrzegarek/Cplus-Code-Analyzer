#include "ui_mainwindow.h"
#include "wczytajpliki.h"
#include "mainwindow.h"
#include "analiza.h"
#include "listy.h"
#include <fstream>
#include <QMainWindow>
#include <QLineEdit>

using namespace std;

string wybierzNazwe(nazwy *start,int i){        //zwraca i-tą nazwę pliku do wczytania z listy jednokierunkowej z nazwami plików do analizowania
    nazwy *akt = start;
    for(int j=0; j<i ; j++){
        akt =akt->next;
    }
    string nazwa = akt->nazwa;
    return nazwa;
}

bool wczytajPlik(ifstream &plik, string nazwa, Ui::MainWindow *ui){ //otwiera dany plik do analizy
    plik.open(nazwa);
    if(plik.good()){
        ui->teWynik->append(QString::fromStdString("Plik "+nazwa+" zostal otworzony pomyslnie"));
        return true;
    }
    else{
      ui->teWynik->append(QString::fromStdString("Blad z plikiem "+nazwa));
      plik.close();
      return false;
    }
}

void dodajNazwe(nazwy *&start,string nowa_nazwa){   //dodaje nazwe pliku do listy jednokierunkowej na nazwy plików do analizy
  nazwy *nowy=new nazwy;
  nowy->nazwa=nowa_nazwa;
  if(start==nullptr){
    nowy->next=nullptr;
    start=nowy;
  }else{
    nazwy *ost=start;
    while(ost->next!=nullptr){   //przejscie do ostatniego elem listy
      ost=ost->next;
    }
    ost->next=nowy;
    nowy->next=nullptr;
  }
}
