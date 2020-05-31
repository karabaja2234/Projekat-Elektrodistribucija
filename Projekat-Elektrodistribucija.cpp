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

char * crt = "\n\t\t======================================================\n";

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
	char JMBG[14];
	korisnickaUloga uloga;
	Racun racuni[12];
	int brojacRacuna = 0;
	
	void pregledRacuna() {
		
	}
	void pregledDuga() {
		
	}
	void platiRacun() {
		
	}
};

//Globalne varijable koje se koriste u cijelom programu
const int maxBrKorisnika = 50;
Korisnik* nizKorisnika = new Korisnik[maxBrKorisnika];
int brojKorisnika = 0;
Administrator admin;

//Funkcija za ucitavanje korisnika iz baze podataka
void ucitajKorisnike() {
	ifstream ucitaj;
	ucitaj.open("bazaPodataka.txt");
	int uloga;
	while(ucitaj >> nizKorisnika[brojKorisnika].korisnik.ime >> nizKorisnika[brojKorisnika].korisnik.prezime >> nizKorisnika[brojKorisnika].korisnik.username >> nizKorisnika[brojKorisnika].korisnik.password >> nizKorisnika[brojKorisnika].idKorisnika >> nizKorisnika[brojKorisnika].JMBG >> uloga) {
		nizKorisnika[brojKorisnika].uloga = (korisnickaUloga)uloga;
		brojKorisnika++;
	}
	ucitaj.close();
}

