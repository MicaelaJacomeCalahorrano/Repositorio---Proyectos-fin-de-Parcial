

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <cmath>

using namespace std;

struct LineaProduccion {
 
    string nombre;
    double produccionActual;  
    double metaDiaria;      
    int    fallosMecanicos;   
    bool   supervisorValido;   

  
    bool P;  
    bool Q;   
    bool R;  

    string estadoInferido;     

  
    LineaProduccion* siguiente;

    LineaProduccion() : produccionActual(0), metaDiaria(0),
                        fallosMecanicos(0), supervisorValido(false),
                        P(false), Q(false), R(false),
                        estadoInferido(""), siguiente(nullptr) {}
};



class ListaLineas {
private:
    LineaProduccion* cabeza;
    int              cantidad;

public:
    ListaLineas() : cabeza(nullptr), cantidad(0) {}

    ~ListaLineas() {
        LineaProduccion* actual = cabeza;
        while (actual) {
            LineaProduccion* temp = actual;
            actual = actual->siguiente;
            delete temp;
        }
    }
    void insertar(LineaProduccion* nueva) {
        nueva->siguiente = nullptr;
        if (!cabeza) {
            cabeza = nueva;
        } else {
            LineaProduccion* aux = cabeza;
            while (aux->siguiente) aux = aux->siguiente;
            aux->siguiente = nueva;
        }
        cantidad++;
    }

    LineaProduccion* getCabeza() const { return cabeza; }
    int getCantidad()           const { return cantidad; }

    LineaProduccion* buscar(const string& nombre) const {
        LineaProduccion* aux = cabeza;
        while (aux) {
            if (aux->nombre == nombre) return aux;
            aux = aux->siguiente;
        }
        return nullptr;
    }
};

struct NodoPila {
    string      mensaje;
    NodoPila*   siguiente;
    NodoPila(const string& m) : mensaje(m), siguiente(nullptr) {}
};

class Pila {
private:
    NodoPila* tope;
    int       tamanio;
public:
    Pila() : tope(nullptr), tamanio(0) {}

    ~Pila() {
        while (tope) {
            NodoPila* temp = tope;
            tope = tope->siguiente;
            delete temp;
        }
    }

    void push(const string& msg) {
        NodoPila* nuevo = new NodoPila(msg);
        nuevo->siguiente = tope;
        tope = nuevo;
        tamanio++;
    }

    string pop() {
        if (!tope) return "";
        string msg = tope->mensaje;
        NodoPila* temp = tope;
        tope = tope->siguiente;
        delete temp;
        tamanio--;
        return msg;
    }

    bool   estaVacia() const { return tope == nullptr; }
    int    getTamanio() const { return tamanio; }
    string verTope()   const { return tope ? tope->mensaje : ""; }
};

struct NodoCola {
    string    nombreLinea;
    NodoCola* siguiente;
    NodoCola(const string& n) : nombreLinea(n), siguiente(nullptr) {}
};

class Cola {
private:
    NodoCola* frente;
    NodoCola* final_;
    int       tamanio;
public:
    Cola() : frente(nullptr), final_(nullptr), tamanio(0) {}

    ~Cola() {
        while (frente) {
            NodoCola* temp = frente;
            frente = frente->siguiente;
            delete temp;
        }
    }

    void encolar(const string& nombre) {
        NodoCola* nuevo = new NodoCola(nombre);
        if (!final_) {
            frente = final_ = nuevo;
        } else {
            final_->siguiente = nuevo;
            final_ = nuevo;
        }
        tamanio++;
    }

    string desencolar() {
        if (!frente) return "";
        string n = frente->nombreLinea;
        NodoCola* temp = frente;
        frente = frente->siguiente;
        if (!frente) final_ = nullptr;
        delete temp;
        tamanio--;
        return n;
    }

    bool estaVacia() const { return frente == nullptr; }
    int  getTamanio() const { return tamanio; }
};

