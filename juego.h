// =====================================================================
//  juego.h  -  El juego completo (NO MODIFICAR)
//
//  Lo incluye fantasma.cpp. Para la tarea solo necesitas entender la
//  PARTE 1 (Pos y Mapa). El resto es el juego en si.
// =====================================================================
#ifndef JUEGO_H
#define JUEGO_H

#define NOMINMAX
#include <windows.h>
#include <conio.h>
#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

// =====================================================================
//  PARTE 1: Pos y Mapa
// =====================================================================

// Una posicion: fila f y columna c (la fila 0 es la de arriba).
struct Pos {
    int f, c;
};

bool operator==(Pos a, Pos b) {
    return a.f == b.f && a.c == b.c;
}

bool operator!=(Pos a, Pos b) {
    return !(a == b);
}

// El mapa:  '#' pared   '.' comida   'o' pildora   '~' lodo   ' ' vacio
struct Mapa {
    vector<string> celdas;
    int filas, cols;

    // Que hay en p. Si p esta fuera del mapa, cuenta como pared.
    char en(Pos p) {
        if (p.f < 0 || p.f >= filas || p.c < 0 || p.c >= cols) {
            return '#';
        }
        return celdas[p.f][p.c];
    }

    bool esPared(Pos p) {
        return en(p) == '#';
    }

    bool esComida(Pos p) {
        return en(p) == '.' || en(p) == 'o';
    }

    // Turnos que tarda un fantasma en entrar a p: 3 si es lodo, 1 si no.
    int costo(Pos p) {
        int costo = 1;
        if (en(p) == '~') {
            costo = 3;
        }
        return costo;
    }
};

// Crea una matriz de filas x cols llena con 'valor'.
vector<vector<int> > matriz(int filas, int cols, int valor) {
    return vector<vector<int> >(filas, vector<int>(cols, valor));
}

// =====================================================================
//  PARTE 2: las funciones que programas en fantasma.cpp
//  (se avisan aqui porque el juego las usa antes de que aparezcan)
// =====================================================================

bool mapaValido(Mapa& m, Pos pacman);
Pos pasoPerseguir(Mapa& m, Pos fantasma, Pos pacman);
Pos pasoEscapar(Mapa& m, Pos fantasma, Pos pacman);

// =====================================================================
//  PARTE 3: cargar un nivel desde mapas/nivelN.txt
// =====================================================================

int CANTIDAD_NIVELES = 6;  // si agregas mapas/nivel7.txt, cambia esto a 7

string archivoNivel(int n) {
    string nombre = "mapas/nivel" + to_string(n) + ".txt";
    return nombre;
}

// Lee el nivel n. Guarda donde estan la 'P' (Pac-Man) y las 'G' (fantasmas)
// y las borra del mapa, porque se dibujan aparte (se mueven).
bool cargarNivel(int n, Mapa& m, Pos& pacman, vector<Pos>& fantasmas) {
    ifstream archivo;
    string linea;
    Pos fantasma;

    archivo.open(archivoNivel(n));
    if (!archivo) {
        return false;
    }

    m.celdas.clear();
    fantasmas.clear();
    while (getline(archivo, linea)) {
        if (!linea.empty()) {
            m.celdas.push_back(linea);
        }
    }
    m.filas = m.celdas.size();
    m.cols = m.celdas[0].size();

    for (int f = 0; f < m.filas; f++) {
        for (int c = 0; c < m.cols; c++) {
            if (m.celdas[f][c] == 'P') {
                pacman.f = f;
                pacman.c = c;
                m.celdas[f][c] = ' ';
            }
            if (m.celdas[f][c] == 'G') {
                fantasma.f = f;
                fantasma.c = c;
                fantasmas.push_back(fantasma);
                m.celdas[f][c] = ' ';
            }
        }
    }
    return true;
}

// =====================================================================
//  PARTE 4: jugar un nivel
// =====================================================================

struct Fantasma {
    Pos pos;        // donde esta ahora
    Pos inicio;     // su casa: vuelve aqui si se lo comen o si Pac-Man pierde una vida
    int espera;     // turnos que le faltan para volver a moverse (lodo o recien comido)
    bool asustado;  // true despues de que Pac-Man come una pildora
    bool comido;    // true si Pac-Man se lo acaba de comer: espera en su casa y no hace dano
};

int contarComida(Mapa& m) {
    int n = 0;
    for (int f = 0; f < m.filas; f++) {
        for (int c = 0; c < m.cols; c++) {
            if (m.celdas[f][c] == '.' || m.celdas[f][c] == 'o') {
                n++;
            }
        }
    }
    return n;
}

