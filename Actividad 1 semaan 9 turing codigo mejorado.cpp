#include <iostream>
#include <string>
#include <vector>
#include <algorithm> // Para std::find_if

using namespace std;

// Enumeración para la dirección del movimiento del cabezal
enum Direccion { IZQ = -1, DER = 1 };

// Estructura para representar una única transición de la máquina
struct Transicion {
    string estadoActual;
    char simboloLectura;
    string estadoSiguiente;
    char simboloEscritura;
    Direccion dirMov;
};

// Clase que encapsula la lógica de la Máquina de Turing
class MaquinaTuring {
private:
    string estadoActual;
    int cabezal;
    vector<char> cinta;
    const vector<Transicion>& transiciones;
    string estadoInicial;

public:
    // Constructor
    MaquinaTuring(string _estadoInicial, const vector<Transicion>& tr, const string& entrada)
        : transiciones(tr), estadoInicial(_estadoInicial) {
        
        // Inicializa la cinta con un símbolo en blanco al principio y al final
        cinta.push_back('_');
        for (char c : entrada) {
            cinta.push_back(c);
        }
        cinta.push_back('_');

        // El cabezal comienza en el primer símbolo de la entrada
        cabezal = 1;
        estadoActual = estadoInicial;
    }

    // Método para ejecutar la simulación de la máquina
    void ejecutar() {
        cout << "--- Iniciando Simulacion ---" << endl;
        imprimirEstado();

        while (true) {
            // Aseguramos que la cinta sea "infinita" hacia la derecha
            if (cabezal == cinta.size()) {
                cinta.push_back('_');
            }
            // Aseguramos que la cinta sea "infinita" hacia la izquierda
            if (cabezal < 0) {
                cinta.insert(cinta.begin(), '_');
                cabezal = 0;
            }

            char simboloActual = cinta[cabezal];
            
            // Buscar la transición que coincida con el estado y símbolo actual
            auto it = find_if(transiciones.begin(), transiciones.end(), 
                [this, simboloActual](const Transicion& t) {
                    return t.estadoActual == estadoActual && t.simboloLectura == simboloActual;
                });

            // Si no se encuentra una transición válida, la máquina se detiene
            if (it == transiciones.end()) {
                break; 
            }

            const Transicion& t = *it;

            // Aplicar la transición
            cinta[cabezal] = t.simboloEscritura;
            estadoActual = t.estadoSiguiente;
            cabezal += t.dirMov;

            imprimirEstado();
        }

        cout << "\n--- Simulacion Finalizada ---" << endl;
    }

    // Método para imprimir el estado final de la máquina
    void imprimirResultadoFinal() {
        cout << "Estado final: " << estadoActual << endl;
        cout << "Cinta final:  ";
        for (char c : cinta) {
            cout << c;
        }
        cout << endl;
    }

private:
    // Método auxiliar para imprimir el estado actual de la cinta y el cabezal
    void imprimirEstado() {
        cout << "Estado: " << estadoActual << "\t| Cinta: ";
        for(int i = 0; i < cinta.size(); ++i) {
            if(i == cabezal) {
                cout << "[" << cinta[i] << "]";
            } else {
                cout << cinta[i];
            }
        }
        cout << endl;
    }
};

int main() {
    // Definición de todas las transiciones basadas en el diagrama de JFLAP
    // El caracter '_' representa el símbolo en blanco (☐)
    vector<Transicion> transiciones = {
        // Estado q0: Moverse a la derecha hasta encontrar un espacio en blanco
        {"q0", '0', "q0", '0', DER},
        {"q0", '1', "q0", '1', DER},
        {"q0", '_', "q1", '_', IZQ},

        // Estado q1: Moverse a la izquierda. Si encuentra un '1', lo cambia a '0' y pasa a q2. Si encuentra un blanco, termina una fase.
        {"q1", '0', "q1", '0', IZQ},
        {"q1", '1', "q2", '0', IZQ},
        {"q1", '_', "q4", '_', DER},

        // Estado q2: Moverse a la izquierda sobre los '1's y '0's. Al encontrar un blanco, escribe '1'.
        {"q2", '1', "q2", '1', IZQ},
        {"q2", '0', "q1", '1', IZQ},
        {"q2", '_', "q3", '1', IZQ},

        // Estado q3: Ha escrito un '1' al principio. Vuelve al inicio del nuevo segmento.
        {"q3", '_', "q4", '0', DER},

        // Estado q4: Avanza hacia la derecha sobre todos los símbolos hasta el final.
        {"q4", '1', "q4", '1', DER},
        {"q4", '0', "q4", '0', DER},
        {"q4", '_', "q5", '_', IZQ},

        // Estado q5: Ha llegado al final. Escribe los dos últimos '1's para completar 2X+1.
        {"q5", '0', "q6", '1', IZQ},
        {"q5", '1', "q7", '0', IZQ},

        // Estado q7: Estado auxiliar
        {"q7", '1', "q7", '0', IZQ},
    };
    
    // El estado de aceptación es q6, donde la máquina se detiene naturalmente.

    string entrada;
    cout << "Esta maquina calcula 2X + 1." << endl;
    cout << "Ingrese un numero en formato binario (ej: 111 para 3): ";
    cin >> entrada;

    string estadoInicial = "q0";

    // Crear y ejecutar la máquina de Turing
    MaquinaTuring mt(estadoInicial, transiciones, entrada);
    mt.ejecutar();
    mt.imprimirResultadoFinal();

    return 0;
}
