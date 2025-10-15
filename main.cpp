#include <iostream>
using namespace std;

const int MAX_FILAS = 20;
const int MAX_COLS = 20;
const double EPSILON = 1e-10;

class Simplex
{
private:
    double tabla[MAX_FILAS][MAX_COLS];
    int filas, cols;
    char nombresColumnas[MAX_COLS][100];  // Para guardar nombres de variables (x1, x2, etc.)
    char nombresFilas[MAX_FILAS][100];    // Para guardar nombres de variables básicas

    void imprimirTabla()
    {
        cout << "\n--- Tabla Simplex ---\n";
        cout << "        ";
        for (int j = 0; j < cols; j++)
        {
            cout.width(12);
            cout << nombresColumnas[j] << " ";
        }
        cout << "\n";

        for (int i = 0; i < filas; i++)
        {
            cout.width(6);
            cout << nombresFilas[i] << "  ";
            for (int j = 0; j < cols; j++)
            {
                cout.width(12);
                cout << tabla[i][j] << " ";
            }
            cout << "\n";
        }
        cout << "\n";
    }

    int encontrarColumnaEntrante()
    {
        int col = -1;
        double valorMin = 0;
        for (int j = 0; j < cols - 1; j++)
        {
            if (tabla[0][j] < valorMin)
            {
                valorMin = tabla[0][j];
                col = j;
            }
        }
        return col;
    }

    int encontrarFilaSaliente(int colEntrante)
    {
        int fila = -1;
        double razonMin = 1e9;

        for (int i = 1; i < filas; i++)
        {
            if (tabla[i][colEntrante] > EPSILON)
            {
                double razon = tabla[i][cols - 1] / tabla[i][colEntrante];
                if (razon >= 0 && razon < razonMin)
                {
                    razonMin = razon;
                    fila = i;
                }
            }
        }
        return fila;
    }

    void copiarString(char destino[], const char origen[])
    {
        int i = 0;
        while (origen[i] != '\0' && i < 9)
        {
            destino[i] = origen[i];
            i++;
        }
        destino[i] = '\0';
    }

    void pivotear(int filaPivote, int colPivote)
    {
        cout << "\n-------------------- OPERACIONES REALIZADAS -----------------------------------------\n\n";

        double tablaAnterior[MAX_FILAS][MAX_COLS];
        for (int i = 0; i < filas; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                tablaAnterior[i][j] = tabla[i][j];
            }
        }

        double pivote = tabla[filaPivote][colPivote];
        copiarString(nombresFilas[filaPivote], nombresColumnas[colPivote]);
        cout << "Paso 1: Normalizar fila pivote F" << filaPivote << " (dividir entre " << pivote << ")\n";
        cout << "F" << filaPivote << " -> F" << filaPivote << " / (" << pivote << ")\n";

        for (int j = 0; j < cols; j++)
        {
            tabla[filaPivote][j] /= pivote;
        }
        cout << "Resultado: ";
        for (int j = 0; j < cols; j++)
        {
            cout << tabla[filaPivote][j];
            if (j < cols - 1) cout << "  ";
        }
        cout << "\n\n";
        cout << "Paso 2: Hacer ceros en la columna " << nombresColumnas[colPivote] << "\n\n";

        for (int i = 0; i < filas; i++)
        {
            if (i != filaPivote)
            {
                double factor = tablaAnterior[i][colPivote];

                if (factor > EPSILON || factor < -EPSILON)
                {
                    cout << "F" << i << " -> F" << i;

                    if (factor >= 0)
                        cout << " + (" << -factor << ") * F" << filaPivote << "\n";
                    else
                        cout << " + (" << -factor << ") * F" << filaPivote << "\n";

                    cout << "   = ";
                    for (int j = 0; j < cols; j++)
                    {
                        double valorAnterior = tablaAnterior[i][j];
                        double valorPivote = tabla[filaPivote][j];
                        double resultado = valorAnterior - factor * valorPivote;

                        if (j > 0) cout << " , ";
                        cout << valorAnterior << " + (" << -factor << ")(" << valorPivote << ")";
                    }
                    cout << "\n";

                    cout << "   = ";
                    for (int j = 0; j < cols; j++)
                    {
                        tabla[i][j] = tablaAnterior[i][j] - factor * tabla[filaPivote][j];
                        if (j > 0) cout << " , ";
                        cout << tabla[i][j];
                    }
                    cout << "\n\n";
                }
            }
        }
    }

    bool esOptimo()
    {
        for (int j = 0; j < cols - 1; j++)
        {
            if (tabla[0][j] < -EPSILON)
            {
                return false;
            }
        }
        return true;
    }

public:
    Simplex() : filas(0), cols(0) {}

    void leerTabla()
    {
        cout << "------- ERICK ABEL ARIZACA MACHACA ----------------------------------------\n\n";
        cout << "Numero de filas (incluyendo fila Z): ";
        cin >> filas;
        cout << "Numero de columnas (incluyendo b): ";
        cin >> cols;

        cout << "\nIngrese los nombres de las columnas (ej: x1 x2 x3 x4 b):\n";
        for (int j = 0; j < cols; j++)
        {
            cin >> nombresColumnas[j];
        }

        cout << "\nIngrese la tabla simplex completa:\n";

        for (int i = 0; i < filas; i++)
        {
            if (i == 0)
            {
                cout << "Fila Z (ingrese nombre y valores, ej: z -40 -25 0 0 0):\n";
            }
            else
            {
                cout << "Restriccion " << i << " (ingrese nombre y valores, ej: x3 4 3 1 0 20):\n";
            }
            cin >> nombresFilas[i];

            for (int j = 0; j < cols; j++)
            {
                cin >> tabla[i][j];
            }
        }
    }

    void resolver()
    {
        cout << "\n -------------- SOLUCION -------------------------------\n";
        cout << "\nTabla inicial:";
        imprimirTabla();

        int iteracion = 0;

        while (!esOptimo())
        {
            iteracion++;

            int colEntrante = encontrarColumnaEntrante();
            if (colEntrante == -1)
            {
                cout << "Solucion optima alcanzada.\n";
                break;
            }

            int filaSaliente = encontrarFilaSaliente(colEntrante);
            if (filaSaliente == -1)
            {
                cout << "Problema no acotado (sin solucion finita).\n";
                return;
            }

            cout << "\nIteracion " << iteracion << ":";
            cout << "\nColumna entrante: " << nombresColumnas[colEntrante];
            cout << "\nFila saliente: " << nombresFilas[filaSaliente];
            cout << "\nElemento pivote: " << tabla[filaSaliente][colEntrante] << "\n";

            pivotear(filaSaliente, colEntrante);
            imprimirTabla();
        }

        mostrarSolucion();
    }

    void mostrarSolucion()
    {
        cout << "\n ----------------------- SOLUCION FINAL ---------------\n";
        cout << "Valor Z* = " << tabla[0][cols - 1] << "\n\n";
        cout << "Variables basicas:\n";

        for (int i = 1; i < filas; i++)
        {
            cout << nombresFilas[i] << "* = " << tabla[i][cols - 1] << "\n";
        }
    }
};

int main()
{
    //freopen("entrada.txt","r",stdin);
    Simplex simplex;
    simplex.leerTabla();
    simplex.resolver();

    return 0;
}
