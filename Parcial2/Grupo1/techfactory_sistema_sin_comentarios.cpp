#include <iostream>
using namespace std;

// CONSTANTES para tamaños y estados de líneas de producción
const int TAM_NOMBRE = 100;
const int TAM_MENSAJE = 150;

const int ESTADO_OPERATIVA = 0;
const int ESTADO_BAJO_RENDIMIENTO = 1;
const int ESTADO_DETENIDA = 2;
const int ESTADO_SIN_DEFINIR = -1;

// Copia segura de cadena con límite de capacidad y terminador nulo
void copiarTexto(char *destino, const char *origen, int capacidad)
{ // Recorre origen carácter por carácter hasta encontrar '\0' o llegar al límite
    int i = 0;
    // Bucle que copia cada carácter mientras no sea fin de cadena y haya espacio disponible
    while (origen[i] != '\0' && i < capacidad - 1)
    {
        destino[i] = origen[i];
        i++;
    }
    destino[i] = '\0'; // Agrega el carácter nulo al final para cerrar la cadena
}

// Estructura que modela una línea de producción con todos sus atributos y estado
struct LineaProduccion
{
    // Datos básicos de identificación y rendimiento de la línea
    char nombre[TAM_NOMBRE];
    double produccionActual;
    double metaDiaria;
    int fallosMecanicos;
    bool supervisorValido;

    // Flags de estado general (P, Q, R) para condiciones operativas
    bool P;
    bool Q;
    bool R;

    // Estado calculado a partir de los flags y métricas
    int estadoInferido;

    // Puntero para estructurar lista enlazada de líneas
    LineaProduccion *siguiente;

    // Constructor que inicializa todos los atributos a valores predeterminados
    LineaProduccion() : produccionActual(0), metaDiaria(0),
                        fallosMecanicos(0), supervisorValido(false),
                        P(false), Q(false), R(false),
                        estadoInferido(ESTADO_SIN_DEFINIR), siguiente(nullptr)
    { // Inicializa el nombre como cadena vacía
        nombre[0] = '\0';
    }
};

class ListaLineas
{
private:
    LineaProduccion *cabeza;
    int cantidad;

public:
    ListaLineas() : cabeza(nullptr), cantidad(0) {}

    ~ListaLineas()
    {
        LineaProduccion *actual = cabeza;
        while (actual)
        {
            LineaProduccion *temp = actual;
            actual = actual->siguiente;
            delete temp;
        }
    }

    void insertar(LineaProduccion *nueva)
    {
        nueva->siguiente = nullptr;
        if (!cabeza)
        {
            cabeza = nueva;
        }
        else
        {
            LineaProduccion *aux = cabeza;
            while (aux->siguiente)
                aux = aux->siguiente;
            aux->siguiente = nueva;
        }
        cantidad++;
    }

    LineaProduccion *getCabeza() const { return cabeza; }
    int getCantidad() const { return cantidad; }

    LineaProduccion *buscar(const char *nombreBuscado) const
    {
        LineaProduccion *aux = cabeza;
        while (aux)
        {
            bool sonIguales = true;
            int i = 0;
            while (aux->nombre[i] != '\0' || nombreBuscado[i] != '\0')
            {
                if (aux->nombre[i] != nombreBuscado[i])
                {
                    sonIguales = false;
                    break;
                }
                i++;
            }
            if (sonIguales)
                return aux;
            aux = aux->siguiente;
        }
        return nullptr;
    }
};

struct NodoPila
{
    char mensaje[TAM_MENSAJE];
    NodoPila *siguiente;
    NodoPila(const char *m) : siguiente(nullptr)
    {
        copiarTexto(mensaje, m, TAM_MENSAJE);
    }
};

class Pila
{
private:
    NodoPila *tope;
    int tamanio;

public:
    Pila() : tope(nullptr), tamanio(0) {}

