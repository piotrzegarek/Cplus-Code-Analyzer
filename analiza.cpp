#include "analiza.h"
#include "ui_mainwindow.h"
#include "wczytajpliki.h"
#include "mainwindow.h"
#include "listy.h"
#include <fstream>
#include <iomanip>
#include <QMainWindow>
#include <QLineEdit>

using namespace std;


void analizujKod(ifstream &plik, dane *&glowa,dane *&struktury, Ui::MainWindow *ui){
  dlugoscKodu(plik,ui); // drukuje długość kodu
  szukaj_bibliotek(glowa,plik,ui);  //drukuje biblioteki w kodzie
  szukaj_funkcji(glowa,plik,ui);    //drukuje funkcje z kodu
  szukaj_struktur(struktury,plik,ui);   //drukuje struktury z kodu
  dane *akt=struktury;
  for(int i=0;i<zliczListeDane(struktury);i++){ //wyszukiwanie zmiennych struktur
    string nazwa=akt->zmienna;  //wgrywa nazwe struktury jako nazwe zmiennej do szukania
    szukaj_zmiennej(glowa,plik,nazwa,ui);   //szuka zmiennych od struktur
    akt=akt->next;
  }kasujListeDane(struktury);
  szukaj_zmiennej(glowa,plik,"int",ui);     //szuka zmiennej int
  szukaj_zmiennej(glowa,plik,"char",ui);        //szuka zmiennej char
  szukaj_zmiennej(glowa,plik,"string",ui);      //szuka zmiennej string
  szukaj_zmiennej(glowa,plik,"double",ui);      //szuka zmiennej double
  szukaj_zmiennej(glowa,plik,"float",ui);       //szuka zmiennej float
  szukaj_zmiennej(glowa,plik,"fstream",ui);     //szuka zmiennej fstream
  kasujListeDane(glowa);
  kasujListeDane(struktury);
}

void dlugoscKodu(ifstream &plik, Ui::MainWindow *ui){
    string linia;
    int linijki=0;
    while(true){
      if(plik.eof()==true){     //jeśli dojdzie do końca kodu ustawia wskaźnik na początek i wychodzi z pętli
        plik.clear();
        plik.seekg(0);
        break;
      }
      getline(plik,linia);
      linijki++;
    }
    QString dlugosc = QStringLiteral("Ilosc linijek w kodzie: %1. ").arg(linijki);
    ui->teWynik->append(dlugosc);   //drukuje informacje o dlugosci w GUI
}

bool sprawdz_powtorzenie(dane *glowa,string nazwa_zmiennej){
  dane *akt=glowa;
  while(akt!=nullptr){  //przejscie po całej liście
    if(akt->zmienna==nazwa_zmiennej) return true;
    akt=akt->next;
  }
  return false;
}

void szukaj_bibliotek(dane *&glowa, ifstream &plik, Ui::MainWindow *ui){
  string linia,nazwa;
  while(true){
    if(plik.eof()==true) { //jeśli dojdzie do końca kodu ustawia wskaźnik na początek i wychodzi z pętli
      plik.clear();
      plik.seekg(0);
      break;
    }
    getline(plik,linia);
    size_t pozycja_bib=linia.find("#include "); //szuka frazy #include w linijce
    if(pozycja_bib!=string::npos){      //jeśli taka fraza występuje to pobiera nazwę biblioteki
      if(linia.find('<')!=string::npos && linia.find('>')!=string::npos){       //pobiera nazwe jeśli biblioteka jest zapisana w <>
        size_t poczatek=linia.find('<',pozycja_bib+8);
        size_t koniec=linia.find('>',poczatek+1);
        if(!((linia[poczatek+1]>='A'&&linia[poczatek+1]<='Z')||(linia[poczatek+1]>='a'&&linia[poczatek+1]<='z'))) continue;
        nazwa=linia.substr(poczatek+1,koniec-(poczatek+1));
        if(!sprawdz_powtorzenie(glowa,nazwa)){
          dodajEnd(glowa,nazwa);
        }
      }
      else if(linia.find('"')!=string::npos){       //pobiera nazwę jeśli biblioteka jest zapisana w " "
        size_t poczatek=linia.find('"',pozycja_bib+8);
        size_t koniec=linia.find('"',poczatek+1);
        if(!((linia[poczatek+1]>='A'&&linia[poczatek+1]<='Z')||(linia[poczatek+1]>='a'&&linia[poczatek+1]<='z'))) continue;
        nazwa=linia.substr(poczatek+1,koniec-(poczatek+1));
        if(!sprawdz_powtorzenie(glowa,nazwa)){
          dodajEnd(glowa,nazwa);
        }
      }
    }
  }
  int liczba= zliczListeDane(glowa);
  if(liczba>0){
    QString licz = QStringLiteral("Ilosc bibliotek w kodzie: %1. ").arg(liczba);
    ui->teWynik->append(licz);  //wypisuje w GUI ilość bibliotek w kodzie
    ui->teWynik->append("Biblioteki uzyte w kodzie:");
    ui->teWynik->append("");
    drukujListeDane(glowa,ui);      //wypisuje nazwy bibliotek
  }
  kasujListeDane(glowa);
}

