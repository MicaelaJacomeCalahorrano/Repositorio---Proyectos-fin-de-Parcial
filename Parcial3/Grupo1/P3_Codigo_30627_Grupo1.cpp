#include <iostream>
using namespace std;

const int MAX_PARTICIPANTES = 6;
const int MAX_ETAPAS        = 6;
const int NUM_OPCIONES      = 3;

const int ESTADO_ACUERDO   = 0;
const int ESTADO_TENSION   = 1;
const int ESTADO_CONFLICTO = 2;

const int SIMBOLO_ALTA_COOP  = 0;
const int SIMBOLO_MEDIA_COOP = 1;
const int SIMBOLO_BAJA_COOP  = 2;

struct Participante {
    char nombre[30];
    int  intereses[NUM_OPCIONES];
    int  puntaje;
};

struct NodoArbol {
    int participante;
    int opcion;
    int puntajeAcumulado;
    NodoArbol* hijos[NUM_OPCIONES];
};

void inicializarParticipantes(Participante participantes[], int numParticipantes) {
    cout << "\n--- Registro de participantes ---\n";
    for (int i = 0; i < numParticipantes; i++) {
        cout << "\nParticipante " << (i + 1) << "\n";
        cout << "Nombre (sin espacios): ";
        cin >> participantes[i].nombre;

        cout << "Interes hacia Cooperar (0-10): ";
        cin >> participantes[i].intereses[0];
        cout << "Interes hacia Competir (0-10): ";
        cin >> participantes[i].intereses[1];
        cout << "Interes hacia Ceder    (0-10): ";
        cin >> participantes[i].intereses[2];

        participantes[i].puntaje = 0;
    }
}

int generarNumeroAleatorio(int &semilla, int limite) {
    semilla = (semilla * 1103515245 + 12345) % 2147483647;
    if (semilla < 0) {
        semilla = -semilla;
    }
    return semilla % limite;
}

void mostrarNombreOpcion(int opcion) {
    if (opcion == 0) {
        cout << "Cooperar";
    } else if (opcion == 1) {
        cout << "Competir";
    } else {
        cout << "Ceder";
    }
}

int elegirOpcion(Participante participantes[], int idx, int &semilla, int opcionAnterior) {
    int mejorOpcion = 0;
    for (int o = 1; o < NUM_OPCIONES; o++) {
        if (participantes[idx].intereses[o] > participantes[idx].intereses[mejorOpcion]) {
            mejorOpcion = o;
        }
    }

    int azar = generarNumeroAleatorio(semilla, 10);

    if (opcionAnterior == 1 && azar < 4) {
        return 1;
    }

    if (azar < 7) {
        return mejorOpcion;
    }

    return generarNumeroAleatorio(semilla, NUM_OPCIONES);
}

void simularNegociacion(Participante participantes[], int numParticipantes,
                         int numEtapas, int decisiones[][MAX_PARTICIPANTES], int &semilla) {
    for (int e = 0; e < numEtapas; e++) {
        int opcionAnterior = -1;
        for (int p = 0; p < numParticipantes; p++) {
            int opcion = elegirOpcion(participantes, p, semilla, opcionAnterior);
            decisiones[e][p] = opcion;
            opcionAnterior = opcion;
        }
    }
}

void mostrarDecisiones(Participante participantes[], int numParticipantes,
                        int numEtapas, int decisiones[][MAX_PARTICIPANTES]) {
    cout << "\n--- Tabla de decisiones por etapa ---\n";

    cout << "Etapa\t";
    for (int p = 0; p < numParticipantes; p++) {
        cout << participantes[p].nombre << "\t";
    }
    cout << "\n";

    for (int e = 0; e < numEtapas; e++) {
        cout << (e + 1) << "\t";
        for (int p = 0; p < numParticipantes; p++) {
            mostrarNombreOpcion(decisiones[e][p]);
            cout << "\t";
        }
        cout << "\n";
    }
}

