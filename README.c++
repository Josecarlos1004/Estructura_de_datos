//Estructura_de_datos
//Sistema de gestion de un restaurante
#include <iostream>
#include <string>
using namespace std;

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
    // Ese numero ayuda a leer despues exactamente la misma cantidad de lineas.
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







cout << menu de opciones << endl;
cout << 1. Registrar Pedido << endl;
cout << 2. Mostrar pedidos << endl;
cout << 3. Borrar pedido  << endl;
cout << 4. Validar pedido << endl;