void szukaj_funkcji(dane *&glowa, ifstream &plik, Ui::MainWindow *ui){
  string linia,nazwa;
  while(true){
    if(plik.eof()==true) {  //jeśli dojdzie do końca kodu ustawia wskaźnik na początek i wychodzi z pętli
      plik.clear();
      plik.seekg(0);
      break;
    }
    getline(plik,linia);
    if((linia.find('(')!=string::npos && linia.find(')')!=string::npos)&&linia.find('(')<linia.find(')')){
      //jesli w lini są nawiasy i pierwszy nawias jest lewy to jest funkcja

      int nawias=linia.find('(');  //pozycja lewego nawiasu = koniec nazwy funkcji
      int pocz=nawias;    //zmienna na początek nazwy funkcji
      int spacje=0;
      while(true){    //petla wyszukująca początku nazwy funkcji (gdy napotka spacje lub poczatek linii)
        if(linia[pocz] == ' ' || pocz==0 || linia[pocz]=='='||linia[pocz]=='<'||linia[pocz]=='>' || linia[pocz]=='!'){
          if(linia[pocz]==' ' && spacje==0){  //jesli napotakmy pierwsza spacje to kontynuacja
            if((linia[pocz-1]==' ') || !((linia[pocz-1]>='a' && linia[pocz-1]<='z') || (linia[pocz-1]>='A' && linia[pocz-1]<='Z'))){
              //jesli sa 2 spacje po sobie lub nie litera przed spacja to dodajemy bez spacji
              spacje=0;
              break;
            }
            spacje++;
            pocz--;
            continue;
          }
          spacje=0;
          break;
        }
        pocz--;
      }
      if(pocz==0){
        nazwa=linia.substr(pocz,nawias-pocz);   //wrzuca nazwe funkcji do stringa nazwa
        nazwa=nazwa+"()";}
      if(linia[pocz] == ' ' || linia[pocz] == '='||linia[pocz]=='<'||linia[pocz]=='>' || linia[pocz]=='!'){
        nazwa=linia.substr(pocz+1,nawias-(pocz+1));   //wrzuca nazwe funkcji do stringa nazwa omijajac pierwszy znak ( spacje lub rowna sie)
        nazwa=nazwa+"()";}
      bool petla=false;
      if(nazwa=="for()" || nazwa=="if()"||nazwa=="while()"||nazwa=="else if()"){
        petla=true;
      }
      if(nazwa.find(" ")!=string::npos){
        string pentla=nazwa.substr(0,nazwa.find(' '));
        if(pentla=="for"||pentla=="if"||pentla=="while") petla=true;
      }
      if(!sprawdz_powtorzenie_funkcji(glowa,nazwa) && petla==false){    //jesli nie ma powtorzenia i nie jest to petla to dodaje do listy funkcji
          dodajEnd(glowa,nazwa);
      }
    }
  }
  int liczba=zliczListeDane(glowa);
  if(liczba>0){
    QString licz = QStringLiteral("Ilosc funkcji w kodzie: %1. ").arg(liczba);
    ui->teWynik->append(licz);  //wypisuje w GUI ilość funkcji użytych w kodzie
    ui->teWynik->append("Funkcje uzyte w kodzie:");
    ui->teWynik->append("");
    drukujListeDane(glowa,ui);      //wypisuje nazwy funkcji
  }
  kasujListeDane(glowa);
}

bool sprawdz_powtorzenie_funkcji(dane *glowa,string nazwa_zmiennej){
  dane *akt=glowa;
  while(akt!=nullptr){  //przechodzi po całej liście jednokierunkowej
    string zapisana=akt->zmienna;
    if(zapisana==nazwa_zmiennej) {return true;} //jeśli któraś z zapisanych danych jest równa sprawdzenej nazwie zwraca true
    if(zapisana.find(' ')!=string::npos){   //wywolanie funkcji bez deklaracji
      int spacja=zapisana.find(' ');
      zapisana=zapisana.substr(spacja+1);
      if(zapisana==nazwa_zmiennej){return true;}
    }
    akt=akt->next;
  }
  return false;
}

