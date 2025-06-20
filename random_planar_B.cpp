#include <iostream>
#include <set>
#include <vector>
using namespace std;

typedef int node;

struct node_linked_list {
    node v;
    node_linked_list *next;
};

struct edge {
    node v1;
    node v2;
};

struct graf {
    vector<node> V;
    vector<edge> E;
};

static int id = 1;
node buatSimpul()
{
    return id++;
}

int length(node_linked_list *A) {
    int lengthA = 0;
    node_linked_list *Ai = A;
    do {
        lengthA++;
        Ai = Ai->next;
    } while (Ai != A);
    return lengthA;
}

node_linked_list *getRandomNode(node_linked_list *A)
{
    node_linked_list *Ai; // Untuk iterasi

    // Dapatkan simpul acak
    int r = rand() % length(A);
    Ai = A;
    while (r--) {
        Ai = Ai->next;
    }

    return Ai;
}

void tambahSimpul(vector<node> &V, vector<edge> &E, node_linked_list **A)
{
    node p = buatSimpul();
    V.push_back(p);
    node_linked_list *Ai;        // Untuk iterasi
    node_linked_list *A1 = NULL; // Urutan pertama yang terpilih di list A
    node_linked_list *Ak = NULL; // Simpul terakhir yang terhubung dengan p
    set<node> B;                 // Himpunan simpul yang telah terhubung

    // Dapatkan A1 random
    A1 = getRandomNode(*A);
    E.push_back({p, A1->v});
    Ai = A1->next;
    Ak = A1;
    do {
        // Peluang 0.5 untuk membuat sisi (p, Ai), dengan syarat p dan Ai belum terhubung
        if (!B.count(Ai->v) && rand() % 2) {
            E.push_back({p, Ai->v});
            B.insert(Ai->v);
            Ak = Ai;
        }
        // Iterasi simpul selanjutnya
        Ai = Ai->next;
    } while (Ai != A1);

    node_linked_list *link_p = (node_linked_list *)malloc(sizeof(node_linked_list));
    link_p->v = p;
    if (A1 != Ak) { // p berderajat setidaknya 2
        A1->next = link_p;
        link_p->next = Ak;
    }
    else { // p berderajat 1
        // Salin A1 untuk diletakkan setelah p
        node_linked_list *r = (node_linked_list *)malloc(sizeof(node_linked_list));
        r->v = A1->v;
        r->next = A1->next;

        A1->next = link_p;
        link_p->next = r;
    }

    *A = A1;
}

static double panjangA = 0;
graf buatGrafPlanar(int n)
{
    node v1 = buatSimpul();
    vector<node> V = {v1};
    vector<edge> E = {};

    node_linked_list *A = (node_linked_list *)malloc(sizeof(node_linked_list));
    *A = {v1, A};

    double panjang = 1;
    for (int i = 2; i <= n; i++) {
        tambahSimpul(V, E, &A);
    }
    return {V, E};
}

int main()
{
    srand(time(NULL));
    int simpul = 10;
    int banyakCoba = 10;
    for (int i = 1; i <= banyakCoba; i++) {
        cout << "GRAF KE-" << i << endl;
        clock_t t1, t2;
        t1 = clock();
        graf G = buatGrafPlanar(simpul);
        t2 = clock();
        cout << "EXECUTION TIME: " << t2 - t1 << "ms" << endl;

        cout << "NODES: " << endl;
        for (auto v : G.V) {
            cout << v << " ";
        }
        cout << endl;

        cout << "EDGES: " << endl;
        for (auto e : G.E) {
            cout << "" << e.v1 << " " << e.v2 << endl;
        }
        cout << endl;
    }

    return 0;
}