#include "bzstablo.h"
#include <fstream>
using namespace std;


void meni() {
	cout << "1. Pronadji kljuc."<<endl
		<< "2. Ispisi stablo."<<endl
		<< "3. Umetni kljuceve iz datoteke."<<endl
		<< "4. Umetanje pojedinacnog kljuca."<<endl
		<< "5. Izbrisi kljuc."<<endl
		<< "6. Pronadji k-ti najmanji u stablu."<<endl
		<< "7. Obrisi stablo."<<endl
		<< "8. Kraj." << endl << endl;
}
int main() {
	Bz stablo;
	int kraj = 0;
	ifstream dat;
	dat.open("dat.txt");
	while (!kraj) {
		meni();
		int opcija; cout << "Unesite zeljenu opciju:";
		cin >> opcija;
		switch (opcija) {
			
		case 1: {
			if (stablo.root == nullptr) {
				cout << "Morate prvo napraviti stablo.";
				cout << endl;
				break;
			}
			string te;
			cout << "Koja vrednost se trazi:";
			cin >> te;

			int j = 0;
			Bz::Bd* q = nullptr;
			j = stablo.pretrazi(te, stablo.root, &q);
			if (q) {
				cout << "Kljuc je na poziciji " << j << ",zajedno sa:";
				for (int i = 0; i < q->brel; i++) {
					cout << q->el[i] << " ";
				}
			}
			else {
				cout << "Nema tog kljuca";
			}
			cout << endl;
			break;
		}
		case 2: {
			cout << endl << endl << endl;
			stablo.ispis();
			cout << endl << endl;
			break;
		}
		case 3: {
			string te;
			while (dat) {
				dat >> te;
				stablo.umetni(te, &stablo.root, 5);
				
				cout << endl << endl << endl;
			}
			stablo.ispis();

			break;
		}
		case 4: {
			cout << "Koju vrednost umecemo:";
			string te;
			cin >> te;
			stablo.umetni(te, &stablo.root);
			stablo.ispis();
			cout << endl;
			break;
		}
		case 5: {
			cout << "Koja vrednost se brise:"; string te; cin >> te;
			stablo.izbrisi(&stablo.root, te);
			cout << endl << endl << endl;
			stablo.ispis();
			cout << endl;
			break;
		}
		case 6:{
			cout << "Unesite k-ti element:"; int k; cin >> k;
			cout << endl;
			cout<<endl<<"K-ti:" << stablo.vrati_kti(k) << endl;
			break;
		}

		case 7: {
			stablo.isprazni();
			cout << "Stablo je ispraznjeno";
			cout << endl;
			break;
		}
		case 8: {
			kraj = 1;
			break;
		}
		default:
			cout << "Pogresan izbor. Pokusaj ponovo.";
			cout << endl;
			continue;
		}

	}
	return 0;
}