#ifndef STRUCTHEADER_H
#define STRUCTHEADER_H

#include <string>

struct dane{
    /// Zmienna string przechowująca nazwy szukanych danych
    std::string zmienna;
    /// Zmienna struktury dane zawierająca wskaźnik na kolejny element listy jednokierunkowej
    dane *next;
};

struct nazwy{
    /// Zmienna nazwa przekazująca nazwy plików wgrywanych do programu
    std::string nazwa;
    /// Zmienna struktury nazwy zawierająca wskaźnik na kolejny element listy jednokierunkowej
    nazwy *next;
};


#endif // STRUCTHEADER_H
