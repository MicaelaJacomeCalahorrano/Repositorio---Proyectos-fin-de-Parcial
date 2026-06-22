#include <iostream>

using namespace std;

/* ================= PROTOTIPOS DE FUNCIONES ================= */
void iniciarSistema();
void evaluarEstudiante();

/* Las 3 partes de la evaluacion del estudiante */
float evaluarConjuntoP();
float evaluarConjuntoA();
int evaluarConjuntoC();
void imprimirClasificacion(float p, float a, int c);

/* Funciones de calculo de Modelos Discretos */
void mostrarAnalisisMatematico();
int calcularEstados(int cardP, int cardA, int cardC);
int calcularFactorial(int n);
int calcularCombinacion(int n, int k);
float calcularProbabilidadTotal(float p_a1, float p_a2, float cond1, float cond2);

/* ================= FUNCION PRINCIPAL ================= */
/* RESTRICCION CUMPLIDA: El main solo coordina, no tiene logica */
int main() {
    iniciarSistema();
    return 0;
}

/* ================= IMPLEMENTACION DE FUNCIONES ================= */

void iniciarSistema() {
    int opcion = 0;
    
    do {
        cout << "\n============================================\n";
        cout << " SISTEMA DE MODELOS DISCRETOS DE EVALUACION\n";
        cout << "============================================\n";
        cout << "1. Ingresar y evaluar a un estudiante\n";
        cout << "2. Ejecutar algoritmos de analisis matematico\n";
        cout << "3. Salir\n";
        cout << "Ingrese su opcion: ";
        cin >> opcion;
        
        if (cin.fail()) {
            cout << "\nError de entrada. Digite un numero.\n";
            cin.clear();
            cin.ignore(10000, '\n');
            opcion = 0;
            continue;
        }
        
        if (opcion == 1) {
            evaluarEstudiante();
        } else if (opcion == 2) {
            mostrarAnalisisMatematico();
        } else if (opcion == 3) {
            cout << "\nFinalizando sistema...\n";
        } else {
            cout << "\nError: Opcion no valida.\n";
        }
    } while (opcion != 3);
}

/* --- DIVISION DEL ESTUDIANTE EN 3 PARTES --- */

void evaluarEstudiante() {
    cout << "\n--- RECOLECCION DE VARIABLES CATEGORICAS ---\n";
    
    /* Parte 1 */
    float promedio = evaluarConjuntoP();
    /* Parte 2 */
    float asistencia = evaluarConjuntoA();
    /* Parte 3 */
    int actividades = evaluarConjuntoC();
    
    /* Imprimir resultados de la inferencia */
    imprimirClasificacion(promedio, asistencia, actividades);
}

float evaluarConjuntoP() {
    float p = -1.0;
    while (p < 0.0 || p > 10.0) {
        cout << "[Fase 1] Ingrese el promedio (0.0 a 10.0): ";
        cin >> p;
        if (cin.fail() || p < 0.0 || p > 10.0) {
            cout << "Error en Promedio. Intente de nuevo.\n";
            cin.clear();
            cin.ignore(10000, '\n');
            p = -1.0;
        }
    }
    return p;
}

float evaluarConjuntoA() {
    float a = -1.0;
    while (a < 0.0 || a > 100.0) {
        cout << "[Fase 2] Ingrese % de asistencia (0 a 100): ";
        cin >> a;
        if (cin.fail() || a < 0.0 || a > 100.0) {
            cout << "Error en Asistencia. Intente de nuevo.\n";
            cin.clear();
            cin.ignore(10000, '\n');
            a = -1.0;
        }
    }
    return a;
}

int evaluarConjuntoC() {
    int c = 0;
    while (c < 1 || c > 3) {
        cout << "[Fase 3] Actividades (1:Completo, 2:Parcial, 3:Nulo): ";
        cin >> c;
        if (cin.fail() || c < 1 || c > 3) {
            cout << "Error en Actividades. Seleccione 1, 2 o 3.\n";
            cin.clear();
            cin.ignore(10000, '\n');
            c = 0;
        }
    }
    return c;
}

