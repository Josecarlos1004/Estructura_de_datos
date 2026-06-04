#include <iostream>
#include <string>

using namespace std;

struct Pedido {
    int id;
    int mesa;
    string plato;
    string estado; 
    Pedido* siguiente;

    Pedido(int _id, int _mesa, string _plato) {
        id = _id;
        mesa = _mesa;
        plato = _plato;
        estado = "Pendiente";
        siguiente = nullptr;
    }
};

class GestionRestaurante {
private:
    Pedido* listaInicio; 
    Pedido* cimaPila;
    int contadorID;

public:
    GestionRestaurante() : listaInicio(nullptr), cimaPila(nullptr), contadorID(1) {}

    ~GestionRestaurante() {
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

    void nuevoPedido(int mesa, string plato) {
        int idGenerado = contadorID++;
        Pedido* nuevo = new Pedido(idGenerado, mesa, plato);
        
        if (!listaInicio) {
            listaInicio = nuevo;
        } else {
            Pedido* aux = listaInicio;
            while (aux->siguiente) aux = aux->siguiente;
            aux->siguiente = nuevo;
        }
        cout << "\n OK Pedido #" << idGenerado << " registrado." << endl;
    }

    // --- NUEVA OPERACIÓN: VER FRENTE DE LA COLA ---
    void verProximoPedido() {
        if (!listaInicio) {
            cout << "\n No hay pedidos en cola." << endl;
            return;
        }
        cout << "\n>>> PROXIMO PEDIDO A ATENDER <<<" << endl;
        cout << "ID: " << listaInicio->id << " | Mesa: " << listaInicio->mesa << " | Plato: " << listaInicio->plato << endl;
    }

    // --- NUEVA OPERACIÓN: ELIMINAR PEDIDO (POR ID) ---
    void eliminarPedido(int idEliminar) {
        if (!listaInicio) return;

        Pedido* aux = listaInicio;
        Pedido* anterior = nullptr;

        while (aux != nullptr && aux->id != idEliminar) {
            anterior = aux;
            aux = aux->siguiente;
        }

        if (aux == nullptr) {
            cout << "\n [!] No se encontro el ID para eliminar." << endl;
            return;
        }

        if (anterior == nullptr) {
            listaInicio = listaInicio->siguiente;
        } else {
            anterior->siguiente = aux->siguiente;
        }

        delete aux;
        cout << "\n [OK] Pedido #" << idEliminar << " eliminado de la lista." << endl;
    }

    void cambiarEstado() {
        if (!listaInicio) {
            cout << "\n AVISO No hay pedidos." << endl;
            return;
        }
        mostrarReporteGeneral();
        int idBusqueda;
        cout << "\nIngrese el ID del pedido a gestionar: ";
        cin >> idBusqueda;

        Pedido* aux = listaInicio;
        while (aux) {
            if (aux->id == idBusqueda) {
                cout << "\n1. Cocina | 2. Entregar | 3. Cancelar\nOpcion: ";
                int subOp; cin >> subOp;

                if (subOp == 1) aux->estado = "En preparacion";
                else if (subOp == 2) {
                    aux->estado = "Entregado";
                    Pedido* h = new Pedido(aux->id, aux->mesa, aux->plato);
                    h->siguiente = cimaPila;
                    cimaPila = h;
                    cout << "\n[OK] Enviado al historial." << endl;
                }
                return;
            }
            aux = aux->siguiente;
        }
    }

    // --- NUEVA OPERACIÓN: DESAPILAR (BORRAR ULTIMO DEL HISTORIAL) ---
    void borrarUltimoHistorial() {
        if (!cimaPila) {
            cout << "\n Historial ya esta vacio." << endl;
            return;
        }
        Pedido* temp = cimaPila;
        cimaPila = cimaPila->siguiente;
        cout << "\n [!] Borrando del historial: " << temp->plato << endl;
        delete temp;
    }

    void mostrarReporteGeneral() {
        if (!listaInicio) { cout << "\nLista vacia." << endl; return; }
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
        if (!encontrado) cout << "Mesa " << m << " sin pedidos." << endl;
    }

    void verHistorial() {
        if (!cimaPila) { cout << "\nHistorial vacio." << endl; return; }
        Pedido* aux = cimaPila;
        cout << "\n--- HISTORIAL (LIFO) ---" << endl;
        while (aux) {
            cout << "ID: " << aux->id << " | Mesa " << aux->mesa << " - " << aux->plato << endl;
            aux = aux->siguiente;
        }
    }
};

int main() {
    GestionRestaurante rest;
    int opcion = 0;

    while (opcion != 9) {
        cout << "\n** SISTEMA RESTAURANTE (V2.0) **";
        cout << "\n1. Nuevo Pedido (Encolar)";
        cout << "\n2. Gestionar Estado (Modificar/Apilar)";
        cout << "\n3. Reporte General (Mostrar Lista)";
        cout << "\n4. Buscar por Mesa";
        cout << "\n5. Historial de Ventas (Ver Pila)";
        cout << "\n6. Ver proximo a preparar (Frente Cola)";
        cout << "\n7. Eliminar pedido por ID (Borrar Nodo)";
        cout << "\n8. Borrar ultimo del historial (Desapilar)";
        cout << "\n9. Salir";
        cout << "\nSeleccione: ";

        if (!(cin >> opcion)) { cin.clear(); while (cin.get() != '\n'); continue; }

        switch(opcion) {
            case 1: {
                int m; string p;
                cout << "Mesa (1-10): "; cin >> m;
                cout << "Plato: "; cin.ignore(); getline(cin, p);
                rest.nuevoPedido(m, p);
                break;
            }
            case 2: rest.cambiarEstado(); break;
            case 3: rest.mostrarReporteGeneral(); break;
            case 4: {
                int m; cout << "Mesa: "; cin >> m;
                rest.buscarMesa(m);
                break;
            }
            case 5: rest.verHistorial(); break;
            case 6: rest.verProximoPedido(); break;
            case 7: {
                int id; cout << "ID a eliminar: "; cin >> id;
                rest.eliminarPedido(id);
                break;
            }
            case 8: rest.borrarUltimoHistorial(); break;
        }
    }
    return 0;
}
