#ifndef _bzstablo_h_
#define _bzstablo_h_

#include <iostream>
#include <string>
#include <cmath>
#include <queue>
#include <iomanip>
#include <stack>
using namespace std;

const int MAXSIR =2;//red stabla, promenjljiv

class Bz {
public:
	struct Bd {
		int brel, list;//koliko ima elemenata i da li je list
		string el[2 * MAXSIR - 1];
		struct Bd* nasl[2 * MAXSIR] = { nullptr };
		int brlevo = 0;
		string str = "***";
		Bd() = default;
		Bd(string vr, Bd* next = nullptr) {}
	};
	struct Node {
		string str;
		Node* next;
		Node(string str, Node* next = nullptr) :str(str), next(next) {}
	};

	Node* prvi = nullptr, * posl = nullptr;

public:
	Bd* root = nullptr;

	Bz() = default;

	int pretrazi(string te, struct Bd* p, struct Bd** q) {
		//te- string za pretragu
		//p==root
		//q-pokazivac na cvor koji se vraca

		int i, j;
		struct Bd* r = p;
		while (1) {
			i = r->brel;
			for (j = 0; (j < i) && (r->el[j] < te); j++);
			if ((j < i) && (te == r->el[j])) {
				*q = r; return j;
			}

			if (!(r->list)) {
				r = r->nasl[j];
			}
			else {
				return 0;
			}

		}


	}

	bool razbij_cvor_drveta(struct Bd* p, int bn) {
		//p==roditelj
		//bn== redni broj cvora za razbijanje
		//0==razbijen
		//-1==nije razbijen

		int i, j, k, l;
		Bd *q, * r, * t;
		q = new Bd;
		if (!q) return -1;
		r = p->nasl[bn];
		i = (l = r->brel) / 2;//i br elem koji ostaju u r
		k = l - i - 1;//broj elemenata u cvoru q

		for (j = 0; j < k; j++) {
			q->el[j] = r->el[i + j + 1];
			if (!(r->list)) {
				q->nasl[j] = r->nasl[i + j + 1];
				r->nasl[i + j + 1] = nullptr;
			}
		}


		if(!(r->list)){
			q->nasl[k] = r->nasl[l];
			r->nasl[l] = nullptr;
		}
		q->list = r->list;
		r->brel = i;
		q->brel = k;
		//ubacuje se u oca
		p->nasl[p->brel + 1] = p->nasl[p->brel];
		for (j = p->brel; j > bn + 1; j--) {
			p->el[j] = p->el[j - 1];
			p->nasl[j] = p->nasl[j - 1];
		}

		if (bn < 2 * MAXSIR - 2) {
			p->el[bn + 1] = p->el[bn];
		}

		p->el[bn] = r->el[i];
		p->nasl[bn + 1] = q;
		(p->brel)++;
		return 0;
	}

	//spajanje cvorova
	int spoj_cvorove(Bd* p, int bn) {
		//p=cvor otac
		//bn redni broj sina-levog

		int i, j, k;
		Bd* q, * r;
		q = p->nasl[bn];
		r = p->nasl[bn + 1];
		i = q->brel;
		j = r->brel;
		q->el[i] = p->el[bn];
		for (k = 0; k < j; k++) {
			q->el[i + 1 + k] = r->el[k];
			if (!(q->list)) {
				q->nasl[i + 1 + k] = r->nasl[k];
			}
		}
		
		if (!(q->list)) {
			q->nasl[i + j + 1] = r->nasl[j];//posl naslednik
		}

		q->brel = i + j + 1;

		for (k = bn; k < p->brel - 1; k++) {
			p->el[k] = p->el[k + 1];
			p->nasl[k + 1] = p->nasl[k + 2];
		}

		p->nasl[k + 1] = nullptr;
		//p->nasl[k] = nullptr;;
		(p->brel)--;
		delete r;
		return 0;
	}

	//premestanje iz susednog cvora-levog
	int prebaci_sleva(Bd* p, int bn) {
		//p==otac
		//iz cvora bn-1 prebacujemo u oca pa iz oca u bn

		int i, j, k;
		Bd* q, * r;
		q = p->nasl[bn - 1];
		r = p->nasl[bn];
		i = q->brel;
		j = r->brel;
		r->nasl[j + 1] = r->nasl[j];
		for (k = j; k > 0; k--) {
			r->el[k] = r->el[k - 1];
			r->nasl[k] = r->nasl[k - 1];
		}

		r->el[0] = p->el[bn - 1];
		r->nasl[0] = q->nasl[i];
		q->nasl[i] = nullptr;
		p->el[bn - 1] = q->el[i - 1];
		(r->brel)++;
		(q->brel)--;
		return 0;
	}

