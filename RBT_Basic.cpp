#include <iostream>

enum Color {BLACK, RED};

typedef struct Nodo{
    int dato;
    Color col;
    struct Nodo* parent;
    struct Nodo* izq;
    struct Nodo* der;
} Nodo;

enum Side {LEFT,RIGHT};

using namespace std;

Nodo* crear(){
    return NULL;
}

bool estaVacio(Nodo* T){
    return (T == NULL)?true:false;
}

Nodo* crearNodo(int n){
    Nodo* nuevo;
    nuevo = (Nodo*) malloc(sizeof(Nodo)); //Alternativa 1
    //nuevo = new Nodo;                     //Alternativa 2
    if(nuevo == NULL){
        cout << "CATASTROPHIC FAILURE!!!!! :'(\n";
        exit(EXIT_FAILURE);
    }
    nuevo->dato = n;
    nuevo->col = RED;
    nuevo->parent = NULL;
    nuevo->izq = NULL;
    nuevo->der = NULL;
   
    return nuevo;
}

void mostrarInOrder(Nodo* T){
    if(estaVacio(T)){
        cout << "nil" << "-" << BLACK << "\n";
        return;
    }
    mostrarInOrder(T->izq);
    cout << T->dato << "-" << T->col << "\n";
    mostrarInOrder(T->der);
}

void mostrarPreOrder(Nodo* T){
    if(estaVacio(T)){
        cout << "nil" << "-" << BLACK << "\n";
        return;
    }
    cout << T->dato << "-" << T->col << "\n";
    mostrarPreOrder(T->izq);
    mostrarPreOrder(T->der);
}

void mostrarPostOrder(Nodo* T){
    if(estaVacio(T)){
        cout << "nil" << "-" << BLACK << "\n";
        return;
    }
    mostrarPostOrder(T->izq);
    mostrarPostOrder(T->der);
    cout << T->dato << "-" << T->col << "\n";
}

bool esta(Nodo* T, int n){
    if(estaVacio(T)) return false;
    if(T->dato == n) return true;
    if(n <= T->dato) return esta(T->izq, n);
    else return esta(T->der, n);
}

int inOrderSuccessor(Nodo* T){
    if(T->izq == NULL) return T->dato;
    return inOrderSuccessor(T->izq);
}

void destruirArbol(Nodo* T){
    if(estaVacio(T)) return;
    destruirArbol(T->izq);
    destruirArbol(T->der);
    free(T);
}

///////////////////////////////START//////////////////////////////

void rotarLEFT(Nodo *&A, Nodo *&B){
    cout << "Rotando LEFT: " << A->dato << " y " << B->dato << "\n";
    Nodo* Adad = A->parent;
    Nodo* Bizq = B->izq;
   
    if(Bizq != NULL) Bizq->parent = A;
    B->izq = A;
    A->der = Bizq;
    A->parent = B;
    B->parent = Adad;
    if(Adad != NULL)
        if(Adad->izq == A)
            Adad->izq = B;
        else
            Adad->der = B;
}

void rotarRIGHT(Nodo *&A, Nodo *&B){
    cout << "Rotando RIGHT: " << A->dato << " y " << B->dato << "\n";
    Nodo* Adad = A->parent;
    Nodo* Bder = B->der;
   
    if(Bder != NULL) Bder->parent = A;
    B->der = A;
    A->izq = Bder;
    A->parent = B;
    B->parent = Adad;
    if(Adad != NULL)
        if(Adad->izq == A)
            Adad->izq = B;
        else
            Adad->der = B;
}

Nodo* getRoot(Nodo* n){
    if(n->parent == NULL) return n;
    else return getRoot(n->parent);
}

void fixRoot(Nodo *&T, Nodo *&recent){
    Nodo* root_actual = getRoot(recent);
    if(T != root_actual) T = root_actual;
}

void fixDoubleRed(Nodo *&T, Nodo *&recent){
    Side s1,s2;
    Nodo* daddy;
    Nodo* grandpa;
    Nodo* uncle;
    while(true){
        grandpa = NULL;
        uncle = NULL;
        daddy = recent->parent;
        if(daddy != NULL) grandpa = daddy->parent;
        if(daddy != NULL && grandpa != NULL && daddy->col != BLACK){
            if(daddy == grandpa->izq){
                s1 = LEFT;
                uncle = grandpa->der;
            }else{
                s1 = RIGHT;
                uncle = grandpa->izq;
            }
            if(recent == daddy->izq) s2 = LEFT;
            else s2 = RIGHT;
            if(uncle == NULL || uncle->col == BLACK){
                if(s1 == LEFT && s2 == LEFT){
                    rotarRIGHT(grandpa,daddy);
                    daddy->col = BLACK;
                    grandpa->col = RED;
                }
                if(s1 == LEFT && s2 == RIGHT){
                    rotarLEFT(daddy,recent);
                    rotarRIGHT(grandpa,recent);
                    recent->col = BLACK;
                    grandpa->col = RED;
                }
                if(s1 == RIGHT && s2 == LEFT){
                    rotarRIGHT(daddy,recent);
                    rotarLEFT(grandpa,recent);
                    recent->col = BLACK;
                    grandpa->col = RED;
                }
                if(s1 == RIGHT && s2 == RIGHT){
                    rotarLEFT(grandpa,daddy);
                    daddy->col = BLACK;
                    grandpa->col = RED;
                }
                break;
            }else{
                daddy->col = BLACK;
                uncle->col = BLACK;
                grandpa->col = RED;
                recent = grandpa;
                continue;
            }
        }else break;
    }
    fixRoot(T,recent);
    T->col = BLACK;
}