//Funkcija za ispravan unos novog korisnika
void unosKorisnika() {
	system("cls");
	
	//Ukoliko je broj korisnika 0, tj. ako se unosi prvi korisnik, za njega se provjerava samo unos username-a, koji ne smije biti isti kao adminov
	if(brojKorisnika==0) {
		cout << "\n\n\n\n";
		cout << crt << "\t\t\t::UNOS NOVOG KORISNIKA::" << crt;
		cout << "\t\tRegistracija za " << brojKorisnika+1 << ". korisnika:\n";
		//Unos imena
		cout << "\t\t\tUnesite ime korisnika: ";
		getline(cin, nizKorisnika[brojKorisnika].korisnik.ime);
		cout << "\t\t\tUnesite prezime: ";
		getline(cin, nizKorisnika[brojKorisnika].korisnik.prezime);
		
		//Petlja za unos username-a, koja radi sve dok se ne unese username razlicit od adminovog
		do {
			cout << "\t\t\tUnesite username: ";
			getline(cin, nizKorisnika[brojKorisnika].korisnik.username);
			if(nizKorisnika[brojKorisnika].korisnik.username == admin.administrator.username) {
				cout << "\t\t\tGreska: Uneseni username je vec u upotrebi\n";
			} else break;	
		} while(1);
		
		//Unos passworda (moze biti bilo koji)
		cout << "\t\t\tUnesite password: ";
		getline(cin, nizKorisnika[brojKorisnika].korisnik.password);
		
		//NAPOMENA: osmi i deveti karakter su grad
		//Do while petlja koja radi sve dok se JMBG korisnika ne unese kao broj
		do {
			cout << "\t\t\tUnesite JMBG korisnika: ";
			cin.getline(nizKorisnika[brojKorisnika].JMBG, 14);
			if(strlen(nizKorisnika[brojKorisnika].JMBG)!=13) {	//Uneseni broj mora biti integer, ne prima string
    			cout << "\t\t\tGreska: Uneseni JMBG mora imati 13 cifara\n";
			} else break;
		} while(1);
		
		
		//Do while petlja koja radi sve dok se id korisnika ne unese kao broj
		do {
			bool validan = false;	//Vrijednost za provjeru validnosti indexa
			
			//do while petlja koja radi sve dok se ne pronadje indeks razlicit od svih unesenih do tog trenutka
			do {
				cout << "\t\t\tUnesite ID korisnika: ";
				cin >> nizKorisnika[brojKorisnika].idKorisnika;
				if(nizKorisnika[brojKorisnika].idKorisnika == admin.specijalniID) {
					cout << "\t\t\tGreska: Uneseni ID je vec u upotrebi\n";
					validan = false;
				} else if(!cin) {
					cin.clear();
    				cout << "\t\t\tGreska: Uneseni ID mora biti broj\n";
    				cin.ignore(10000, '\n');
				} else {
					validan=true;
				}	
			} while(validan==false);
			break;
		} while(1);
		
		//Dodjeljivanje default vrijednosti
		nizKorisnika[brojKorisnika].uloga = (korisnickaUloga)korisnik;
		
		//Dodavanje korisnika u bazu podataka
		ofstream dodajKorisnika("bazaPodataka.txt", ios::app);
		dodajKorisnika << nizKorisnika[brojKorisnika].korisnik.ime << " ";
		dodajKorisnika << nizKorisnika[brojKorisnika].korisnik.prezime << " ";
		dodajKorisnika << nizKorisnika[brojKorisnika].korisnik.username << " ";
		dodajKorisnika << nizKorisnika[brojKorisnika].korisnik.password << " ";
		dodajKorisnika << nizKorisnika[brojKorisnika].idKorisnika << " ";
		dodajKorisnika << nizKorisnika[brojKorisnika].JMBG << " ";
		dodajKorisnika << nizKorisnika[brojKorisnika].uloga << endl;
		dodajKorisnika.close();
		
		//Dodavanje datoteke za racune korisnika: ime datoteke ce biti: ImePrezime_racun.txt
		string nazivDatoteke;
		nazivDatoteke = nizKorisnika[brojKorisnika].korisnik.ime + nizKorisnika[brojKorisnika].korisnik.prezime + "_racun.txt";
		ofstream dodajRacun(nazivDatoteke, ios::app);
		for(int i=0; i<nizKorisnika[brojKorisnika].brojacRacuna; i++) {
			dodajRacun << nizKorisnika[brojKorisnika].racuni[i].idRacuna << " " << nizKorisnika[brojKorisnika].racuni[i].iznos << " " 
					   << nizKorisnika[brojKorisnika].racuni[i].status << endl;
		}
		dodajRacun.close();
		brojKorisnika++;
		
	} else if (brojKorisnika>0) {	//Ukoliko je broj korisnika veci od 0, znaci da moramo za sve takve korisnike provjeravati i username i ID, 
									//jer nijedan od njih ne smije imati ni isti username kao prethodno uneseni korinik/admin, kao ni broj ID-a
		cout << "\n\n\n\n";
		cout << crt << "\t\t\t::UNOS NOVOG KORISNIKA::" << crt;
		cout << "\t\tRegistracija za " << brojKorisnika+1 << ". korisnika:\n";
		//Unos imena
		cout << "\t\t\tUnesite ime korisnika: ";
		getline(cin, nizKorisnika[brojKorisnika].korisnik.ime);
		cout << "\t\t\tUnesite prezime: ";
		getline(cin, nizKorisnika[brojKorisnika].korisnik.prezime);
		
		//Petlja za provjeru validnosti unesenog username-a
		do {
			bool validan = false;	//Vrijednost za provjeru validnosti username-a
			
			//do while petlja koja radi sve dok se ne pronadje username razlicit od svih unesenih do tog trenutka, ukljucujuci i username admina
			do {
				cout << "\t\t\tUnesite username: ";
				getline(cin, nizKorisnika[brojKorisnika].korisnik.username);
				for(int i=0; i<brojKorisnika; i++) {
					if(nizKorisnika[brojKorisnika].korisnik.username == nizKorisnika[i].korisnik.username || nizKorisnika[brojKorisnika].korisnik.username == admin.administrator.username) {
						cout << "\t\t\tGreska: Uneseni username je vec u upotrebi\n";
						validan = false;
						break;
					} else validan=true;	
				}	
			} while(validan==false);
			break;
		} while(1);
		
		//Unos passworda (moze biti bilo koji)
		cout << "\t\t\tUnesite password: ";
		getline(cin, nizKorisnika[brojKorisnika].korisnik.password);
		
		//NAPOMENA: osmi i deveti broj su grad
		//do while petlja koja radi sve dok se ne pronadje JMBG dug 13 karaktera, te razlicit od svih unesenih JMBG-ova do tog trenutka
		do {
			bool validan = false;	//Vrijednost za provjeru validnosti indexa
			do {
				cout << "\t\t\tUnesite JMBG korisnika: ";
				cin.getline(nizKorisnika[brojKorisnika].JMBG, 14);
				for(int i=0; i<brojKorisnika; i++) {
					if(strcmp(nizKorisnika[brojKorisnika].JMBG, nizKorisnika[i].JMBG) == 0) {
						cout << "\t\t\tGreska: Uneseni JMBG je vec u upotrebi\n";
						validan = false;
					} else if(strlen(nizKorisnika[brojKorisnika].JMBG)!=13) {
    					cout << "\t\t\tGreska: Uneseni JMBG mora imati 13 cifara\n";
    					validan = false;
					} else {
						validan=true;
					}
					break;	
				}	
			} while(validan==false);
			break;
		} while(1);
		
		//do while petlja koja radi sve dok se ne unese ID koji je broj i koji je razlicit od svih prethodno unesenih ID-eva
		do {
			bool validan = false;	//Vrijednost za provjeru validnosti indexa
			
			//do while petlja koja radi sve dok se ne pronadje indeks razlicit od svih unesenih do tog trenutka
			do {
				cout << "\t\t\tUnesite ID korisnika: ";
				cin >> nizKorisnika[brojKorisnika].idKorisnika;
				for(int i=0; i<brojKorisnika; i++) {
					if(nizKorisnika[brojKorisnika].idKorisnika == nizKorisnika[i].idKorisnika || nizKorisnika[brojKorisnika].idKorisnika == admin.specijalniID) {
						cout << "\t\t\tGreska: Uneseni ID je vec u upotrebi\n";
						validan = false;
					} else if(!cin) {
						cin.clear();
    					cout << "\t\t\tGreska: Uneseni ID mora biti broj\n";
    					cin.ignore(10000, '\n');
					} else {
						validan=true;
					}
					break;	
				}	
			} while(validan==false);
			break;
		} while(1);
	
		//Dodjeljivanje default vrijednosti
		nizKorisnika[brojKorisnika].uloga = (korisnickaUloga)korisnik;
		
		//Dodavanje korisnika u bazu podataka
		ofstream dodajKorisnika("bazaPodataka.txt", ios::app);
		dodajKorisnika << nizKorisnika[brojKorisnika].korisnik.ime << " ";
		dodajKorisnika << nizKorisnika[brojKorisnika].korisnik.prezime << " ";
		dodajKorisnika << nizKorisnika[brojKorisnika].korisnik.username << " ";
		dodajKorisnika << nizKorisnika[brojKorisnika].korisnik.password << " ";
		dodajKorisnika << nizKorisnika[brojKorisnika].idKorisnika << " ";
		dodajKorisnika << nizKorisnika[brojKorisnika].JMBG << " ";
		dodajKorisnika << nizKorisnika[brojKorisnika].uloga << endl;
		dodajKorisnika.close();
		
		//Dodavanje datoteke za racune korisnika: ime datoteke ce biti: ImePrezime_racun.txt
		string nazivDatoteke;
		nazivDatoteke = nizKorisnika[brojKorisnika].korisnik.ime + nizKorisnika[brojKorisnika].korisnik.prezime + "_racun.txt";
		ofstream dodajRacun(nazivDatoteke, ios::app);
		for(int i=0; i<nizKorisnika[brojKorisnika].brojacRacuna; i++) {
			dodajRacun << nizKorisnika[brojKorisnika].racuni[i].idRacuna << " " << nizKorisnika[brojKorisnika].racuni[i].iznos << " " 
					   << nizKorisnika[brojKorisnika].racuni[i].status << endl;
		}
		dodajRacun.close();
		brojKorisnika++;
	}
}

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

