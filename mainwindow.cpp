#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>
#include "wczytajpliki.h"
#include "analiza.h"
#include "listy.h"
#include <QLineEdit>
#include <fstream>
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}


MainWindow::~MainWindow()
{
    delete ui;
    delete glowa;
    delete struktury;
    delete start;
}


void MainWindow::on_pbDodajPlik_clicked()
{
    QString nazwa_pliku = QFileDialog::getOpenFileName(this, "Open a file", "C://");        //funkcja zwraca nazwe pliku wybranego przez uzytkownika
    std::string nazwa = nazwa_pliku.toStdString();
    dodajNazwe(start,nazwa);                                        //wpisuje nazwe pliku do listy plikow do analizy
    std::size_t nawias = nazwa.find_last_of("/");
    nazwa = nazwa.substr(nawias+1);
    ui->teWpisanePliki->append(QString::fromStdString(" •  " + nazwa));
}

void MainWindow::on_pbZamknijProgram_clicked()      //zamyka program po nacisnieciu przycisku "zamknij program"
{
    kasujListeDane(glowa);      //czysci liste dynamiczna z danymi
    kasujListeNazwy(start);     //czysci liste dynamiczna z nazwami plikow
    QCoreApplication::quit();
}

void MainWindow::on_pdAnalizuj_clicked()
{
    int ilosc_plikow = zliczListeNazwy(start);
    QString ilosc_plikow_napis = QStringLiteral("Liczba wpisanych plikow wynosi: %1. ").arg(ilosc_plikow);
    ui->teWynik->append(ilosc_plikow_napis);            //wyswietlenie informacji o ilosci plikow
    std::ifstream plik;
    for(int i=0; i<ilosc_plikow; i++){                  //petla dla analizowania kazdego wpisanego pliku
        std::string nazwa = wybierzNazwe(start,i);           //wybor nazw pliku po kolei
        if(wczytajPlik(plik,nazwa, ui)){                  //wczytanie pliku do odczytu
            analizujKod(plik,glowa,struktury,ui);
            ui->teWynik->append("---------------------------");
        }
        plik.close();                                   //zamkniecie pliku po skonczeniu analizy
    }
}
