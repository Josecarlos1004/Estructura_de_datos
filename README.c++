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
void
void
void
void
void
void
void


//=========== Funciones Auxiliares ========================
Pedido* crearNodoPedido(intid, intmesa,  string plato, string estado){
  Pedido* nuevo = new Pedido;
  
  nuevo-> id;
  nuevo-> mesa;
  nuevo-> plato;
  nuevo-> estado;
  nuevo-> nullptr;
  return nuevo;
}
void agregarNodoAlFinal(Pedido*& inicio, Pedido* nuevo){
  if (inicio == nullptr){
    inicio = nuevo;
    return;
  }
  Pedido* aux = inicio;
  while (aux -> siguiente != nullptr)
    aux = aux -> siguientte;

  aux -> siguiente = nuevo;
  }
int contarNodos(Pedidos* inicio){
  int cantidad =0;
  Pedido* aux = inicio;

while (aux != nullptr){
aux = aux -> siguiente;
}
return cantidad;
}
void mostrarPedido(Pedido* pedido){
  cout<< "ID" << pedido -> id
<<"Mesa: " << pedidop -> mesa

}






cout << menu de opciones << endl;
cout << 1. Registrar Pedido << endl;
cout << 2. Mostrar pedidos << endl;
cout << 3. Borrar pedido  << endl;
cout << 4. Validar pedido << endl;
