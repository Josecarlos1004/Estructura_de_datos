#include <iostream>
#include <string>

using namespace std;

//Prototipos
bool esNumeroEnteroPositivo(string texto);
bool esSoloLetrasYEspacios(string texto);
int convertirEntero(string texto);
int pedirEntero();
int pedirEnteroRango(int minimo, int maximo);
string pedirPlato();

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
    string estado;
    Pedido* siguiente;
};

//================ DATOS DEL RESTAURANTE ================

Pedido* listaInicio = nullptr;
Pedido* cimaPila = nullptr;
int contadorID = 1;

//================ OPERACIONES ================

Pedido* crearPedido(int id, int mesa, string plato) {
    Pedido* nuevo = new Pedido;

    nuevo->id = id;
    nuevo->mesa = mesa;
    nuevo->plato = plato;
    nuevo->estado = "Pendiente";
    nuevo->siguiente = nullptr;

    return nuevo;
}

void liberarMemoria() {
    while (listaInicio) {
        Pedido* temp = listaInicio;
        listaInicio = listaInicio->siguiente;
        delete temp;
    }

    while (cimaPila) {
        Pedido* temp = cimaPila;
        cimaPila = cimaPila->siguiente;
        delete temp;
    }
}

bool existeID(int idBuscado) {
    Pedido* aux = listaInicio;

    while (aux) {
        if (aux->id == idBuscado)
            return true;

        aux = aux->siguiente;
    }

    return false;
}

void nuevoPedido(int mesa, string plato) {
    int idGenerado = contadorID++;
    Pedido* nuevo = crearPedido(idGenerado, mesa, plato);

    if (!listaInicio) {
        listaInicio = nuevo;
    }
    else {
        Pedido* aux = listaInicio;

        while (aux->siguiente)
            aux = aux->siguiente;

        aux->siguiente = nuevo;
    }

    cout << "\n[OK] Pedido #" << idGenerado << " registrado." << endl;
}

void verProximoPedido() {
    if (!listaInicio) {
        cout << "\nNo hay pedidos en cola." << endl;
        return;
    }

    cout << "\n>>> PROXIMO PEDIDO A ATENDER <<<" << endl;
    cout << "ID: " << listaInicio->id
         << " | Mesa: " << listaInicio->mesa
         << " | Plato: " << listaInicio->plato << endl;
}

void eliminarPedido(int idEliminar) {
    if (!listaInicio) {
        cout << "\nNo hay pedidos registrados." << endl;
        return;
    }

    Pedido* aux = listaInicio;
    Pedido* anterior = nullptr;

    while (aux != nullptr && aux->id != idEliminar) {
        anterior = aux;
        aux = aux->siguiente;
    }

    if (aux == nullptr) {
        cout << "\nOpcion invalida." << endl;
        return;
    }

    if (anterior == nullptr) {
        listaInicio = listaInicio->siguiente;
    }
    else {
        anterior->siguiente = aux->siguiente;
    }

    delete aux;

    cout << "\n[OK] Pedido #" << idEliminar
         << " eliminado de la lista." << endl;
}

void mostrarReporteGeneral() {
    if (!listaInicio) {
        cout << "\nLista vacia." << endl;
        return;
    }

    Pedido* aux = listaInicio;

    cout << "\nID\tMESA\tESTADO\t\tPLATO" << endl;

    while (aux) {
        cout << aux->id << "\t"
             << aux->mesa << "\t"
             << aux->estado << "\t"
             << aux->plato << endl;

        aux = aux->siguiente;
    }
}

void cambiarEstado() {
    if (!listaInicio) {
        cout << "\nNo hay pedidos." << endl;
        return;
    }

    mostrarReporteGeneral();

    int idBusqueda;

    cout << "\nIngrese el ID del pedido a gestionar: ";

    while (true) {
        idBusqueda = pedirEntero();

        if (existeID(idBusqueda))
            break;

        cout << "Opcion invalida. Intente nuevamente: ";
    }

    Pedido* aux = listaInicio;

    while (aux) {
        if (aux->id == idBusqueda) {

            cout << "\n1. Cocina";
            cout << "\n2. Entregar";
            cout << "\n3. Cancelar";
            cout << "\nOpcion: ";

            int subOp = pedirEnteroRango(1, 3);

            if (subOp == 1) {
                aux->estado = "En preparacion";
            }
            else if (subOp == 2) {
                aux->estado = "Entregado";

                Pedido* h = crearPedido(aux->id, aux->mesa, aux->plato);
                h->estado = aux->estado;
                h->siguiente = cimaPila;
                cimaPila = h;

                cout << "\n[OK] Enviado al historial." << endl;
            }
            else if (subOp == 3) {
                aux->estado = "Cancelado";
            }

            return;
        }

        aux = aux->siguiente;
    }
}