    ~Pila()
    {
        while (tope)
        {
            NodoPila *temp = tope;
            tope = tope->siguiente;
            delete temp;
        }
    }

    void push(const char *msg)
    {
        NodoPila *nuevo = new NodoPila(msg);
        nuevo->siguiente = tope;
        tope = nuevo;
        tamanio++;
    }

    void pop(char *destino)
    {
        if (!tope)
        {
            destino[0] = '\0';
            return;
        }
        copiarTexto(destino, tope->mensaje, TAM_MENSAJE);
        NodoPila *temp = tope;
        tope = tope->siguiente;
        delete temp;
        tamanio--;
    }

    bool estaVacia() const { return tope == nullptr; }
    int getTamanio() const { return tamanio; }
};

struct NodoCola
{
    char nombreLinea[TAM_NOMBRE];
    NodoCola *siguiente;
    NodoCola(const char *n) : siguiente(nullptr)
    {
        copiarTexto(nombreLinea, n, TAM_NOMBRE);
    }
};

class Cola
{
private:
    NodoCola *frente;
    NodoCola *final_;
    int tamanio;

public:
    Cola() : frente(nullptr), final_(nullptr), tamanio(0) {}

    ~Cola()
    {
        while (frente)
        {
            NodoCola *temp = frente;
            frente = frente->siguiente;
            delete temp;
        }
    }

    void encolar(const char *nombre)
    {
        NodoCola *nuevo = new NodoCola(nombre);
        if (!final_)
        {
            frente = final_ = nuevo;
        }
        else
        {
            final_->siguiente = nuevo;
            final_ = nuevo;
        }
        tamanio++;
    }

    void desencolar(char *destino)
    {
        if (!frente)
        {
            destino[0] = '\0';
            return;
        }
        copiarTexto(destino, frente->nombreLinea, TAM_NOMBRE);
        NodoCola *temp = frente;
        frente = frente->siguiente;
        if (!frente)
            final_ = nullptr;
        delete temp;
        tamanio--;
    }

    bool estaVacia() const { return frente == nullptr; }
    int getTamanio() const { return tamanio; }
};

struct Conjunto
{
    LineaProduccion **elementos;
    int tamanio;
    int capacidad;
    char nombre[10];

    Conjunto(const char *n, int cap = 50)
        : tamanio(0), capacidad(cap)
    {
        elementos = new LineaProduccion *[cap];
        copiarTexto(nombre, n, 10);
    }

    ~Conjunto() { delete[] elementos; }

    void agregar(LineaProduccion *linea)
    {
        if (tamanio < capacidad)
            elementos[tamanio++] = linea;
    }

    void limpiar() { tamanio = 0; }

    bool contiene(const char *nombreBuscado) const
    {
        for (int i = 0; i < tamanio; i++)
        {
            bool sonIguales = true;
            int j = 0;
            while (elementos[i]->nombre[j] != '\0' || nombreBuscado[j] != '\0')
            {
                if (elementos[i]->nombre[j] != nombreBuscado[j])
                {
                    sonIguales = false;
                    break;
                }
                j++;
            }
            if (sonIguales)
                return true;
        }
        return false;
    }

    void imprimir() const;
};

void evaluarProposiciones(LineaProduccion *linea)
{
    linea->P = (linea->produccionActual >= linea->metaDiaria);
    linea->Q = (linea->fallosMecanicos == 0);
    linea->R = linea->supervisorValido;

    if (!linea->Q)
    {
        linea->estadoInferido = ESTADO_DETENIDA;
    }

    else if (linea->P && linea->Q && linea->R)
    {
        linea->estadoInferido = ESTADO_OPERATIVA;
    }

    else
    {
        linea->estadoInferido = ESTADO_BAJO_RENDIMIENTO;
    }
}