void evaluarResultados(Participante participantes[], int numParticipantes,
                        int decisiones[][MAX_PARTICIPANTES], int numEtapas) {
    for (int p = 0; p < numParticipantes; p++) {
        participantes[p].puntaje = 0;
    }

    for (int e = 0; e < numEtapas; e++) {
        for (int p = 0; p < numParticipantes; p++) {
            int opcionElegida = decisiones[e][p];
            participantes[p].puntaje += participantes[p].intereses[opcionElegida];
        }
    }
}

void ordenarPorPuntaje(Participante participantes[], int indices[], int numParticipantes) {
    for (int i = 0; i < numParticipantes; i++) {
        indices[i] = i;
    }
    for (int i = 0; i < numParticipantes - 1; i++) {
        for (int j = 0; j < numParticipantes - 1 - i; j++) {
            if (participantes[indices[j]].puntaje < participantes[indices[j + 1]].puntaje) {
                int temp = indices[j];
                indices[j] = indices[j + 1];
                indices[j + 1] = temp;
            }
        }
    }
}

void mostrarResumenFinal(Participante participantes[], int numParticipantes) {
    int indices[MAX_PARTICIPANTES];
    ordenarPorPuntaje(participantes, indices, numParticipantes);

    cout << "\n--- Resumen final de puntajes ---\n";
    for (int i = 0; i < numParticipantes; i++) {
        int idx = indices[i];
        cout << (i + 1) << ". " << participantes[idx].nombre
             << " -> Puntaje: " << participantes[idx].puntaje << "\n";
    }
}

void analizarEstabilidad(Participante participantes[], int numParticipantes,
                          int decisiones[][MAX_PARTICIPANTES], int numEtapas) {
    cout << "\n--- Analisis de estabilidad del acuerdo ---\n";

    int cooperacionesFinales = 0;
    int ultimaEtapa = numEtapas - 1;
    for (int p = 0; p < numParticipantes; p++) {
        if (decisiones[ultimaEtapa][p] == 0) {
            cooperacionesFinales++;
        }
    }

    int totalCooperaciones = 0;
    int totalDecisiones = numEtapas * numParticipantes;
    for (int e = 0; e < numEtapas; e++) {
        for (int p = 0; p < numParticipantes; p++) {
            if (decisiones[e][p] == 0) {
                totalCooperaciones++;
            }
        }
    }

    cout << "Cooperaciones en la ultima etapa: " << cooperacionesFinales
         << " de " << numParticipantes << " participantes.\n";
    cout << "Cooperaciones totales durante la negociacion: " << totalCooperaciones
         << " de " << totalDecisiones << " decisiones tomadas.\n";

    if (cooperacionesFinales == numParticipantes) {
        cout << "\nRESULTADO: ACUERDO ESTABLE.\n";
        cout << "Justificacion: en la etapa final todos los participantes eligieron\n";
        cout << "Cooperar, lo que indica convergencia de intereses y un acuerdo\n";
        cout << "sostenible para todas las partes.\n";
    } else if (cooperacionesFinales * 2 >= numParticipantes) {
        cout << "\nRESULTADO: ACUERDO PARCIAL.\n";
        cout << "Justificacion: mas de la mitad de los participantes cooperaron en\n";
        cout << "la etapa final, pero persisten diferencias de interes que impiden\n";
        cout << "un acuerdo total y estable.\n";
    } else {
        cout << "\nRESULTADO: CONFLICTO.\n";
        cout << "Justificacion: en la etapa final predominaron actitudes de\n";
        cout << "Competir o Ceder por presion, no de cooperacion genuina, lo cual\n";
        cout << "evidencia que los intereses de los participantes no lograron\n";
        cout << "alinearse y la negociacion no produjo un acuerdo estable.\n";
    }
}

void construirGrafoInfluencia(int grafo[][MAX_PARTICIPANTES], int numParticipantes) {
    for (int i = 0; i < numParticipantes; i++) {
        for (int j = 0; j < numParticipantes; j++) {
            grafo[i][j] = 0;
        }
    }
    for (int i = 0; i < numParticipantes; i++) {
        int siguiente = (i + 1) % numParticipantes;
        grafo[i][siguiente] = 1;
    }
}

