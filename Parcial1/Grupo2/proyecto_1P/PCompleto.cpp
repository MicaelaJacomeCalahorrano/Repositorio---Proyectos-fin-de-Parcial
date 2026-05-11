#include <iostream>
#include <string>
#include <limits> // Requerido para limpiar el buffer y evitar bucles infinitos

using namespace std;

// ==========================================
// 1. MODELO DE DATOS Y NODOS BASE
// ==========================================
// Estructura del dominio: Representacion de la entidad Proyecto
struct Proyecto {
    string nombre;
    string estado; 
    int erroresCriticos;
    int diasRetraso;
    bool revisionUrgente; 
};

// Nodo base para la implementacion de Estructuras Dinamicas
struct Nodo {
    Proyecto dato;
    Nodo* siguiente; // Puntero de enlace
};

// Variables globales para el control de metricas (Auditoria)
int totalAnalizados = 0;
int totalAlertas = 0;
int totalEstables = 0;

// ==========================================
// 2. TDA: ESTRUCTURAS DE DATOS
// ==========================================

// --- TDA COLA (Control de Flujo FIFO) ---
struct Cola { Nodo* frente = nullptr; Nodo* final = nullptr; };

void encolar(Cola& c, Proyecto p) {
    Nodo* nuevo = new Nodo{p, nullptr}; // Asignacion dinamica de memoria
    if (c.frente == nullptr) c.frente = nuevo;
    else c.final->siguiente = nuevo;
    c.final = nuevo;
}

Proyecto desencolar(Cola& c) {
    if (c.frente == nullptr) return {"", "", 0, 0, false};
    Nodo* aux = c.frente;
    Proyecto p = aux->dato;
    c.frente = c.frente->siguiente;
    if (c.frente == nullptr) c.final = nullptr;
    delete aux; // Liberacion de memoria
    return p;
}

// --- TDA LISTA (Manejo de Conjuntos) ---
struct Lista { Nodo* cabeza = nullptr; };

void insertarEnLista(Lista& l, Proyecto p) {
    Nodo* nuevo = new Nodo{p, nullptr};
    if (l.cabeza == nullptr) l.cabeza = nuevo;
    else {
        Nodo* aux = l.cabeza;
        while (aux->siguiente != nullptr) aux = aux->siguiente; // Traversal
        aux->siguiente = nuevo;
    }
}

void imprimirLista(Lista l, string nombreConjunto) {
    cout << "\n--- CONJUNTO " << nombreConjunto << " ---\n";
    if (l.cabeza == nullptr) { cout << "  (Vacio)\n"; return; }
    Nodo* aux = l.cabeza;
    while (aux != nullptr) {
        cout << "  -> " << aux->dato.nombre << " | Estado: " << aux->dato.estado << "\n";
        aux = aux->siguiente;
    }
}

// Validacion de pertenencia (x E A)
bool existeEnLista(Lista l, string nombre) {
    Nodo* aux = l.cabeza;
    while (aux != nullptr) {
        if (aux->dato.nombre == nombre) return true;
        aux = aux->siguiente;
    }
    return false;
}

// --- TDA PILA (Manejo LIFO para Evaluacion de Expresiones) ---
struct NodoPila {
    bool valorLogico;
    NodoPila* siguiente;
};

struct Pila { NodoPila* cima = nullptr; };

void apilar(Pila& p, bool v) {
    NodoPila* nuevo = new NodoPila{v, p.cima};
    p.cima = nuevo;
}

bool desapilar(Pila& p) {
    if (p.cima == nullptr) return false;
    NodoPila* aux = p.cima;
    bool v = aux->valorLogico;
    p.cima = p.cima->siguiente;
    delete aux;
    return v;
}

// ==========================================
// 3. LOGICA PROPOSICIONAL Y DE PREDICADOS
// ==========================================
// Definicion de Proposiciones (Funciones booleanas puras)
bool evaluarP(Proyecto p) { return p.diasRetraso > 10; } 
bool evaluarQ(Proyecto p) { return p.erroresCriticos > 3; } 
bool evaluarR(Proyecto p) { return p.revisionUrgente; } 

// ==========================================
// 4. OPERACIONES MATEMATICAS DE CONJUNTOS
// ==========================================
void mostrarInterseccion(Lista A, Lista B) {
    cout << "\n--- INTERSECCION (A n B): Activos con Retraso ---\n";
    Nodo* auxA = A.cabeza;
    bool flag = false;
    while (auxA != nullptr) {
        if (existeEnLista(B, auxA->dato.nombre)) {
            cout << "  -> " << auxA->dato.nombre << "\n"; flag = true;
        }
        auxA = auxA->siguiente;
    }
    if (!flag) cout << "  (Vacio)\n";
}

