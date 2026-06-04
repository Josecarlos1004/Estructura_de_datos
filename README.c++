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
        cout << "\n OK Pedido #" << idGenerado << " registrado en Mesa " << mesa << "." << endl;
    }

    void cambiarEstado() {
        if (!listaInicio) {
            cout << "\n AVISO No hay pedidos para modificar." << endl;
            return;
        }
//ENCOLAR
        cout << "\n--- PEDIDOS EN COLA (Orden de llegada) ---" << endl;
        cout << "ID\tMESA\tESTADO\t\tPLATO" << endl;
        Pedido* temp = listaInicio;
        bool hayActivos = false;
        while (temp) {
            if (temp->estado != "Entregado") {
                cout << temp->id << "\t" << temp->mesa << "\t" << temp->estado << "\t" << temp->plato << endl;
                hayActivos = true;
            }
            temp = temp->siguiente;
        }

        if (!hayActivos) {
            cout << "No hay pedidos pendientes de entrega." << endl;
            return;
        }
//MODIFICAR ESTADO
        int idBusqueda;
        cout << "\nIngrese el ID del pedido a gestionar: ";
        while (!(cin >> idBusqueda)) {
            cout << "Error. Ingrese un ID numerico: ";
            cin.clear();
            while (cin.get() != '\n');
        }

        Pedido* aux = listaInicio;
        while (aux) {
            if (aux->id == idBusqueda) {
                if (aux->estado == "Entregado") {
                    cout << "\n ! Este pedido ya fue entregado." << endl;
                    return;
                }

                cout << "\nPedido #" << aux->id << ": " << aux->plato;
                cout << "\n1. Enviar a Cocina\n2. Marcar como Entregado\n3. Cancelar\nOpcion: ";
                
                int subOp;
                while (!(cin >> subOp) || subOp < 1 || subOp > 3) {
                    cout << "Opcion invalida (1-3): ";
                    cin.clear();
                    while (cin.get() != '\n');
                }

                if (subOp == 1) {
                    aux->estado = "En preparacion";
                    cout << "\n[OK] Estado: COCINA." << endl;
                } 
                else if (subOp == 2) {
                    aux->estado = "Entregado";
                    Pedido* h = new Pedido(aux->id, aux->mesa, aux->plato);
                    h->siguiente = cimaPila;
                    cimaPila = h;
                    cout << "\n[OK] Estado: ENTREGADO." << endl;
                } 
                return;
            }
            aux = aux->siguiente;
        }
        cout << "\n[ERROR] No se encontro el ID " << idBusqueda << "." << endl;
    }

    void mostrarReporteGeneral() {
        if (!listaInicio) { cout << "\nNo hay pedidos." << endl; return; }
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
        if (!cimaPila) { cout << "\nHistorial vacio." << endl; return; }
        Pedido* aux = cimaPila;
        cout << "\n--- HISTORIAL (Mas recientes primero) ---" << endl;
        while (aux) {
            cout << "ID: " << aux->id << " | Mesa " << aux->mesa << " entregada: " << aux->plato << endl;
            aux = aux->siguiente;
        }
    }
};

int main() {
    GestionRestaurante rest;
    int opcion = 0;

    while (opcion != 6) {
        cout << "\n** MENU RESTAURANTE **";
        cout << "\n1. Nuevo Pedido";
        cout << "\n2. Gestionar Estado";
        cout << "\n3. Reporte General";
        cout << "\n4. Buscar por Mesa";
        cout << "\n5. Historial de Ventas";
        cout << "\n6. Salir";
        cout << "\nSeleccione (1-6): ";

        if (!(cin >> opcion) || opcion < 1 || opcion > 6) { 
            cout << "\n[!] ERROR: Ingrese un numero del 1 al 6.\n";
            cin.clear();              
            while (cin.get() != '\n'); 
            continue;                 
        }

        switch(opcion) {
            case 1: {
                int mesa; string plato;
                // MODIFICACI N: Validaci n estricta de mesa (1-10)
                cout << "Mesa (1-10): ";
                while (!(cin >> mesa) || mesa < 1 || mesa > 10) {
                    cout << "Error: Solo existen mesas del 1 al 10. Reintente: ";
                    cin.clear();
                    while (cin.get() != '\n');
                }
                cout << "Plato: "; 
                cin.ignore(); 
                getline(cin, plato);
                rest.nuevoPedido(mesa, plato);
                break;
            }
            case 2:
                rest.cambiarEstado(); 
                break;
            case 3:
                rest.mostrarReporteGeneral();
                break;
            case 4: {
                int m; 
                cout << "Mesa a buscar (1-10): "; 
                while (!(cin >> m) || m < 1 || m > 10) {
                    cout << "Invalido. Ingrese mesa entre 1 y 10: ";
                    cin.clear();
                    while (cin.get() != '\n');
                }
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