//Poseban meni kojem moze pristupiti samo admin
int adminMeni() {
	int izbor;
	cout << "\n\n\n\n";
	do {
		cout << crt << "\t\t\t\t::ADMIN MENI::" << crt;
		cout << "\t\t1. Pregled svih korisnika" << endl;
		cout << "\t\t2. Pregled svih racuna" << endl;
		cout << "\t\t3. Dodavanje racuna" << endl;
		cout << "\t\t4. Registracija novog korisnika" << endl;
		cout << "\t\t5. Brisanje racuna" << endl;
		cout << "\t\t6. Pretraga korisnika" << endl;
		cout << "\t\t7. Evidencija racuna" << endl;
		cout << "\t\t8. Ispis uplatnice za korisnika" << endl;
		cout << "\t\t9. Kraj rada" << crt;
		cout << "\t\tUnesite izbor: ";
		cin >> izbor;
	} while (izbor<1 || izbor>9);
	return izbor;
}

void pregledKorisnika(){
	cout << crt << "\t\t\t\t::PREGLED KORISNIKA::" << crt;
	cout << "\t\t" << setw(5) << left << "ID" << setw(15) << left << "Ime" << setw(15) << left << "Prezime" << setw(15) << left << "JMBG" << endl; 
	for(int i=0; i<brojKorisnika; i++) {
		cout << "\t\t" << setw(5) << left << nizKorisnika[i].idKorisnika << setw(15) << left << nizKorisnika[i].korisnik.ime << setw(15) 
			 << left << nizKorisnika[i].korisnik.prezime << setw(15) << left << nizKorisnika[i].JMBG << endl; 
	}
}