void borrarUltimoHistorial() {
    if (cimaPila == nullptr) {
        cout << "\nHistorial vacio." << endl;
        return;
    }

    Pedido* temp = cimaPila;
    cimaPila = cimaPila->siguiente;

    cout << "\n>>> PEDIDO RETIRADO DEL HISTORIAL <<<" << endl;
    cout << "ID: " << temp->id
         << " | Mesa: " << temp->mesa
         << " | Plato: " << temp->plato
         << endl;

    delete temp;
}

void buscarMesa(int m) {
    Pedido* aux = listaInicio;
    bool encontrado = false;

    while (aux) {
        if (aux->mesa == m) {
            cout << "ID: " << aux->id
                 << " | " << aux->plato
                 << " [" << aux->estado << "]"
                 << endl;

            encontrado = true;
        }

        aux = aux->siguiente;
    }

    if (!encontrado) {
        cout << "Mesa " << m
             << " sin pedidos." << endl;
    }
}

void verHistorial() {
    if (!cimaPila) {
        cout << "\nHistorial vacio." << endl;
        return;
    }

    Pedido* aux = cimaPila;

    cout << "\n--- HISTORIAL (LIFO) ---" << endl;

    while (aux) {
        cout << "ID: " << aux->id
             << " | Mesa " << aux->mesa
             << " - " << aux->plato
             << endl;

        aux = aux->siguiente;
    }
}

void tomarPedido() {
    if (!listaInicio) {
        cout << "\nNo hay pedidos en cola." << endl;
        return;
    }

    Pedido* temp = listaInicio;

    cout << "\n>>> PEDIDO TOMADO POR COCINA <<<" << endl;
    cout << "ID: " << temp->id
         << " | Mesa: " << temp->mesa
         << " | Plato: " << temp->plato
         << endl;

    listaInicio = listaInicio->siguiente;

    delete temp;
}

void verUltimoEntregado() {
    if (!cimaPila) {
        cout << "\nHistorial vacio." << endl;
        return;
    }

    cout << "\n>>> ULTIMO PEDIDO ENTREGADO <<<" << endl;
    cout << "ID: " << cimaPila->id
         << " | Mesa: " << cimaPila->mesa
         << " | Plato: " << cimaPila->plato
         << endl;
}

void mostrarColaPedidos() {
    if (!listaInicio) {
        cout << "\nCola vacia." << endl;
        return;
    }

    Pedido* aux = listaInicio;

    cout << "\n--- COLA DE PEDIDOS (FIFO) ---" << endl;

    while (aux) {
        cout << "ID: " << aux->id
             << " | Mesa: " << aux->mesa
             << " | " << aux->plato
             << endl;

        aux = aux->siguiente;
    }
}

//================ MAIN ================

int main() {

    int opcion = 0;

    while (opcion != 12) {

        cout << "\n1. Nuevo Pedido (Encolar)";
        cout << "\n2. Gestionar Estado (Modificar/Apilar)";
        cout << "\n3. Reporte General (Mostrar Lista)";
        cout << "\n4. Buscar por Mesa";
        cout << "\n5. Historial de Ventas (Ver Pila)";
        cout << "\n6. Ver proximo a preparar (Frente Cola)";
        cout << "\n7. Eliminar pedido por ID (Borrar Nodo)";
        cout << "\n8. Borrar ultimo del historial (Desapilar)";
        cout << "\n9. Tomar pedido (Desencolar)";
        cout << "\n10. Mostrar cola FIFO";
        cout << "\n11. Ver ultimo entregado (Cima Pila)";
        cout << "\n12. Salir";
        cout << "\n Seleccione una opcion (1-12): ";

        opcion = pedirEnteroRango(1, 12);

        switch (opcion) {

        case 1: {
            int m;
            string p;

            cout << "Mesa (1-10): ";
            m = pedirEnteroRango(1, 10);

            cout << "Plato: ";
            p = pedirPlato();

            nuevoPedido(m, p);
            break;
        }

        case 2:
            cambiarEstado();
            break;

        case 3:
            mostrarReporteGeneral();
            break;

        case 4: {
            int m;

            cout << "Mesa: ";
            m = pedirEnteroRango(1, 10);

            buscarMesa(m);
            break;
        }

        case 5:
            verHistorial();
            break;

        case 6:
            verProximoPedido();
            break;

        case 7: {
            int id;

            cout << "ID a eliminar: ";

            while (true) {
                id = pedirEntero();

                if (existeID(id))
                    break;

                cout << "Opcion invalida. Intente nuevamente: ";
            }

            eliminarPedido(id);
            break;
        }

        case 8:
            borrarUltimoHistorial();
            break;

        case 9:
            tomarPedido();
            break;

        case 10:
            mostrarColaPedidos();
            break;

        case 11:
            verUltimoEntregado();
            break;

        case 12:
            cout << "\nSaliendo del sistema..." << endl;
            break;
        }
    }

    liberarMemoria();

    return 0;
}