void clasificarEnConjuntos(ListaLineas &lista,
                           Conjunto &A, Conjunto &B, Conjunto &C)
{
    A.limpiar();
    B.limpiar();
    C.limpiar();
    LineaProduccion *aux = lista.getCabeza();
    while (aux)
    {
        if (aux->estadoInferido == ESTADO_OPERATIVA)
            A.agregar(aux);
        else if (aux->estadoInferido == ESTADO_BAJO_RENDIMIENTO)
            B.agregar(aux);
        else if (aux->estadoInferido == ESTADO_DETENIDA)
            C.agregar(aux);
        aux = aux->siguiente;
    }
}

void interseccion(const Conjunto &A, const Conjunto &B, Conjunto &resultado)
{
    resultado.limpiar();
    for (int i = 0; i < A.tamanio; i++)
        if (B.contiene(A.elementos[i]->nombre))
            resultado.agregar(A.elementos[i]);
}

void diferencia(const Conjunto &A, const Conjunto &C, Conjunto &resultado)
{
    resultado.limpiar();
    for (int i = 0; i < A.tamanio; i++)
        if (!C.contiene(A.elementos[i]->nombre))
            resultado.agregar(A.elementos[i]);
}

void unionConjuntos(const Conjunto &B, const Conjunto &C, Conjunto &resultado)
{
    resultado.limpiar();
    for (int i = 0; i < B.tamanio; i++)
        resultado.agregar(B.elementos[i]);
    for (int i = 0; i < C.tamanio; i++)
        if (!resultado.contiene(C.elementos[i]->nombre))
            resultado.agregar(C.elementos[i]);
}

void separador(char c = '=', int n = 60)
{
    for (int i = 0; i < n; i++)
        cout << c;
    cout << "\n";
}

void encabezado(const char *titulo)
{
    separador();
    cout << "  " << titulo << "\n";
    separador();
}

void imprimirBool(bool v)
{
    cout << (v ? "VERDADERO" : "FALSO");
}

void imprimirEstadoTag(int estado)
{
    if (estado == ESTADO_OPERATIVA)
        cout << "[ OPERATIVA]";
    else if (estado == ESTADO_BAJO_RENDIMIENTO)
        cout << "[ BAJO RENDIMIENTO]";
    else if (estado == ESTADO_DETENIDA)
        cout << "[ DETENIDA]";
    else
        cout << "[?]";
}

void imprimirConUnDecimal(double valor)
{
    long long centesimas = (long long)(valor * 10.0 + (valor >= 0 ? 0.5 : -0.5));
    long long parteEntera = centesimas / 10;
    long long parteDecimal = centesimas % 10;
    if (parteDecimal < 0)
        parteDecimal = -parteDecimal;
    cout << parteEntera << "." << parteDecimal;
}

void imprimirConDosDecimales(double valor)
{
    long long centesimas = (long long)(valor * 100.0 + (valor >= 0 ? 0.5 : -0.5));
    long long parteEntera = centesimas / 100;
    long long parteDecimal = centesimas % 100;
    if (parteDecimal < 0)
        parteDecimal = -parteDecimal;
    cout << parteEntera << ".";
    if (parteDecimal < 10)
        cout << "0";
    cout << parteDecimal;
}

void Conjunto::imprimir() const
{
    if (tamanio == 0)
    {
        cout << "  (vacío)\n";
        return;
    }
    for (int i = 0; i < tamanio; i++)
    {
        cout << "  [" << (i + 1) << "] " << elementos[i]->nombre << "  (";
        imprimirEstadoTag(elementos[i]->estadoInferido);
        cout << ")\n";
    }
}