struct Conjunto {
    LineaProduccion** elementos;
    int               tamanio;
    int               capacidad;
    string            nombre;

    Conjunto(const string& n, int cap = 50)
        : tamanio(0), capacidad(cap), nombre(n) {
        elementos = new LineaProduccion*[cap];
    }

    ~Conjunto() { delete[] elementos; }

    void agregar(LineaProduccion* linea) {
        if (tamanio < capacidad)
            elementos[tamanio++] = linea;
    }

    void limpiar() { tamanio = 0; }

    bool contiene(const string& nombre) const {
        for (int i = 0; i < tamanio; i++)
            if (elementos[i]->nombre == nombre) return true;
        return false;
    }

    void imprimir() const {
        if (tamanio == 0) { cout << "  (vacío)\n"; return; }
        for (int i = 0; i < tamanio; i++)
            cout << "  [" << (i+1) << "] " << elementos[i]->nombre
                 << "  (" << elementos[i]->estadoInferido << ")\n";
    }
};

 */
void evaluarProposiciones(LineaProduccion* linea) {
    linea->P = (linea->produccionActual >= linea->metaDiaria);
    linea->Q = (linea->fallosMecanicos == 0);
    linea->R = linea->supervisorValido;

    if (!linea->Q) {
        linea->estadoInferido = "DETENIDA";
    }

    else if (linea->P && linea->Q && linea->R) {
        linea->estadoInferido = "OPERATIVA";
    }

    else {
        linea->estadoInferido = "BAJO_RENDIMIENTO";
    }
}


 */
void clasificarEnConjuntos(ListaLineas& lista,
                           Conjunto& A, Conjunto& B, Conjunto& C) {
    A.limpiar(); B.limpiar(); C.limpiar();
    LineaProduccion* aux = lista.getCabeza();
    while (aux) {
        if      (aux->estadoInferido == "OPERATIVA")        A.agregar(aux);
        else if (aux->estadoInferido == "BAJO_RENDIMIENTO") B.agregar(aux);
        else if (aux->estadoInferido == "DETENIDA")         C.agregar(aux);
        aux = aux->siguiente;
    }
}

 */
void interseccion(const Conjunto& A, const Conjunto& B, Conjunto& resultado) {
    resultado.limpiar();
    for (int i = 0; i < A.tamanio; i++)
        if (B.contiene(A.elementos[i]->nombre))
            resultado.agregar(A.elementos[i]);
}

void diferencia(const Conjunto& A, const Conjunto& C, Conjunto& resultado) {
    resultado.limpiar();
    for (int i = 0; i < A.tamanio; i++)
        if (!C.contiene(A.elementos[i]->nombre))
            resultado.agregar(A.elementos[i]);
}

 */
void unionConjuntos(const Conjunto& B, const Conjunto& C, Conjunto& resultado) {
    resultado.limpiar();
    for (int i = 0; i < B.tamanio; i++)
        resultado.agregar(B.elementos[i]);
    for (int i = 0; i < C.tamanio; i++)
        if (!resultado.contiene(C.elementos[i]->nombre))
            resultado.agregar(C.elementos[i]);
}


void separador(char c = '=', int n = 60) {
    cout << string(n, c) << "\n";
}

void encabezado(const string& titulo) {
    separador();
    cout << "  " << titulo << "\n";
    separador();
}

string boolStr(bool v) { return v ? "VERDADERO" : "FALSO"; }

string estadoTag(const string& estado) {
    if (estado == "OPERATIVA")        return "[✔ OPERATIVA]";
    if (estado == "BAJO_RENDIMIENTO") return "[~ BAJO RENDIMIENTO]";
    if (estado == "DETENIDA")         return "[✘ DETENIDA]";
    return "[?]";
}