int prikaziKorisnike() {
	if (brojKorisnika == 0) {
		cout << "\t\tTrenutno nema registrovanih korisnika!\n";
		Sleep(2000);
		system("cls");
	} else {
		int rb;
		cout << "\t\t" << "Rb." << setw(5) << left << "\tID" << setw(15) << left << " Ime" << setw(15) << left << " Prezime" << setw(15) << left << " JMBG" << endl;
		for (int i=0; i<brojKorisnika; i++) {
			cout << "\t\t" << i+1 << ".\t" << setw(5) << left << nizKorisnika[i].idKorisnika << setw(15) << left << nizKorisnika[i].korisnik.ime << setw(15) 
			 << left << nizKorisnika[i].korisnik.prezime << setw(15) << left << nizKorisnika[i].JMBG << endl; 
		}
		do {
			cout << "\t\tUnesite redni broj korisnika: ";
			cin >> rb;
		} while(rb<1 || rb>brojKorisnika);
		return rb-1;
	}
}

Racun napraviRacun(string imeDatoteke) {
	Racun noviRacun;
	cout << crt << "\t\t\t\t::UNOS RACUNA::" << crt;
	do {
		ifstream ucitajRacune;
		ucitajRacune.open(imeDatoteke);
		int ID, placen;
		float iznos;
		int brojac = 0;
		Racun ucitaniRacuni[12];
		while(ucitajRacune >> ucitaniRacuni[brojac].idRacuna >> ucitaniRacuni[brojac].iznos >> placen) {
			ucitaniRacuni[brojac].status = (Status)placen;
			brojac++;
		}
		ucitajRacune.close();
		for(int i=0; i<brojac; i++) {
			cout << "\t\t\tBROJ: " << ucitaniRacuni[i].idRacuna << endl;
		} 
		cout << "BROJAC: " << brojac << endl;
		bool validan = false;	//Vrijednost za provjeru validnosti indexa	
		//do while petlja koja radi sve dok se ne pronadje indeks razlicit od svih unesenih do tog trenutka
		do {
			cout << "\t\tUnesite ID racuna: ";
			cin >> noviRacun.idRacuna;
			for(int i=0; i<brojac; i++) {
				if (noviRacun.idRacuna == ucitaniRacuni[i].idRacuna) {
					cout << "\t\t\tGreska: Uneseni ID racuna vec postoji\n";
					cout << "OVDJE: " << i << endl;
					validan = false;
					break;
				} else if (noviRacun.idRacuna != ucitaniRacuni[i].idRacuna){
					validan = true;
				} else {
					validan = true;
				}	
			}	
		} while(validan==false);
		break;
	} while(1);
	
	cout << "\t\tUnesite iznos racuna: "; cin >> noviRacun.iznos;
	noviRacun.status = (Status)neplacen;
	return noviRacun;
}


