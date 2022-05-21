#include <iostream>
#include <ctime>
#include <algorithm>
#include "IoBase.h"
using namespace std;

time_t t = time(nullptr);
tm* const data = localtime(&t);

class ProduseNeexpirabile: public IoBase {
protected:
    string nume;
    double cantitate, pret;
    vector<string> sponsori;
    unsigned int id;
public:
    friend class Menu;
    ProduseNeexpirabile():cantitate(0), pret(0),id(0) {}
    ProduseNeexpirabile(string nume, double cantitate, double pret, vector<string> sponsori, unsigned int id):
        nume(nume), cantitate(cantitate), pret(pret), sponsori(sponsori), id(id) {}
    istream& read(istream& is) override {
        IoBase::read(is);
        cout << "Nume: ";
        is >> nume;
        cout << "Cantitate: ";
        is >> cantitate;
        cout << "Pret: ";
        is >> pret;
        cout << "Sponsori: ";
        is >> sponsori;
        cout << "ID: ";
        is >> id;
        return is;
    }
    ostream& write(ostream& os) const override {
        IoBase::write(os);
        os << "Nume: " << nume << "\nCantitate: " << cantitate << "\nPret: " << pret << "\nSponsori: " << sponsori<<"\nID: "<<id<<endl;
        return os;
    }
    friend istream& operator>>(istream& is, ProduseNeexpirabile& p) {
        return p.read(is);
    }
    friend ostream& operator<<(ostream& os, ProduseNeexpirabile& p) {
        return p.write(os);
    }
};

class ProduseExpirabile : public ProduseNeexpirabile {
protected:
    unsigned int zi, luna, an;
public:
    friend class Menu;
    ProduseExpirabile(): ProduseNeexpirabile(), zi(zi), luna(luna), an(an) {}
    ProduseExpirabile(string nume, double cantitate, double pret, vector<string> sponsori, unsigned int id, unsigned int zi, unsigned int luna, unsigned int an):
        ProduseNeexpirabile(nume,cantitate,pret,sponsori,id), zi(zi), luna(luna),an(an) {}
    istream& read(istream& is) override{
        ProduseNeexpirabile::read(is);
        cout << "Data de expirare(zz ll aaaa): ";
        is >> zi >> luna >> an;
        return is;
    }
    ostream& write(ostream& os) const override {
        ProduseNeexpirabile::write(os);
        os << "Data de expirare: " << zi << "/" << luna << "/" << an << endl;
        return os;
    }
    friend istream& operator>>(istream& is, ProduseExpirabile& p) {
        return p.read(is);
    }
    friend ostream& operator<<(ostream& os, ProduseExpirabile& p) {
        return p.write(os);
    }
    bool isExpired();
};

bool ProduseExpirabile::isExpired() {
    return an < 1900 + ::data->tm_year || luna < 1 + ::data->tm_mon || zi < ::data->tm_mday;
}

class ProduseExpirabileClasice : public ProduseExpirabile {
protected:
    vector<string> proprietati, efecte_adverse;
public:
    friend class Menu;
    ProduseExpirabileClasice(): ProduseExpirabile() {}
    ProduseExpirabileClasice(string nume, double cantitate, double pret, vector<string> sponsori, unsigned int id, unsigned int zi, unsigned int luna, unsigned int an, vector<string> proprietati, vector<string> efecte_adverse):
        ProduseExpirabile(nume,cantitate,pret,sponsori,id,zi,luna,an), proprietati(proprietati), efecte_adverse(efecte_adverse){}
    istream& read(istream& is) override {
        ProduseExpirabile::read(is);
        cout << "Proprietati: ";
        is >> proprietati;
        cout << "Efecte adverse: ";
        is >> efecte_adverse;
        return is;
    }
    ostream& write(ostream& os) const override {
        ProduseExpirabile::write(os);
        cout << "Proprietati:" << proprietati<<"\nEfecte adverse:"<<efecte_adverse<<endl;
        return os;
    }
    friend istream& operator>>(istream& is, ProduseExpirabileClasice& p) {
        return p.read(is);
    }
    friend ostream& operator<<(ostream& os, ProduseExpirabileClasice& p) {
        return p.write(os);
    }
};

