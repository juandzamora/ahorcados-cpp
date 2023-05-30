//Juan David Zamora Guarin 25/04/2023
//Archivo principal del proyecto final 2023

//Librerias y documentos
#include <iostream>
using std::cout;
using std::cin;

#include <cctype>
using std::isspace;

#include <fstream>
using std::ifstream;

#include <string>
using std::string;
using std::getline;
using std::to_string;

#include <locale.h>
using std::setlocale;

#include <random>
using std::random_device;
using std::mt19937;
using std::uniform_int_distribution;

#include <locale>
using std::setlocale;

#include <iomanip>
using std::setw;
using std::setfill;

/**
 * Antes de empezar a leer este codigo es importante que se visite Guia.md y Enunciado.pdf para entender el porque de ciertas cosas.
 * Esta sera la unica vez que ambos documentos sera mencionado en lo que queda de codigo.
*/

//Declaración de estructuras:

/**
 * @brief personaPuntaje almacena el nombre d euna persona junto con su puntaje.
 * @param nombrePersona[string] Nombre de la persona.
 * @param puntaje[int] Puntjae de la persona.
*/
struct personaPuntaje
{
    string nombrePersona;
    int puntaje;
};

//Declaración de funciones:

/**
 * @brief para conseguir el nombre del archivo.
*/
string nombreArchivo()
{
    return "frases.dat";
}

/**
 * @brief Funcion para leer el archivo y guardar la cantidad de frases por dificultad. Son solo 3 posibles dificultades.
 * @param cantidadDificultad[int] Arreglo traido desde main. En cada uno de sus 3 espacios se guardan la cantidad de frases de una dificultad diferente.
 * @return Un arreglo con la cantidad de frases por dificultad.
*/
void leerArchivo(int cantidadDificultad[3])
{
    ifstream abrirArchivo(nombreArchivo());

    for (int i = 0; i < 3; i++)
    {
        //Creamos una string vacia donde se guardan las frases una a una
        string frase = " ";
        
        //Convertimos la dificultad de la frase a un entero, y ese numero lo usamos para saber que elemento del arreglo sumamos. 
        while (getline(abrirArchivo, frase))
        {
            cantidadDificultad[frase[0]-'0'-1] += 1; 
        }
    }       
}

/**
 * @brief Función para generar un número random del 1 al número dado por el jugador.
 * @param CantidadMaxima[int] Es el número mayor que la función va a usar para generar el número random.
 * @return Retorna el número random generado.
*/
int generarRandom(int CantidadMaxima)
{
    random_device RandomDevice;
    mt19937 Generar(RandomDevice());
    uniform_int_distribution<> distr(1, CantidadMaxima);

    return distr(Generar);
}