void mostrarGrafoInfluencia(int grafo[][MAX_PARTICIPANTES], Participante participantes[], int numParticipantes) {
    cout << "\n--- Grafo dirigido de influencia (matriz de adyacencia) ---\n";
    cout << "\t";
    for (int j = 0; j < numParticipantes; j++) {
        cout << participantes[j].nombre << "\t";
    }
    cout << "\n";
    for (int i = 0; i < numParticipantes; i++) {
        cout << participantes[i].nombre << "\t";
        for (int j = 0; j < numParticipantes; j++) {
            cout << grafo[i][j] << "\t";
        }
        cout << "\n";
    }
    cout << "(1 = existe arista dirigida 'influye sobre', 0 = no existe)\n";
}

int gradoSalida(int grafo[][MAX_PARTICIPANTES], int vertice, int numParticipantes) {
    int grado = 0;
    for (int j = 0; j < numParticipantes; j++) {
        if (grafo[vertice][j] == 1) {
            grado++;
        }
    }
    return grado;
}

int gradoEntrada(int grafo[][MAX_PARTICIPANTES], int vertice, int numParticipantes) {
    int grado = 0;
    for (int i = 0; i < numParticipantes; i++) {
        if (grafo[i][vertice] == 1) {
            grado++;
        }
    }
    return grado;
}

void mostrarGrados(int grafo[][MAX_PARTICIPANTES], Participante participantes[], int numParticipantes) {
    cout << "\n--- Grado de los vertices ---\n";
    for (int v = 0; v < numParticipantes; v++) {
        int gs = gradoSalida(grafo, v, numParticipantes);
        int ge = gradoEntrada(grafo, v, numParticipantes);
        cout << participantes[v].nombre
             << " -> grado de salida: " << gs
             << " | grado de entrada: " << ge << "\n";
    }
}

bool esConexo(int grafo[][MAX_PARTICIPANTES], int numParticipantes) {
    bool visitado[MAX_PARTICIPANTES];
    int cola[MAX_PARTICIPANTES];
    int inicioCola = 0, finCola = 0;

    for (int i = 0; i < numParticipantes; i++) {
        visitado[i] = false;
    }

    cola[finCola++] = 0;
    visitado[0] = true;
    int visitados = 1;

    while (inicioCola < finCola) {
        int actual = cola[inicioCola++];
        for (int vecino = 0; vecino < numParticipantes; vecino++) {

            bool hayArista = (grafo[actual][vecino] == 1) || (grafo[vecino][actual] == 1);
            if (hayArista && !visitado[vecino]) {
                visitado[vecino] = true;
                cola[finCola++] = vecino;
                visitados++;
            }
        }
    }

    return visitados == numParticipantes;
}

bool tieneCicloCompleto(int grafo[][MAX_PARTICIPANTES], int numParticipantes) {
    bool visitado[MAX_PARTICIPANTES];
    for (int i = 0; i < numParticipantes; i++) {
        visitado[i] = false;
    }

    int actual = 0;
    int pasos = 0;
    while (pasos < numParticipantes) {
        if (visitado[actual]) {
            return false;
        }
        visitado[actual] = true;

        int siguiente = -1;
        for (int j = 0; j < numParticipantes; j++) {
            if (grafo[actual][j] == 1) {
                siguiente = j;
                break;
            }
        }
        if (siguiente == -1) {
            return false;
        }
        actual = siguiente;
        pasos++;
    }
    return actual == 0;
}

