#include <cstdlib>
#include <iostream>
#include <cmath>
#include <iomanip>
#include <cstring>
#include <string.h>
#include <windows.h>
#include <sstream>
#include <stdio.h>
#include <fstream>

using namespace std;

//Enumeracija za korisnicku ulogu osobe u sistemu
enum korisnickaUloga {
	korisnik, administrator
};

//Enumeracija za status racuna (da li je placen ili ne)
enum Status {
	placen, neplacen
};

//Stuktura za datum
struct Datum{
	int dan, mjesec, godina;
};

//Struktura za svaku osobu koja se registruje u sistemu
//Sadrzi samo najvaznije informacije po kojima je osoba prepoznatljiva
struct Osoba {
	string ime, prezime, username, password;	
};

//Struktura za racune
struct Racun {
	int idRacuna;
	float iznos;
	Status status;
	Datum datum_placanja;
};

//Struktura za administratora
struct Administrator {
	Osoba administrator;
	int specijalniID;
	korisnickaUloga uloga;
};

//Struktura za korisnike
struct Korisnik {
	Osoba korisnik;
	int idKorisnika;
	int JMBG;
	korisnickaUloga uloga;
	Racun racuni[50];
};

//Funkcija za ispis datuma
void info(Datum datum){
	cout << setfill('0') << setw(2)
	     << datum.dan << "." << setfill('0')
	     << setw(2) << datum.mjesec << "."
	     << datum.godina << ".";
}

//Funkcija za provjeru ispravnosti datuma
bool provjera(int dan, int mjesec, int godina) {
	if (dan > 0 && mjesec > 0 && mjesec < 13 && godina > 0) {		//Prvi uslov provjerava da li su je dan, mjesec i godina pozitivni brojevi, ukljucujuci i uslov da je mjesec manji od 13
	//Provjera za prestupne godine (mjesec februar mora biti manji od 30)
      if (godina % 4 == 0 && godina > 0 && mjesec > 0 && mjesec < 13 && dan > 0) {
        if (mjesec == 2) {
          if (dan > 0 && dan < 30) {
            return true;
          } else return false;
        } else if (mjesec == 1 || mjesec == 3 || mjesec == 5 || mjesec == 7 || mjesec == 8 || mjesec == 10 || mjesec == 12) {
          if (dan > 0 && dan < 32) {
            return true;
          } else return false;
        } else {
          if (dan > 0 && dan < 31) {
            return true;
          } else return false;
        }
        //Provjera za godine koje nisu prestupne (mjesec februar mora biti manji od 29)
      } else {
        if (mjesec == 2) {
          if (dan > 0 && dan < 29) {
            return true;
          } else return false;
        } else if (mjesec == 1 || mjesec == 3 || mjesec == 5 || mjesec == 7 || mjesec == 8 || mjesec == 10 || mjesec == 12) {
          if (dan > 0 && dan < 32) {
            return true;
          } else return false;
        } else {
          if (dan > 0 && dan < 31) {
            return true;
          } else return false;
        }
      }
    } else return false;
    //Funkcija vraca 1 ako je datum ispravan, a 0 ako nije
}


int main () {







	system("PAUSE");
	return 0;
}

