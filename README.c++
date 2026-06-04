//Estructura_de_datos
//Sistema de gestion de un restaurante
#include <iostream>
#include <string>
using namespace std;

//================ VALIDACIONES ================

bool esNumeroEnteroPositivo(string texto) {
    if (texto.empty()) return false;

    for (int i = 0; i < texto.length(); i++) {
        if (texto[i] < '0' || texto[i] > '9')
            return false;
    }

    return true;
}

bool esSoloLetrasYEspacios(string texto) {
    if (texto.empty()) return false;

    bool tieneLetra = false;

    for (int i = 0; i < texto.length(); i++) {
        char c = texto[i];

        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
            tieneLetra = true;
        }
        else if (c == ' ') {
            continue;
        }
        else {
            return false;
        }
    }

    return tieneLetra;
}

int pedirEntero() {
    string entrada;

    while (true) {
        getline(cin, entrada);

        if (!esNumeroEnteroPositivo(entrada)) {
            cout << "Opcion invalida. Intente nuevamente: ";
            continue;
        }

        if (entrada.length() > 10) {
            cout << "Opcion invalida. Intente nuevamente: ";
            continue;
        }

        int valor = convertirEntero(entrada);

        if (valor == -1) {
            cout << "Opcion invalida. Intente nuevamente: ";
            continue;
        }

        return valor;
    }
}

int pedirEnteroRango(int minimo, int maximo) {
    while (true) {
        int valor = pedirEntero();

        if (valor < minimo || valor > maximo) {
            cout << "Opcion invalida. Intente nuevamente: ";
            continue;
        }

        return valor;
    }
}

int convertirEntero(string texto) {
    long long numero = 0;

    for (int i = 0; i < texto.length(); i++) {
        numero = numero * 10 + (texto[i] - '0');

        if (numero > 2147483647)
            return -1;
    }

    return (int)numero;
}

string pedirPlato() {
    string plato;

    while (true) {
        getline(cin, plato);

        if (!esSoloLetrasYEspacios(plato)) {
            cout << "Opcion invalida. Intente nuevamente: ";
            continue;
        }

        return plato;
    }
}

//Estructura del pedido
struct pedido {
  int id ;
  int mesa;
  string descripcion;
  string estado;
  Pedido* siguiente;