void analizarGrafo(int grafo[][MAX_PARTICIPANTES], Participante participantes[], int numParticipantes) {
    mostrarGrafoInfluencia(grafo, participantes, numParticipantes);
    mostrarGrados(grafo, participantes, numParticipantes);

    cout << "\n--- Conexidad y ciclos ---\n";
    if (esConexo(grafo, numParticipantes)) {
        cout << "El grafo de influencia ES CONEXO: la influencia entre\n";
        cout << "participantes se propaga a todos los vertices.\n";
    } else {
        cout << "El grafo de influencia NO es conexo: existen participantes\n";
        cout << "aislados que no reciben ni transmiten influencia.\n";
    }

    if (tieneCicloCompleto(grafo, numParticipantes)) {
        cout << "El circuito de influencia forma un CICLO que pasa por\n";
        cout << "todos los vertices exactamente una vez (ciclo hamiltoniano\n";
        cout << "sobre el grafo de influencia), es decir, la influencia\n";
        cout << "retorna al primer participante cerrando el circuito.\n";
    } else {
        cout << "El circuito de influencia NO forma un ciclo hamiltoniano\n";
        cout << "completo sobre este grafo.\n";
    }
}

NodoArbol* crearNodo(int participante, int opcion, int puntajeAcumulado) {
    NodoArbol* nuevo = new NodoArbol;
    nuevo->participante = participante;
    nuevo->opcion = opcion;
    nuevo->puntajeAcumulado = puntajeAcumulado;
    for (int i = 0; i < NUM_OPCIONES; i++) {
        nuevo->hijos[i] = nullptr;
    }
    return nuevo;
}

NodoArbol* construirArbolDecision(Participante participantes[], int numParticipantes,
                                   int participanteActual, int opcionQueLlego, int puntajeAcumulado) {
    NodoArbol* nodo = crearNodo(participanteActual, opcionQueLlego, puntajeAcumulado);

    if (participanteActual >= numParticipantes) {
        return nodo;
    }

    for (int o = 0; o < NUM_OPCIONES; o++) {
        int nuevoPuntaje = puntajeAcumulado + participantes[participanteActual].intereses[o];
        nodo->hijos[o] = construirArbolDecision(participantes, numParticipantes,
                                                 participanteActual + 1, o, nuevoPuntaje);
    }
    return nodo;
}

void recorrerPreorden(NodoArbol* nodo, Participante participantes[], int nivel) {
    if (nodo == nullptr) {
        return;
    }
    for (int i = 0; i < nivel; i++) {
        cout << "  ";
    }
    if (nodo->opcion == -1) {
        cout << "(Raiz)\n";
    } else {
        cout << participantes[nodo->participante - 1].nombre << " elige ";
        mostrarNombreOpcion(nodo->opcion);
        cout << " | puntaje acumulado: " << nodo->puntajeAcumulado << "\n";
    }
    for (int i = 0; i < NUM_OPCIONES; i++) {
        recorrerPreorden(nodo->hijos[i], participantes, nivel + 1);
    }
}

void recorrerPostorden(NodoArbol* nodo, Participante participantes[], int nivel) {
    if (nodo == nullptr) {
        return;
    }
    for (int i = 0; i < NUM_OPCIONES; i++) {
        recorrerPostorden(nodo->hijos[i], participantes, nivel + 1);
    }
    for (int i = 0; i < nivel; i++) {
        cout << "  ";
    }
    if (nodo->opcion == -1) {
        cout << "(Raiz)\n";
    } else {
        cout << participantes[nodo->participante - 1].nombre << " elige ";
        mostrarNombreOpcion(nodo->opcion);
        cout << " | puntaje acumulado: " << nodo->puntajeAcumulado << "\n";
    }
}

void buscarMejorHoja(NodoArbol* nodo, int nivelesTotales, int nivelActual,
                      int rutaActual[], int &mejorPuntaje, int mejorRuta[]) {
    if (nodo == nullptr) {
        return;
    }

    if (nivelActual > 0) {
        rutaActual[nivelActual - 1] = nodo->opcion;
    }

    bool esHoja = true;
    for (int i = 0; i < NUM_OPCIONES; i++) {
        if (nodo->hijos[i] != nullptr) {
            esHoja = false;
        }
    }

    if (esHoja) {
        if (nodo->puntajeAcumulado > mejorPuntaje) {
            mejorPuntaje = nodo->puntajeAcumulado;
            for (int i = 0; i < nivelesTotales; i++) {
                mejorRuta[i] = rutaActual[i];
            }
        }
        return;
    }

    for (int i = 0; i < NUM_OPCIONES; i++) {
        buscarMejorHoja(nodo->hijos[i], nivelesTotales, nivelActual + 1, rutaActual, mejorPuntaje, mejorRuta);
    }
}