/**
 * @brief Mueve hacia atras los datos de un arreglo de forma ordenada eliminando la posicion seleccionada.
 * @param frases[string] El arreglo con las frases a mover hacia atras.
 * @param posicionEliminar[int] El entero que contiene la posición a eliminar.
 * @param totalArreglo[int] El entero que contiene la cantidad de elementos que el arreglo tiene.
 * @return El arreglo modificado para su posterior uso.
*/
void moverAtras(string* frases, int posicionEliminar, int totalArreglo)
{
    //Creamos dos variables string vacias.
    string fraseActual = "";
    string fraseAnterior = "";

    if (posicionEliminar == totalArreglo)
    {
        frases[posicionEliminar] = "";
        return;
    }

    //El for se ejecuta hasta que que llega a la posición a eliminar.
    for (int i = totalArreglo; i != posicionEliminar; i--)
    {
        //Primero se guarda el valor actual de frases[i] en fraseActual.
        fraseActual = frases[i - 1];

        //Luego hacemos que fraseAnterior sea igual al valor de fraseAnterior.
        frases[i - 1] = fraseAnterior;

        //Finalmente hacemos que fraseAnterior sea igual a fraseActual (el valor de frases[i] inicial).
        fraseAnterior = fraseActual;

        //Si en la proxima iteración del bucle, i ya es igual a posición eliminar se ejecuta este if
        if (i == posicionEliminar)
        {
            //Hacemos que el valor de frases[posicionEliminar-1] sea igual a la ultima frase guardada
            frases[posicionEliminar] = fraseAnterior;
        }
    }
}
/**
 * @brief Selecciona todas las frases a jugar por cada jugador en cada turno
 * @param JugadoresFrases[string] Matriz que guardara las frases a adivinar por cada jugador (Por cada columna un jugador, las frases de este se guardan en las filas).
 * @param cantidadPersonas[int] Contiene la cantidad de jugadores actuales.
 * @param limiteFrases[int] Contiene el maximo de frases a adivinar por cada jugador.
 * @param cantidadFrases[int] Contiene la cantidad de frases que hay en la dificultad seleccionada.
 * @param dificultad[int]  Contiene la dificultad actual del juego.
 * @return El arreglo modificado con frases distintas para cada turno de cada jugador (no se repite ninguna frase). 
*/
void frasesDificultad(string** jugadoresFrases, int cantidadPersonas, int limiteFrases, int cantidadFrases, int dificultad)
{
    ifstream abrirArchivo(nombreArchivo());

    //Creamos un arreglo dinamico para guardar las frases de la dificultad seleccionada
    string* frasesDificultad = new string[cantidadFrases];

    //Creamos una string vacia donde se guardan las frases una a una
    string frase = " ";

    //Creamos un contador para poder insertar correctamente las frases en el arreglo
    int contadorFrases = 0;

    //Leer el archivo
    while (getline(abrirArchivo, frase))
    {
        //Una vez encuentre una frase de la dificultad la mete en el arreglo
        if (frase[0] == ('0' + dificultad))
        {
            frasesDificultad[contadorFrases] = frase;
            contadorFrases++;
        }
        //Una vez recoja todas las frases de la dificultad hacemos que el bucle se acabe para evitar explorar más lineas sin necesidad
        if (contadorFrases == cantidadFrases)
        {
            break;
        }
    }

    /**
     * Asignamos una frase distinta para cada jugador en cada turno.
     * cantidadPersonas corresponde a la cantidad de filas.
     * limiteFrases corresponde a la cantidad de columnas.
     * En este for anidado vamos explorando columna por columna.
    */
    for (int i = 0; i < cantidadPersonas; i++)
    {
        for (int j = 0; j < limiteFrases; j++)
        {
            /**
             * Generamos un número random tomando contadorFrases como número maximo, el resultado lo restamos con 1 para que pueda ser usado en el arreglo.
             * Ya que genera un numero del 1 al contadorFrase 
             * y mientras con el 1 como tope no se puede llegar al 0, con contadorFrase exploro un elemento que no existe.
            */
            int numeroAleatorio = generarRandom(contadorFrases) - 1;

            //Asignamos que el elemnto actual sea igual a la frase seleccionada de manera aleatoria.
            jugadoresFrases[i][j] = frasesDificultad[numeroAleatorio];

            //Eliminamos la frase recien escojida y re organizamos el arreglo usando la función moverAtras a no ser de es el objeto seleccionado sea el ultimo.
            moverAtras(frasesDificultad, numeroAleatorio, contadorFrases);

            //ContadorFrase se resta 1 a su propio valor. Este nos sirve de referencia para saber cuantas frase hay en verdad en el arreglo. 
            contadorFrases--;
        }
    }
    //Antes de cerrar la función eliminamos el arreglo dinamico "frasesDificultad", ya que de otro modo podria generarnos errores.
    delete[] frasesDificultad;
}

/**
 * @brief Consigue la cantidad de digitos de un determinado numero.
 * @param[int] el númeor a evaluar.
 * @brief La cantidad de digitos del número dado.
*/
int cantidadDigitos (int numero)
{
    int resultado = 0;

    int valor = numero;
    for (int i = 0; valor != 0; i++)
    {
        valor = valor / 10;

        if (valor == 0)
        {
            resultado = i;
            break;
        }
    }
    return resultado;
}

/**
 * @brief Verifica que el archivo de frases si se pueda abrir.
 * @return Si no se puede abrir retorna false. En caso contrario retorna true.
*/
bool errorArchivo()
{
    ifstream ArchivoTemp(nombreArchivo());

    //Verificica si no se abrio el archivo.
    if(!ArchivoTemp.is_open())
    {
        return false;
    }
    return true;
}


