#ifndef LISTY_H
#define LISTY_H

#include <structheader.h>
#include <mainwindow.h>
#include <fstream>
#include "wczytajpliki.h"
#include "analiza.h"
#include <string>
#include <QMainWindow>
#include <QLineEdit>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


void dodajEnd(dane *&glowa, std::string nazwa);
int zliczListeDane(dane *glowa);
int zliczListeNazwy(nazwy *start);
void kasujListeDane(dane *&glowa);
void kasujListeNazwy(nazwy *&start);
void drukujListeDane(dane *glowa, Ui::MainWindow *ui);

#endif // LISTY_H