void mostrarDetalleLinea(const LineaProduccion *linea)
{
    separador('-', 60);
    cout << "  Línea : " << linea->nombre << "\n";
    separador('-', 60);
    cout << "  Producción actual : ";
    imprimirConUnDecimal(linea->produccionActual);
    cout << " uds\n";
    cout << "  Meta diaria       : ";
    imprimirConUnDecimal(linea->metaDiaria);
    cout << " uds\n";
    cout << "  Fallos mecánicos  : " << linea->fallosMecanicos << "\n";
    cout << "  Validación superv.: " << (linea->supervisorValido ? "SÍ" : "NO") << "\n";
    cout << "\n";
    cout << "  --- Proposiciones lógicas ---\n";
    cout << "  P (cumple meta)   : ";
    imprimirBool(linea->P);
    cout << "\n";
    cout << "  Q (sin fallos)    : ";
    imprimirBool(linea->Q);
    cout << "\n";
    cout << "  R (supervisor OK) : ";
    imprimirBool(linea->R);
    cout << "\n";
    cout << "\n";
    cout << "  --- Inferencia ---\n";
    if (!linea->Q)
        cout << "  Regla aplicada    : ¬Q → DETENIDA\n";
    else if (linea->P && linea->Q && linea->R)
        cout << "  Regla aplicada    : P ∧ Q ∧ R → OPERATIVA\n";
    else
        cout << "  Regla aplicada    : ¬P ∧ Q → BAJO RENDIMIENTO\n";
    cout << "  Estado inferido   : ";
    imprimirEstadoTag(linea->estadoInferido);
    cout << "\n";
}

