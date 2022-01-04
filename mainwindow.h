#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <structheader.h>
#include "wczytajpliki.h"
#include "analiza.h"
#include "listy.h"
#include <QMainWindow>
#include <string>
#include <QLineEdit>
#include <QtCharts>
#include <QChartView>
#include <QBarSeries>
#include <QBarSet>
#include <QtCharts/QBarSet>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    /// Zmienna struktury dane jako wskaźnik na początek listy jednokierunkowej na dane.
    dane *glowa=nullptr;
    /// Zmienna struktury dane jako wskaźnik na początek listy jednokierunkowej na nazwy struktur.
    dane *struktury=nullptr;
    /// Zmienna struktury nazwy jako wskaźnik na początek listy jednokierunkowej na nazwy plików do wczytania.
    nazwy *start=nullptr;


private slots:

    void on_pbDodajPlik_clicked();
    void on_pbZamknijProgram_clicked();
    void on_pdAnalizuj_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