void imprimirClasificacion(float p, float a, int c) {
    cout << "\n--- CLASIFICACION DEL ESTADO (INFERENCIA) ---\n";
    
    if (p >= 8.5) cout << "Conjunto P -> p1 (Alto)\n";
    else if (p >= 7.0) cout << "Conjunto P -> p2 (Medio)\n";
    else cout << "Conjunto P -> p3 (Bajo)\n";
    
    if (a >= 75.0) cout << "Conjunto A -> a1 (Cumple)\n";
    else cout << "Conjunto A -> a2 (No cumple)\n";
    
    if (c == 1) cout << "Conjunto C -> c1 (Completo)\n";
    else if (c == 2) cout << "Conjunto C -> c2 (Parcial)\n";
    else cout << "Conjunto C -> c3 (Nulo)\n";
    cout << "---------------------------------------------\n";
}

/* --- MODELOS MATEMATICOS DISCRETOS --- */

void mostrarAnalisisMatematico() {
    cout << "\n--- EJECUCION DE ALGORITMOS MATEMATICOS ---\n";
    
    /* 1. Calculo de Estados (Principio de Conteo) */
    int cardinalidad_P = 3;
    int cardinalidad_A = 2;
    int cardinalidad_C = 3;
    int total_estados = calcularEstados(cardinalidad_P, cardinalidad_A, cardinalidad_C);
    
    cout << "\n1. PRINCIPIO FUNDAMENTAL DEL CONTEO:\n";
    cout << "Calculando |P| x |A| x |C|...\n";
    cout << "Resultado: El sistema posee " << total_estados << " estados posibles.\n";
    
    /* 2. Calculo Combinatorio */
    int n_evaluaciones = 3;
    int k_aprobadas = 2;
    int maneras = calcularCombinacion(n_evaluaciones, k_aprobadas);
    
    cout << "\n2. ANALISIS COMBINATORIO:\n";
    cout << "Calculando C(" << n_evaluaciones << ", " << k_aprobadas << ")...\n";
    cout << "Resultado: Existen " << maneras << " formas distintas de aprobar.\n";
    
    /* 3. Calculo de Probabilidad Discreta */
    float p_a1 = 0.60;
    float p_a2 = 0.40;
    float prob_apr_dado_a1 = 0.75;
    float prob_apr_dado_a2 = 0.20;
    float prob_total = calcularProbabilidadTotal(p_a1, p_a2, prob_apr_dado_a1, prob_apr_dado_a2);
    
    cout << "\n3. PROBABILIDAD DISCRETA CONDICIONAL:\n";
    cout << "Aplicando Teorema de Probabilidad Total...\n";
    cout << "Resultado: Probabilidad de aprobacion desde recuperacion = " << (prob_total * 100) << "%\n";
    
    /* 4. Validacion Teorica (Se imprime tal cual dicta el documento) */
    cout << "\n4. DEMOSTRACION Y COMPLEJIDAD (Teorico):\n";
    cout << "- Induccion verificada: C(n) = C(n-1) AND E(n) es valida para n >= 1.\n";
    cout << "- Grafo Hasse: Reprobacion -> Recuperacion -> Aprobacion.\n";
    cout << "- Complejidad total para 'm' estudiantes: Lineal O(m).\n";
    cout << "-------------------------------------------\n";
}

int calcularEstados(int cardP, int cardA, int cardC) {
    return cardP * cardA * cardC;
}

int calcularFactorial(int n) {
    int factorial = 1;
    for (int i = 1; i <= n; i++) {
        factorial = factorial * i;
    }
    return factorial;
}

int calcularCombinacion(int n, int k) {
    /* C(n, k) = n! / (k! * (n-k)!) */
    int numerador = calcularFactorial(n);
    int denominador = calcularFactorial(k) * calcularFactorial(n - k);
    return numerador / denominador;
}

float calcularProbabilidadTotal(float p_a1, float p_a2, float cond1, float cond2) {
    /* P(Sapr|Srec) = P(Sapr|a1)*P(a1) + P(Sapr|a2)*P(a2) */
    return (cond1 * p_a1) + (cond2 * p_a2);
}