void mostrarResumen(const ListaLineas &lista,
                    const Conjunto &A, const Conjunto &B, const Conjunto &C)
{
    encabezado("RESUMEN FINAL DEL SISTEMA - TechFactory");

    int total = lista.getCantidad();
    int operativas = A.tamanio;
    int bajoRend = B.tamanio;
    int detenidas = C.tamanio;

    int totalFallos = 0;
    LineaProduccion *aux = lista.getCabeza();
    while (aux)
    {
        totalFallos += aux->fallosMecanicos;
        aux = aux->siguiente;
    }

    double eficiencia = (total > 0)
                            ? (100.0 * operativas / total)
                            : 0.0;

    cout << "  Total de líneas evaluadas : " << total << "\n";
    cout << "  Líneas operativas (A)     : " << operativas << "\n";
    cout << "  Líneas bajo rendim. (B)   : " << bajoRend << "\n";
    cout << "  Líneas detenidas (C)      : " << detenidas << "\n";
    separador('-', 60);
    cout << "  Total de fallos registr.  : " << totalFallos << "\n";
    cout << "  Porcentaje de eficiencia  : ";
    imprimirConDosDecimales(eficiencia);
    cout << " %\n";
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

void menuConjuntos(const Conjunto &A, const Conjunto &B, const Conjunto &C)
{
    Conjunto resInter("A∩B");
    Conjunto resDif("A−C");
    Conjunto resUnion("B∪C");

    int opcion;
    do
    {
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
        cin >> opcion;
        cin.ignore();
        cout << "\n";

        switch (opcion)
        {
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

LineaProduccion *ingresarLinea()
{
    LineaProduccion *linea = new LineaProduccion();
    char respuesta[10];

    encabezado("REGISTRO DE LÍNEA DE PRODUCCIÓN");

    cout << "  Nombre de la línea       : ";
    cin.getline(linea->nombre, TAM_NOMBRE);

    cout << "  Meta de producción diaria: ";
    cin >> linea->metaDiaria;

    cout << "  Producción actual (uds)  : ";
    cin >> linea->produccionActual;

    cout << "  Fallos mecánicos hoy     : ";
    cin >> linea->fallosMecanicos;

    cout << "  Supervisor validó (s/n)  : ";
    cin >> respuesta;
    cin.ignore();
    linea->supervisorValido = (respuesta[0] == 's' || respuesta[0] == 'S');

    evaluarProposiciones(linea);

    cout << "\n  [✔] Línea registrada y analizada: ";
    imprimirEstadoTag(linea->estadoInferido);
    cout << "\n\n";

    return linea;
}

void verHistorial(Pila &historial)
{
    encabezado("HISTORIAL DE OPERACIONES (PILA)");
    if (historial.estaVacia())
    {
        cout << "  El historial está vacío.\n\n";
        return;
    }

    Pila temp;
    char msg[TAM_MENSAJE];

    while (!historial.estaVacia())
    {
        historial.pop(msg);
        temp.push(msg);
    }

    int n = 1;
    while (!temp.estaVacia())
    {
        temp.pop(msg);
        cout << "  [" << n++ << "] " << msg << "\n";
        historial.push(msg);
    }
    cout << "\n";
}

void verColaRevision(Cola &colaRevision)
{
    encabezado("COLA DE LÍNEAS PENDIENTES DE REVISIÓN");
    if (colaRevision.estaVacia())
    {
        cout << "  No hay líneas en cola de revisión.\n\n";
        return;
    }

    Cola temp;
    char nombre[TAM_NOMBRE];
    int n = 1;
    while (!colaRevision.estaVacia())
    {
        colaRevision.desencolar(nombre);
        cout << "  [" << n++ << "] " << nombre << "\n";
        temp.encolar(nombre);
    }

    while (!temp.estaVacia())
    {
        temp.desencolar(nombre);
        colaRevision.encolar(nombre);
    }
    cout << "\n";
}

void verTodasLasLineas(const ListaLineas &lista)
{
    if (lista.getCantidad() == 0)
    {
        cout << "  No hay líneas registradas.\n\n";
        return;
    }
    encabezado("DETALLE DE TODAS LAS LÍNEAS");
    LineaProduccion *aux = lista.getCabeza();
    while (aux)
    {
        mostrarDetalleLinea(aux);
        aux = aux->siguiente;
    }
    cout << "\n";
}

struct EstadoTeorico
{
    int meta;
    int falla;
    int validado;
};

void imprimirMetaTeorico(int meta)
{
    if (meta == 0)
        cout << "Baja";
    else if (meta == 1)
        cout << "Media";
    else
        cout << "Alta";
}

void imprimirSiNoTeorico(int valor)
{
    if (valor == 1)
        cout << "Si";
    else
        cout << "No";
}

int contarEstadosPosibles(int nMeta, int nFalla, int nValidado)
{
    return nMeta * nFalla * nValidado;
}

void listarEstadosTeoricos()
{
    encabezado("LISTADO DE ESTADOS TEÓRICOS (Meta, Falla, Validado)");
    int contador = 1;
    for (int meta = 0; meta < 3; meta++)
    {
        for (int falla = 0; falla < 2; falla++)
        {
            for (int validado = 0; validado < 2; validado++)
            {
                cout << "  " << contador << ". Meta=";
                imprimirMetaTeorico(meta);
                cout << " | Falla=";
                imprimirSiNoTeorico(falla);
                cout << " | Validado=";
                imprimirSiNoTeorico(validado);
                cout << "\n";
                contador++;
            }
        }
    }
    cout << "\n";
}

long long factorial(int n)
{
    if (n == 0)
    {
        return 1;
    }
    return n * factorial(n - 1);
}

long long combinaciones(int n, int k)
{
    if (k < 0 || k > n)
        return 0;
    return factorial(n) / (factorial(k) * factorial(n - k));
}

double probabilidadCondicion(int nMeta, int nFalla, int nValidado,
                             int nivelesFavorablesMeta,
                             int nivelesFavorablesFalla,
                             int nivelesFavorablesValidado)
{
    long long casosTotales = (long long)nMeta * nFalla * nValidado;
    long long casosFavorables = (long long)nivelesFavorablesMeta * nivelesFavorablesFalla * nivelesFavorablesValidado;
    return (double)casosFavorables / casosTotales;
}

double probabilidadFalla(int nMeta, int nFalla, int nValidado, int nivelesQueSonFalla)
{
    return probabilidadCondicion(nMeta, nFalla, nValidado,
                                 nMeta, nivelesQueSonFalla, nValidado);
}

double probabilidadBajoRendimientoTeorico(int nMeta, int nFalla, int nValidado, int nivelesQueSonBajoRendimiento)
{
    return probabilidadCondicion(nMeta, nFalla, nValidado,
                                 nivelesQueSonBajoRendimiento, nFalla, nValidado);
}

double probabilidadOperacionNormal(int nMeta, int nFalla, int nValidado,
                                   int nivelesSinFalla, int nivelesValidado)
{
    return probabilidadCondicion(nMeta, nFalla, nValidado,
                                 nMeta, nivelesSinFalla, nivelesValidado);
}

long long costoRecurrencia(int n)
{
    if (n == 0)
        return 0;
    return costoRecurrencia(n - 1) + 777;
}

long long costoFormaCerrada(int n)
{
    return 777LL * n;
}

bool esMenorOIgualTeorico(const EstadoTeorico &a, const EstadoTeorico &b)
{
    return (b.meta >= a.meta) && (b.falla <= a.falla) && (b.validado >= a.validado);
}

bool sonIgualesTeorico(const EstadoTeorico &a, const EstadoTeorico &b)
{
    return (a.meta == b.meta) && (a.falla == b.falla) && (a.validado == b.validado);
}

EstadoTeorico obtenerEstadoPorIndice(int indice)
{
    int meta = indice / 4;
    int resto = indice % 4;
    int falla = resto / 2;
    int validado = resto % 2;
    EstadoTeorico e = {meta, falla, validado};
    return e;
}

bool existeIntermedio(const EstadoTeorico &a, const EstadoTeorico &b, int totalEstados)
{
    for (int i = 0; i < totalEstados; i++)
    {
        EstadoTeorico c = obtenerEstadoPorIndice(i);
        bool esDistintoDeAyB = !sonIgualesTeorico(c, a) && !sonIgualesTeorico(c, b);
        if (esDistintoDeAyB && esMenorOIgualTeorico(a, c) && esMenorOIgualTeorico(c, b))
        {
            return true;
        }
    }
    return false;
}

void construirDiagramaHasse(int totalEstados)
{
    encabezado("DIAGRAMA DE HASSE (relaciones de cobertura)");
    cout << "  Lectura: 'A -> B' significa que B es el siguiente nivel\n";
    cout << "  de eficiencia inmediatamente superior a A.\n\n";

    for (int i = 0; i < totalEstados; i++)
    {
        EstadoTeorico a = obtenerEstadoPorIndice(i);
        for (int j = 0; j < totalEstados; j++)
        {
            if (i == j)
                continue;
            EstadoTeorico b = obtenerEstadoPorIndice(j);

            bool esRelacionValida = esMenorOIgualTeorico(a, b) && !sonIgualesTeorico(a, b);
            if (esRelacionValida && !existeIntermedio(a, b, totalEstados))
            {
                cout << "  [Meta=";
                imprimirMetaTeorico(a.meta);
                cout << ",Falla=";
                imprimirSiNoTeorico(a.falla);
                cout << ",Val=";
                imprimirSiNoTeorico(a.validado);
                cout << "]  ->  [Meta=";
                imprimirMetaTeorico(b.meta);
                cout << ",Falla=";
                imprimirSiNoTeorico(b.falla);
                cout << ",Val=";
                imprimirSiNoTeorico(b.validado);
                cout << "]\n";
            }
        }
    }
    cout << "\n";
}

void analisisComplejidad()
{
    encabezado("ANÁLISIS DE COMPLEJIDAD COMPUTACIONAL");
    cout << "  1. Conteo de estados:        O(1)   -> multiplicación directa.\n";
    cout << "  2. Listado de estados:       O(E)   -> E = 12 estados fijos.\n";
    cout << "  3. Combinaciones C(n,k):     O(n)   -> factorial recursivo,\n";
    cout << "     con caso base 0!=1, evita el cálculo indefinido.\n";
    cout << "  4. Probabilidad discreta:    O(1)   -> división de conteos fijos.\n";
    cout << "  5. Recurrencia T(n)=T(n-1)+777: O(n) -> costo lineal en líneas.\n";
    cout << "  6. Diagrama de Hasse:        O(E^3) -> por cada par (E^2) se\n";
    cout << "     revisa un posible intermedio (E), con E=12 fijo y pequeño,\n";
    cout << "     por lo que en la práctica el costo es constante (E es fijo),\n";
    cout << "     pero crecería como cubo si E aumentara con más variables.\n";
    cout << "\n  Relación con la jerarquía: el Hasse organiza los 12 estados\n";
    cout << "  en niveles de eficiencia. Cada nivel adicional de la jerarquía\n";
    cout << "  implica más comparaciones (mayor profundidad), igual que más\n";
    cout << "  líneas de producción implican más evaluaciones en T(n)=777n.\n";
    cout << "  Ambos crecimientos son controlados (lineal o polinomial fijo),\n";
    cout << "  lo que confirma que el sistema escala de forma predecible.\n\n";
}

void menuGestionLineas(ListaLineas &lista, Conjunto &A, Conjunto &B, Conjunto &C,
                       Pila &historial, Cola &colaRevision)
{
    int opcion;
    do
    {
        encabezado("GESTIÓN DE LÍNEAS DE PRODUCCIÓN");
        cout << "  [1] Registrar nueva línea de producción\n";
        cout << "  [2] Ver detalle de todas las líneas\n";
        cout << "  [3] Analizar y clasificar en conjuntos (A, B, C)\n";
        cout << "  [4] Operaciones de conjuntos\n";
        cout << "  [5] Cola de revisión (líneas con incidencias)\n";
        cout << "  [6] Historial de operaciones (pila)\n";
        cout << "  [7] Resumen final del sistema\n";
        cout << "  [0] Volver al menú principal\n";
        separador('-', 60);
        cout << "  Líneas registradas: " << lista.getCantidad() << "\n";
        cout << "  Seleccione: ";
        cin >> opcion;
        cin.ignore();
        cout << "\n";

        switch (opcion)
        {

        case 1:
        {
            LineaProduccion *nueva = ingresarLinea();
            lista.insertar(nueva);

            if (nueva->estadoInferido != ESTADO_OPERATIVA)
                colaRevision.encolar(nueva->nombre);

            char msg[TAM_MENSAJE];
            int i = 0;
            const char *prefijo = "Línea registrada: ";
            while (prefijo[i] != '\0')
            {
                msg[i] = prefijo[i];
                i++;
            }
            int j = 0;
            while (nueva->nombre[j] != '\0' && i < TAM_MENSAJE - 1)
            {
                msg[i++] = nueva->nombre[j++];
            }
            const char *sufijo = " | Estado registrado";
            j = 0;
            while (sufijo[j] != '\0' && i < TAM_MENSAJE - 1)
            {
                msg[i++] = sufijo[j++];
            }
            msg[i] = '\0';

            historial.push(msg);
            break;
        }

        case 2:
            verTodasLasLineas(lista);
            historial.push("Consulta: detalle de todas las líneas");
            break;

        case 3:
            if (lista.getCantidad() == 0)
            {
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
            if (lista.getCantidad() == 0)
            {
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
            if (lista.getCantidad() == 0)
            {
                cout << "  [!] No hay líneas registradas.\n\n";
                break;
            }
            clasificarEnConjuntos(lista, A, B, C);
            mostrarResumen(lista, A, B, C);
            historial.push("Resumen final generado");
            break;

        case 0:
            cout << "  Volviendo al menú principal...\n\n";
            break;

        default:
            cout << "  [!] Opción no válida. Intente nuevamente.\n\n";
        }
    } while (opcion != 0);
}

int main()
{

    ListaLineas lista;
    Conjunto A("A");
    Conjunto B("B");
    Conjunto C("C");
    Pila historial;
    Cola colaRevision;

    int opcion;

    separador('*');
    cout << "  TECHFACTORY — Sistema de Análisis Lógico de Producción\n";
    cout << "  Lógica Proposicional + Conjuntos + Estructuras Dinámicas\n";
    separador('*');
    cout << "\n";

    do
    {
        encabezado("MENÚ PRINCIPAL");
        cout << "  [1] Gestión de líneas de producción (registro, conjuntos, etc.)\n";
        cout << "  [2] Principio de conteo (estados teóricos posibles)\n";
        cout << "  [3] Combinaciones (elegir k líneas óptimas de n)\n";
        cout << "  [4] Probabilidad discreta (falla, bajo rendim., normal)\n";
        cout << "  [5] Ecuación de recurrencia (costo computacional)\n";
        cout << "  [6] Diagrama de Hasse (orden parcial de eficiencia)\n";
        cout << "  [7] Análisis de complejidad computacional\n";
        cout << "  [0] Salir\n";
        separador('-', 60);
        cout << "  Líneas registradas: " << lista.getCantidad() << "\n";
        cout << "  Seleccione: ";
        cin >> opcion;
        cin.ignore();
        cout << "\n";

        const int nMeta = 3;
        const int nFalla = 2;
        const int nValidado = 2;
        const int totalEstados = contarEstadosPosibles(nMeta, nFalla, nValidado);

        switch (opcion)
        {

        case 1:
            menuGestionLineas(lista, A, B, C, historial, colaRevision);
            break;

        case 2:
            cout << "  Meta(" << nMeta << ") x Falla(" << nFalla << ") x Validado("
                 << nValidado << ") = " << totalEstados << " estados posibles.\n\n";
            listarEstadosTeoricos();
            historial.push("Principio de conteo consultado");
            break;

        case 3:
        {
            int totalLineas, lineasOptimas;
            cout << "  Total de líneas disponibles : ";
            cin >> totalLineas;
            cout << "  Líneas óptimas a elegir      : ";
            cin >> lineasOptimas;
            cin.ignore();
            cout << "\n  C(" << totalLineas << "," << lineasOptimas << ") = "
                 << combinaciones(totalLineas, lineasOptimas)
                 << "  formas de elegir, sin importar el orden.\n\n";
            historial.push("Combinaciones calculadas");
            break;
        }

        case 4:
            cout << "  Probabilidad discreta sobre " << totalEstados << " estados:\n";
            cout << "  P(Falla)            = " << probabilidadFalla(nMeta, nFalla, nValidado, 1) << "\n";
            cout << "  P(Bajo rendimiento) = " << probabilidadBajoRendimientoTeorico(nMeta, nFalla, nValidado, 1) << "\n";
            cout << "  P(Operación normal) = " << probabilidadOperacionNormal(nMeta, nFalla, nValidado, 1, 1) << "\n\n";
            historial.push("Probabilidad discreta calculada");
            break;

        case 5:
        {
            int n;
            cout << "  Número de líneas a evaluar (n): ";
            cin >> n;
            cin.ignore();
            cout << "\n  Ecuación de recurrencia: T(n) = T(n-1) + 777, T(0) = 0\n";
            for (int i = 1; i <= n; i++)
            {
                cout << "  T(" << i << ") recursivo = " << costoRecurrencia(i)
                     << "  |  forma cerrada 777*n = " << costoFormaCerrada(i) << "\n";
            }
            cout << "\n";
            historial.push("Ecuación de recurrencia calculada");
            break;
        }

        case 6:
            construirDiagramaHasse(totalEstados);
            historial.push("Diagrama de Hasse generado");
            break;

        case 7:
            analisisComplejidad();
            historial.push("Análisis de complejidad consultado");
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
