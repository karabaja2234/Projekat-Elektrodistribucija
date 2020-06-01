# Projekat Elektrodistribucija

## Karakteristike:
- Funkcionalan **login/register sistem** za sve korisnike elektrodistribucije: Postoji jedan administrator i max 50 korisnika (može se mijenjati)
- Sistem na osnovu unesenih login informacije prepoznaje da li je riječ o administratoru ili o nekom korisniku (prepoznaje svakog korisnika)
- Na osnovu prepoznatog korisnika/administratora, može se otvoriti jedan od dva glavna menija: **adminMeni** ili **korisnikMeni**
- Sve glavne informacije o registrovanim korisnicima pohranjene su u datoteku **bazaPodataka.txt**, u formatu: ime prezime username password ID JMBG korisnickaUloga
- Sistem ne dopušta registraciju korisnika sa istim username-om, ID-em ili JMBG-om, kao ni istim informacijama koje su pridružene administratoru

###### Karakteristike adminMenija:
- **Pregled svih registrovanih korisnika**: Admin u bilo kojem trenutku može provjeriti registrovane korisnike u sistemu, prilikom čega se ispisuju osnovne informacije o istima
- **Pregled svih računa korisnika**: Adminu se daje mogućnost da bira jednog od registrovanih korisnika, a zatim se ispisuju svi računi vezani za odabranog korisnika
- **Dodavanje računa**: Adminu se daje mogućnost da dodaje pojedinačne račune za odabranog korisnika
- **Registracija novog korisnika**: Admin u bilo kojem momentu može registrovati novog korisnika, kao što bi korisnik registrovao sam sebe, uz obaveznu provjeru unesenih podataka
- **Pretraga korisnika**: Admin može pretraživati korisnike prema izlistanim gradovima ili prema maksimalnom dugu (izlistava sve korisnike sortirane prema najvećem dugu)
- **Ispis uplatnica**: Admin može ispisivati pojedinačne uplatnice za specifičan račun odabranog korisnika, prilikom čega ima mogućnost unositi datum ispisa uplatnice (provjerava se i validnost datuma), te svoje ime, nakon čega se uplatnica ispisuje u text file u formatu: **ImeKorisnikaPrezimeKorisnika_uplatnica_idRacuna.txt** 

###### **Karakteristike korisnikMenija**:
- **Pregled računa**: Korisnik priligom logiranja na svoj profil, u bilo kojem trenutku može provjeriti sve svoje račune, njihov status i ostale bitne informacije
- **Pregled duga**: Korisnik može provjeravati svoj ukupan dug, koji se računa putem rekurzivne funkcije, ubrajajući samo račune koji nisu plaćeni
- **Plaćanje računa**: Korisnik ima mogućnost da plati odabrani račun, nakon čega se i ta promjena evidentira u sistemu, kao i u datoteci računa korisnika.

###### **Bitne napomene**:
- Za pristup administrator meniju koriste se login informacije: **username**: grupa8 i **password**: admin
- Prilikom registrovanja novog korisnika, automatski se pravi datoteka formata: **ImeKorisnikaPrezimeKorisnika_racuni.txt** u koju se spremaju svi računi koji se dodaju za tog korisnika i iz koje se prate sve moguće promjene
- Sistem je **interaktivan** i sve promjene se dešavaju u realnom vremenu (nije potrebno zatvarati program), te se mogu provjeravati u navedenim datotekama
- Pretraga korisnika po gradovima izvršava se na osnovu 8. i 9. karaktera u JMBG-u datog korisnika
- Sistem je regulisan da provjerava sve unose, prilikom čega ne dopušta unos vrijednosti koje nisu moguće/dopuštene.

###### Projekat radili: **Grupa 8**
- Patković Selman
- Kubat Adnan
- Mujkić Tarik