void szukaj_struktur(dane *&glowa, ifstream &plik, Ui::MainWindow *ui){
  string linia,nazwa;
  while(true){  //jeśli dojdzie do końca kodu ustawia wskaźnik na początek i wychodzi z pętli
    if(plik.eof()==true) {
      plik.clear();
      plik.seekg(0);
      break;
    }
    getline(plik,linia);
    size_t pozycja_struktury=linia.find("struct "); //szuka frazy "struct " w linijce
    if(pozycja_struktury!=string::npos){    //jeśli znajdzie frazę to pobiera nazwę struktury
        size_t poczatek=pozycja_struktury+6;
        size_t koniec;
        if(linia.find(" ",poczatek+1)<linia.find('{',poczatek+1)){
          koniec=linia.find(' ',poczatek+1);
        }
        else if(linia.find(" ",poczatek+1)>linia.find('{',poczatek+1)){
          koniec=linia.find('{',poczatek+1);
        }
        if(!((linia[poczatek+1]>='A'&&linia[poczatek+1]<='Z')||(linia[poczatek+1]>='a'&&linia[poczatek+1]<='z'))) continue;
        nazwa=linia.substr(poczatek+1,koniec-(poczatek+1));
        if(!sprawdz_powtorzenie(glowa,nazwa)){
          dodajEnd(glowa,nazwa);
        }
    }
  }
  int liczba=zliczListeDane(glowa);
  if(liczba>0){
    QString licz = QStringLiteral("Ilosc struktur w kodzie: %1. ").arg(liczba);
    ui->teWynik->append(licz);  //wypisuje w GUI ilość struktur w kodzie
    ui->teWynik->append("Struktury uzyte w kodzie:");
    ui->teWynik->append("");
    drukujListeDane(glowa,ui);  //wypisuje w GUI nazwy struktur
  }
}

void szukaj_zmiennej(dane *&glowa,ifstream &plik,string zmienna, Ui::MainWindow *ui){
  zmienna=zmienna+" ";
  string linia;
  while(true){ //jeśli dojdzie do końca kodu ustawia wskaźnik na początek i wychodzi z pętli
    if(plik.eof()==true) {
      plik.clear();
      plik.seekg(0);
      break;
    }
    getline(plik,linia);
    int ilosc_zmiennej=zlicz_dane(linia,zmienna); //liczy ilosc wystapienia zmiennej w linijce
    if(ilosc_zmiennej>0){zapisz_dane(glowa,linia,ilosc_zmiennej,zmienna);} //jesli zmienna sie znajduje to zapisuje nazwy zmiennej

  }
  if(glowa!=nullptr){   //jesli lista nie jest pusta to wypisuje informacje o zmiennej
    int liczba=zliczListeDane(glowa);
    QString qliczba = QVariant(liczba).toString();
    QString qzmienna = QString::fromStdString(zmienna);
    QString licz = "Ilosc zmiennej "+qzmienna+"w kodzie wynosi: "+qliczba;
    ui->teWynik->append(licz);  //wypisuje w GUI ilosc zmiennej w kodzie
    QString kom = "Zmienne "+qzmienna+"uzyte w kodzie:";
    ui->teWynik->append(kom);
    ui->teWynik->append("");
    drukujListeDane(glowa,ui);  //wypisuje nazwy danej zmiennej w GUI
  }
  kasujListeDane(glowa);
}


int zlicz_dane(string linia,string zmienna){
    size_t znalezionaPozycja = linia.find(zmienna);
    size_t przeskok=zmienna.size(); //dlugosc nazwy szukanej zmiennej
    if(znalezionaPozycja==string::npos )    //jesli nie ma zmiennej zwraca 0
    {
        return 0;
    }
    else{
      int licznik=1;    //jesli jest licznik startuje z 1 i sprawdza czy dalej z linii po "przeskoku" jest wiecej zmiennych
      if(!((linia[znalezionaPozycja+przeskok]>='A'&&linia[znalezionaPozycja+przeskok]<='Z')||(linia[znalezionaPozycja+przeskok]>='a'&&linia[znalezionaPozycja+przeskok]<='z')||linia[znalezionaPozycja+przeskok]=='*'||linia[znalezionaPozycja+przeskok]=='&')){ licznik=0;}
        do
        {
            znalezionaPozycja = linia.find(zmienna, znalezionaPozycja+1 ); //szuka kolejnej zmiennej w linijce
            if(znalezionaPozycja!=string::npos && ((linia[znalezionaPozycja+przeskok]>='A'&&linia[znalezionaPozycja+przeskok]<='Z')||(linia[znalezionaPozycja+przeskok]>='a'&&linia[znalezionaPozycja+przeskok]<='z'))) licznik++;
        } while(znalezionaPozycja!=string::npos);
        return licznik;
    }
}

