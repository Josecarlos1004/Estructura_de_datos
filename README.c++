//Estructura_de_datos
//Sistema de gestion de un restaurante
#include <iostream>
#include <string>
#include <fstream>
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
