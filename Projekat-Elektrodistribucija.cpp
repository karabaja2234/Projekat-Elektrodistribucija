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
	int grad;
	
	//Funkcija za postavljanje vrijednosti grada na osnovu JMBG-a kako bi se znalo iz kojeg grada dolazi korisnik
	void postaviGradKorisnika() {
		char osmi, deveti;
		osmi = this->JMBG[7];
		deveti = this->JMBG[8];
		int prvaCifra = (int)osmi - 48;
		int drugaCifra = (int)deveti - 48;
		int brojGrada = prvaCifra*10 + drugaCifra;
		this->grad = brojGrada;
	}
	
	//Funkcija za pregled racuna datog korisnika
	void pregledRacuna() {
		string imeDatoteke = this->korisnik.ime + this->korisnik.prezime + "_racuni.txt";
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
		cout << crt << "\t\t\t\t::PREGLED RACUNA::" << crt;
		ucitajRacune.close();
		if(brojac==0) cout << "\n\t\tKorisnik trenutno nema racuna!\n\n";
		else {
			cout << endl;
			cout << "\t\t" << setw(5) << left << "ID" << setw(6) << left << "Iznos" << setw(15) << left << "\tStatus" << endl; 
			for(int i=0; i<brojac; i++) {
				cout << "\t\t" << setw(5) << left << ucitaniRacuni[i].idRacuna << setw(6) << left << ucitaniRacuni[i].iznos << "KM |";
				cout << setw(15) << left;
				if(ucitaniRacuni[i].status == 0) {
					cout << " Placen" << endl;
				} else cout << " Neplacen" << endl;
			}	
			cout << endl;
		}	
	}
	
	//Funkcija koja povlaci sve racune iz datoteke korisnika, te nudi odabir istih
	Racun odaberiRacun() {
		string imeDatoteke = this->korisnik.ime + this->korisnik.prezime + "_racuni.txt";
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
		cout << crt << "\t\t\t\t::PREGLED RACUNA::" << crt;
		cout << endl;
		cout << "\t\t" << "Rb." << setw(5) << left << "\tID" << setw(6) << left << " Iznos" << setw(15) << left << " \tStatus" << endl; 
		for(int i=0; i<brojac; i++) {
			cout << "\t\t" << i+1 << ".\t" << setw(5) << left << ucitaniRacuni[i].idRacuna << setw(6) << left << ucitaniRacuni[i].iznos << "KM |";
			cout << setw(15) << left;
			if(ucitaniRacuni[i].status == 0) {
				cout << " Placen" << endl;
			} else cout << " Neplacen" << endl;
		}
		
		int rb;
		do {
			cout << "\n\t\tUnesite redni broj racuna: ";
			cin >> rb;
		} while(rb<1 || rb>brojac);
		return ucitaniRacuni[rb-1];	
	}
	
	//Rekurzivna funkcija za racunanje ukupnog duga neplacenih racuna
	float dug(float iznosi[], int brojac) {
		if(brojac == 0) {
			return 0;
		} else {
			return iznosi[brojac-1] + dug(iznosi,brojac-1);		
		}
	}
	
	//Funkcija koja uz pomoc rekurzivne funkcije "dug" vraca vrijednost ukupnog duga datog korisnika
	void pregledDuga() {
		string imeDatoteke = this->korisnik.ime + this->korisnik.prezime + "_racuni.txt";
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
		float iznosi[12];
		int brojacIznosa = 0;
		for(int i=0; i<brojac; i++) {
			if(ucitaniRacuni[i].status == 1) {
				iznosi[brojacIznosa] = ucitaniRacuni[i].iznos;
				brojacIznosa++;
			}
		}
		ucitajRacune.close();
		cout << "\n\n\n\n";
		cout << crt << "\t\t\t\t::PREGLED DUGA::" << crt;
		cout << endl;
		float suma = 0;
		cout << "\t\tUkupan dug koji treba platiti iznosi: " << this->dug(iznosi, brojac) << "\n\n";
	}
	
	//Funkcija za placanje racuna
	void platiRacun() {
		string imeDatoteke = this->korisnik.ime + this->korisnik.prezime + "_racuni.txt";
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
		cout << "\n\n\n\n";
		cout << crt << "\t\t\t\t::PREGLED RACUNA::" << crt;
		if(brojac==0) cout << "\n\t\tKorisnik trenutno nema racuna!\n\n";
		else {
			cout << endl;
			cout << "\t\t" << "Rb." << setw(5) << left << "\tID" << setw(15) << left << " Iznos" << setw(15) << left << " \tStatus" << endl;
			for(int i=0; i<brojac; i++) {
				cout << "\t\t" << i+1 << ".\t" << setw(5) << left << ucitaniRacuni[i].idRacuna << setw(6) << left << ucitaniRacuni[i].iznos << "KM |";
				cout << setw(15) << right;
				if(ucitaniRacuni[i].status == 0) {
					cout << " Placen" << endl;
				} else cout << " Neplacen" << endl;
			}
		
			int rb;
			do {
				cout << "\n\t\tUnesite redni broj racuna: ";
				cin >> rb;
			} while(rb<1 || rb>brojac);
		
			ucitaniRacuni[rb-1].status = (Status)0;
			ofstream obnoviRacune;
			obnoviRacune.open(imeDatoteke, ios::trunc);
			for(int i=0; i<brojac; i++) {
				obnoviRacune << ucitaniRacuni[i].idRacuna << " " << ucitaniRacuni[i].iznos << " " << ucitaniRacuni[i].status << endl;
			}
			obnoviRacune.close();	
		
			cout << "\n\t\t="; Sleep(100); cout << "="; Sleep(100); cout << "="; Sleep(100); 
			cout << "="; Sleep(100); cout << "="; Sleep(100); cout << " Racun uspjesno placen!\n"; Sleep(1500);
		}	
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
	while(ucitaj >> nizKorisnika[brojKorisnika].korisnik.ime >> nizKorisnika[brojKorisnika].korisnik.prezime >> nizKorisnika[brojKorisnika].korisnik.username >> nizKorisnika[brojKorisnika].korisnik.password >> nizKorisnika[brojKorisnika].idKorisnika >> nizKorisnika[brojKorisnika].JMBG >> uloga >> nizKorisnika[brojKorisnika].grad) {
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
		cout << crt << "\t\t\t\t::UNOS NOVOG KORISNIKA::" << crt;
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
			bool validan = false;	//Vrijednost za provjeru validnosti ID-a
			
			//do while petlja koja radi sve dok se ne pronadje ID razlicit od svih unesenih do tog trenutka
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
		nizKorisnika[brojKorisnika].postaviGradKorisnika();
		
		//Dodavanje korisnika u bazu podataka
		ofstream dodajKorisnika("bazaPodataka.txt", ios::app);
		dodajKorisnika << nizKorisnika[brojKorisnika].korisnik.ime << " ";
		dodajKorisnika << nizKorisnika[brojKorisnika].korisnik.prezime << " ";
		dodajKorisnika << nizKorisnika[brojKorisnika].korisnik.username << " ";
		dodajKorisnika << nizKorisnika[brojKorisnika].korisnik.password << " ";
		dodajKorisnika << nizKorisnika[brojKorisnika].idKorisnika << " ";
		dodajKorisnika << nizKorisnika[brojKorisnika].JMBG << " ";
		dodajKorisnika << nizKorisnika[brojKorisnika].uloga << " ";
		dodajKorisnika << nizKorisnika[brojKorisnika].grad << endl;
		dodajKorisnika.close();
		
		//Dodavanje datoteke za racune korisnika: ime datoteke ce biti: ImePrezime_racun.txt
		string nazivDatoteke;
		nazivDatoteke = nizKorisnika[brojKorisnika].korisnik.ime + nizKorisnika[brojKorisnika].korisnik.prezime + "_racuni.txt";
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
		cout << crt << "\t\t\t\t::UNOS NOVOG KORISNIKA::" << crt;
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
		
		//do while petlja koja radi sve dok se ne pronadje JMBG dug 13 karaktera, te razlicit od svih unesenih JMBG-ova do tog trenutka
		do {
			bool validan = false;	//Vrijednost za provjeru validnosti JMBG-a
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
		nizKorisnika[brojKorisnika].postaviGradKorisnika();
		
		//Dodavanje korisnika u bazu podataka
		ofstream dodajKorisnika("bazaPodataka.txt", ios::app);
		dodajKorisnika << nizKorisnika[brojKorisnika].korisnik.ime << " ";
		dodajKorisnika << nizKorisnika[brojKorisnika].korisnik.prezime << " ";
		dodajKorisnika << nizKorisnika[brojKorisnika].korisnik.username << " ";
		dodajKorisnika << nizKorisnika[brojKorisnika].korisnik.password << " ";
		dodajKorisnika << nizKorisnika[brojKorisnika].idKorisnika << " ";
		dodajKorisnika << nizKorisnika[brojKorisnika].JMBG << " ";
		dodajKorisnika << nizKorisnika[brojKorisnika].uloga << " ";
		dodajKorisnika << nizKorisnika[brojKorisnika].grad << endl;
		dodajKorisnika.close();
		
		//Dodavanje datoteke za racune korisnika: ime datoteke ce biti: ImePrezime_racun.txt
		string nazivDatoteke;
		nazivDatoteke = nizKorisnika[brojKorisnika].korisnik.ime + nizKorisnika[brojKorisnika].korisnik.prezime + "_racuni.txt";
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
		cout << "\t\t5. Pretraga korisnika" << endl;
		cout << "\t\t6. Ispis uplatnice za korisnika" << endl;
		cout << "\t\t7. Kraj rada" << crt;
		cout << "\t\tUnesite izbor: ";
		cin >> izbor;
	} while (izbor<1 || izbor>7);
	return izbor;
}

//Funkcija za pregled svih registrovanih korisnika
void pregledKorisnika(){
	cout << "\n\n\n\n";
	cout << crt << "\t\t\t\t::PREGLED KORISNIKA::" << crt;
	cout << "\t\t" << setw(5) << left << "ID" << setw(15) << left << "Ime" << setw(15) << left << "Prezime" << setw(15) << left << "JMBG" << setw(10) << left << "Grad" << endl; 
	for(int i=0; i<brojKorisnika; i++) {
		cout << "\t\t" << setw(5) << left << nizKorisnika[i].idKorisnika << setw(15) << left << nizKorisnika[i].korisnik.ime << setw(15) 
			 << left << nizKorisnika[i].korisnik.prezime << setw(15) << left << nizKorisnika[i].JMBG << setw(10) << left;
		if(nizKorisnika[i].grad == 10) {
			cout << "Banjaluka" << endl; 
		} else if (nizKorisnika[i].grad == 11){
			cout << "Bihac" << endl;
		} else if (nizKorisnika[i].grad == 12){
			cout << "Doboj" << endl;
		} else if (nizKorisnika[i].grad == 13){
			cout << "Gorazde" << endl;
		} else if (nizKorisnika[i].grad == 14){
			cout << "Livno" << endl;
		} else if (nizKorisnika[i].grad == 15){
			cout << "Mostar" << endl;
		} else if (nizKorisnika[i].grad == 16){
			cout << "Prijedor" << endl;
		} else if (nizKorisnika[i].grad == 17){
			cout << "Sarajevo" << endl;
		} else if (nizKorisnika[i].grad == 18){
			cout << "Tuzla" << endl;
		} else if (nizKorisnika[i].grad == 19){
			cout << "Zenica" << endl;
		} else {
			cout << "NULL" << endl;
		}
	}
	cout << endl;
}

//Funkcija za prikaz korisnika koja nudi mogucnost odabira jednog od njih
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
			cout << "\n\t\tUnesite redni broj korisnika: ";
			cin >> rb;
		} while(rb<1 || rb>brojKorisnika);
		return rb-1;
	}
}