Nodo* insertarAux(Nodo *&T, int n){
    Nodo* recent = NULL;
   
    if(n <= T->dato){
        if(T->izq == NULL){
            recent = crearNodo(n);
            recent->parent = T;
            T->izq = recent;
        }else recent = insertarAux(T->izq, n);
    }else{
        if(T->der == NULL){
            recent = crearNodo(n);
            recent->parent = T;
            T->der = recent;
        }else recent = insertarAux(T->der, n);
    }
   
    return recent;
}

void insertar(Nodo *&T, int n){
    if(estaVacio(T)){
        T = crearNodo(n);
        T->col = BLACK;
    }else{
        Nodo* nuevo = insertarAux(T, n);
        if(nuevo->parent->col != BLACK) fixDoubleRed(T,nuevo);
    }
}

void fixDoubleBlack(Nodo *&T, Nodo *&H, Nodo *&P){
    Nodo* hijo;
    Nodo* sibling;
    Nodo* cousin;
    Nodo* daddy;
    hijo = H;
    while(true){
        sibling = NULL;
        cousin = NULL;
        if(hijo != NULL) daddy = hijo->parent;
        else daddy = P;
        if(daddy != NULL){
            Side lado_hijo = (daddy->izq == hijo)?LEFT:RIGHT;
            Side lado_sibling, lado_cousin;
            if(lado_hijo == LEFT){
                sibling = daddy->der;
                lado_sibling = RIGHT;
            }else{
                sibling = daddy->izq;
                lado_sibling = LEFT;
            }
            if(sibling->col == BLACK){
                if((sibling->izq == NULL || sibling->izq->col == BLACK) && (sibling->der == NULL || sibling->der->col == BLACK)){
                    if(daddy->col == BLACK){
                        sibling->col = RED;
                        if(hijo != NULL) hijo->col = BLACK;
                        hijo = daddy;
                        continue;
                    }else{
                        sibling->col = RED;
                        daddy->col = BLACK;
                        if(hijo != NULL) hijo->col = BLACK;
                        break;
                    }
                }else{
                    if(sibling->izq == NULL || sibling->izq->col == BLACK){
                        cousin = sibling->der;
                        lado_cousin = RIGHT;
                    }else{
                        cousin = sibling->izq;
                        lado_cousin = LEFT;
                    }
                    if(lado_sibling == RIGHT && lado_cousin == RIGHT){
                        rotarLEFT(daddy,sibling);
                        cousin->col = BLACK;
                        sibling->col = daddy->col;
                    }
                    if(lado_sibling == RIGHT && lado_cousin == LEFT){
                        rotarRIGHT(sibling,cousin);
                        rotarLEFT(daddy,cousin);
                        cousin->col = daddy->col;
                    }
                    if(lado_sibling == LEFT && lado_cousin == RIGHT){
                        rotarLEFT(sibling,cousin);
                        rotarRIGHT(daddy,cousin);
                        cousin->col = daddy->col;
                    }
                    if(lado_sibling == LEFT && lado_cousin == LEFT){
                        rotarRIGHT(daddy,sibling);
                        cousin->col = BLACK;
                        sibling->col = daddy->col;
                    }
                    daddy->col = BLACK;
                    if(hijo != NULL) hijo->col = BLACK;
                    break;
                }
            }else{
                if(lado_sibling == RIGHT) rotarLEFT(daddy,sibling);
                else rotarRIGHT(daddy,sibling);
                sibling->col = BLACK;
                daddy->col = RED;
                continue;
            }
        }else{
            if(hijo != NULL) hijo->col = BLACK;
            break;
        }
    }
    fixRoot(T,daddy);
    T->col = BLACK;
}

Color borrarAux(Nodo *&T, int n, Nodo *&recent, Nodo *&daddy){
    if(T->dato == n){
        if(T->izq == NULL && T->der == NULL){
            Color d = T->col;
            daddy = T->parent;
            free(T);
            T = NULL;
            return d;
        }
        if(T->izq == NULL || T->der == NULL){
            Side lado_nulo = (T->izq == NULL)?LEFT:RIGHT;
            Nodo* temp = T;
            if(lado_nulo == LEFT) T = T->der;
            else T = T->izq;
            T->parent = temp->parent;
            Color d = temp->col;
            Color h = T->col;
            free(temp);
            if(d == BLACK && h == BLACK){
                recent = T;
                daddy = T->parent;
                return BLACK;
            }else{
                T->col = BLACK;
                return RED;
            }
        }
        int sustituto = inOrderSuccessor(T->der);
        T->dato = sustituto;
        return borrarAux(T->der, sustituto, recent, daddy);
    }
    if(n < T->dato) return borrarAux(T->izq, n, recent, daddy);
    else return borrarAux(T->der, n, recent, daddy);
}

void borrar(Nodo *&T, int n){
    Nodo* hijo = NULL;
    Nodo* daddy = NULL;
    Color caso = borrarAux(T, n, hijo, daddy);
    if(caso != RED) fixDoubleBlack(T, hijo, daddy);
}

//////////////////////////////END/////////////////////////////////////

int main()
{
    Nodo* T;
   
    T = crear();
   
    int datos[10] = {4,7,12,15,3,5,14,18,16,17};
    for(int i=0;i<10;i++){
        insertar(T, datos[i]);
        mostrarPreOrder(T);
        cin.get();
    }
    
    cout << "========================================\n";
    cin.get();
    /*
    int datos2[6] = {3,12,17,18,15,16};
    for(int i=0;i<6;i++){
        borrar(T, datos2[i]);
        mostrarPreOrder(T);
        cin.get();
    }*/
    
    //destruirArbol(T);

    return 0;
}