	int prebaci_sdesna(Bd* p, int bn) {
		//p==otac
		//u oca prebacujemo iz bn+1,iz oca prebacujemo u sina bn

		int i, j, k;
		Bd* q, * r;
		r = p->nasl[bn];
		q = p->nasl[bn + 1];
		i = q->brel;
		j = r->brel;
		r->nasl[j + 1] = q->nasl[0];
		r->el[j] = p->el[bn];
		(r->brel)++;
		p->el[bn] = q->el[0];
		for (k = 0; k < i - 1; k++) {
			q->el[k] = q->el[k + 1];
			q->nasl[k] = q->nasl[k + 1];
		}

		q->nasl[i - 1] = q->nasl[i];
		q->nasl[i] = nullptr;
		(q->brel)--;
		return 0;
	}

	int u_stablu(string vr) {
		for (Node* tek = prvi; tek; tek = tek->next) {
			if (tek->str == vr) {
				return 1;
			}
		}
		return 0;

	}

	void ubaci(string vr) {
		posl=(posl ? posl->next : prvi) = new Node(vr);
		for (Node* i = prvi; i; i = i->next) {
			for (Node* j = i->next; j; j = j->next) {
				if (i->str > j->str) {
					string tmp = i->str;
					i->str = j->str;
					j->str = tmp;
				}
			}
		}
	}



	void izbaci(string vr) {
		Node* pret = nullptr;
		Node* po = prvi;
		while (po) {
			if (po->str == vr) {
				Node* tek = po;
				po = po->next;
				delete tek;

				if (!pret) {
					prvi = po;
				}
				else {
					pret->next = po;
				}

				if (!po) {
					posl = pret;
				}
				break;
			}
			else {
				pret = po;
				po = po->next;
			}
		}

	}
	int umetni(string vr,Bd**p,int il=0){
		//vrednos koja se umece-vr
		//p==root
		if (u_stablu(vr)){
			if (il == 0) {
				cout << "Vrednost je vec u stablu";
			}
			return 0;
		}
		ubaci(vr);
		int i, j, k, l;
		Bd* q, * r, * s;
		q = *p;

		if (q == nullptr) {
			r = new Bd;
			if (r == nullptr) return -1;
			r->list = 1;
			r->brel = 1;
			//r->brlevo = 0;
			r->el[0] = vr;
			*p = r;
			return 0;
		}


		if (q->brel == 2 * MAXSIR - 1) {
			r = new Bd;
			if (r == nullptr) return -1;
			r->list = 0;
			r->brel = 0;
			r->nasl[0] = q;
			*p = r;
			razbij_cvor_drveta(r, 0);
		}

		q = *p;
		while (1) {
			if (q->list) {
				i = q->brel - 1;
				while ((i >= 0) && (q->el[i] > vr)) {
					//q->brlevo = q->brlevo + 1;
					q->el[i + 1] = q->el[i];
					i--;
				}
				q->el[i + 1] = vr;
				(q->brel)++;
				return 0;
			}
			else {
				for (i = 0; (i < q->brel) && (q->el[i] < vr);) {
					//q->brelvi = q->brelvi + 1;/greska
					i++;
				}
				if (q->nasl[i]->brel == 2 * MAXSIR - 1) {
					if (razbij_cvor_drveta(q, i) == -1) return -1;
					if (vr > q->el[i]) i++;
				}
				q = q->nasl[i];
			}
		}

	}

	int brisi_s(Bd* p, string vr) {
		return brisi(p, vr);
	}

	void ispis() {
		int nivo = 10;
		int nivop = 10;
		Bd* pom = new Bd;
		pom->str = "str";
		pom->brel = 0;
		queue<Bd*> red;
		red.push(root);
		red.push(nullptr);
		int ka = 0;
		cout << "                  ";
		while (!red.empty()) {
			
			Bd* next = red.front();
			red.pop();
			if (next) {
				if (next->brel == 0) ka++;
				cout <<setw(4*(nivop=-2))<<"   ";
				for (int i = 0; i < next->brel; i++) {
					cout << next->el[i] << " ";
 				}
				for (int i = next->brel; i < 2 * MAXSIR - 1; i++) {
					cout << "*";
				}

				for (int i = 0; i < 2 * MAXSIR; i++) {
					if (next->nasl[i]) {
						red.push(next->nasl[i]);
					}
					else {
						red.push(pom);
					}
				}

				if (ka > 30) break;
			}
			if (!next) {
				cout << endl;
				cout << setw(nivo-=2) << "   ";
				//cout << "   ";
				ka = 0;
				red.push(nullptr);
				if (red.front() == nullptr) break;
			}

			if (!next && !red.front()) {
				break;
			}

		}
	}