void mostrarMejorEstrategia(NodoArbol* raiz, Participante participantes[], int numParticipantes) {
    int rutaActual[MAX_PARTICIPANTES];
    int mejorRuta[MAX_PARTICIPANTES];
    int mejorPuntaje = -1;

    buscarMejorHoja(raiz, numParticipantes, 0, rutaActual, mejorPuntaje, mejorRuta);

    cout << "\n--- Mejor combinacion conjunta (teoria de juegos) ---\n";
    cout << "Recorriendo el arbol de juego se encontro la combinacion de\n";
    cout << "decisiones que maximiza el puntaje conjunto de la etapa 1:\n";
    for (int i = 0; i < numParticipantes; i++) {
        cout << "  " << participantes[i].nombre << " -> ";
        mostrarNombreOpcion(mejorRuta[i]);
        cout << "\n";
    }
    cout << "Puntaje conjunto maximo alcanzable: " << mejorPuntaje << "\n";
}

void liberarArbol(NodoArbol* nodo) {
    if (nodo == nullptr) {
        return;
    }
    for (int i = 0; i < NUM_OPCIONES; i++) {
        liberarArbol(nodo->hijos[i]);
    }
    delete nodo;
}

int clasificarSimbolo(int cooperacionesEtapa, int numParticipantes) {
    if (cooperacionesEtapa == numParticipantes) {
        return SIMBOLO_ALTA_COOP;
    } else if (cooperacionesEtapa * 2 >= numParticipantes) {
        return SIMBOLO_MEDIA_COOP;
    } else {
        return SIMBOLO_BAJA_COOP;
    }
}

void nombreSimbolo(int simbolo) {
    if (simbolo == SIMBOLO_ALTA_COOP) {
        cout << "alta_cooperacion";
    } else if (simbolo == SIMBOLO_MEDIA_COOP) {
        cout << "media_cooperacion";
    } else {
        cout << "baja_cooperacion";
    }
}

void nombreEstado(int estado) {
    if (estado == ESTADO_ACUERDO) {
        cout << "ACUERDO";
    } else if (estado == ESTADO_TENSION) {
        cout << "TENSION";
    } else {
        cout << "CONFLICTO";
    }
}

int transicion(int estadoActual, int simbolo) {
    if (estadoActual == ESTADO_ACUERDO) {
        if (simbolo == SIMBOLO_ALTA_COOP)  return ESTADO_ACUERDO;
        if (simbolo == SIMBOLO_MEDIA_COOP) return ESTADO_TENSION;
        return ESTADO_CONFLICTO;
    }
    if (estadoActual == ESTADO_TENSION) {
        if (simbolo == SIMBOLO_ALTA_COOP)  return ESTADO_ACUERDO;
        if (simbolo == SIMBOLO_MEDIA_COOP) return ESTADO_TENSION;
        return ESTADO_CONFLICTO;
    }

    if (simbolo == SIMBOLO_ALTA_COOP)  return ESTADO_TENSION;
    if (simbolo == SIMBOLO_MEDIA_COOP) return ESTADO_CONFLICTO;
    return ESTADO_CONFLICTO;
}