//Funkcija za pravljenje racuna za prosljedjenog korisnika
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
		bool validan = false;	//Vrijednost za provjeru validnosti indexa	
		//do while petlja koja radi sve dok se ne pronadje indeks razlicit od svih unesenih do tog trenutka
		do {
			cout << "\t\tUnesite ID racuna: ";
			cin >> noviRacun.idRacuna;
			if(brojac==0) {
				validan = true;
			} else {
				for(int i=0; i<brojac; i++) {
					if (noviRacun.idRacuna == ucitaniRacuni[i].idRacuna) {
						cout << "\t\t\tGreska: Uneseni ID racuna vec postoji\n";
						validan = false;
						break;
					} else if (noviRacun.idRacuna != ucitaniRacuni[i].idRacuna){
						validan = true;
					} else {
						validan = true;
					}		
				}	
			}	
		} while(validan==false);
		break;
	} while(1);
	cout << "\t\tUnesite iznos racuna: "; cin >> noviRacun.iznos;
	noviRacun.status = (Status)neplacen;
	
	cout << "\n\t\t="; Sleep(100); cout << "="; Sleep(100); cout << "="; Sleep(100); 
	cout << "="; Sleep(100); cout << "="; Sleep(100); cout << " Racun uspjesno dodan!\n\n"; 
	Sleep(1500);  
	return noviRacun;
}