void mostrarDiferencia(Lista A, Lista C) {
    cout << "\n--- DIFERENCIA (A - C): En Desarrollo puro ---\n";
    Nodo* auxA = A.cabeza;
    bool flag = false;
    while (auxA != nullptr) {
        if (!existeEnLista(C, auxA->dato.nombre)) {
            cout << "  -> " << auxA->dato.nombre << "\n"; flag = true;
        }
        auxA = auxA->siguiente;
    }
    if (!flag) cout << "  (Vacio)\n";
}

void mostrarUnion(Lista B, Lista C) {
    cout << "\n--- UNION (B U C): Operaciones criticas ---\n";
    Nodo* aux = B.cabeza;
    bool flag = false;
    while (aux != nullptr) {
        cout << "  -> " << aux->dato.nombre << " (De B)\n";
        flag = true; aux = aux->siguiente;
    }
    aux = C.cabeza;
    while (aux != nullptr) {
        if (!existeEnLista(B, aux->dato.nombre)) { // Filtro de duplicidad
            cout << "  -> " << aux->dato.nombre << " (De C)\n"; flag = true;
        }
        aux = aux->siguiente;
    }
    if (!flag) cout << "  (Vacio)\n";
}

// ==========================================
// 5. EVALUACION DE CUANTIFICADORES (LPPO)
// ==========================================
void auditoriaUniversal(Lista C) {
    cout << "\n--- AUDITORIA LPPO: Cuantificador Universal ---\n";
    if (C.cabeza == nullptr) { cout << "  [-] Dominio C vacio.\n"; return; }
    Nodo* aux = C.cabeza;
    bool validacion = true;
    while (aux != nullptr) {
        if (evaluarQ(aux->dato)) { 
            cout << "  [!] ERROR: " << aux->dato.nombre << " rompe la regla universal (~Q(x)).\n";
            validacion = false;
        }
        aux = aux->siguiente;
    }
    if (validacion) cout << "  [OK] Regla universal cumplida.\n";
}

void auditoriaExistencial(Lista A) {
    cout << "\n--- AUDITORIA LPPO: Cuantificador Existencial ---\n";
    Nodo* aux = A.cabeza;
    while (aux != nullptr) {
        if (evaluarP(aux->dato)) { 
            cout << "  [OK] Regla existencial cumplida. Prueba: " << aux->dato.nombre << "\n";
            return; // Cortocircuito iterativo
        }
        aux = aux->siguiente;
    }
    cout << "  [-] Regla existencial falsa en el dominio A.\n";
}

// ==========================================
// 6. MODULOS PRINCIPALES DEL SISTEMA
// ==========================================

void mostrarMenu() {
    cout << "\n========================================\n";
    cout << "  SISTEMA DE DIAGNOSTICO - SOFTLOGIC SA\n";
    cout << "========================================\n";
    cout << "1. Ingresar Proyecto (Encolar)\n";
    cout << "2. Ejecutar Motor de Inferencia\n";
    cout << "3. Imprimir Conjunto A (Activos)\n";
    cout << "4. Imprimir Conjunto B (Retraso > 10)\n";
    cout << "5. Imprimir Conjunto C (Completados)\n";
    cout << "6. Resolver: Interseccion (A n B)\n";
    cout << "7. Resolver: Diferencia (A - C)\n";
    cout << "8. Resolver: Union (B U C)\n";
    cout << "9. Ejecutar Auditoria LPPO\n";
    cout << "10. Generar Resumen Estadistico\n";
    cout << "11. Finalizar Sesion\n";
    cout << "========================================\n";
    cout << "Seleccione un codigo de operacion: ";
}

void registrarProyecto(Cola& c) {
    Proyecto p;
    cout << "ID/Nombre del proyecto: "; cin >> p.nombre;
    cout << "Estado (Activo/Completado/Revision): "; cin >> p.estado;
    cout << "Dias de retraso: "; cin >> p.diasRetraso;
    cout << "Nro. errores criticos: "; cin >> p.erroresCriticos;
    int rev; cout << "Requiere revision urgente? (1=Si, 0=No): "; cin >> rev;
    p.revisionUrgente = (rev == 1);
    encolar(c, p);
    cout << "[+] Datos registrados en buffer exitosamente.\n";
}