  Pedido( int id, int mesa, )
//Declaracion de Modulos 
void crearPedido(int mesa, string plato);
void menuGestionar();
void menuVerPedidos();
void mostrarListaPedidos();
void buscarPorMesa(int mesa);
void tomarPedido();
void marcarPedidoEntregado();
void eliminarPedidoPorMesa();
void verFrenteCola();
void mostrarCola();
void apilarPedidoEntregado(Pedido* pedido);
void desapilarHistorial();
void verCimaPila();
void verHistorial();
void liberarMemoria();
void cargarDatosDesdeArchivo();
void guardarDatosEnArchivo();


//=========== Funciones Auxiliares ========================
Pedido* crearNodoPedido(int id, int mesa, string plato, string estado) {
    Pedido* nuevo = new Pedido;

    nuevo->id = id;
    nuevo->mesa = mesa;
    nuevo->plato = plato;
    nuevo->estado = estado;
    nuevo->siguiente = nullptr;

    return nuevo;
}

void agregarNodoAlFinal(Pedido*& inicio, Pedido* nuevo) {
    // Funcion nueva: se usa para cargar nodos desde el archivo y mantener
    // el mismo orden en que estaban guardados en la lista o en la pila.
    if (inicio == nullptr) {
        inicio = nuevo;
        return;
    }

    Pedido* aux = inicio;

    while (aux->siguiente != nullptr)
        aux = aux->siguiente;

    aux->siguiente = nuevo;
}

int contarNodos(Pedido* inicio) {
    // Funcion nueva: permite saber cuantos nodos se van a guardar.
    // Este numero ayuda a leer despues exactamente la misma cantidad de lineas.
    int cantidad = 0;
    Pedido* aux = inicio;

    while (aux != nullptr) {
        cantidad++;
        aux = aux->siguiente;
    }

    return cantidad;
}

void mostrarPedido(Pedido* pedido) {
    cout << "ID: " << pedido->id
         << " | Mesa: " << pedido->mesa
         << " | Pedido: " << pedido->plato
         << " | Estado: " << pedido->estado
         << endl;
}

void mostrarPedidoSinEstado(Pedido* pedido) {
    cout << "ID: " << pedido->id
         << " | Mesa: " << pedido->mesa
         << " | Pedido: " << pedido->plato
         << endl;
}

Pedido* buscarPedidoPorID(int idBuscado) {
    Pedido* aux = listaInicio;

    while (aux != nullptr) {
        if (aux->id == idBuscado)
            return aux;

        aux = aux->siguiente;
    }

    return nullptr;
}

bool hayPedidosConEstado(string estado) {
    Pedido* aux = listaInicio;

    while (aux != nullptr) {
        if (aux->estado == estado)
            return true;

        aux = aux->siguiente;
    }

    return false;
}

void mostrarPedidosPorEstado(string estado) {
    Pedido* aux = listaInicio;
    bool encontrado = false;

    while (aux != nullptr) {
        if (aux->estado == estado) {
            mostrarPedido(aux);
            encontrado = true;
        }

        aux = aux->siguiente;
    }

    if (!encontrado)
        cout << "\nNo hay pedidos con estado: " << estado << "." << endl;
}

void mostrarPedidosPendientesGestion() {
    Pedido* aux = listaInicio;
    bool encontrado = false;

    while (aux != nullptr) {
        if (aux->estado == "Pendiente") {
            mostrarPedidoSinEstado(aux);
            encontrado = true;
        }

        aux = aux->siguiente;
    }

    if (!encontrado)
        cout << "\nNo hay pedidos pendientes." << endl;
}

void liberarMemoria() {
    while (listaInicio != nullptr) {
        Pedido* temp = listaInicio;
        listaInicio = listaInicio->siguiente;
        delete temp;
    }

    while (cimaPila != nullptr) {
        Pedido* temp = cimaPila;
        cimaPila = cimaPila->siguiente;
        delete temp;
    }
}

//================ PERSISTENCIA DE DATOS ================

// Funcion nueva: guardarDatosEnArchivo.
// Por que existe: permite que los pedidos no se pierdan al cerrar el programa.
// Guarda la lista completa y la pila del historial en pedidos.txt usando fstream.
void guardarDatosEnArchivo() {
    ofstream archivo(NOMBRE_ARCHIVO.c_str());

    if (!archivo.is_open()) {
        cout << "\n[AVISO] No se pudo guardar el archivo de datos." << endl;
        return;
    }

    archivo << contadorID << endl;

    archivo << "LISTA" << endl;
    archivo << contarNodos(listaInicio) << endl;

    Pedido* auxLista = listaInicio;

    while (auxLista != nullptr) {
        archivo << auxLista->id << "|"
                << auxLista->mesa << "|"
                << auxLista->estado << "|"
                << auxLista->plato << endl;

        auxLista = auxLista->siguiente;
    }

    archivo << "PILA" << endl;
    archivo << contarNodos(cimaPila) << endl;

    Pedido* auxPila = cimaPila;

    while (auxPila != nullptr) {
        archivo << auxPila->id << "|"
                << auxPila->mesa << "|"
                << auxPila->estado << "|"
                << auxPila->plato << endl;

        auxPila = auxPila->siguiente;
    }

    archivo.close();
}

// Funcion nueva: obtenerCampo.
// Por que existe: cada linea del archivo usa el separador |, entonces esta
// funcion extrae cada dato sin pedirlo otra vez por teclado.
string obtenerCampo(string linea, int numeroCampo) {
    int campoActual = 1;
    int inicio = 0;

    for (int i = 0; i <= linea.length(); i++) {
        if (i == linea.length() || linea[i] == '|') {
            if (campoActual == numeroCampo)
                return linea.substr(inicio, i - inicio);

            campoActual++;
            inicio = i + 1;
        }
    }

    return "";
}

// Funcion nueva: cargarNodoDesdeLinea.
// Por que existe: convierte una linea guardada en el archivo en un nodo Pedido.
// Asi se reconstruyen las estructuras al iniciar el programa.
Pedido* cargarNodoDesdeLinea(string linea) {
    string idTexto = obtenerCampo(linea, 1);
    string mesaTexto = obtenerCampo(linea, 2);
    string estado = obtenerCampo(linea, 3);
    string plato = obtenerCampo(linea, 4);

    if (!esNumeroEnteroPositivo(idTexto) || !esNumeroEnteroPositivo(mesaTexto))
        return nullptr;

    if (estado != "Pendiente" && estado != "En preparacion" && estado != "Entregado")
        return nullptr;

    int id = convertirEntero(idTexto);
    int mesa = convertirEntero(mesaTexto);

    if (id == -1 || mesa == -1)
        return nullptr;

    if (id >= contadorID)
        contadorID = id + 1;

    return crearNodoPedido(id, mesa, plato, estado);
}

// Funcion nueva: cargarDatosDesdeArchivo.
// Por que existe: al abrir el programa, lee pedidos.txt automaticamente y deja
// cargados los pedidos anteriores para que el sistema tenga persistencia.
void cargarDatosDesdeArchivo() {
    ifstream archivo(NOMBRE_ARCHIVO.c_str());

    if (!archivo.is_open())
        return;

    liberarMemoria();

    string linea;

    if (getline(archivo, linea) && esNumeroEnteroPositivo(linea)) {
        int valor = convertirEntero(linea);

        if (valor != -1)
            contadorID = valor;
    }

    getline(archivo, linea); // Texto LISTA
    getline(archivo, linea); // Cantidad de nodos de la lista

    int cantidadLista = 0;

    if (esNumeroEnteroPositivo(linea)) {
        int valor = convertirEntero(linea);

        if (valor != -1)
            cantidadLista = valor;
    }

    for (int i = 0; i < cantidadLista; i++) {
        if (!getline(archivo, linea))
            break;

        Pedido* pedido = cargarNodoDesdeLinea(linea);

        if (pedido != nullptr)
            agregarNodoAlFinal(listaInicio, pedido);
    }

    getline(archivo, linea); // Texto PILA
    getline(archivo, linea); // Cantidad de nodos de la pila

    int cantidadPila = 0;

    if (esNumeroEnteroPositivo(linea)) {
        int valor = convertirEntero(linea);

        if (valor != -1)
            cantidadPila = valor;
    }

    for (int i = 0; i < cantidadPila; i++) {
        if (!getline(archivo, linea))
            break;

        Pedido* pedido = cargarNodoDesdeLinea(linea);

        if (pedido != nullptr)
            agregarNodoAlFinal(cimaPila, pedido);
    }

    archivo.close();
}



//================ OPERACIONES DE COLA ================

// Operacion Cola: Encolar.
// En este programa se encola al crear un pedido con estado Pendiente.

// Operacion Cola: Desencolar.
// Toma el primer pedido pendiente, lo retira de la cola y lo marca En preparacion.
void tomarPedido() {
    Pedido* aux = listaInicio;

    while (aux != nullptr && aux->estado != "Pendiente")
        aux = aux->siguiente;

    if (aux == nullptr) {
        cout << "\nNo hay pedidos pendientes en la cola." << endl;
        return;
    }

    aux->estado = "En preparacion";
    guardarDatosEnArchivo();

    cout << "\n>>> PEDIDO TOMADO POR COCINA <<<" << endl;
    mostrarPedido(aux);
}

// Operacion Cola: Ver frente.
// Consulta el primer pedido pendiente sin retirarlo.
void verFrenteCola() {
    Pedido* aux = listaInicio;

    while (aux != nullptr && aux->estado != "Pendiente")
        aux = aux->siguiente;

    if (aux == nullptr) {
        cout << "\nNo hay pedidos pendientes en la cola." << endl;
        return;
    }

    cout << "\n>>> PROXIMO PEDIDO A PREPARAR <<<" << endl;
    mostrarPedido(aux);
}

// Operacion Cola: Mostrar cola.
// Se recorre la lista mostrando solo los pedidos pendientes.
void mostrarCola() {
    if (!hayPedidosConEstado("Pendiente")) {
        cout << "\nCola vacia." << endl;
        return;
    }

    cout << "\n--- COLA DE PEDIDOS PENDIENTES ---" << endl;
    mostrarPedidosPorEstado("Pendiente");
}




//================ MENUS ================

void menuGestionar() {
    int opcion = 0;

    while (opcion != 4) {
        cout << "\n========== GESTIONAR PEDIDOS ==========";
        cout << "\nPedidos pendientes:";
        mostrarPedidosPendientesGestion();

        cout << "\n1. Tomar pedido";
        cout << "\n2. Marcar pedido entregado";
        cout << "\n3. Eliminar pedido pendiente";
        cout << "\n4. Volver al menu principal";
        cout << "\nSeleccione una opcion (1-4): ";

        opcion = pedirEnteroRango(1, 4);

        switch (opcion) {
        case 1:
            tomarPedido();
            break;

        case 2:
            marcarPedidoEntregado();
            break;

        case 3:
            eliminarPedidoPorMesa();
            break;

        case 4:
            cout << "\nVolviendo al menu principal..." << endl;
            break;
        }
    }
}

void menuVerPedidos() {
    int opcion = 0;

    while (opcion != 5) {
        cout << "\n========== VER PEDIDOS ==========";
        cout << "\n1. Ver proximo a preparar";
        cout << "\n2. Ver ultimo entregado";
        cout << "\n3. Ver cola de pedidos pendientes";
        cout << "\n4. Ver pila de pedidos entregados";
        cout << "\n5. Volver al menu principal";
        cout << "\nSeleccione una opcion (1-5): ";

        opcion = pedirEnteroRango(1, 5);

        switch (opcion) {
        case 1:
            verFrenteCola();
            break;

        case 2:
            verCimaPila();
            break;

        case 3:
            mostrarCola();
            break;

        case 4:
            verHistorial();
            break;

        case 5:
            cout << "\nVolviendo al menu principal..." << endl;
            break;
        }
    }
}






cout << menu de opciones << endl;
cout << 1. Registrar Pedido << endl;
cout << 2. Mostrar pedidos << endl;
cout << 3. Borrar pedido  << endl;
cout << 4. Validar pedido << endl;