//Funkcija koja azurira podatke o racunima prosljedjenog korisnika
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

//Funkcija koja overwrite-a stare informacije o racunima korisnika sa novim informacijama
void ispisiRacuneKorisnika(Korisnik korisnik, string imeDatoteke) {
	ofstream ispisiRacune;
	ispisiRacune.open(imeDatoteke, ios::trunc);
	for(int i=0; i<korisnik.brojacRacuna; i++) {
		ispisiRacune << korisnik.racuni[i].idRacuna << " " << korisnik.racuni[i].iznos << " " << korisnik.racuni[i].status << endl;
	}
	ispisiRacune.close();
}

//Funkcija za ispis svih racuna prosljedjenog korisnika
void pregledRacunaKorisnika(Korisnik korisnik){
	string imeDatoteke = korisnik.korisnik.ime + korisnik.korisnik.prezime + "_racuni.txt";
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
	
	if(brojac==0) cout << "\n\t\tKorisnik trenutno nema racuna!\n\n";
	else {
		cout << crt << "\t\t\t\t::RACUNI KORISNIKA::" << crt;
		cout << endl;
		cout << "\t\t" << setw(5) << left << "ID" << setw(6) << left << "Iznos" << setw(15) << left << "\tStatus" << endl; 
		for(int i=0; i<brojac; i++) {
			cout << "\t\t" << setw(5) << left << ucitaniRacuni[i].idRacuna << setw(6) << left << ucitaniRacuni[i].iznos << "KM |";
			cout << setw(15) << left;
			if(ucitaniRacuni[i].status == 0) {
				cout << " Placen" << endl;
			} else cout << " Neplacen" << endl;
		}
	}	
	cout << endl;
}