void ejecutarMotorInferencia(Cola& colaRecepcion, Lista& A, Lista& B, Lista& C) {
    if (colaRecepcion.frente == nullptr) {
        cout << "[-] Buffer de recepcion vacio.\n"; return;
    }
    
    cout << "\n--- INICIALIZANDO MOTOR DE INFERENCIA ---\n";
    while (colaRecepcion.frente != nullptr) {
        Proyecto actual = desencolar(colaRecepcion);
        totalAnalizados++; 

        cout << "\n----------------------------------------\n";
        cout << "Evaluando: " << actual.nombre << "\n";
        
        // Mapeo Proposicional
        bool valP = evaluarP(actual);
        bool valQ = evaluarQ(actual);
        bool valR = evaluarR(actual);

        cout << "  Estado Logico -> P: " << valP << " | Q: " << valQ << " | R: " << valR << "\n";

        // Inicializacion de Pila para parsing logico
        Pila pilaInferencia; 

        if (valP) cout << "  [i] Modus Ponens detectado: P=1 -> Implica R=1.\n";

        // Resolucion: Q ^ P
        apilar(pilaInferencia, valP);
        apilar(pilaInferencia, valQ);
        bool op1 = desapilar(pilaInferencia); 
        bool op2 = desapilar(pilaInferencia); 
        bool riesgoAlto = (op1 && op2); 
        
        // Resolucion: ~P ^ ~Q
        apilar(pilaInferencia, !valP); 
        apilar(pilaInferencia, !valQ); 
        bool op3 = desapilar(pilaInferencia); 
        bool op4 = desapilar(pilaInferencia); 
        bool estable = (op3 && op4);

        // Clasificacion de salidas
        if (riesgoAlto) {
            cout << "  [!] EXCEPCION: RIESGO ALTO.\n"; totalAlertas++;
        } else if (estable) {
            cout << "  [*] STATUS: ESTABLE.\n"; totalEstables++;
        } else {
            cout << "  [-] STATUS: Regular.\n";
        }

        // Insercion en TDA Lista segun reglas de negocio
        if (actual.estado == "Activo") insertarEnLista(A, actual);
        if (valP) insertarEnLista(B, actual); 
        if (actual.estado == "Completado") insertarEnLista(C, actual);
    }
}

// Funcion auxiliar para cargar el buffer de pruebas (Hardcoding de datos)
void cargarDatosPrueba(Cola& c) {
    encolar(c, {"Alpha", "Activo", 1, 5, false});       // Estable
    encolar(c, {"Beta", "Activo", 5, 15, true});        // Riesgo Alto y Modus Ponens
    encolar(c, {"Gamma", "Completado", 0, 0, false});   // Limpio (Conjunto C)
    encolar(c, {"Delta", "Completado", 4, 2, false});   // Trampa: Rompe regla Universal
    encolar(c, {"Epsilon", "Revision", 1, 12, true});   // Retraso para Uniones (Conjunto B)
    cout << "[i] SISTEMA INICIALIZADO: 5 Registros cargados en el buffer de recepcion.\n";
}

// ==========================================
// 7. PUNTO DE ENTRADA (Main Modularizado)
// ==========================================
int main() {
    Cola colaRecepcion;
    Lista A; Lista B; Lista C;
    int opcion;

    // Carga de datos iniciales para agilizar pruebas
    cargarDatosPrueba(colaRecepcion);

    do {
        mostrarMenu();
        cin >> opcion;

        // --- PREVENCION DE BUCLE INFINITO (Manejo de buffer cin) ---
        if (cin.fail()) {
            cin.clear(); // Limpia bandera de error
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Descarta caracteres invalidos
            opcion = -1; // Fuerza la caida en el caso 'default'
        }

        // Estructura de control basada en switch para mejor rendimiento
        switch (opcion) {
            case 1: registrarProyecto(colaRecepcion); break;
            case 2: ejecutarMotorInferencia(colaRecepcion, A, B, C); break;
            case 3: imprimirLista(A, "A (Activos)"); break;
            case 4: imprimirLista(B, "B (Retraso)"); break;
            case 5: imprimirLista(C, "C (Completados)"); break;
            case 6: mostrarInterseccion(A, B); break;
            case 7: mostrarDiferencia(A, C); break;
            case 8: mostrarUnion(B, C); break;
            case 9: 
                auditoriaUniversal(C); 
                auditoriaExistencial(A); 
                break;
            case 10:
                cout << "\n=== REPORTE METRICO ===\n";
                cout << " Procesados: " << totalAnalizados << "\n";
                cout << " Alertas:    " << totalAlertas << "\n";
                cout << " Estables:   " << totalEstables << "\n";
                break;
            case 11: cout << "Cerrando sistema...\n"; break;
            default: cout << "Comando no reconocido. Por favor, ingrese un numero valido.\n";
        }
    } while (opcion != 11);

    return 0; // Termino de ejecucion exitosa
}