/*Incio función main*/
int main()
{
    //Tildes y ñ's
    setlocale(LC_ALL, "es_ES.UTF-8");

    //Verifica si el archivo de frases puede ser abierto, de no ser asi el programa se termina.
    if(errorArchivo() == false)
    {
        char AcabarPrograma = ' ';
        cout << "\nSucedio un error, verifique el nombre del archivo o la existencia de este (Nombre actual: " << nombreArchivo() << ").";
        cout << "\nIngrese una tecla para terminar el programa: ";
        cin >> AcabarPrograma;
        return 0;
    }

    //Creamos una variable char salir para lograr un bucle while infinito hasta que el anfitrion decida que es suficiente
    char Salir = 'n';
    while(Salir != 'y')
    {
        int cantidadJugadores = 0;

        /**
         * Se crea un arreglo con 3 columnas que sera llenado en la función leerArchivo.
         * Cada espacio del arreglo sera llenado con la cantidad de frases de cada dificultad.
         * Ejemplo: cantidadFrases[0] = cantidad de frases de dificiultad facil.
        */
        int cantidadFrases[3] = {0, 0, 0};

        leerArchivo(cantidadFrases);

        int mayorFrases = 0;
        for (int i = 0; i <3; i++)
        {
            if (cantidadFrases[i] > mayorFrases)
            {
                mayorFrases = cantidadFrases[i];
            }
        }
        //Le pedimos al anfitrion la cantidad de jugadores que van a jugar
        cout << "Cuantos jugadores van a ingresar al juego?:\n";
        cin >> cantidadJugadores;
        cin.ignore();

        //Verificar que la cantidad de personas si sean suficientes 
        //En caso de que el anfitrion ingrese una cantidad menor a 1 se le pide que ingrese un valor mayor infitamente hasta que lo haga.
        while (cantidadJugadores < 1 || cantidadJugadores > mayorFrases)
        {
            cout << "\nPor favor ingrese una cantidad de jugadores entre 1 y "<< mayorFrases <<":\n";
            cin >> cantidadJugadores;
            cin.ignore();
        }

        //Creamos un arreglo dinamico en el que guardaremos los nombres de cada jugador
        string* nombresLista = new string[cantidadJugadores];
        int* cantidadPuntos = new int[cantidadJugadores];

        /**
         * Iniciamos un for:
         * Cada vez que se ejecute va a preguntar el nombre de un jugador y en que posición jugara este.
         * Esto lo va a hacer uno por uno hasta que cubra tada la cantidad de jugadores.
        */
        for(int i = 0; i < cantidadJugadores; i++)
        {
            //Iniciamos dos variables vacias
            string nombre;
            int posicion = 0;

            //Le pedimos que ingrese el nombre del jugador
            cout << "\nIngrese el nombre del jugador: ";
            getline(cin, nombre);

            //Le pedimos que ingrese en que posición va a participar el jugador
            cout << "\nIngrese en que posicion participara el jugador: ";
            cin >> posicion;
            cin.ignore();
            
            //Verifica que el valor dado en la variable posicion este ubicado entre 1 a cantidadJugadores
            while(posicion > cantidadJugadores || posicion < 1)
            {
                cout << "\nIngrese una posicion correcta (Entre 1 y "<<cantidadJugadores<<") :";
                cin >> posicion;
                cin.ignore();
            }

            /**
             * Iniciamos un ciclo while:
             * Este se activara si la posición escrita por le anfitrion ya esta ocupada dandole una lista de los puestos vacios.
             * Posteriormente vuelve a revisar si el valor si entra en el rango de 1 a cantidadJugadores.
            */
            while(nombresLista[posicion-1].length() != 0)
            {
                //Imprime la lista de puestos vacia.
                cout << "\nEsta posicion esta llena, por favor ingrese una posicion vacia.\nLa lista de puestos vacios es esta: ";

                for (int j = 0; j < cantidadJugadores; j++)
                {
                    if (nombresLista[j].length() == 0)
                    {
                        cout << j + 1 << ", ";
                    }
                }
                cout << "ingrese cualquiera de esas posiciones: ";
                cin >> posicion;
                cin.ignore();

                //Verifica que el valor sea valido.
                while(posicion>cantidadJugadores || posicion<1)
                {
                    cout << "\nIngrese una posicion correcta (Entre 1 y " << cantidadJugadores << "): ";
                    cin >> posicion;
                    cin.ignore();
                }
            }
            //una vez todos los valores son validos ingresa el nombre en la posición indicada.
            nombresLista[posicion-1] = nombre;
        }

        //Segundo punto del taller.

        //Definimos dificultad en 0.
        int dificultad = 0;

        //Le preguntamos la dificultad que se quiera al anfitrion.
        cout << "\n\nPor favor ingrese la dificultad a la que van a jugar (Entre 1 a 3):\n1. Facil.\n2. Medio.\n3. Dificil.\nDificultad: ";
        cin >> dificultad;

        //Si el valor de dificultad no esta entre 1 y 3 se ejecuta el while para asegurarse de que se escriba un valor corrrecto.
        while (dificultad > 3 || dificultad < 1 || cantidadFrases[dificultad - 1] < cantidadJugadores)
        {
            if (dificultad > 3 || dificultad < 1 )
            {
                cout << "\nIngrese una dificultad validad (Entre 1 a 3): ";
                cin >> dificultad;
            }
            else if (cantidadFrases[dificultad - 1] < cantidadJugadores)
            {
                cout << "La dificultad elegida tiene menos palabras que la cantidad de jugadores.\nSi quiere jugar elija alguna de estas dificultades:"
                        "\nDificultades posibles: ";
                for (int i = 0; i < 3; i++)
                {
                    if (cantidadFrases[i] >= cantidadJugadores)
                    {
                        cout << i + 1 << ", ";
                    }
                }
                cout << " por favor escoja una de estas.\nDificultad: ";
                cin >> dificultad;
            }
        }

        //Se crea una variable vacia para declarar el limite.
        int limite = 0;

        //Se le pregunta al anfitrion por el limite .
        cout << "\nIngrese la cantidad de frases a adivinar por cada jugador: ";
        cin >> limite;

        /**
         * Se incia un while:
         * Este solo se ejecutara por 2 motivos: 
         * 1. Si limite es menor a 1.
         * 2. Si limite es menor a la división entre la cantidad de frases de la dificultad seleccionada y la cantidad de jugadores 
         *    (El maximo de frases posible por jugador de esa dificultad).
         *  
         * Esto se hace con el objetivo de que no se le pida al programa más frases de las que hay.
         * Ejemplo: Hay 3 jugadores, piden 3 frases por jugador de dificultad media pero solo hay 7 frases de esa dificultad. 
         * Si se deja prodecer el programa fallaria eventualmente, por lo que es necesario este ciclo while.
        */
        while (limite > (cantidadFrases[dificultad-1] / cantidadJugadores) || limite < 1)
        {
            cout << "\nPor favor ingrese una cantidad de frases más pequeña (entre 1 y " << cantidadFrases[dificultad-1] / cantidadJugadores << "): ";
            cin >> limite;
        }

        /**
         * Creamos una matriz con tamaño dinamico para guardar las frases de cada turno de cada jugador donde:
         * cantidadJugadores va a ser igual a la cantidad de filas.
         * limite va a ser igual a la cantidad de columnas.
         * De ese modo cada persona va a tener una columnas para sus frases.
        */
        string** frasesPersonas = new string*[cantidadJugadores];

        //Insertamos las columnas en cada una de las filas.
        for (int i = 0; i < cantidadJugadores; i++)
        {
            frasesPersonas[i] = new string[limite];
        }
        
        /**
         * Llamamos la función frasesDificultad que se va a encargar de dejarnos las frases de cada jugador en cada turno
         * y encargarse de que estas no se repitan ninguna vez.
         * Esta lo hace sobreescribiendo en la matriz frasesPersonas.
        */
        frasesDificultad(frasesPersonas, cantidadJugadores, limite, cantidadFrases[dificultad-1], dificultad);

        int puntosDificultad[3] = {110, 210, 310};
        /**
         * Se inicia un bucle for anidado:
         * El primer bucle explora los elementos fila por fila. 
         * El segundo bucle explora los elementos columna por columna.
         * i = filas.
         * j = columas.
         * De modo tal que por cada iteración de i el programa va a mostrar todas las columnas de esa fila antes de continuar con la siguiente.
        */
        for (int i = 0; i < limite; i++)
        {
            for (int j = 0; j < cantidadJugadores; j++)
            {
                //Se consigue la frase del turno actual.
                string frases = frasesPersonas[j][i];

                string fraseSinModificar = frasesPersonas[j][i];

                int cantidadPalabras = fraseSinModificar[2] - '0';
                if (cantidadPalabras == 1)
                {
                    if (fraseSinModificar[3] == '0')
                    {
                        cantidadPalabras = 10;
                    }
                }

                string fraseMinuscula = frases;

                /**
                 * Pasar todo a minusculas menos la primera letra. 
                 * Elimina el formato x|y| por lo que mueve la frase para atras desde la primera posición.
                 * Ej: frases = 1|3|DIOS ES AMOR : frases = Dios es amor.
                 * Al hacer esto tambien se modifica el tamaño directo del string
                */
                int contadorLetras = 0;

                int determinar = 4;

                if(cantidadPalabras == 10)
                {
                    determinar = 5;
                }

                for (; determinar < static_cast<int>(frases.length()); determinar++)
                {
                    //Se deja la primera letra mayuscula
                    if (contadorLetras == 0)
                    {
                        frases[contadorLetras] = frases[determinar];
                        fraseMinuscula[contadorLetras] = tolower(fraseMinuscula[determinar]);
                        contadorLetras++;
                        continue;
                    }
                    
                    //Se deja el resto minuscula con la ayuda de tolower().
                    char actual = frases[determinar];

                    frases[contadorLetras] = tolower(actual);
                    fraseMinuscula[contadorLetras] = tolower(actual);

                    contadorLetras++;

                    /**
                     * Antes de que se termino el ciclo for se eliminan los valores desde el final de la frase minuscula hasta el restante de las mayusculas:
                     * Ej: Sin esto la frase "Dios es amor" terminaria "Dios es amorAMOR" ya que no se elimina lo que queda de mayuculas
                    */
                    if (determinar + 1 == static_cast<int>(frases.length()))
                    {
                        frases.erase(contadorLetras, frases.length() - contadorLetras);
                        fraseMinuscula.erase(contadorLetras, fraseMinuscula.length() - contadorLetras);
                    }       
                }

                //Se crea una string con un tamaño definido y completamente vacia.
                string fraseCompletar (frases.length(), ' ');
                //fraseCompletar se convierte a formato de ahorcados (ej: uno es bueno = ___ __ _____).
                for (int k = 0; k < static_cast<int>(frases.length()); k++)
                {
                    if (frases[k] == ' ')
                    {
                        fraseCompletar[k] = ' ';
                    }
                    else
                    {
                        fraseCompletar[k] = '_';
                    }
                }

                if (i == 0)
                {
                    cantidadPuntos[j] = 0;
                }

                int* espacios = new int[cantidadPalabras];

                int contador = 0;
                //Encontrar la posición de los espacios entre palabras.
                for (int k = 0; k < static_cast<int>(frases.length()); k++)
                {      
                    if (frases[k] == ' ')
                    {
                        espacios[contador] = k;
                        contador++;
                    }
                }
                espacios[contador + 1] = frases.length() + 1;
                contador++;

                int cantidadIntentosLetras = 0;
                int cantidadIntentosPalabras = 0;
                int cantidadIntentosFrases = 0;

                int cantidadDigitosPuntos = cantidadDigitos(cantidadPuntos[j]);
                int cantidadDigitosLetras = cantidadDigitos(fraseCompletar.length() - cantidadPalabras);
                int cantidadDigitosPalabras = cantidadDigitos(cantidadPalabras);
                

                int contadorCaracteresStatus = 0;
                string palabrasStatus;
                palabrasStatus.reserve(80);
                palabrasStatus = "\n|  Letra: ";
                for (int k = 0; k < cantidadPalabras; k++)
                {
                    if (k == 0)
                    {
                        string texto = to_string(k + 1) + "-" + to_string(espacios[k]);
                        contadorCaracteresStatus += texto.length();
                        palabrasStatus += texto;
                    }
                    else if (k + 1 == contador)
                    {
                        string texto =  to_string(k + 1) + "-" + to_string(fraseCompletar.length() - espacios[k-1] - 1);
                        contadorCaracteresStatus += texto.length();
                        palabrasStatus += texto;
                    }
                    else
                    {
                        string texto = to_string(k + 1) + "-" + to_string(espacios[k] - espacios[k-1] - 1);
                        contadorCaracteresStatus += texto.length();
                        palabrasStatus += texto;   
                    }

                    if (k + 1 == contador)
                    {
                        palabrasStatus += ".";
                        contadorCaracteresStatus += 1;
                    }
                    else
                    {
                        palabrasStatus += ", ";
                        contadorCaracteresStatus += 2;
                    }
                }

                delete[] espacios;

                //Inicia el juego en si dando como maximo 10 turnos por jugador.
                for (int turnos = 0; turnos < 10; turnos++)
                {
                    //desicionTurno inicia en 0 y guarda la acción que decide hacer el jugador cada turno.
                    int desicionTurno = 0;


                    //Se imprime el estado actual de la frase a adivinar.
                    cout << "\n--------------------------------------------------------------"
                         << "\n|  Su frase contiene " << cantidadPalabras << " palabras y " << fraseCompletar.length() - cantidadPalabras << " letras." << setw(21 - cantidadDigitosLetras - cantidadDigitosPalabras - 2) << "|";
                    cout << palabrasStatus << setw(63 - palabrasStatus.length()) << "|";
                    cout << "\n|" << setw(61) << "|"
                         << "\n|  "<<  fraseCompletar << setw(59 - fraseCompletar.length()) << "|"
                         << "\n|------------------------------------------------------------|"
                         << "\n|" << setw(61) << "|"
                         << "\n|  -Ingrese 1 si quiere ingresar una letra (" << cantidadIntentosLetras << ")." << setw(15) << "|"
                         << "\n|  -Ingrese 2 si quiere ingresar una palabra(" << cantidadIntentosPalabras << ")." << setw(14) << "|"
                         << "\n|  -Ingrese 3 si quiere intentar la frase completa (" << cantidadIntentosFrases << ")." << setw(7) << "|"
                         << "\n|" << setw(61) << "|"
                         << "\n|  Turnos jugados: " << turnos << setw(42) << "|"
                         << "\n|  Puntos obtenidos: " << cantidadPuntos[j] << setw(41 - cantidadDigitosPuntos - 1) << "|"
                         << "\n--------------------------------------------------------------"
                         << "\n\nEscoja una: ";

                    cin >> desicionTurno;
                    cin.ignore();

                    //En caso de que el jugador haya elejido un valor distinto a 1 y 2 se ejecuta un while que le pide un valor correcto.
                    while (desicionTurno < 0 || desicionTurno > 3)
                    {
                        cout << "\n--------------------------------------------------------------"
                             << "\n|  Incorrecto! Elija un valor entre 1 y 3." << setw(20) << "|"
                             << "\n|------------------------------------------------------------|"
                             << "\n|  -Ingrese 1 si quiere ingresar una letra (" << cantidadIntentosLetras << ")." << setw(15) << "|"
                             << "\n|  -Ingrese 2 si quiere ingresar una palabra(" << cantidadIntentosPalabras << ")." << setw(14) << "|"
                             << "\n|  -Ingrese 3 si quiere intentar la frase completa (" << cantidadIntentosFrases << ")." << setw(7) << "|"
                             << "\n--------------------------------------------------------------\n\n";
                        cin >> desicionTurno;
                        cin.ignore();
                    }
                    //En caso de querer ingresar una unica letra:
                    if (desicionTurno == 1)
                    {
                        //Se suma un intento de letras
                        cantidadIntentosLetras++;

                        //Variable donde se guarda la letra
                        char letraActual = ' ';

                        cout << "\nIngrese una letra: ";
                        cin >> letraActual;

                        //Booleano que nos ayuda a determinar si la letra estaba en la oración
                        bool verificar = false ;

                        //Verifica que la letra este en la frase y de ser asi la pone en todas las posiciones donde se encuentra
                        for (int k = 0; k < static_cast<int>(fraseCompletar.length()); k++)
                        {
                            if (fraseMinuscula[k] == letraActual && fraseCompletar[k] != letraActual)
                            {
                                if (k == 0)
                                {
                                    fraseCompletar[k] = frases[0];  
                                }

                                fraseCompletar[k] = letraActual;
                                verificar = true;
                            }
                        }
                        if(!verificar)
                        {
                            cout << "\nLa letra no fue encontrada en la oración o ya habia sido puesta";
                        }
                    }

                    //Detectar palabra
                    if (desicionTurno == 2)
                    {
                        cantidadIntentosPalabras++;

                        string palabra;

                        cout << "\nIngrese una palabra: ";
                        getline(cin, palabra);

                        bool verificar = false;

                        int palabraActualPosicion = 0;

                        for (int k = 0; k < static_cast<int>(fraseCompletar.length()); k++)
                        {
                            
                            string verificarPalabra  = fraseMinuscula; 
                            if (verificarPalabra[k] == ' ')
                            {
                                if (palabraActualPosicion!=  0)
                                {           
                                    int meVale = palabraActualPosicion + 1;
                                    for (int l = 0; l < k - palabraActualPosicion - 1; l ++)
                                    {
                                        verificarPalabra[l] = verificarPalabra[meVale];
                                        meVale++;
                                    }
                                    verificarPalabra.erase(k - palabraActualPosicion - 1, verificarPalabra.length());
                                }
                                else
                                {
                                    verificarPalabra.erase(k, verificarPalabra.length());
                                }

                                if (verificarPalabra == palabra)
                                {
                                    if (palabraActualPosicion == 0)
                                    {
                                        for (int l = 0; l < k; l++)
                                        {
                                            fraseCompletar[l] = frases[l];
                                        }
                                    }
                                    else
                                    {
                                        int guiaLetraCambiar = palabraActualPosicion + 1;
                                        for (int l = 0; l < k - palabraActualPosicion - 1; l++)
                                        {
                                            fraseCompletar[guiaLetraCambiar] = palabra[l];
                                            guiaLetraCambiar++;
                                        }
                                    }
                                    verificar = true;
                                }
                                palabraActualPosicion = k;
                            }
                            if (k + 1 == static_cast<int>(frases.length()))
                            {    
                                int meVale = palabraActualPosicion + 1;
                                for (int l = 0; l < k - palabraActualPosicion + 1; l ++)
                                {
                                    verificarPalabra[l] = verificarPalabra[meVale];
                                    meVale++;
                                }
                                verificarPalabra.erase(k - palabraActualPosicion, verificarPalabra.length());
                                if (verificarPalabra == palabra)
                                {
                                    int guiaLetraCambiar = palabraActualPosicion + 1;

                                    for (int l = 0; l < k - palabraActualPosicion + 1; l++)
                                    {
                                        fraseCompletar[guiaLetraCambiar] = palabra[l];
                                        guiaLetraCambiar++;
                                    }
                                    verificar = true;
                                }
                            }
                        }
                        if (!verificar)
                        {
                            cout << "\nLa palabra ingresada no se encuentra en la frase";
                        }
                    }

                    //Para adivinar la frase completa
                    if(desicionTurno == 3)
                    {
                        string frase;
                        cout << "\nIngrese la frase: ";
                        getline (cin, frase);

                        if (frase == fraseMinuscula || frase == frases)
                        {
                            fraseCompletar = frases;
                            cout << "\nFrase correcta!";
                        }
                        else
                        {
                            cout << "\nFrase incorrecta.";
                        }
                        cantidadIntentosFrases++;
                    }
                    if (fraseCompletar == frases)
                    {
                        cout << "\n\nCompleto la frase!. Sigue el siguiente jugador.";
                        cantidadPuntos[j] += puntosDificultad[dificultad - 1] - cantidadIntentosLetras;
                        break;
                    }
                    if (turnos + 1 == 10)
                    {
                        cout << "\n\nNo logro adivinar la frase, pasando al siguiente jugador.";
                    }
                }
            }
        }

        personaPuntaje* puntajes = new personaPuntaje[cantidadJugadores];

        for (int i = 0; i < cantidadJugadores; i++)
        {
            puntajes[i].nombrePersona = nombresLista[i];
            puntajes[i].puntaje = cantidadPuntos[i];
        }
        
        int ganador = 0;

        for (int i = cantidadJugadores; i >= 0; i--)
        {
            int actual = puntajes[i].puntaje;

            if (actual > ganador)
            {
                ganador = i;
            }
        }

        cout << "\n\nGanador!\n\nNombre: " << nombresLista[ganador] << "\nPuntos: " << cantidadPuntos[ganador] << "\n";

        cout << "\nQuiere cabar el programa? y para si cualquier tecla para no: ";
        cin >> Salir;

        cout << "\n\n";

        //Eliminando las variable usadas en la memoria heap.
        for (int i = 0; i < cantidadJugadores; i++)
        {
            delete[] frasesPersonas[i];
        }

        delete[] cantidadPuntos;

        delete[] frasesPersonas;

        delete[] nombresLista;

        delete[] puntajes;
    }
    return 0;
}