// Dibuja un cuadro del juego encima del anterior.
void dibujar(Mapa& m, Pos pac, vector<Fantasma>& fs, int puntos, int vidas, int susto, string mensaje) {
    string fila;

    // Llevar el cursor a la esquina de arriba a la izquierda (columna 0, fila 0)
    COORD esquina = {0, 0};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), esquina);

    cout << endl<<"";
    for (int f = 0; f < m.filas; f++) {
        fila = m.celdas[f];
        for (int i = 0; i < fs.size(); i++) {
            if (fs[i].pos.f == f) {
                if (fs[i].asustado) {
                    fila[fs[i].pos.c] = 'g';
                } else {
                    fila[fs[i].pos.c] = 'G';
                }
            }
        }
        if (pac.f == f) {
            fila[pac.c] = 'C';
        }
        cout << "  " << fila << endl<<"";
    }

    cout << endl;
    cout << "  Puntos: " << puntos << "      ";
    cout << "Vidas: " << vidas;
    cout << "   Comida: " << contarComida(m) << "    " << endl;

    if (susto > 0) {
        cout << "  Fantasmas asustados: comelos!              " << endl;
    } else {
        cout << "                                             " << endl;
    }

    cout << "  Flechas o W A S D para moverte, Q para salir" << endl;
    cout << "  " << mensaje << "                                             " << endl;

}

// Lee una tecla SIN DETENER el juego. Devuelve 'w', 'a', 's', 'd', 'q' o 0 si no hay.
char leerTecla() {
    char tecla = 0;
    int codigo;
    while (_kbhit()) {
        codigo = _getch();
        if (codigo == 0 || codigo == 224) {  // las flechas mandan dos codigos
            codigo = _getch();
            if (codigo == 72) {
                tecla = 'w';
            }
            if (codigo == 75) {
                tecla = 'a';
            }
            if (codigo == 80) {
                tecla = 's';
            }
            if (codigo == 77) {
                tecla = 'd';
            }
        } else {
            tecla = tolower(codigo);
        }
    }
    return tecla;
}

// Crea los fantasmas en sus casas.
vector<Fantasma> crearFantasmas(vector<Pos>& casas) {
    vector<Fantasma> fs(casas.size());
    for (int i = 0; i < fs.size(); i++) {
        fs[i].pos = casas[i];
        fs[i].inicio = casas[i];
        fs[i].espera = 0;
        fs[i].asustado = false;
        fs[i].comido = false;
    }
    return fs;
}

// Cambia la direccion de Pac-Man (df, dc) segun la tecla.
void cambiarDireccion(char tecla, int& df, int& dc) {
    if (tecla == 'w') {
        df = -1;
        dc = 0;
    }
    if (tecla == 's') {
        df = 1;
        dc = 0;
    }
    if (tecla == 'a') {
        df = 0;
        dc = -1;
    }
    if (tecla == 'd') {
        df = 0;
        dc = 1;
    }
}

// Pac-Man avanza una casilla (si no hay pared) y come lo que haya.
void moverPacman(Mapa& m, Pos& pac, int df, int dc, vector<Fantasma>& fs, int& puntos, int& susto) {
    Pos siguiente = {pac.f + df, pac.c + dc};
    if (!m.esPared(siguiente)) {
        pac = siguiente;
    }
    if (m.celdas[pac.f][pac.c] == '.') {
        puntos += 10;
        m.celdas[pac.f][pac.c] = ' ';  // se comio la comida: la casilla queda vacia
    }
    if (m.celdas[pac.f][pac.c] == 'o') {
        puntos += 50;
        susto = 50;
        for (int i = 0; i < fs.size(); i++) {
            fs[i].asustado = true;
        }
        m.celdas[pac.f][pac.c] = ' ';  // se comio la pildora: la casilla queda vacia
    }
}

// Los fantasmas son mas lentos que Pac-Man: no se mueven en todos los turnos.
bool leTocaMoverse(Fantasma& g, int turno) {
    if (g.asustado) {
        return turno % 2 == 0;  // asustado: 1 de cada 2 turnos
    }
    return turno % 4 != 0;      // normal: 3 de cada 4 turnos
}

// true si b esta justo arriba, abajo, a la izquierda o a la derecha de a.
bool esVecina(Pos a, Pos b) {
    return abs(a.f - b.f) + abs(a.c - b.c) == 1;
}

// true si algun fantasma esta en la casilla p.
bool hayFantasma(vector<Fantasma>& fs, Pos p) {
    for (int i = 0; i < fs.size(); i++) {
        if (fs[i].pos == p) {
            return true;
        }
    }
    return false;
}

