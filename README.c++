#include <iostream>
#include <string>

using namespace std;

struct Pedido;

//Prototipos de validacion
bool esNumeroEnteroPositivo(string texto);
bool esSoloLetrasYEspacios(string texto);
int convertirEntero(string texto);
int pedirEntero();
int pedirEnteroRango(int minimo, int maximo);
string pedirPlato();

//Prototipos de operaciones
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

//================ ESTRUCTURA PEDIDO ================

struct Pedido {
    int id;
    int mesa;
    string plato;
    string estado; // Estados permitidos: Pendiente, En preparacion, Entregado
    Pedido* siguiente;
};

//================ DATOS DEL RESTAURANTE ================

Pedido* listaInicio = nullptr; // Lista enlazada: guarda todos los pedidos.
Pedido* cimaPila = nullptr;    // Pila: guarda los pedidos entregados.
int contadorID = 1;

//================ FUNCIONES AUXILIARES ================

Pedido* crearNodoPedido(int id, int mesa, string plato, string estado) {
    Pedido* nuevo = new Pedido;

    nuevo->id = id;
    nuevo->mesa = mesa;
    nuevo->plato = plato;
    nuevo->estado = estado;
    nuevo->siguiente = nullptr;

    return nuevo;
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

//================ OPERACIONES DE LISTA ================

// Operacion Lista: Crear pedido.
// Se crea un nodo nuevo y se agrega al final de la lista enlazada.
// Tambien queda en la cola porque su estado inicial es Pendiente.
void crearPedido(int mesa, string plato) {
    int idGenerado = contadorID++;
    Pedido* nuevo = crearNodoPedido(idGenerado, mesa, plato, "Pendiente");

    if (listaInicio == nullptr) {
        listaInicio = nuevo;
    }
    else {
        Pedido* aux = listaInicio;

        while (aux->siguiente != nullptr)
            aux = aux->siguiente;

        aux->siguiente = nuevo;
    }

    cout << "\n[OK] Pedido #" << idGenerado << " registrado." << endl;
}

// Operacion Lista: Eliminar pedido.
// Se recorre la lista hasta encontrar el ID indicado y se elimina ese nodo.
void eliminarPedido(int idEliminar) {
    Pedido* aux = listaInicio;
    Pedido* anterior = nullptr;

    while (aux != nullptr && aux->id != idEliminar) {
        anterior = aux;
        aux = aux->siguiente;
    }

    if (aux == nullptr) {
        cout << "\nNo se encontro el pedido." << endl;
        return;
    }

    if (anterior == nullptr) {
        listaInicio = aux->siguiente;
    }
    else {
        anterior->siguiente = aux->siguiente;
    }

    delete aux;

    cout << "\n[OK] Pedido #" << idEliminar << " eliminado." << endl;
}

// Operacion Lista: Buscar por mesa.
// Se recorre toda la lista mostrando los nodos cuya mesa coincide.
void buscarPorMesa(int mesa) {
    Pedido* aux = listaInicio;
    bool encontrado = false;

    cout << "\n--- PEDIDOS DE LA MESA " << mesa << " ---" << endl;

    while (aux != nullptr) {
        if (aux->mesa == mesa) {
            mostrarPedido(aux);
            encontrado = true;
        }

        aux = aux->siguiente;
    }

    if (!encontrado)
        cout << "La mesa " << mesa << " no tiene pedidos." << endl;
}

// Operacion Lista: Modificar estado.
// Se recorre la lista hasta encontrar el ID indicado y se actualiza el estado.
void marcarPedidoEntregado() {
    if (!hayPedidosConEstado("En preparacion")) {
        cout << "\nNo hay pedidos en preparacion." << endl;
        return;
    }

    cout << "\n--- PEDIDOS EN PREPARACION ---" << endl;
    mostrarPedidosPorEstado("En preparacion");

    cout << "\nIndique el ID del pedido que desea marcar como entregado: ";
    int id = pedirEntero();
    Pedido* pedido = buscarPedidoPorID(id);

    if (pedido == nullptr || pedido->estado != "En preparacion") {
        cout << "\nOpcion invalida. Debe elegir un pedido en preparacion." << endl;
        return;
    }

    pedido->estado = "Entregado";
    apilarPedidoEntregado(pedido);

    cout << "\n[OK] Pedido #" << id << " marcado como entregado." << endl;
}

// Operacion Lista: Mostrar lista.
// Se recorre la lista mostrando cada pedido: Pendiente, En preparacion o Entregado.
void mostrarListaPedidos() {
    if (listaInicio == nullptr) {
        cout << "\nLista vacia." << endl;
        return;
    }

    Pedido* aux = listaInicio;

    cout << "\n--- LISTA GENERAL DE PEDIDOS ---" << endl;

    while (aux != nullptr) {
        mostrarPedido(aux);
        aux = aux->siguiente;
    }
}

// Operacion Lista: Eliminar pedido pendiente por mesa.
// Primero pide la mesa, muestra sus pedidos pendientes y luego elimina el ID elegido.
void eliminarPedidoPorMesa() {
    if (!hayPedidosConEstado("Pendiente")) {
        cout << "\nNo hay pedidos pendientes para eliminar." << endl;
        return;
    }

    cout << "\nMesa (1-10): ";
    int mesa = pedirEnteroRango(1, 10);

    Pedido* aux = listaInicio;
    bool encontrado = false;

    cout << "\n--- PEDIDOS PENDIENTES DE LA MESA " << mesa << " ---" << endl;

    while (aux != nullptr) {
        if (aux->mesa == mesa && aux->estado == "Pendiente") {
            mostrarPedido(aux);
            encontrado = true;
        }

        aux = aux->siguiente;
    }

    if (!encontrado) {
        cout << "No hay pedidos pendientes para esa mesa." << endl;
        return;
    }

    cout << "\nID del pedido a eliminar: ";
    int id = pedirEntero();
    Pedido* pedido = buscarPedidoPorID(id);

    if (pedido == nullptr || pedido->mesa != mesa || pedido->estado != "Pendiente") {
        cout << "\nOpcion invalida. Debe elegir un pedido pendiente de esa mesa." << endl;
        return;
    }

    eliminarPedido(id);
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

//================ OPERACIONES DE PILA ================

// Operacion Pila: Apilar.
// Al entregar un pedido, se copia y se coloca en la cima del historial.
void apilarPedidoEntregado(Pedido* pedido) {
    Pedido* nuevo = crearNodoPedido(pedido->id, pedido->mesa, pedido->plato, "Entregado");

    nuevo->siguiente = cimaPila;
    cimaPila = nuevo;
}

// Operacion Pila: Desapilar.
// Retira el pedido entregado mas reciente de la cima del historial.
void desapilarHistorial() {
    if (cimaPila == nullptr) {
        cout << "\nHistorial vacio." << endl;
        return;
    }

    Pedido* temp = cimaPila;
    cimaPila = cimaPila->siguiente;

    cout << "\n>>> PEDIDO RETIRADO DEL HISTORIAL <<<" << endl;
    mostrarPedido(temp);

    delete temp;
}

// Operacion Pila: Ver cima.
// Consulta el ultimo pedido entregado sin retirarlo.
void verCimaPila() {
    if (cimaPila == nullptr) {
        cout << "\nHistorial vacio." << endl;
        return;
    }

    cout << "\n>>> ULTIMO PEDIDO ENTREGADO <<<" << endl;
    mostrarPedido(cimaPila);
}

// Operacion Pila: Ver historial / Mostrar pila.
// Muestra todos los pedidos entregados desde el mas reciente al mas antiguo.
void verHistorial() {
    if (cimaPila == nullptr) {
        cout << "\nHistorial vacio." << endl;
        return;
    }

    Pedido* aux = cimaPila;

    cout << "\n--- HISTORIAL DE PEDIDOS VENDIDOS ---" << endl;

    while (aux != nullptr) {
        mostrarPedido(aux);
        aux = aux->siguiente;
    }
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

//================ MAIN ================

int main() {
    int opcion = 0;

    while (opcion != 8) {
        cout << "\n========== SISTEMA DE PEDIDOS ==========";
        cout << "\n1. Crear pedido";
        cout << "\n2. Gestionar pedidos";
        cout << "\n3. Mostrar pedidos";
        cout << "\n4. Buscar por mesa";
        cout << "\n5. Historial de pedidos vendidos";
        cout << "\n6. Ver pedidos";
        cout << "\n7. Borrar ultimo del historial";
        cout << "\n8. Salir";
        cout << "\nSeleccione una opcion (1-8): ";

        opcion = pedirEnteroRango(1, 8);

        switch (opcion) {
        case 1: {
            int mesa;
            string plato;

            cout << "Mesa (1-10): ";
            mesa = pedirEnteroRango(1, 10);

            cout << "Nombre del pedido: ";
            plato = pedirPlato();

            crearPedido(mesa, plato);
            break;
        }

        case 2:
            menuGestionar();
            break;

        case 3:
            mostrarListaPedidos();
            break;

        case 4: {
            cout << "Mesa (1-10): ";
            int mesa = pedirEnteroRango(1, 10);
            buscarPorMesa(mesa);
            break;
        }

        case 5:
            verHistorial();
            break;

        case 6:
            menuVerPedidos();
            break;

        case 7:
            desapilarHistorial();
            break;

        case 8:
            cout << "\nSaliendo del sistema..." << endl;
            break;
        }
    }

    liberarMemoria();

    return 0;
}

cout << 3. Borrar pedido  << endl;
cout << 4. Validar pedido << endl;
