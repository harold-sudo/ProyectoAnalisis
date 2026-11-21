// =====================================================================
//  fantasma.cpp  -  EL UNICO ARCHIVO QUE MODIFICAN
//
//  Integrantes:
//    1.
//    2.
//    3.
//
//  Compilen y ejecuten ESTE archivo para jugar.
// =====================================================================
#include "juego.h"

// Los 4 movimientos: arriba, izquierda, abajo, derecha
int DF[4] = {-1, 0, 1, 0};
int DC[4] = {0, -1, 0, 1};


// ---------------------------------------------------------------------
//  PARTE 1: detectar niveles rotos
// ---------------------------------------------------------------------


// true si Pac-Man puede llegar a toda la comida ('.' y 'o').
bool mapaValido(Mapa& m, Pos pacman) {
    // TODO
    return true;
}

// ---------------------------------------------------------------------
//  PARTE 2: perseguir a Pac-Man
// ---------------------------------------------------------------------

// La casilla vecina a la que se mueve el fantasma para atrapar a Pac-Man
// lo antes posible. Si no puede llegar, devuelve 'fantasma' (se queda).
Pos pasoPerseguir(Mapa& m, Pos fantasma, Pos pacman) {
    // TODO
    return fantasma;
}

// ---------------------------------------------------------------------
//  PARTE 3: escapar de Pac-Man
// ---------------------------------------------------------------------

// La casilla vecina a la que se mueve un fantasma asustado para quedar
// lo mas lejos posible de Pac-Man. Si no le conviene moverse, devuelve
// 'fantasma' (se queda).
Pos pasoEscapar(Mapa& m, Pos fantasma, Pos pacman) {
    // TODO
    return fantasma;
}

// INICIO
int main() {
    char opcion;
    int nivel, puntos, vidas;
    bool sigue;

    while (true) {
        system("cls");
        cout << endl<<"  ========== P A C - M A N =========="<<endl<<endl;
        cout << "   1. Jugar"<<endl;
        cout << "   2. Elegir nivel (1 a " << CANTIDAD_NIVELES << ")"<<endl;
        cout << "   3. Salir"<<endl<<endl;
        cout << "  Elige una opcion: ";
        opcion = _getch();
        puntos = 0;
        vidas = 3;

        if (opcion == '1') {
            // Todos los niveles en orden, desde el 1. Los rotos se saltan.
            sigue = true;
            for (nivel = 1; nivel <= CANTIDAD_NIVELES; nivel++) {
                if(sigue){
                    sigue = probarNivel(nivel, puntos, vidas);
                }
            }
            system("cls");
            if (sigue) {
                cout << endl<<"  GANASTE TODOS LOS NIVELES!";
            } else {
                cout << endl<<"  Fin del juego.";
            }
            cout << "  Puntos: " << puntos << endl<<"  Presiona una tecla...";
            _getch();
        }
        if (opcion == '2') {
            cout << endl<<endl<<"  Numero de nivel: ";
            nivel = _getch() - '0';  // la tecla '3' se convierte en el numero 3
            if (nivel >= 1 && nivel <= CANTIDAD_NIVELES) {
                probarNivel(nivel, puntos, vidas);
                system("cls");
                cout << endl<<"  Puntos: " << puntos << endl<<"  Presiona una tecla...";
                _getch();
            }
        }
        if (opcion == '3') {
            return 0;
        }
    }
}