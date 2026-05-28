#include <iostream>
#include <string>


using namespace std;


// Estructura que representa cada nodo de la lista (un pedido)
struct Pedido {
    int id;
    int mesa;
    string plato;
    string estado;
    Pedido* siguiente;// Puntero al siguiente nodo de la lista


    Pedido(int _id, int _mesa, string _plato) {
        id = _id;
        mesa = _mesa;
        plato = _plato;
        estado = "Pendiente";// Estado inicial por defecto
        siguiente = nullptr;
    }
};


class GestionRestaurante {
private:
    Pedido* listaInicio; // Cabeza de la Lista Enlazada (Todos los pedidos)
    Pedido* cimaPila;   //Solo pedidos entregados


public:
    // El restaurante empieza vacio
    GestionRestaurante() : listaInicio(nullptr), cimaPila(nullptr) {}


    void nuevoPedido(int id, int mesa, string plato) {
        Pedido* nuevo = new Pedido(id, mesa, plato);
        if (!listaInicio) {
            listaInicio = nuevo;
        } else {
            Pedido* aux = listaInicio;
            while (aux->siguiente) aux = aux->siguiente;// Recorremos hasta encontrar el ultimo nodo
            aux->siguiente = nuevo;// El ultimo ahora apunta al nuevo
        }
        cout << "\n OK Pedido #" << id << " registrado como Pendiente." << endl;
    }


    // Submenú y condiciones
    void cambiarEstado(int idBusqueda) {
        if (!listaInicio) {
            cout << "\n AVISO No hay pedidos para modificar." << endl;
            return;
        }


        Pedido* aux = listaInicio;
        while (aux) {
            if (aux->id == idBusqueda) {
                cout << "\nPedido encontrado: " << aux->plato << " (Mesa " << aux->mesa << ")";
                cout << "\nEstado actual: " << aux->estado;
               
                // Si ya está entregado, lo finaliza
                if (aux->estado == "Entregado") {
                    cout << "\n ! Este pedido ya fue finalizado y entregado. No se puede cambiar." << endl;
                    return;
                }


                cout << "\n\nSeleccione el nuevo estado:";
                cout << "\n1. Enviar a Cocina (En preparacion)";
                cout << "\n2. Marcar como Entregado (Finalizar)";
                cout << "\n3. Cancelar cambio";
                cout << "\nOpcion: ";
               
                int subOp;
                cin >> subOp;


                if (subOp == 1) {
                    aux->estado = "En preparacion";
                    cout << "\n[OK] El pedido #" << idBusqueda << " ahora esta en COCINA." << endl;
                }
                else if (subOp == 2) {
                    aux->estado = "Entregado";
                    // Guardar en Historial (Pila)
                    Pedido* h = new Pedido(aux->id, aux->mesa, aux->plato);
                    h->siguiente = cimaPila;
                    cimaPila = h;
                    cout << "\n[OK] El pedido #" << idBusqueda << " ha sido ENTREGADO." << endl;
                }
                else {
                    cout << "\nAccion cancelada." << endl;
                }
                return;
            }
            aux = aux->siguiente;
        }
        cout << "\n[ERROR] No se encontro el pedido con ID " << idBusqueda << "." << endl;
    }


    void mostrarReporteGeneral() {
        if (!listaInicio) {
            cout << "\nNo hay pedidos." << endl;
            return;
        }
        Pedido* aux = listaInicio;
        cout << "\nID\tMESA\tESTADO\t\tPLATO" << endl;
        while (aux) {
            cout << aux->id << "\t" << aux->mesa << "\t" << aux->estado << "\t" << aux->plato << endl;
            aux = aux->siguiente;
        }
    }


    void buscarMesa(int m) {
        Pedido* aux = listaInicio;
        bool encontrado = false;
        while (aux) {
            if (aux->mesa == m) {
                cout << "ID: " << aux->id << " | " << aux->plato << " [" << aux->estado << "]" << endl;
                encontrado = true;
            }
            aux = aux->siguiente;
        }
        if (!encontrado) cout << "Sin pedidos en mesa " << m << endl;
    }


    void verHistorial() {
        if (!cimaPila) {
            cout << "\nHistorial vacio." << endl;
            return;
        }
        Pedido* aux = cimaPila;
        cout << "\n--- HISTORIAL (PILA) ---" << endl;
        while (aux) {
            cout << "Mesa " << aux->mesa << " entregada: " << aux->plato << endl;
            aux = aux->siguiente;
        }
    }
};


int main() {
    GestionRestaurante rest;
    int opcion = 0;


    while (opcion != 6) {
        cout << "\n*** MENU RESTAURANTE ***";
        cout << "\n1. Nuevo Pedido";
        cout << "\n2. Gestionar Estado (Cocina/Entregado)";
        cout << "\n3. Reporte General";
        cout << "\n4. Buscar por Mesa";
        cout << "\n5. Historial de Ventas";
        cout << "\n6. Salir";
        cout << "\nSeleccione: ";


        if (!(cin >> opcion)) {
            cin.clear(); string ign; cin >> ign; continue;
        }


        switch(opcion) {
            case 1: {
                int id, mesa; string plato;
                cout << "ID: "; cin >> id;
                cout << "Mesa: "; cin >> mesa;
                cout << "Plato: "; cin >> plato;
                rest.nuevoPedido(id, mesa, plato);
                break;
            }
            case 2: {
                int id;
                cout << "Ingrese el ID del pedido a gestionar: ";
                cin >> id;
                rest.cambiarEstado(id);
                break;
            }
            case 3:
                rest.mostrarReporteGeneral();
                break;
            case 4: {
                int m; cout << "Mesa: "; cin >> m;
                rest.buscarMesa(m);
                break;
            }
            case 5:
                rest.verHistorial();
                break;
        }
    }
    return 0;
}