// Muestra el detalle proposicional de una línea
void mostrarDetalleLinea(const LineaProduccion* linea) {
    separador('-', 60);
    cout << "  Línea : " << linea->nombre << "\n";
    separador('-', 60);
    cout << fixed << setprecision(1);
    cout << "  Producción actual : " << linea->produccionActual << " uds\n";
    cout << "  Meta diaria       : " << linea->metaDiaria       << " uds\n";
    cout << "  Fallos mecánicos  : " << linea->fallosMecanicos  << "\n";
    cout << "  Validación superv.: " << (linea->supervisorValido ? "SÍ" : "NO") << "\n";
    cout << "\n";
    cout << "  --- Proposiciones lógicas ---\n";
    cout << "  P (cumple meta)   : " << boolStr(linea->P) << "\n";
    cout << "  Q (sin fallos)    : " << boolStr(linea->Q) << "\n";
    cout << "  R (supervisor OK) : " << boolStr(linea->R) << "\n";
    cout << "\n";
    cout << "  --- Inferencia ---\n";
    if (!linea->Q)
        cout << "  Regla aplicada    : ¬Q → DETENIDA\n";
    else if (linea->P && linea->Q && linea->R)
        cout << "  Regla aplicada    : P ∧ Q ∧ R → OPERATIVA\n";
    else
        cout << "  Regla aplicada    : ¬P ∧ Q → BAJO RENDIMIENTO\n";
    cout << "  Estado inferido   : " << estadoTag(linea->estadoInferido) << "\n";
}


void mostrarResumen(const ListaLineas& lista,
                    const Conjunto& A, const Conjunto& B, const Conjunto& C) {
    encabezado("RESUMEN FINAL DEL SISTEMA - TechFactory");

    int total      = lista.getCantidad();
    int operativas = A.tamanio;
    int bajoRend   = B.tamanio;
    int detenidas  = C.tamanio;


    int totalFallos = 0;
    LineaProduccion* aux = lista.getCabeza();
    while (aux) {
        totalFallos += aux->fallosMecanicos;
        aux = aux->siguiente;
    }


    double eficiencia = (total > 0)
                        ? (100.0 * operativas / total)
                        : 0.0;

    cout << fixed << setprecision(2);
    cout << "  Total de líneas evaluadas : " << total      << "\n";
    cout << "  Líneas operativas (A)     : " << operativas << "\n";
    cout << "  Líneas bajo rendim. (B)   : " << bajoRend   << "\n";
    cout << "  Líneas detenidas (C)      : " << detenidas  << "\n";
    separador('-', 60);
    cout << "  Total de fallos registr.  : " << totalFallos << "\n";
    cout << "  Porcentaje de eficiencia  : " << eficiencia << " %\n";
    separador();

    if (eficiencia == 100.0)
        cout << "  >> ESTADO GLOBAL: PLANTA EN ÓPTIMAS CONDICIONES <<\n";
    else if (eficiencia >= 60.0)
        cout << "  >> ESTADO GLOBAL: PLANTA CON RENDIMIENTO ACEPTABLE <<\n";
    else if (eficiencia >= 30.0)
        cout << "  >> ESTADO GLOBAL: PLANTA CON RENDIMIENTO CRÍTICO <<\n";
    else
        cout << "  >> ESTADO GLOBAL: PLANTA EN ESTADO DE EMERGENCIA <<\n";
    separador();
}