class ProduseExpirabileComplexe : public ProduseExpirabileClasice {
protected:
    string aroma;
public:
    friend class Menu;
    ProduseExpirabileComplexe(): ProduseExpirabileClasice(){}
    ProduseExpirabileComplexe(string nume, double cantitate, double pret, vector<string> sponsori, unsigned int id, unsigned int zi, unsigned int luna, unsigned int an, vector<string> proprietati, vector<string> efecte_adverse, string aroma):
        ProduseExpirabileClasice(nume,cantitate,pret,sponsori,id,zi,luna,an,proprietati,efecte_adverse), aroma(aroma){}
    istream& read(istream& is) override{
        ProduseExpirabileClasice::read(is);
        cout << "Aroma: ";
        is >> aroma;
        return is;
    }
    ostream& write(ostream& os) const override {
        ProduseExpirabileClasice::write(os);
        cout << "Aroma: " << aroma << endl;
        return os;
    }
    friend istream& operator>>(istream& is, ProduseExpirabileComplexe& p) {
        return p.read(is);
    }
    friend ostream& operator<<(ostream& os, ProduseExpirabileComplexe& p) {
        return p.write(os);
    }
};

class Furnizor :public IoBase {
    vector<string> sponsori;
    vector<double> reduceri;
public:
    friend class Menu;
    Furnizor(){}
    Furnizor(vector<string> sponsori, vector<double> reduceri): sponsori(sponsori), reduceri(reduceri){}
    istream& read(istream& is) override {
        IoBase::read(is);
        cout << "Sponsori:";
        is >> sponsori;
        cout << "\nReduceri:\n";
        double reducere;
        for (unsigned int i = 0; i < sponsori.size(); i++)
        {
            cin >> reducere;
            reduceri.push_back(reducere);
        }
        return is;
    }
    ostream& write(ostream& os) const override {
        IoBase::write(os);
        os << "Sponsori:" << sponsori << "\nReduceri:"<<reduceri << endl;
        return os;
    }
    friend istream& operator>>(istream& is, Furnizor& f) {
        return f.read(is);
    }
    friend ostream& operator<<(ostream& os, Furnizor& f) {
        return f.write(os);
    }
};

class Menu : public IoBase {
    vector<shared_ptr<ProduseNeexpirabile>> produse;
    double suma=0.0,reducere=0.0;
    int reducere_tip_produse=0;
public:
    Menu(){}
    void mainLoop();
    void chooseOption();
    void listOptions();
};

void Menu::mainLoop() {
    while (true) {
        listOptions();
        chooseOption();
    }
}

void Menu::listOptions() {
    cout << "1. Adauga produse.\n";
    cout << "2. Cati bani au fost investiti pentru produsele din stoc?\n";
    cout << "3. Retine o reducere procentuala.\n";
    cout << "4. Afiseaza pretul total pentru anumite produse.\n";
    cout << "5. Introdu date despre o masina si afiseza costul total.\n";
}

