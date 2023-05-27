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

#include <locale.h>
using std::setlocale;

#include <random>
using std::random_device;
using std::mt19937;
using std::uniform_int_distribution;

#include <locale>
using std::setlocale;


#include <cctype>

#include <algorithm>
/**
 * Antes de empezar a leer este codigo es importante que se visite Guia.md y Enunciado.pdf para entender el porque de ciertas cosas.
 * Esta sera la unica vez que ambos documentos sera mencionado en lo que queda de codigo.
*/

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
 * @brief Verifica que el archivo de frases si se pueda abrir.
 * @return Si no se puede abrir retorna false. En caso contrario retorna true
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

        //Le pedimos al anfitrion la cantidad de jugadores que van a jugar
        cout << "Cuantos jugadores van a ingresar al juego?:\n";
        cin >> cantidadJugadores;
        cin.ignore();

        //En caso de que el anfitrion ingrese una cantidad menor a 1 se le pide que ingrese un valor mayor infitamente hasta que lo haga.
        while (cantidadJugadores < 1)
        {
            cout << "\nPor favor ingrese una cantidad de jugadores igual o  mayor a 1:\n";
            cin >> cantidadJugadores;
            cin.ignore();
        }

        //Creamos un arreglo dinamico en el que guardaremos los nombres de cada jugador
        string* nombresLista = new string[cantidadJugadores];

        /**
         * Iniciamos un for:
         * Cada vez que se ejecute va a preguntar el nombre de un jugador y en que posición jugara este.
         * Esto lo va a hacer uno por uno hasta que cubra tada la cantidad de jugadores.
        */
        for(int i = 0; i < cantidadJugadores; i++)
        {
            //Iniciamos dos variables vacias
            string nombre = " ";
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
                    cout << "\nIngrese una posicion correcta (Entre 1 y " << cantidadJugadores << ") :";
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
        cout << "\nPor favor ingrese la dificultad a la que van a jugar (Entre 1 a 3):\n1. Facil.\n2. Medio.\n3. Dificil.\n";
        cin >> dificultad;

        //Si el valor de dificultad no esta entre 1 y 3 se ejecuta el while para asegurarse de que se escriba un valor corrrecto.
        while (dificultad > 3 || dificultad < 1)
        {
            cout << "\nIngrese una dificultad validad (Entre 1 a 3): ";
            cin >> dificultad;
        }

        /**
         * Se crea un arreglo con 3 columnas que sera llenado en la función leerArchivo.
         * Cada espacio del arreglo sera llenado con la cantidad de frases de cada dificultad.
         * Ejemplo: cantidadFrases[0] = cantidad de frases de dificiultad facil.
        */
        int cantidadFrases[3] = {0, 0, 0};

        leerArchivo(cantidadFrases);

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

                int cantidadPalabras = frases[2] + '0';
                
                string fraseMinuscula = frases;

                /**
                 * Pasar todo a minusculas menos la primera letra. 
                 * Elimina el formato x|y| por lo que mueve la frase para atras desde la primera posición.
                 * Ej: frases = 1|3|DIOS ES AMOR : frases = Dios es amor.
                 * Al hacer esto tambien se modifica el tamaño directo del string
                */
                int contadorLetras = 0;
                for (int k = 4; k < frases.length(); k++)
                {
                    //Se deja la primera letra mayuscula
                    if (contadorLetras == 0){
                        frases[contadorLetras] = frases[k];
                        fraseMinuscula[contadorLetras] = tolower(fraseMinuscula[k]);
                        contadorLetras++;
                        continue;
                    }
                    
                    //Se deja el resto minuscula con la ayuda de tolower().
                    char actual = frases[k];
                    char actualMinuscula = fraseMinuscula[k];

                    frases[contadorLetras] = tolower(actual);
                    fraseMinuscula[contadorLetras] = tolower(actual);

                    contadorLetras++;
                    /**
                     * Antes de que se termino el ciclo for se eliminan los valores desde el final de la frase minuscula hasta el restante de las mayusculas:
                     * Ej: Sin esto la frase "Dios es amor" terminaria "Dios es amorAMOR" ya que no se elimina lo que queda de mayuculas
                    */
                    if (k + 1 == frases.length())
                    {
                        frases.erase(contadorLetras, frases.length());
                        fraseMinuscula.erase(contadorLetras, fraseMinuscula.length());
                    }       
                }
                
                //Se crea una string con un tamaño definido y completamente vacia.
                string fraseCompletar (frases.length(), ' ');
                //fraseCompletar se convierte a formato de ahorcados (ej: uno es bueno = ___ __ _____).
                for (int k = 0; k < frases.length(); k++)
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

                        cout << "\n" << fraseMinuscula;
                //Inicia el juego en si dando como maximo 10 turnos por jugador.
                for (int turnos = 0; turnos < 10; turnos++)
                {
                    //desicionTurno inicia en 0 y guarda la acción que decide hacer el jugador cada turno.
                    int desicionTurno = 0;
                    //Se imprime el estado actual de la frase a adivinar.
                    cout << "\n"<<  fraseCompletar;

                    cout << "\n-Ingrese 1 si quiere ingresar una letra.\n-Ingrese 2 si quiere ingresar una palabra.\n";
                    cin >> desicionTurno;
                    cin.ignore();

                    //En caso de que el jugador haya elejido un valor distinto a 1 y 2 se ejecuta un while que le pide un valor correcto.
                    while (desicionTurno != 1 && desicionTurno != 2)
                    {
                        cout << "\nElija un valor entre 1 y 2.\n-Ingrese 1 si quiere ingresar una letra.\n-Ingrese 2 si quiere ingresar una palabra.\n";
                        cin >> desicionTurno;
                        cin.ignore();
                    }
                    //En caso de querer ingresar una unica letra:
                    if (desicionTurno == 1)
                    {
                        //Variable donde se guarda la letra
                        char letraActual = ' ';

                        cout << "\nIngrese una letra: ";
                        cin >> letraActual;

                        //Booleano que nos ayuda a determinar si la letra estaba en la oración
                        bool verificar = false ;

                        //Verifica que la letra este en la frase y de ser asi la pone en todas las posiciones donde se encuentra
                        for (int k = 0; k < fraseCompletar.length(); k++)
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
                        string palabra = " ";

                        cout << "\nIngrese una palabra: ";
                        getline(cin, palabra);

                        bool verificar = false;

                        int palabraActualPosicion = 0;

                        for (int k = 0; k < frases.length(); k++)
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
                            if (k + 1 == frases.length())
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
                    if (fraseCompletar == frases)
                    {
                        cout << "\nCompleto la frase!. Sigue el siguiente jugador.";
                        break;
                    }
                }
            }
        }
    
        

        //Eliminando las variable usadas en la memoria heap.
        for (int i = 0; i < cantidadJugadores; i++)
        {
            delete[] frasesPersonas[i];
        }

        delete[] frasesPersonas;

        delete[] nombresLista;
    }
    return 0;
}