// Cada fantasma pregunta a TUS funciones a donde moverse.
void moverFantasmas(Mapa& m, vector<Fantasma>& fs, Pos pac, int turno) {
    Pos nueva;
    for (int i = 0; i < fs.size(); i++) {
        if (fs[i].pos == pac || !leTocaMoverse(fs[i], turno)) {
            continue;
        }
        if (fs[i].espera > 0) {
            fs[i].espera--;
            continue;
        }
        fs[i].comido = false;  // ya salio de su casa: vuelve a ser peligroso

        if (fs[i].asustado) {
            nueva = pasoEscapar(m, fs[i].pos, pac);
        } else {
            nueva = pasoPerseguir(m, fs[i].pos, pac);
        }

        // Solo se acepta moverse a una casilla vecina que no sea pared
        // ni tenga otro fantasma (asi nunca quedan dos en la misma casilla)
        if (esVecina(fs[i].pos, nueva) && !m.esPared(nueva) && !hayFantasma(fs, nueva)) {
            fs[i].pos = nueva;
            if (m.en(nueva) == '~') {
                fs[i].espera = 2;  // se queda pegado en el lodo
            }
        }
    }
}

// Descuenta un turno de susto. Cuando se acaba, nadie esta asustado.
void actualizarSusto(int& susto, vector<Fantasma>& fs) {
    if (susto > 0) {
        susto--;
    }
    if (susto == 0) {
        for (int i = 0; i < fs.size(); i++) {
            fs[i].asustado = false;
        }
    }
}

// Revisa si algun fantasma esta en la casilla de Pac-Man.
// Si estaba asustado, Pac-Man se lo come. Devuelve true si atraparon a Pac-Man.
bool revisarChoques(vector<Fantasma>& fs, Pos pac, int& puntos) {
    for (int i = 0; i < fs.size(); i++) {
        if (fs[i].pos == pac && !fs[i].comido) {
            if (!fs[i].asustado) {
                return true;
            }
            puntos += 200;           // Pac-Man se come al fantasma
            fs[i].pos = fs[i].inicio;
            fs[i].asustado = false;
            fs[i].comido = true;
            fs[i].espera = 10;       // espera un rato en su casa
        }
    }
    return false;
}

// Todos vuelven a donde empezaron (despues de perder una vida).
void volverAlInicio(Pos& pac, Pos inicioPac, vector<Fantasma>& fs, int& df, int& dc) {
    pac = inicioPac;
    df = 0;
    dc = 0;
    for (int i = 0; i < fs.size(); i++) {
        fs[i].pos = fs[i].inicio;
        fs[i].espera = 0;
        fs[i].comido = false;
    }
}

// Juega un nivel. Devuelve true si Pac-Man se comio toda la comida.
bool jugarNivel(Mapa m, Pos inicioPac, vector<Pos> casasFantasmas, int& puntos, int& vidas) {
    vector<Fantasma> fs = crearFantasmas(casasFantasmas);
    Pos pac = inicioPac;
    int df = 0, dc = 0;  // direccion de Pac-Man
    int susto = 0;       // turnos que les quedan asustados a los fantasmas
    char tecla;

    system("cls");
    int turno = 0;
    while (true) {
        tecla = leerTecla();
        if (tecla == 'q') {
            return false;
        }
        cambiarDireccion(tecla, df, dc);

        moverPacman(m, pac, df, dc, fs, puntos, susto);
        moverFantasmas(m, fs, pac, turno);
        actualizarSusto(susto, fs);

        if (revisarChoques(fs, pac, puntos)) {
            vidas--;
            if (vidas == 0) {
                dibujar(m, pac, fs, puntos, vidas, susto, "GAME OVER");
                Sleep(1500);
                return false;
            }
            dibujar(m, pac, fs, puntos, vidas, susto, "Te atraparon!");
            Sleep(1500);
            volverAlInicio(pac, inicioPac, fs, df, dc);
        }

        if (contarComida(m) == 0) {
            dibujar(m, pac, fs, puntos, vidas, susto, "NIVEL COMPLETADO!");
            Sleep(1500);
            return true;
        }

        dibujar(m, pac, fs, puntos, vidas, susto, "");
        Sleep(130);  // espera 130 milisegundos entre cuadros
        turno++;
    }
}

// =====================================================================
//  PARTE 5: el menu
// =====================================================================

// Carga el nivel n, lo revisa con TU mapaValido() y, si esta bien, lo juega.
// Devuelve true si se puede seguir al siguiente nivel (gano o estaba roto).
bool probarNivel(int n, int& puntos, int& vidas) {
    Mapa m;
    Pos pac;
    vector<Pos> fantasmas;

    system("cls");
    if (!cargarNivel(n, m, pac, fantasmas)) {
        cout << endl<<"  No se encontro el archivo " << archivoNivel(n) << endl<<"";
        cout << "  La carpeta 'mapas' tiene que estar junto a fantasma.cpp"<<endl;
        Sleep(3000);
        return false;
    }
    if (!mapaValido(m, pac)) {
        cout << endl<<"  El nivel " << n << " esta ROTO: tiene comida a la que Pac-Man no puede llegar."<<endl;
        cout << "  No se carga."<<endl;
        Sleep(2500);
        return true;
    }
    return jugarNivel(m, pac, fantasmas, puntos, vidas);
}

#endif