void Menu::chooseOption() {
    cout << "Introdu o valoare(1-5): ";
    int choice;
    cin >> choice;
    switch (choice) {
    case 1:
    {
        int n, tip;
        cout << "Numar produse: ";
        cin >> n;
        for (unsigned int i = 0; i < n; i++)
        {
            do
            {
                cout << i+1 << "/" << n << endl;
                cout << "Tip produs (0 = neexpirabil / 1 = expirabil pentru adulti / 2 = expirabil clasic pentru copii / 3 = expirabil complex pentru copii): ";
                cin >> tip;
                if (!tip)
                {
                    ProduseNeexpirabile neexpirabil;
                    cin >> neexpirabil;
                    produse.push_back(make_shared<ProduseNeexpirabile>(neexpirabil));
                }
                else if (tip == 1)
                {
                    ProduseExpirabile expirabil;
                    cin >> expirabil;
                    produse.push_back(make_shared<ProduseExpirabile>(expirabil));
                }
                else if (tip == 2) {
                    ProduseExpirabileClasice clasic;
                    cin >> clasic;
                    produse.push_back(make_shared<ProduseExpirabileClasice>(clasic));
                }
                else if (tip == 3) {
                    ProduseExpirabileComplexe complex;
                    cin >> complex;
                    produse.push_back(make_shared<ProduseExpirabileComplexe>(complex));
                }
                else
                    cout << "Invalid\n";
            } while (tip < 0 || tip >= 4);
        }
        cout << "Produse citite cu succes!\n";
        break;
    }
    case 2:
    {
        for (auto produs : produse)
            suma += produs->pret * produs->cantitate;
        cout << "Au fost investiti " << suma << " lei pentru a achizitiona toate produsele de pe stoc.\n";
        break;
    }
    case 3:
    {
        cout << "Pentru ce tip de produse se aplica reducerea? (0 = copii / 1 = adulti) ";
        do {
            cin >> reducere_tip_produse;
            if (reducere_tip_produse > 1 || reducere_tip_produse < 0)
                cout << "Invalid\n";
        } while (reducere_tip_produse > 1 || reducere_tip_produse < 0);
        cout << "Introduceti reducerea (1-100): ";
        cin >> reducere;
        cout << "Reducere aplicata cu succes!\n";
        break;
    }
    case 4:
    {
        int k;
        double pret = 0.0;
        do {
            cout << "Introduceti numarul de produse pentru a afla pretul total: ";
            cin >> k;
            if (k > produse.size())
                cout << "Invalid.\n";
        } while (k > produse.size());
        unsigned int id;
        if (!reducere_tip_produse)
            for (unsigned int i = 0; i < k; i++)
            {
                cout << "Introdu id-ul produsului: ";
                cin >> id;
                for (auto produs : produse)
                    if (produs->id == id)
                        if (dynamic_pointer_cast<ProduseExpirabileClasice>(produs) || dynamic_pointer_cast<ProduseExpirabileComplexe>(produs))
                            pret += produs->cantitate * produs->pret * (100 - reducere) / 100;
                        else
                            pret += produs->cantitate * produs->pret;
            }
        else
            for (unsigned int i = 0; i < k; i++)
            {
                cout << "Introdu id-ul produsului: ";
                cin >> id;
                for (auto produs : produse)
                    if (produs->id == id)
                        if (dynamic_pointer_cast<ProduseExpirabile>(produs))
                            pret += produs->cantitate * produs->pret * (100 - reducere) / 100;
                        else
                            pret += produs->cantitate * produs->pret;
            }
        cout << "Pretul total pentru cele " << k << " produse alese este de " << pret << " lei.\n";
        break;
    }
    case 5:
    {
        Furnizor furnizor;
        cin >> furnizor;
        double reducereprodus = 0.0;
        double pret_neexpirabile = 0.0;
        double pret_adulti = 0.0;
        double pret_clasice = 0.0;
        double pret_complexe = 0.0;
        if (!reducere_tip_produse)
            for (auto produs : produse) {
                if (dynamic_pointer_cast<ProduseExpirabileComplexe>(produs) && !(dynamic_pointer_cast<ProduseExpirabileComplexe>(produs)->isExpired()))
                {
                    reducereprodus = reducere;
                    for (unsigned int i = 0; i < furnizor.sponsori.size(); i++)
                        if (find(produs->sponsori.begin(), produs->sponsori.end(), furnizor.sponsori[i]) != produs->sponsori.end())
                            reducereprodus += furnizor.reduceri[i];
                    pret_complexe+= produs->cantitate * produs->pret * (100 - reducereprodus) / 100;
                }
                else if (dynamic_pointer_cast<ProduseExpirabileClasice>(produs) && !(dynamic_pointer_cast<ProduseExpirabileClasice>(produs)->isExpired()))
                {
                    reducereprodus = reducere;
                    for (unsigned int i = 0; i < furnizor.sponsori.size(); i++)
                        if (find(produs->sponsori.begin(), produs->sponsori.end(), furnizor.sponsori[i]) != produs->sponsori.end())
                            reducereprodus += furnizor.reduceri[i];
                    pret_clasice+= produs->cantitate * produs->pret * (100 - reducereprodus) / 100;
                }
                else if (dynamic_pointer_cast<ProduseExpirabile>(produs) && !(dynamic_pointer_cast<ProduseExpirabile>(produs)->isExpired()))
                {
                    reducereprodus = 0.0;
                    for (unsigned int i = 0; i < furnizor.sponsori.size(); i++)
                        if (find(produs->sponsori.begin(), produs->sponsori.end(), furnizor.sponsori[i]) != produs->sponsori.end())
                            reducereprodus += furnizor.reduceri[i];
                    pret_adulti += produs->cantitate * produs->pret * (100 - reducereprodus) / 100;
                }
                else
                {
                    reducereprodus = 0.0;
                    for (unsigned int i = 0; i < furnizor.sponsori.size(); i++)
                        if (find(produs->sponsori.begin(), produs->sponsori.end(), furnizor.sponsori[i]) != produs->sponsori.end())
                            reducereprodus += furnizor.reduceri[i];
                    pret_neexpirabile += produs->cantitate * produs->pret * (100 - reducereprodus) / 100;
                }
            }
        else
            for (auto produs : produse) {
                reducereprodus = 0.0;
                if (dynamic_pointer_cast<ProduseExpirabileComplexe>(produs) && !(dynamic_pointer_cast<ProduseExpirabileComplexe>(produs)->isExpired()))
                {
                    reducereprodus = 0.0;
                    for (unsigned int i = 0; i < furnizor.sponsori.size(); i++)
                        if (find(produs->sponsori.begin(), produs->sponsori.end(), furnizor.sponsori[i]) != produs->sponsori.end())
                            reducereprodus += furnizor.reduceri[i];
                    pret_complexe += produs->cantitate * produs->pret * (100 - reducereprodus) / 100;
                }
                else if (dynamic_pointer_cast<ProduseExpirabileClasice>(produs) && !(dynamic_pointer_cast<ProduseExpirabileClasice>(produs)->isExpired()))
                {
                    reducereprodus = 0.0;
                    for (unsigned int i = 0; i < furnizor.sponsori.size(); i++)
                        if (find(produs->sponsori.begin(), produs->sponsori.end(), furnizor.sponsori[i]) != produs->sponsori.end())
                            reducereprodus += furnizor.reduceri[i];
                    pret_clasice += produs->cantitate * produs->pret * (100 - reducereprodus) / 100;
                }
                else if (dynamic_pointer_cast<ProduseExpirabile>(produs) && !(dynamic_pointer_cast<ProduseExpirabile>(produs)->isExpired()))
                {
                    reducereprodus = reducere;
                    for (unsigned int i = 0; i < furnizor.sponsori.size(); i++)
                        if (find(produs->sponsori.begin(), produs->sponsori.end(), furnizor.sponsori[i]) != produs->sponsori.end())
                            reducereprodus += furnizor.reduceri[i];
                    pret_adulti+= produs->cantitate * produs->pret * (100 - reducereprodus) / 100;
                }
                else
                {
                    reducereprodus = reducere;
                    for (unsigned int i = 0; i < furnizor.sponsori.size(); i++)
                        if (find(produs->sponsori.begin(), produs->sponsori.end(), furnizor.sponsori[i]) != produs->sponsori.end())
                            reducereprodus += furnizor.reduceri[i];
                    pret_neexpirabile += produs->cantitate * produs->pret * (100 - reducereprodus) / 100;
                }
            }
        cout << "Costul total:\n";
        cout << "Produse care nu expira: " << pret_neexpirabile << endl;
        cout << "Produse pentru adulti: " << pret_adulti << endl;
        cout << "Produse clasice pentru copii: " << pret_clasice << endl;
        cout << "Produse complexe pentru copii: " << pret_complexe << endl;
        break;
    }
    default:
        cout << "Invalid.\n";
    }
};

int main()
{
    Menu m;
    m.mainLoop();
    return 0;
}