void menuConjuntos(const Conjunto& A, const Conjunto& B, const Conjunto& C) {
    Conjunto resInter("A∩B");
    Conjunto resDif  ("A−C");
    Conjunto resUnion("B∪C");

    int opcion;
    do {
        encabezado("OPERACIONES DE CONJUNTOS");
        cout << "  A = Líneas activas (operativas)\n";
        cout << "  B = Líneas con bajo rendimiento\n";
        cout << "  C = Líneas detenidas\n\n";
        cout << "  [1] A ∩ B  —  Líneas operativas en revisión\n";
        cout << "  [2] A − C  —  Líneas activas sin fallos\n";
        cout << "  [3] B ∪ C  —  Líneas con incidencias\n";
        cout << "  [4] Ver conjuntos base (A, B, C)\n";
        cout << "  [0] Volver al menú principal\n";
        separador('-', 60);
        cout << "  Seleccione: ";
        cin  >> opcion;
        cin.ignore();
        cout << "\n";

        switch (opcion) {
        case 1:
            interseccion(A, B, resInter);
            separador('-', 60);
            cout << "  A ∩ B  (líneas operativas en revisión):\n";
            resInter.imprimir();
            separador('-', 60);
            break;
        case 2:
            diferencia(A, C, resDif);
            separador('-', 60);
            cout << "  A − C  (líneas activas sin fallos):\n";
            resDif.imprimir();
            separador('-', 60);
            break;
        case 3:
            unionConjuntos(B, C, resUnion);
            separador('-', 60);
            cout << "  B ∪ C  (líneas con incidencias):\n";
            resUnion.imprimir();
            separador('-', 60);
            break;
        case 4:
            separador('-', 60);
            cout << "  Conjunto A — Líneas operativas:\n";
            A.imprimir();
            cout << "\n  Conjunto B — Líneas con bajo rendimiento:\n";
            B.imprimir();
            cout << "\n  Conjunto C — Líneas detenidas:\n";
            C.imprimir();
            separador('-', 60);
            break;
        case 0:
            cout << "  Volviendo al menú principal...\n\n";
            break;
        default:
            cout << "  [!] Opción no válida.\n\n";
        }
    } while (opcion != 0);
}


LineaProduccion* ingresarLinea() {
    LineaProduccion* linea = new LineaProduccion();
    string respuesta;

    encabezado("REGISTRO DE LÍNEA DE PRODUCCIÓN");

    cout << "  Nombre de la línea       : ";
    getline(cin, linea->nombre);

    cout << "  Meta de producción diaria: ";
    cin  >> linea->metaDiaria;

    cout << "  Producción actual (uds)  : ";
    cin  >> linea->produccionActual;

    cout << "  Fallos mecánicos hoy     : ";
    cin  >> linea->fallosMecanicos;

    cout << "  Supervisor validó (s/n)  : ";
    cin  >> respuesta;
    cin.ignore();
    linea->supervisorValido = (respuesta == "s" || respuesta == "S");


    evaluarProposiciones(linea);

    cout << "\n  [✔] Línea registrada y analizada: "
         << estadoTag(linea->estadoInferido) << "\n\n";

    return linea;
}


void verHistorial(Pila& historial) {
    encabezado("HISTORIAL DE OPERACIONES (PILA)");
    if (historial.estaVacia()) {
        cout << "  El historial está vacío.\n\n";
        return;
    }

    Pila copia;
    Pila temp;


    while (!historial.estaVacia())
        temp.push(historial.pop());


    int n = 1;
    while (!temp.estaVacia()) {
        string msg = temp.pop();
        cout << "  [" << n++ << "] " << msg << "\n";
        historial.push(msg);
    }
    cout << "\n";
}

void verColaRevision(Cola& colaRevision) {
    encabezado("COLA DE LÍNEAS PENDIENTES DE REVISIÓN");
    if (colaRevision.estaVacia()) {
        cout << "  No hay líneas en cola de revisión.\n\n";
        return;
    }

    Cola temp;
    int n = 1;
    while (!colaRevision.estaVacia()) {
        string nombre = colaRevision.desencolar();
        cout << "  [" << n++ << "] " << nombre << "\n";
        temp.encolar(nombre);
    }

    while (!temp.estaVacia())
        colaRevision.encolar(temp.desencolar());
    cout << "\n";
}