void zapisz_dane(dane *&glowa,string linia,int ilosc,string zmienna){
    string nazwa_zmiennej;
    size_t przeskok=zmienna.size();
    size_t pozycja_int=0;
    for(int x=0;x<ilosc;x++){
    pozycja_int=linia.find(zmienna,pozycja_int+x);  //pozycja x-tej zmiennej w linijce
    if(!((linia[pozycja_int+przeskok]>='A'&&linia[pozycja_int+przeskok]<='Z')||(linia[pozycja_int+przeskok]>='a'&&linia[pozycja_int+przeskok]<='z')||linia[pozycja_int+przeskok]=='*'||linia[pozycja_int+przeskok]=='&')){      //sprawdzenie czy to int czy napis w ""
    continue;}
    size_t pozycja_nazwy;
    for(int i=pozycja_int+przeskok;i<linia.size();i++){
      if((linia[i]>='A'&&linia[i]<='Z')||(linia[i]>='a'&&linia[i]<='z')){   //pozycja początku nazwy x-tej zmiennej w linijce
        pozycja_nazwy=i;
        break;
      }
    }
        for(size_t i=pozycja_nazwy;i<linia.size();i++){ //
            if(linia[i]==','){                                    //jesli napotkany przecinek to wrzuca zmienna do tabeli i zmienia poz zmiennej za przecinek
                nazwa_zmiennej=linia.substr(pozycja_nazwy,i-pozycja_nazwy);
                if(nazwa_zmiennej.find('(')!=string::npos || nazwa_zmiennej.find(' ')!=string::npos) continue;
                if(!sprawdz_powtorzenie(glowa,nazwa_zmiennej)) dodajEnd(glowa,nazwa_zmiennej);
                pozycja_nazwy=i+1;
            }
            if(linia[i]==';'){                                   //jesli srednik to zapisujemy zmienna i przerywamy przejscie po linii
                nazwa_zmiennej=linia.substr(pozycja_nazwy,i-pozycja_nazwy);
                if(nazwa_zmiennej.find('(')!=string::npos || nazwa_zmiennej.find(' ')!=string::npos) continue;
                if(!sprawdz_powtorzenie(glowa,nazwa_zmiennej)) dodajEnd(glowa,nazwa_zmiennej);
                break;
            }
            if(linia[i]==')'){                                                //jesli nawias(na koncu funkcji) to zapisujemy zmienna i przerywamy przejscie po linii
                nazwa_zmiennej=linia.substr(pozycja_nazwy,i-pozycja_nazwy);
                if(nazwa_zmiennej.find('(')!=string::npos || nazwa_zmiennej.find(' ')!=string::npos) continue;
                if(!sprawdz_powtorzenie(glowa,nazwa_zmiennej)) dodajEnd(glowa,nazwa_zmiennej);
                break;
            }
            if(linia[i]=='='){                                               //jesli = to zapisujemy zmienna i sprawdzamy czy po wartosci jest kolejna zmienna czy koniec
                nazwa_zmiennej=linia.substr(pozycja_nazwy,i-pozycja_nazwy);
                if(nazwa_zmiennej.find('(')!=string::npos || nazwa_zmiennej.find(' ')!=string::npos) continue;
                if(!sprawdz_powtorzenie(glowa,nazwa_zmiennej)) dodajEnd(glowa,nazwa_zmiennej);
                bool koniec=false;
                for(size_t j=i;j<linia.size();j++){
                    if(linia[j]==';') {koniec=true; break;}
                    else if(linia[j]==',') {
                      string napis=linia.substr(i+1,j-(i+1));
                      if(napis.find('(')!=string::npos){  //po rowna jest wywolana funkcja
                        for(size_t k=i;k<linia.size();k++){
                          if(linia[k]==')'){
                            if(linia[k+1]==';'){
                              koniec=true; break;
                            }
                            else{
                              pozycja_nazwy=k+2;
                              i=k+2;
                              break;}
                            }
                        }
                        break;
                      }
                      else{
                      pozycja_nazwy=j+1; i=j+1; break;}
                      }
                }
                if(koniec==true) break;
            }
        }
    }
}
