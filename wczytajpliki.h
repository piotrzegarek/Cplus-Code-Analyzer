#ifndef WCZYTAJPLIKI_H
#define WCZYTAJPLIKI_H

#include <structheader.h>
#include <mainwindow.h>
#include "analiza.h"
#include "listy.h"
#include <string>
#include <QMainWindow>
#include <QLineEdit>
#include <fstream>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }

QT_END_NAMESPACE

bool wczytajPlik(std::ifstream &plik, std::string nazwa, Ui::MainWindow *ui);
void dodajNazwe(nazwy *&start, std::string nowa_nazwa);
std::string wybierzNazwe(nazwy *start, int i);

#endif // WCZYTAJPLIKI_H
