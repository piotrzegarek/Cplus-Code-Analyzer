#ifndef ANALIZA_H
#define ANALIZA_H

#include <structheader.h>
#include <mainwindow.h>
#include <fstream>
#include "wczytajpliki.h"
#include <string>
#include <QMainWindow>
#include <QLineEdit>
#include "listy.h"
#include <QtCharts>
#include <QChartView>
#include <QBarSeries>
#include <QBarSet>
#include <QtCharts/QBarSet>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


void analizujKod(std::ifstream &plik, dane *&glowa, dane *&struktury, Ui::MainWindow *ui);
void dlugoscKodu(std::ifstream &plik, Ui::MainWindow *ui);
bool sprawdz_powtorzenie(dane *glowa, std::string nazwa_zmiennej);
void szukaj_bibliotek(dane *&glowa, std::ifstream &plik, Ui::MainWindow *ui);
void szukaj_funkcji(dane *&glowa, std::ifstream &plik, Ui::MainWindow *ui);
bool sprawdz_powtorzenie_funkcji(dane *glowa, std::string nazwa_zmiennej);
void szukaj_struktur(dane *&glowa, std::ifstream &plik, Ui::MainWindow *ui);
void szukaj_zmiennej(dane *&glowa, std::ifstream &plik, std::string zmienna, Ui::MainWindow *ui);
int zlicz_dane(std::string linia, std::string zmienna);
void zapisz_dane(dane *&glowa, std::string linia, int ilosc, std::string zmienna);

#endif // ANALIZA_H
