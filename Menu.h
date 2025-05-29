// MenuJuego.h
#ifndef MENU_JUEGO_H
#define MENU_JUEGO_H

#include <iostream>
#include <string>

void mostrarMenuPrincipal() {
    std::cout << "----------------------" << std::endl;
    std::cout << "       PUCHAMON" << std::endl;
    std::cout << "----------------------" << std::endl;
    std::cout << "1. start" << std::endl;
    std::cout << "2. Ver Instrucciones" << std::endl;
    std::cout << "3. Salir del Juego" << std::endl;
    std::cout << "----------------------" << std::endl;
}

int obtenerOpcionMenu() {
    int opcion;
    std::cout << "Elige una opcion: ";
    std::cin >> opcion;
    std::cin.ignore(256, '\n');
    return opcion;
}

#endif // MENU_JUEGO_H