void verTodasLasLineas(const ListaLineas& lista) {
    if (lista.getCantidad() == 0) {
        cout << "  No hay líneas registradas.\n\n";
        return;
    }
    encabezado("DETALLE DE TODAS LAS LÍNEAS");
    LineaProduccion* aux = lista.getCabeza();
    while (aux) {
        mostrarDetalleLinea(aux);
        aux = aux->siguiente;
    }
    cout << "\n";
}



int main() {

    ListaLineas lista;      
    Conjunto A("A");     
    Conjunto B("B");           
    Conjunto C("C");            
    Pila     historial;         
    Cola     colaRevision;      

    int opcion;


    separador('*');
    cout << "  TECHFACTORY — Sistema de Análisis Lógico de Producción\n";
    cout << "  Lógica Proposicional + Conjuntos + Estructuras Dinámicas\n";
    separador('*');
    cout << "\n";

    do {
        encabezado("MENÚ PRINCIPAL");
        cout << "  [1] Registrar nueva línea de producción\n";
        cout << "  [2] Ver detalle de todas las líneas\n";
        cout << "  [3] Analizar y clasificar en conjuntos (A, B, C)\n";
        cout << "  [4] Operaciones de conjuntos\n";
        cout << "  [5] Cola de revisión (líneas con incidencias)\n";
        cout << "  [6] Historial de operaciones (pila)\n";
        cout << "  [7] Resumen final del sistema\n";
        cout << "  [0] Salir\n";
        separador('-', 60);
        cout << "  Líneas registradas: " << lista.getCantidad() << "\n";
        cout << "  Seleccione: ";
        cin  >> opcion;
        cin.ignore();
        cout << "\n";

        switch (opcion) {


        case 1: {
            LineaProduccion* nueva = ingresarLinea();
            lista.insertar(nueva);


            if (nueva->estadoInferido != "OPERATIVA")
                colaRevision.encolar(nueva->nombre);

            string msg = "Línea registrada: " + nueva->nombre +
                         " | Estado: " + nueva->estadoInferido;
            historial.push(msg);
            break;
        }


        case 2:
            verTodasLasLineas(lista);
            historial.push("Consulta: detalle de todas las líneas");
            break;

        case 3:
            if (lista.getCantidad() == 0) {
                cout << "  [!] No hay líneas registradas.\n\n";
                break;
            }
            clasificarEnConjuntos(lista, A, B, C);
            encabezado("CLASIFICACIÓN EN CONJUNTOS");
            cout << "  A — Líneas OPERATIVAS (" << A.tamanio << "):\n";
            A.imprimir();
            cout << "\n  B — Líneas BAJO RENDIMIENTO (" << B.tamanio << "):\n";
            B.imprimir();
            cout << "\n  C — Líneas DETENIDAS (" << C.tamanio << "):\n";
            C.imprimir();
            cout << "\n";
            separador();
            historial.push("Clasificación en conjuntos ejecutada");
            break;


        case 4:
            if (lista.getCantidad() == 0) {
                cout << "  [!] No hay líneas registradas.\n\n";
                break;
            }
  
            clasificarEnConjuntos(lista, A, B, C);
            menuConjuntos(A, B, C);
            historial.push("Menú de operaciones de conjuntos ejecutado");
            break;

        case 5:
            verColaRevision(colaRevision);
            historial.push("Consulta: cola de revisión");
            break;

        case 6:
            verHistorial(historial);
            break;


        case 7:
            if (lista.getCantidad() == 0) {
                cout << "  [!] No hay líneas registradas.\n\n";
                break;
            }
            clasificarEnConjuntos(lista, A, B, C);
            mostrarResumen(lista, A, B, C);
            historial.push("Resumen final generado");
            break;

        case 0:
            separador('*');
            cout << "  Cerrando TechFactory. ¡Hasta pronto!\n";
            separador('*');
            break;

        default:
            cout << "  [!] Opción no válida. Intente nuevamente.\n\n";
        }

    } while (opcion != 0);

    return 0;
}