void azurirajRacuneZaKorisnika(Korisnik korisnik, string imeDatoteke) {
	ifstream ucitajRacune;
	ucitajRacune.open(imeDatoteke);
	int status;
	while(ucitajRacune >> korisnik.racuni[korisnik.brojacRacuna].idRacuna >> korisnik.racuni[korisnik.brojacRacuna].iznos >> status) {
		korisnik.racuni[korisnik.brojacRacuna].status = (Status)status;
		korisnik.brojacRacuna++;
	}
	ucitajRacune.close();
}

void ispisiRacuneKorisnika(Korisnik korisnik, string imeDatoteke) {
	ofstream ispisiRacune;
	ispisiRacune.open(imeDatoteke, ios::app);
	for(int i=0; i<korisnik.brojacRacuna; i++) {
		ispisiRacune << korisnik.racuni[i].idRacuna << " " << korisnik.racuni[i].iznos << " " << korisnik.racuni[i].status << endl;
	}
	ispisiRacune.close();
}


/*
void dodajRacun(){
	cout << crt << "\t\t\t\t::ODABERITE KORISNIKA::" << crt;
	Korisnik &korisnik = nizKorisnika[prikaziKorisnike()];
	string imeDatoteke;
	imeDatoteke = korisnik.korisnik.ime + korisnik.korisnik.prezime + "_racun.txt";
	azurirajRacuneZaKorisnika(korisnik, imeDatoteke);
}*/

void pregledRacunaKorisnika(Korisnik korisnik){
	cout << crt << "\t\t\t\t::RACUNI KORISNIKA::" << crt;
	string imeDatoteke = korisnik.korisnik.ime + korisnik.korisnik.prezime + "_racun.txt";
	//azurirajRacuneZaKorisnika(korisnik, imeDatoteke);
	cout << endl << endl;
	cout << "\t\t" << setw(5) << left << "ID" << setw(10) << left << "Iznos" << setw(10) << left << "Status" << setw(20) << left << "Datum placanja" << endl; 
	for(int i=0; i<korisnik.brojacRacuna; i++) {
		cout << "\t\t" << setw(10) << left << korisnik.racuni[i].idRacuna << setw(10) << left << korisnik.racuni[i].iznos << setw(10) << left << korisnik.racuni[i].status << setw(20) << left;
		info(korisnik.racuni[i].datum_placanja);
		cout << endl;
	}
}

void obrisiRacun(Korisnik korisnik){
	
}

void evidentirajRacune(Korisnik korisnik){
	
}

void ispisUplatnice(Korisnik korisnik){
	
}

//Poseban meni kojem moze pristupiti bilo koji od korisnika
int korisnikMeni() {
	int izbor;
	cout << "\n\n\n\n";
	do {
		cout << crt << "\t\t\t\t::KORISNIK MENI::" << crt;
		cout << "\t\t1. Pregled racuna" << endl;
		cout << "\t\t2. Pregled duga" << endl;
		cout << "\t\t3. Plati racun " << endl;
		cout << "\t\t4. Kraj rada" << crt;
		cout << "\t\tUnesite izbor: ";
		cin >> izbor;
	} while (izbor<1 || izbor>4);
	return izbor;
}