//Funkcija za kreiranje i ispisivanje uplatnice u custom text file
void ispisUplatnice(Korisnik korisnik){
	string imeDatoteke = korisnik.korisnik.ime + korisnik.korisnik.prezime + "_racuni.txt";
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

	Racun noviRacun = korisnik.odaberiRacun();
	Datum datumEvidencije;	//Datum evidencije
	//Unos datuma evidentiranja podataka za studente
	system("cls");
	cout << "\n\n\n\n";
	cout << crt << "\t\t\t\t::POPUNJAVANJE UPLATNICE::" << crt;
	do {
		cout << "\t\tUnesite datum evidentiranja (d,m,g): ";
		cin >> datumEvidencije.dan >> datumEvidencije.mjesec >> datumEvidencije.godina;
		if(!provjera(datumEvidencije.dan, datumEvidencije.mjesec, datumEvidencije.godina)) {	//Ukoliko datum nije validan, petlja ce ispisati error
			cout << "\t\t\tUneseni datum je neispravan!\n";
		}		
	} while(!provjera(datumEvidencije.dan, datumEvidencije.mjesec, datumEvidencije.godina));	//Petlja ce raditi sve dok datum nije validan, jer ocekuje da je on validan
	
	string imeOperatora;
	cin.ignore();
	cout << "\t\tUnesite svoje ime (ime operatora): "; getline(cin, imeOperatora);
	string nazivUplatnice;
	nazivUplatnice += korisnik.korisnik.ime + korisnik.korisnik.prezime + "_uplatnica_" + to_string(noviRacun.idRacuna) + ".txt";
	ofstream ispisiUplatnicu;
	ispisiUplatnicu.open(nazivUplatnice, ios::trunc);
	ispisiUplatnicu << crt << "\t\t\t\tUPLATNICA za racun ID: " << noviRacun.idRacuna << crt;
	ispisiUplatnicu << "\t\tIme i prezime osobe: " << korisnik.korisnik.ime << " " << korisnik.korisnik.prezime << endl;
	ispisiUplatnicu << "\t\tID racuna: " << noviRacun.idRacuna << endl;
	ispisiUplatnicu << "\t\tIznos racuna: " << noviRacun.iznos << endl;
	ispisiUplatnicu << "\t\tStatus racuna: ";
	if(noviRacun.status == 0) {
		ispisiUplatnicu << "Placen" << endl;
	} else ispisiUplatnicu << "Neplacen" << endl;
	ispisiUplatnicu << "\t\tIme operatora: " << imeOperatora << endl;
	ispisiUplatnicu << "\t\t\t\t\t\t\t\t\t _____________________________________" << endl;
	ispisiUplatnicu << "\t\t\t\t\t\t\t\t\t\t\t Potpis" << endl;
	ispisiUplatnicu << "\t\t\t\t\t\t\t\t\t\t\t ";
	ispisiUplatnicu << setfill('0') << setw(2)
	     << datumEvidencije.dan << "." << setfill('0')
	     << setw(2) << datumEvidencije.mjesec << "."
	     << datumEvidencije.godina << ".";
	ispisiUplatnicu << endl;  
	ispisiUplatnicu.close();
	
	cout << "\n\t\t="; Sleep(100); cout << "="; Sleep(100); cout << "="; Sleep(100); 
	cout << "="; Sleep(100); cout << "="; Sleep(100); cout << " Uplatnica ispravno popunjena! \n\t\tMozete je provjeriti pod imenom: ";
	cout << nazivUplatnice << endl; 
	Sleep(1500);  
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

//Prvi meni koji se pojavljuje u sistemu - sastoji se od glavnih opcija za login/register
int loginMeni() {
	int izbor;
	cout << "\n\n\n\n";
	do {
		cout << crt << "\t\t\t\t::Login / Register::" << crt;
		cout << "\t\t\t\t1. Login" << endl;
		cout << "\t\t\t\t2. Register" << endl;
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

//Funkcija za ispis menija za custom pretrage
int pretragaMeni() {
	int izbor;
	cout << "\n\n\n\n";
	do {
		cout << crt << "\t\t\t\t::PRETRAGA MENI::" << crt;
		cout << "\t\t1. Pretraga korisnika po gradovima" << endl;
		cout << "\t\t2. Sortiranje korisnika po maksimalnom dugu" << endl;
		cout << "\t\t3. Kraj rada" << crt;
		cout << "\t\tIzbor: ";
		cin >> izbor;
	} while (izbor<1 || izbor>3);
	return izbor;
}

//Funkcija za pretragu korisnika na osnovu grada u kojem zive
void pretragaPoGradu() {
	int izbor;
	cout << "\n\n\n\n";
	do {
		cout << crt << "\t\t\t\t::PRETRAGA PO GRADOVIMA::" << crt;
		cout << "\t\t1. Banjaluka" << endl;
		cout << "\t\t2. Bihac" << endl;
		cout << "\t\t3. Doboj" << endl;
		cout << "\t\t4. Gorazde" << endl;
		cout << "\t\t5. Livno" << endl;
		cout << "\t\t6. Mostar" << endl;
		cout << "\t\t7. Prijedor" << endl;
		cout << "\t\t8. Sarajevo" << endl;
		cout << "\t\t9. Tuzla" << endl;
		cout << "\t\t10. Zenica" << endl;
		cout << "\t\t11. Kraj rada" << crt;
		cout << "\t\tIzbor: ";
		cin >> izbor;
		cout << endl;
			if(izbor==11) {
			break;
		} else if (izbor==1){
			int brojac = 0;
			for(int i=0; i<brojKorisnika; i++) {
				if(nizKorisnika[i].grad == 10) {
					cout << "\t\t" << brojac+1 << "." << setw(10) << left << nizKorisnika[i].korisnik.ime << setw(15) 
				 		 << left << nizKorisnika[i].korisnik.prezime << endl; 
				 	brojac++;
				}
			}
			if(brojac == 0) {
				cout << "\t\tNema korisnika iz Banjaluke!\n\n";
			}
		} else if (izbor==2){
			int brojac = 0;
			for(int i=0; i<brojKorisnika; i++) {
				if(nizKorisnika[i].grad == 11) {
					cout << "\t\t" << brojac+1 << "." << setw(10) << left << nizKorisnika[i].korisnik.ime << setw(15) 
				 		 << left << nizKorisnika[i].korisnik.prezime << endl; 
				 	brojac++;
				}
			}
			if(brojac == 0) {
				cout << "\t\tNema korisnika iz Bihaca!\n\n";
			}
		} else if (izbor==3){
			int brojac = 0;
			for(int i=0; i<brojKorisnika; i++) {
				if(nizKorisnika[i].grad == 12) {
					cout << "\t\t" << brojac+1 << "." << setw(10) << left << nizKorisnika[i].korisnik.ime << setw(15) 
				 		 << left << nizKorisnika[i].korisnik.prezime << endl; 
				 	brojac++;
				}
			}
			if(brojac == 0) {
				cout << "\t\tNema korisnika iz Doboja!\n\n";
			}
		} else if (izbor==4){
			int brojac = 0;
			for(int i=0; i<brojKorisnika; i++) {
				if(nizKorisnika[i].grad == 13) {
					cout << "\t\t" << brojac+1 << "." << setw(10) << left << nizKorisnika[i].korisnik.ime << setw(15) 
				 		 << left << nizKorisnika[i].korisnik.prezime << endl; 
				 	brojac++;
				}
			}	
			if(brojac == 0) {
				cout << "\t\tNema korisnika iz Gorazda!\n\n";
			}
		} else if (izbor==5){
			int brojac = 0;
			for(int i=0; i<brojKorisnika; i++) {
				if(nizKorisnika[i].grad == 14) {
					cout << "\t\t" << brojac+1 << "." << setw(10) << left << nizKorisnika[i].korisnik.ime << setw(15) 
				 		 << left << nizKorisnika[i].korisnik.prezime << endl; 
				 	brojac++;
				}
			}
			if(brojac == 0) {
				cout << "\t\tNema korisnika iz Livna!\n\n";
			}
		} else if (izbor==6){
			int brojac = 0;
			for(int i=0; i<brojKorisnika; i++) {
				if(nizKorisnika[i].grad == 15) {
					cout << "\t\t" << brojac+1 << "." << setw(10) << left << nizKorisnika[i].korisnik.ime << setw(15) 
				 		 << left << nizKorisnika[i].korisnik.prezime << endl; 
				 	brojac++;
				}
			}
			if(brojac == 0) {
				cout << "\t\tNema korisnika iz Mostara!\n\n";
			}
		} else if (izbor==7){
			int brojac = 0;
			for(int i=0; i<brojKorisnika; i++) {
				if(nizKorisnika[i].grad == 16) {
					cout << "\t\t" << brojac+1 << "." << setw(10) << left << nizKorisnika[i].korisnik.ime << setw(15) 
				 		 << left << nizKorisnika[i].korisnik.prezime << endl; 
				 	brojac++;
				}
			}
			if(brojac == 0) {
				cout << "\t\tNema korisnika iz Prijedora!\n\n";
			}
		} else if (izbor==8){
			int brojac = 0;
			for(int i=0; i<brojKorisnika; i++) {
				if(nizKorisnika[i].grad == 17) {
					cout << "\t\t" << brojac+1 << "." << setw(10) << left << nizKorisnika[i].korisnik.ime << setw(15) 
				 		 << left << nizKorisnika[i].korisnik.prezime << endl; 
				 	brojac++;
				}
			}
			if(brojac == 0) {
				cout << "\t\tNema korisnika iz Sarajeva!\n\n";
			}
		} else if (izbor==9){
			int brojac = 0;
			for(int i=0; i<brojKorisnika; i++) {
				if(nizKorisnika[i].grad == 18) {
					cout << "\t\t" << brojac+1 << "." << setw(10) << left << nizKorisnika[i].korisnik.ime << setw(15) 
				 		 << left << nizKorisnika[i].korisnik.prezime << endl; 
				 	brojac++;
				}
			}
			if(brojac == 0) {
				cout << "\t\tNema korisnika iz Tuzle!\n\n";
			}
		} else if (izbor==10){
			int brojac = 0;
			for(int i=0; i<brojKorisnika; i++) {
				if(nizKorisnika[i].grad == 19) {
					cout << "\t\t" << brojac+1 << "." << setw(10) << left << nizKorisnika[i].korisnik.ime << setw(15) 
				 		 << left << nizKorisnika[i].korisnik.prezime << endl; 
				 	brojac++;
				}
			}
			if(brojac == 0) {
				cout << "\t\tNema korisnika iz Zenice!\n\n";
			}
		}
	} while (izbor<1 || izbor>11);
}

//Funkcija koja ispisuje sortirane korisnike po njihovom dugu
void pretragaPoMaxDugu() {
	float sumeDugova[brojKorisnika] = {0};
	for(int i=0; i<brojKorisnika; i++) {
		string imeDatoteke = nizKorisnika[i].korisnik.ime + nizKorisnika[i].korisnik.prezime + "_racuni.txt";
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
		float iznosi[12];
		int brojacIznosa = 0;
		for(int i=0; i<brojac; i++) {
			if(ucitaniRacuni[i].status == 1) {
				iznosi[brojacIznosa] = ucitaniRacuni[i].iznos;
				brojacIznosa++;
			}
		}
		ucitajRacune.close();
		
		sumeDugova[i] = nizKorisnika[i].dug(iznosi, brojac);
	}
	
	Korisnik tempKorisnik;
	float tempSuma;
	for(int i=0; i<brojKorisnika-1; i++) {
		for (int j=i; j<brojKorisnika; j++) {
			if(sumeDugova[i]<sumeDugova[j]) {
				tempKorisnik = nizKorisnika[j];
				nizKorisnika[j] = nizKorisnika[i];
				nizKorisnika[i] = tempKorisnik;
				tempSuma = sumeDugova[j];
				sumeDugova[j] = sumeDugova[i];
				sumeDugova[i] = tempSuma;
			}
		}
	}
	cout << "\n\n\n\n";
	cout << crt << "\t\t\t::KORISNICI SORTIRANI PO DUGOVIMA::" << crt;
	for(int i=0; i<brojKorisnika; i++) {
		cout << "\t\t" << i+1 << ". " << setw(10) << left << nizKorisnika[i].korisnik.ime << " " << setw(10) << left << nizKorisnika[i].korisnik.prezime << setw(6) << left <<  "| Dug: " << sumeDugova[i] << endl;
	}
	cout << endl;		
}

int main () {

	//Dodaju se informacije za admina (koji je u nasem slucaju samo jedan)
	admin.administrator.ime = "Grupa";					//Ime admina
	admin.administrator.prezime = "osam";				//Prezime admina
	admin.administrator.username = "grupa8";			//Username: selman
	admin.administrator.password = "admin";				//Password: admin
	admin.specijalniID = 234781;						//Specijalni ID po kojem je admin prepoznatljiv
	admin.uloga = (korisnickaUloga)administrator;		//Default uloga admina
	
	//Home screen aplikacije
	cout << "\n\n\n\n\n";
	cout << crt << "\t\t\t   Sistem za elektrodistribuciju" << crt;
	cout << "\t\tPravili: Selman Patkovic, Adnan Kubat i Tarik Mujkic\n\n";
	cout << "\t\t\t*Napomena*\n\n";
	cout << "\t\t\tZa potrebe testiranja administrator menija,\n";
	cout << "\t\t\tkao login podatke koristite:\n\n";
	cout << "\t\t\t\tusername: grupa8\n";
	cout << "\t\t\t\tpassword: admin\n\n\n\n";
	cout << "\t\tPritisnite ENTER da nastavite.............................";
	getchar();
	
	ucitajKorisnike();	//Ucitavanje svih korisnika iz datoteke "bazaPodataka.txt" u niz "nizKorisnika"
	//unosKorisnika();
	
	int odabir;	//Varijabla za odabir panela u glavnom meniju
	system("cls");
	
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
			} else if (odabir == 1) {	//Ukoliko je "odabir" 1, provjerava se da li vrijednost ID pripada adminu ili nekom od korisnika
				system("cls");
				ID = login(username, password);
				system("cls");
				if(ID == admin.specijalniID) {	//Ako pripada adminu, otvara se poseban "adminMeni()" za njega, u kojem ima 6 opcija
					system("cls");
					int izbor;
					do {
						izbor = adminMeni();
						if(izbor == 7) {
							break;
						}
						if (izbor == 1) {	//Opcija 1: Pregled svih korisnika
							system("cls");
							pregledKorisnika();
						} else if (izbor == 2) {	//Opcija 2: Pregled svih racuna datog korisnika
							system("cls");
							cout << "\n\n\n\n";
							cout << crt << "\t\t\t\t::ODABERITE KORISNIKA::" << crt;
							Korisnik &korisnik = nizKorisnika[prikaziKorisnike()];
							pregledRacunaKorisnika(korisnik);
						} else if (izbor == 3) {	//Opcija 3: Dodavanje racuna za korisnika
							system("cls");
							cout << "\n\n\n\n";
							cout << crt << "\t\t\t\t::ODABERITE KORISNIKA::" << crt;
							Korisnik &odabraniKorisnik = nizKorisnika[prikaziKorisnike()];
							string imeDatoteke = odabraniKorisnik.korisnik.ime + odabraniKorisnik.korisnik.prezime + "_racuni.txt";
							azurirajRacuneZaKorisnika(odabraniKorisnik, imeDatoteke);
							odabraniKorisnik.racuni[odabraniKorisnik.brojacRacuna++] = napraviRacun(imeDatoteke);
							ispisiRacuneKorisnika(odabraniKorisnik, imeDatoteke);
						} else if (izbor == 4) {	//Opcija 4: Registrovanje novog korisnika
							system("cls");
							cin.ignore();
							unosKorisnika();
							cout << "\n\t\t="; Sleep(100); cout << "="; Sleep(100); cout << "="; Sleep(100); cout << "=";
							Sleep(100); cout << "="; Sleep(100); cout << " Korisnik uspjesno registrovan!\n";
							Sleep(1500);
						} else if (izbor == 5) {	//Opcija 5: Pretraga korisnika
							system("cls");
							do {
								int odabirMenija;	//Varijabla u koju se smjesta odabir podmenija za pretragu
								odabirMenija = pretragaMeni();
								if(odabirMenija == 3) {
									break;
								} else if(odabirMenija == 1) {
									system("cls");
									pretragaPoGradu();
								} else if (odabirMenija == 2) {
									system("cls");
									pretragaPoMaxDugu();
								}
								system("PAUSE");
								system("cls");	
							} while(1);
						} else if (izbor == 6) {	//Opcija 6: Kreiranje uplatnice za korisnika
							system("cls");
							cout << "\n\n\n\n";
							cout << crt << "\t\t\t\t::KREIRANJE UPLATNICE::" << crt;
							Korisnik &odabraniKorisnik = nizKorisnika[prikaziKorisnike()];
							ispisUplatnice(odabraniKorisnik);
						}
							system("PAUSE");
							system("cls");
					} while(1);				//Kraj admin menija
				} else {	//Ako ID nije pripadao adminu, onda se kroz for petlju ispod provjerava kojem korisniku pripada broj indexa
					for(int i=0; i<brojKorisnika; i++) {
						if(ID == nizKorisnika[i].idKorisnika) {	//Ako petlja nadje datog korisnika, pokrece se korisnikMeni() putem do while petlje ispod
							do {
								int izbor;
								izbor = korisnikMeni();
								if (izbor == 4) {	//Opcija 4: zatvara korisnik meni
									break;
								} else if (izbor == 1) {	//Opcija 1:	Ispisuje sve racune korisnika
									system("cls");
									cout << "\n\n\n\n";
									nizKorisnika[i].pregledRacuna();
								} else if (izbor == 2) {	//Opcija 2: Ispisuje ukupan dug korisnika
									system("cls");
									nizKorisnika[i].pregledDuga();
								} else if (izbor == 3) {	//Opcija 3: Daje opciju placanja odabranog racuna korisniku
									system("cls");
									nizKorisnika[i].platiRacun();
								}
									system("PAUSE");
									system("cls");	
							} while(1);	//Kraj korisnik menija
							break;	//Prekid za for petlju nakon sto nadje odgovarajuceg korisnika	
						} else {
							//Za pogresno unesenu sifru/username sistem nece nista uraditi, nego ce se ponovo ucitati login meni
						}
					}
				}
			} else if (odabir == 2) {	//Opcija 2 iz glavnog menija: Registracija novog korisnika
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