	string vrati_kti(int k) {
		int i = 1;
		for (Node* tek =prvi; tek; tek = tek->next) {
			cout << tek->str << "    ";
		}
		for (Node* tek = prvi; tek;tek = tek->next) {
			if (i == k) {
				return tek->str;
			}
			i++;
		}
		return "Prekoracenje";
	}
	string najmanji(Bd* pok,string vr){
		Bd* pom = pok;
		while (pom->nasl[0]) {
			pom = pom->nasl[0];
		}

		string elem = pom->el[0];
		pom->el[0] = vr;
		return elem;
	}

	string najveci(Bd* pok,string vr) {
		Bd* pom = pok;
		while (pom->nasl[pom->brel]) {
			pom = pom->nasl[pom->brel];
		}
		string elem = pom->el[pom->brel-1];
		pom->el[pom->brel-1] = vr;
		return elem;
	}


	int brisi(Bd* p, string vr) {
		//p==koren
		//vr==vrednost za brisanje

		
		int i;
		string j = vr;
		if (p->list) {
			for (i = 0; (i < p->brel) && (p->el[i] < vr); i++);
			if ((i < p->brel) && (p->el[i] == vr)) {
				p->brel--;
				for (; i < p->brel; i++) {
					p->el[i] = p->el[i + 1];
				}
			}
			return 0;
		}

		for (i = 0; (i < p->brel) && (p->el[i] < vr); i++);
		if (i < p->brel) {
			if (p->el[i] == vr) {
				if (p->nasl[i]->brel >= MAXSIR) {
					j = najveci(p->nasl[i],vr);
					brisi_s(p->nasl[i], vr);
					p->el[i] = j;
					return 0;
				}

				if (p->nasl[i + 1]->brel >= MAXSIR) {
					j = najmanji(p->nasl[i + 1],vr);
					brisi_s(p->nasl[i + 1], vr);
					p->el[i] = j;
					return 0;
				}
				spoj_cvorove(p, i);
				brisi_s(p->nasl[i], vr);
				return 0;
			}
		}

		if (p->nasl[i]->brel >= MAXSIR) {
			brisi_s(p->nasl[i], vr);
			return 0;
		}

		if ((i > 0) && (p->nasl[i - 1]->brel >= MAXSIR)) {
			prebaci_sleva(p, i);
			brisi_s(p->nasl[i], vr);
			return 0;
		}

		if ((i < p->brel) && (p->nasl[i + 1]->brel >= MAXSIR)) {
			prebaci_sdesna(p, i);
			brisi_s(p->nasl[i], vr);
			return 0;
		}

		if (i > 0) {
			spoj_cvorove(p, i - 1);
			brisi_s(p->nasl[i - 1], vr);
			return 0;
		}

		spoj_cvorove(p, i);
		brisi_s(p->nasl[(i - 1)>0?i-1:0], vr);
		return 0;
	}

	int izbrisi(Bd** p, string vr) {
		if (!u_stablu(vr)) {
			cout << "VREDNOST NIJE U STABLU." << endl;
			return 0;
		}
		izbaci(vr);
		Bd* q;
		q = *p;
		if ((q->brel == 1) && (q->nasl[0] && (q->nasl[0]->brel == MAXSIR - 1)) && (q->nasl[1] && (q->nasl[1]->brel == MAXSIR - 1))) {
			spoj_cvorove(q, 0);
			*p = q->nasl[0];
			delete q;
		}
		return brisi(*p, vr);
	}

	void isprazni() {
		queue<Bd*> red;
		red.push(root);
		while (!red.empty()) {
			Bd* pom = red.front();
			red.pop();
			for (int i = 0; i <= pom->brel; i++) {
				if (pom->nasl[i] != nullptr) {
					red.push(pom->nasl[i]);
				}
			}
			delete pom;
		}
		root = nullptr;
	}




};


#endif