int loginMeni() {
	int izbor;
	cout << "\n\n\n\n";
	do {
		cout << crt << "\t\t\t\t::Login / Register::" << crt;
		cout << "\t\t\t\t1. Login" << endl;
		cout << "\t\t\t\t2. Registrujte novog usera" << endl;
		cout << "\t\t\t\t3. Napustite sistem" << endl;
		cout << "\n\t\t\t\tUnesite izbor: ";
		cin >> izbor;
		cin.ignore();
	} while (izbor<1 || izbor>3);
	return izbor;
}

//Funkcija koja unosi tekst za prosljedjene varijable username-a i passworda, te vraca odredjene vrijednosti na osnovu unesenih podataka
int login(string username, string password) {
	//Unos username-a i passworda
	cout << "\n\n\n\n";
	cout << crt << "\t\t\t\t\t::Login::" << crt;
	cout << "\t\t\t\tUsername: ";
	getline(cin, username);
	cout << "\t\t\t\tPassword: ";
	getline(cin, password);
	//Provjera ukoliko je uneseni username/password admina
	if(username == admin.administrator.username && password == admin.administrator.password) {
		return 234781; //Vraca se unikatna vrijednost samo za tu osobu
	} else {
		//U suprotnom, prolazi se kroz niz korisnika koji su registrovani, te funkcija vraca id tog korisnika
		for (int i=0; i<brojKorisnika; i++) {
			if(username == nizKorisnika[i].korisnik.username && password == nizKorisnika[i].korisnik.password) {
				return nizKorisnika[i].idKorisnika;
			}
		}
	}
	cin.ignore();
}

/*
Student &st = nizStudenata[prikaziStudente()];
st.radioTest = false;
*/