void simularAutomata(int decisiones[][MAX_PARTICIPANTES], int numEtapas, int numParticipantes) {
    cout << "\n--- Automata finito determinista de la negociacion ---\n";
    cout << "Estados = {ACUERDO, TENSION, CONFLICTO} | Estado inicial = TENSION\n";
    cout << "Alfabeto = {alta_cooperacion, media_cooperacion, baja_cooperacion}\n";
    cout << "Estado de aceptacion = ACUERDO\n\n";

    int estadoActual = ESTADO_TENSION;
    cout << "Estado inicial: ";
    nombreEstado(estadoActual);
    cout << "\n";

    for (int e = 0; e < numEtapas; e++) {
        int cooperaciones = 0;
        for (int p = 0; p < numParticipantes; p++) {
            if (decisiones[e][p] == 0) {
                cooperaciones++;
            }
        }

        int simbolo = clasificarSimbolo(cooperaciones, numParticipantes);
        int estadoSiguiente = transicion(estadoActual, simbolo);

        cout << "Etapa " << (e + 1) << ": simbolo leido = ";
        nombreSimbolo(simbolo);
        cout << " | transicion: ";
        nombreEstado(estadoActual);
        cout << " -> ";
        nombreEstado(estadoSiguiente);
        cout << "\n";

        estadoActual = estadoSiguiente;
    }

    cout << "\nEstado final del automata: ";
    nombreEstado(estadoActual);
    cout << "\n";

    if (estadoActual == ESTADO_ACUERDO) {
        cout << "La cadena de etapas fue ACEPTADA por el automata: la\n";
        cout << "negociacion converge hacia un acuerdo estable.\n";
    } else {
        cout << "La cadena de etapas NO fue aceptada (no termino en el\n";
        cout << "estado ACUERDO): la negociacion no logro estabilizarse.\n";
    }
}

int main() {
    Participante participantes[MAX_PARTICIPANTES];
    int decisiones[MAX_ETAPAS][MAX_PARTICIPANTES];
    int grafoInfluencia[MAX_PARTICIPANTES][MAX_PARTICIPANTES];

    int numParticipantes = 0;
    int numEtapas = 0;
    int semilla = 0;

    cout << "===================================================\n";
    cout << " SISTEMA DE RESOLUCION DE CONFLICTOS - NEGOCIACION\n";
    cout << " (Grafos + Arboles de decision/juego + Automata finito)\n";
    cout << "===================================================\n\n";

    cout << "Ingrese numero de participantes (2 a " << MAX_PARTICIPANTES << "): ";
    cin >> numParticipantes;
    while (numParticipantes < 2 || numParticipantes > MAX_PARTICIPANTES) {
        cout << "Valor invalido. Ingrese entre 2 y " << MAX_PARTICIPANTES << ": ";
        cin >> numParticipantes;
    }

    cout << "Ingrese numero de etapas de negociacion (1 a " << MAX_ETAPAS << "): ";
    cin >> numEtapas;
    while (numEtapas < 1 || numEtapas > MAX_ETAPAS) {
        cout << "Valor invalido. Ingrese entre 1 y " << MAX_ETAPAS << ": ";
        cin >> numEtapas;
    }

    cout << "Ingrese una semilla numerica para el azar (cualquier entero): ";
    cin >> semilla;

    inicializarParticipantes(participantes, numParticipantes);

    simularNegociacion(participantes, numParticipantes, numEtapas, decisiones, semilla);

    mostrarDecisiones(participantes, numParticipantes, numEtapas, decisiones);

    evaluarResultados(participantes, numParticipantes, decisiones, numEtapas);

    mostrarResumenFinal(participantes, numParticipantes);

    analizarEstabilidad(participantes, numParticipantes, decisiones, numEtapas);

    construirGrafoInfluencia(grafoInfluencia, numParticipantes);
    analizarGrafo(grafoInfluencia, participantes, numParticipantes);

    NodoArbol* raizArbol = construirArbolDecision(participantes, numParticipantes, 0, -1, 0);
    cout << "\n--- Arbol de decision / juego (Etapa 1) ---\n";
    cout << "Recorrido PREORDEN:\n";
    recorrerPreorden(raizArbol, participantes, 0);
    cout << "\nRecorrido POSTORDEN:\n";
    recorrerPostorden(raizArbol, participantes, 0);
    mostrarMejorEstrategia(raizArbol, participantes, numParticipantes);
    liberarArbol(raizArbol);

    simularAutomata(decisiones, numEtapas, numParticipantes);

    cout << "\nFin de la simulacion. Gracias por usar el sistema.\n";
    return 0;
}
