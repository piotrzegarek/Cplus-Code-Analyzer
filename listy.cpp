#include "listy.h"
#include "analiza.h"
#include "ui_mainwindow.h"
#include "wczytajpliki.h"
#include "mainwindow.h"
#include <fstream>
#include <iomanip>
#include <QMainWindow>
#include <QLineEdit>

using namespace std;

void dodajEnd(dane *&glowa,string nazwa){   //dodaje element do listy jednokierunkowej na końcu
  dane *nowy=new dane;
  nowy->zmienna=nazwa;
  if(glowa==nullptr){
    nowy->next=nullptr;
    glowa=nowy;
  }else{
    dane *ost=glowa;
    while(ost->next!=nullptr){   //przejscie do ostatniego elem listy
      ost=ost->next;
    }
    ost->next=nowy;
    nowy->next=nullptr;
  }
}

int zliczListeDane(dane *glowa){        //zlicza i zwraca ilość danych w liście jednokierunkowej
  dane *akt=glowa;
  int liczba=0;
  while(akt!=nullptr){
    liczba++;
    akt=akt->next;
  }

  return liczba;
}

int zliczListeNazwy(nazwy *start){      //zlicza i zwraca ilość danych w liście jednokierunkowej
  nazwy *akt=start;
  int liczba=0;
  while(akt!=nullptr){
    liczba++;
    akt=akt->next;
  }
  return liczba;
}

void drukujListeDane(dane *glowa, Ui::MainWindow *ui){      //drukuje zawartość listy jednokierunkowej w gui
  dane *akt=glowa;
  string wynik;
  while(akt!=nullptr){
    wynik=wynik+akt->zmienna+" | ";
    akt=akt->next;
  }
  ui->teWynik->append(QString::fromStdString(wynik));
  ui->teWynik->append("");
}

void kasujListeDane(dane *&glowa){      //kasuje liste jednokierunkową danych
  while(glowa!=nullptr){
    dane *tmp=glowa;
    glowa=glowa->next;
    delete tmp;
  }
}
void kasujListeNazwy(nazwy *&start){        //kasuje liste jednokierunkową nazw plików
  while(start!=nullptr){
    nazwy *tmp=start;
    start=start->next;
    delete tmp;
  }
}