int main () {

	//Dodaju se informacije za admina (koji je u nasem slucaju samo jedan)
	admin.administrator.ime = "Grupa";					//Ime admina
	admin.administrator.prezime = "osam";				//Prezime admina
	admin.administrator.username = "admin";				//Username: selman
	admin.administrator.password = "admin";				//Password: admin
	admin.specijalniID = 234781;						//Specijalni ID po kojem je admin prepoznatljiv
	admin.uloga = (korisnickaUloga)administrator;		//Default uloga admina	

	ucitajKorisnike();	//Ucitavanje svih korisnika iz datoteke "bazaPodataka.txt" u niz "nizKorisnika"
	//unosKorisnika();
	
	int odabir;	//Varijabla za odabir panela u glavnom meniju
	system("cls");
	
	//cout << "Broj studenata: " << brojStudenata << "\n\n";
	//Do while petlja koja radi sve dok se ne unese vrijednost 3, kojom se glavni meni zatvara
	do {
		int ID;	//Varijabla u koju se smjesta rezultat funkcije login()
		string username, password;
		system("cls");
		odabir = loginMeni();	//Poziva se glavni meni i njegova vrijednost se smjesta u varijablu "odabir"
		//Ukoliko je "odabir" 3 petlja se prekida
		if(!cin) {
			cin.clear();
    		cout << "\tGreska: Unesite broj od 1 do 3\n";
    		cin.ignore(100000, '\n');
		} else if (odabir == 3) {
				break;
			} else if (odabir == 1) {	//Ukoliko je "odabir" 1, provjerava se da li vrijednost ID pripada adminu ili studentima
				system("cls");
				ID = login(username, password);
				system("cls");
				if(ID == admin.specijalniID) {	//Ako pripada adminu, otvara se poseban "adminMeni()" za njega, u kojem ima 7 opcija
					system("cls");
					int izbor;
					do {
						izbor = adminMeni();
						if(izbor == 9) {
							break;
						}
						if (izbor == 1) {	//Opcija 1: 
							system("cls");
							pregledKorisnika();
						} else if (izbor == 2) {	//Opcija 2: 
							system("cls");
							cout << crt << "\t\t\t\t::ODABERITE KORISNIKA::" << crt;
							Korisnik korisnik = nizKorisnika[prikaziKorisnike()];
							string imeDatoteke = korisnik.korisnik.ime + korisnik.korisnik.prezime + "_racun.txt";
							azurirajRacuneZaKorisnika(korisnik, imeDatoteke);
							pregledRacunaKorisnika(korisnik);
						} else if (izbor == 3) {	//Opcija 3: 
							system("cls");
							cout << crt << "\t\t\t\t::ODABERITE KORISNIKA::" << crt;
							Korisnik odabraniKorisnik = nizKorisnika[prikaziKorisnike()];
							string imeDatoteke = odabraniKorisnik.korisnik.ime + odabraniKorisnik.korisnik.prezime + "_racun.txt";
							azurirajRacuneZaKorisnika(odabraniKorisnik, imeDatoteke);
							odabraniKorisnik.racuni[odabraniKorisnik.brojacRacuna++] = napraviRacun(imeDatoteke);
							ispisiRacuneKorisnika(odabraniKorisnik, imeDatoteke);
							//azurirajRacuneZaKorisnika(korisnik, imeDatoteke);
							//dodajRacun();
							/*
							azurirajRacuneZaKorisnika(korisnik, imeDatoteke);
							ofstream ispisiRacune;
							ispisiRacune.open(imeDatoteke, ios::app);
							for(int i=0; i<korisnik.brojacRacuna; i++) {
								ispisiRacune << korisnik.racuni[i].idRacuna << " " << korisnik.racuni[i].iznos << " " << korisnik.racuni[i].status << endl;
							}*/
						} else if (izbor == 4) {	//Opcija 4: 
							
						} else if (izbor == 5) {	//Opcija 5: 
							
						} else if (izbor == 6) {	//Opcija 6: 
							
						} else if (izbor == 7) {	//Opcija 7: 
							
						} else if (izbor == 8) {	//Opcija 8: 
							
						}
							system("PAUSE");
							system("cls");
					} while(1);				//Kraj admin menija
				} else {	//Ako ID nije pripadao adminu, onda se kroz for petlju ispod provjerava kojem studentu pripada broj indexa
					for(int i=0; i<brojKorisnika; i++) {
						if(ID == nizKorisnika[i].idKorisnika) {	//Ako petlja nadje datog studenta, pokrece se studentMeni() putem do while petlje ispod
							do {
								int izbor;
								izbor = korisnikMeni();
								if (izbor == 4) {	//Opcija 4: zatvara korisnik meni
									break;
								} else if (izbor == 1) {	//Opcija 1:
									
								
								} else if (izbor == 2) {	//Opcija 2: Student moze pregledati rezultate testova svih studenata
									
								} else if (izbor == 3) {
									
								}
									system("PAUSE");
									system("cls");	
							} while(1);	//Kraj student menija
							break;	//Prekid za for petlju nakon sto nadje odgovarajuceg studenta	
						} else {
							//Za pogresno unesenu sifru/username sistem nece nista uraditi, nego ce se ponovo ucitati login meni
						}
					}
				}
			} else if (odabir == 2) {	//Opcija 2 iz glavnog menija: Registracija novog studenta
				unosKorisnika();
				cout << "\n\t\t="; Sleep(100); cout << "="; Sleep(100); cout << "="; Sleep(100); cout << "=";
				Sleep(100); cout << "="; Sleep(100); cout << " Korisnik uspjesno registrovan!\n";
				Sleep(1500);
				system("cls");
			} 
		} while(1);	

	system("PAUSE");
	return 0;
}

