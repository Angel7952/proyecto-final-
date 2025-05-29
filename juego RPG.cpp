#include <iostream>
#include <string>
#include <vector>
#include <map>
// #include <algorithm> // ¡QUITADA DEFINITIVAMENTE!
#include "Menu.h" // Asumiendo que este es el archivo que contiene mostrarMenuPrincipal() y obtenerOpcionMenu()

using namespace std;

// Función auxiliar para obtener el mínimo de dos números (reemplaza std::min)
int getMin(int a, int b) {
    return (a < b) ? a : b;
}

// Tipos de objetos en la mochila
enum class TipoObjeto {
    CURACION,
    ENERGIA,
    EVOLUCION
};

// Estructura para los objetos
struct Objeto {
    string nombre;
    TipoObjeto tipo;
    int cantidad;
};

// Tipos de efectos que pueden tener los Puchamon
enum class Estado {
    NINGUNO,
    VENENO,
    ELECTROCUTADO,
    QUEMADURA
};

// Las habilidades de cada Puchamon
struct Habilidad {
    string nombre;
    int dano;
    Estado efecto;
    int costo;
};

// Info de cada Puchamon
struct Puchamon {
    string nombre;
    int salud;
    int salud_max;
    int puntos;
    int puntos_max;
    vector<Habilidad> habilidades;
    Estado estado_actual;
    int contador_estado;
    bool puede_evolucionar;
};

// Info del jugador
struct Jugador {
    string nombre;
    vector<Puchamon> puchamones;
    int puchamon_actual;
    map<string, Objeto> mochila;
};

// Función para evolucionar un Puchamon
void evolucionarPuchamon(Puchamon& p) {
    string nombre_original = p.nombre; // Guarda el nombre original antes de la evolución

    if (p.nombre == "Pichaku") {
        p.nombre = "Riatachu";
        p.salud_max += 30;
        p.salud = p.salud_max;
        p.puntos_max += 20;
        p.puntos = p.puntos_max;
        p.habilidades.push_back({ "Trueno Brutal", 60, Estado::ELECTROCUTADO, 35 });
    }
    else if (p.nombre == "Chambander") {
        p.nombre = "Chambilion";
        p.salud_max += 40;
        p.salud = p.salud_max;
        p.puntos_max += 15;
        p.puntos = p.puntos_max;
        p.habilidades.push_back({ "Llamarada Infernal", 70, Estado::QUEMADURA, 40 });
    }
    else if (p.nombre == "Babososur") {
        p.nombre = "Imbecilux";
        p.salud_max += 50;
        p.salud = p.salud_max;
        p.puntos_max += 10;
        p.puntos = p.puntos_max;
        p.habilidades.push_back({ "Baboso Venenoso", 45, Estado::VENENO, 25 });
    }
    p.puede_evolucionar = false;
    // Mensaje de evolución actualizado
    cout << "¡" << nombre_original << " ha evolucionado a " << p.nombre << "!" << endl;
}

// Función que aplica daño y efectos de un ataque
void aplicarDano(Puchamon& atacante, Puchamon& defensor, const Habilidad& habilidad) {
    if (atacante.puntos >= habilidad.costo) {
        int dano_final = habilidad.dano;

        cout << atacante.nombre << " usa " << habilidad.nombre
            << " y hace " << dano_final << " de dano a "
            << defensor.nombre << "." << endl;

        defensor.salud -= dano_final;
        if (defensor.salud < 0) defensor.salud = 0;

        cout << defensor.nombre << " ahora tiene " << defensor.salud << " de salud." << endl;
        atacante.puntos -= habilidad.costo;
        cout << atacante.nombre << " le quedan " << atacante.puntos << " puntos." << endl;

        // Aplicar efecto si lo tiene
        if (habilidad.efecto != Estado::NINGUNO) {
            if (defensor.estado_actual != Estado::NINGUNO) {
                cout << defensor.nombre << " ya está afectado por ";
                switch (defensor.estado_actual) {
                case Estado::VENENO: cout << "veneno"; break;
                case Estado::ELECTROCUTADO: cout << "electrocutado"; break;
                case Estado::QUEMADURA: cout << "quemadura"; break;
                default: break;
                }
                cout << ", se reemplaza." << endl;
            }

            defensor.estado_actual = habilidad.efecto;

            // Configurar duración de efectos
            switch (habilidad.efecto) {
            case Estado::VENENO:
                defensor.contador_estado = 3;  // 3 turnos
                break;
            case Estado::QUEMADURA:
                defensor.contador_estado = 1;  // 1 turno
                break;
            case Estado::ELECTROCUTADO:
                defensor.contador_estado = 1;  // 1 turno paralizado
                break;
            default:
                break;
            }

            cout << defensor.nombre << " ahora está ";
            switch (habilidad.efecto) {
            case Estado::VENENO: cout << "envenenado"; break;
            case Estado::ELECTROCUTADO: cout << "electrocutado (paralizado)"; break;
            case Estado::QUEMADURA: cout << "quemado"; break;
            default: break;
            }
            cout << "." << endl;
        }
    }
    else {
        cout << atacante.nombre << " no tiene energia para " << habilidad.nombre << "." << endl;
    }
}

// Función que aplica los efectos al inicio del turno
void aplicarEfectosDeEstado(Puchamon& puchamon) {
    if (puchamon.estado_actual != Estado::NINGUNO) {
        cout << puchamon.nombre << " sufre por estar ";
        switch (puchamon.estado_actual) {
        case Estado::VENENO:
            cout << "envenenado (-5 salud).";
            puchamon.salud -= 5;
            break;
        case Estado::ELECTROCUTADO:
            cout << "electrocutado (paralizado este turno).";
            break;
        case Estado::QUEMADURA:
            cout << "quemado (-15 salud).";
            puchamon.salud -= 15;
            break;
        default: break;
        }
        cout << " Ahora tiene " << puchamon.salud << " de salud." << endl;

        puchamon.contador_estado--;
        if (puchamon.contador_estado <= 0) {
            puchamon.estado_actual = Estado::NINGUNO;
            cout << puchamon.nombre << " ya no está afectado." << endl;
        }
    }

    // Aumentar energía cada turno (5 puntos hasta el máximo)
    if (puchamon.puntos < puchamon.puntos_max) {
        // Usa la función getMin personalizada en lugar de std::min
        int aumento = getMin(5, puchamon.puntos_max - puchamon.puntos);
        puchamon.puntos += aumento;
        cout << puchamon.nombre << " recupera " << aumento << " puntos de energia." << endl;
    }
}

// Funciones adicionales para la mochila
void usarObjeto(Jugador& jugador) {
    if (jugador.mochila.empty()) {
        cout << "No tienes objetos en la mochila!" << endl;
        return;
    }

    cout << "\n--- MOCHILA ---" << endl;
    int index = 1;
    vector<string> nombres_objetos; // Para mapear el numero a el nombre real del objeto
    for (auto& item : jugador.mochila) {
        nombres_objetos.push_back(item.first); // Guarda la clave (nombre del objeto)
        cout << index++ << ". " << item.second.nombre << " (x" << item.second.cantidad << ")" << endl;
    }

    cout << "Elige un objeto (numero) o 0 para cancelar: ";
    int opcion;
    cin >> opcion;
    cin.ignore(256, '\n');

    if (opcion == 0) return;

    if (opcion > 0 && opcion <= nombres_objetos.size()) {
        string nombre_obj_elegido = nombres_objetos[opcion - 1]; // Obtiene el nombre del objeto elegido
        Objeto& obj = jugador.mochila.at(nombre_obj_elegido); // Accede al objeto por su nombre

        Puchamon& pucha = jugador.puchamones[jugador.puchamon_actual];

        // Flag para saber si el objeto fue usado exitosamente (para consumir el turno)
        bool objeto_usado_exitosamente = false;

        switch (obj.tipo) {
        case TipoObjeto::CURACION:
            if (pucha.salud >= pucha.salud_max) {
                cout << pucha.nombre << " ya tiene la salud al maximo!" << endl;
                return; // No se consume el objeto ni el turno
            }
            pucha.salud += 50; // Asumiendo que las pociones curan 50
            if (pucha.salud > pucha.salud_max) pucha.salud = pucha.salud_max;
            cout << pucha.nombre << " recupero 50 puntos de salud!" << endl;
            cout << "Salud actual: " << pucha.salud << "/" << pucha.salud_max << endl;
            objeto_usado_exitosamente = true;
            break;

        case TipoObjeto::ENERGIA:
            if (pucha.puntos >= pucha.puntos_max) {
                cout << pucha.nombre << " ya tiene la energia al maximo!" << endl;
                return; // No se consume el objeto ni el turno
            }
            pucha.puntos += 30; // Asumiendo que la energia X recupera 30
            if (pucha.puntos > pucha.puntos_max) pucha.puntos = pucha.puntos_max;
            cout << pucha.nombre << " recupero 30 puntos de energia!" << endl;
            cout << "Energia actual: " << pucha.puntos << "/" << pucha.puntos_max << endl;
            objeto_usado_exitosamente = true;
            break;

        case TipoObjeto::EVOLUCION:
            if (pucha.puede_evolucionar) {
                evolucionarPuchamon(pucha);
                objeto_usado_exitosamente = true;
            }
            else {
                cout << pucha.nombre << " no puede evolucionar ahora o ya evoluciono!" << endl;
                return; // No se consume el objeto ni el turno
            }
            break;
        }

        // Reducir cantidad del objeto solo si fue usado exitosamente
        if (objeto_usado_exitosamente) {
            // Guarda el nombre del objeto antes de modificar o eliminar
            string nombre_del_objeto_usado = obj.nombre;

            if (--obj.cantidad <= 0) {
                jugador.mochila.erase(nombre_obj_elegido);
                cout << "Te quedaste sin " << nombre_del_objeto_usado << "!" << endl; // Usa la variable guardada
            }
        }
    }
    else {
        cout << "Opcion invalida!" << endl;
    }
}

// Funcion para mostrar los ataques de un Puchamon
void mostrarHabilidades(const Puchamon& puchamon) {
    cout << "Ataques de " << puchamon.nombre << ":" << endl;
    for (int i = 0; i < puchamon.habilidades.size(); ++i) {
        cout << i + 1 << ". " << puchamon.habilidades[i].nombre
            << " (Dano: " << puchamon.habilidades[i].dano
            << ", Costo: " << puchamon.habilidades[i].costo << ")";

        if (puchamon.habilidades[i].efecto != Estado::NINGUNO) {
            cout << ", Efecto: ";
            switch (puchamon.habilidades[i].efecto) {
            case Estado::VENENO: cout << "Veneno"; break;
            case Estado::ELECTROCUTADO: cout << "Electrocutado"; break;
            case Estado::QUEMADURA: cout << "Quemadura"; break;
            default: break;
            }
        }
        cout << endl;
    }
}

// Funcion para ver si al menos un Puchamon del jugador sigue vivo
bool tienePuchamonVivo(const Jugador& jugador) {
    for (const auto& p : jugador.puchamones) {
        if (p.salud > 0) return true;
    }
    return false;
}

// Funcion para cambiar de Puchamon durante la batalla
void cambiarPuchamon(Jugador& jugador) {
    cout << "\n--- Cambiar Puchamon ---" << endl;
    cout << "Tus Puchamon disponibles:" << endl;

    for (int i = 0; i < jugador.puchamones.size(); ++i) {
        string estado_pucha = "";
        if (jugador.puchamones[i].salud <= 0) {
            estado_pucha = " (DEBILITADO)";
        }
        else if (i == jugador.puchamon_actual) {
            estado_pucha = " (ACTUAL)";
        }

        cout << i + 1 << ". " << jugador.puchamones[i].nombre
            << " (Salud: " << jugador.puchamones[i].salud << "/" << jugador.puchamones[i].salud_max << ")"
            << estado_pucha << endl;
    }

    cout << "Elige un Puchamon (numero): ";
    int opcion;
    cin >> opcion;
    cin.ignore(256, '\n');

    if (opcion >= 1 && opcion <= jugador.puchamones.size()) {
        if (jugador.puchamones[opcion - 1].salud > 0) {
            if (jugador.puchamon_actual == (opcion - 1)) {
                cout << "Ya estas usando a " << jugador.puchamones[opcion - 1].nombre << endl;
            }
            else {
                jugador.puchamon_actual = opcion - 1;
                cout << "Ahora usas a " << jugador.puchamones[jugador.puchamon_actual].nombre << "!" << endl;
            }
        }
        else {
            cout << jugador.puchamones[opcion - 1].nombre << " esta debilitado y no puede luchar!" << endl;
        }
    }
    else {
        cout << "Opcion invalida" << endl;
    }
}

// Funcion principal de batalla
void iniciarCombate(Jugador& jugador, Puchamon& enemigo) {
    cout << "¡Comienza la batalla entre " << jugador.nombre << " y " << enemigo.nombre << "!" << endl;

    // Reiniciar salud y puntos de Miaotu para cada combate
    enemigo.salud = enemigo.salud_max;
    enemigo.puntos = enemigo.puntos_max;
    enemigo.estado_actual = Estado::NINGUNO;
    enemigo.contador_estado = 0;

    // Asegurarse de que el puchamon inicial del jugador este vivo
    if (jugador.puchamones[jugador.puchamon_actual].salud <= 0) {
        cout << jugador.puchamones[jugador.puchamon_actual].nombre << " esta debilitado, cambia a otro Puchamon!" << endl;
        cambiarPuchamon(jugador);
        if (jugador.puchamones[jugador.puchamon_actual].salud <= 0) { // Si todos estan debilitados al inicio
            cout << "Todos tus Puchamon estan debilitados. ¡No puedes luchar!" << endl;
            cout << "\nPresiona Enter para continuar...";
            cin.get();
            return;
        }
    }


    while (enemigo.salud > 0 && tienePuchamonVivo(jugador)) {
        // Si el Puchamon actual esta debilitado, forzar cambio
        if (jugador.puchamones[jugador.puchamon_actual].salud <= 0) {
            cout << jugador.puchamones[jugador.puchamon_actual].nombre << " no puede luchar!" << endl;
            cambiarPuchamon(jugador);
            if (!tienePuchamonVivo(jugador)) { // Si después de cambiar, no hay vivos
                cout << "No tienes Puchamon vivos. ¡Perdiste la batalla!" << endl;
                break;
            }
            continue; // Volver al inicio del bucle para el nuevo Puchamon
        }

        // --- Turno del jugador ---
        Puchamon& pucha_actual = jugador.puchamones[jugador.puchamon_actual];
        cout << "\n--- Turno de " << pucha_actual.nombre << " ---" << endl;
        cout << "Salud: " << pucha_actual.salud << "/" << pucha_actual.salud_max
            << " | Energia: " << pucha_actual.puntos << "/" << pucha_actual.puntos_max << endl;
        cout << "Salud de " << enemigo.nombre << ": " << enemigo.salud << "/" << enemigo.salud_max << " | Estado: ";

        if (enemigo.estado_actual != Estado::NINGUNO) {
            switch (enemigo.estado_actual) {
            case Estado::VENENO: cout << "Veneno (" << enemigo.contador_estado << " turnos)"; break;
            case Estado::ELECTROCUTADO: cout << "Electrocutado (" << enemigo.contador_estado << " turnos)"; break;
            case Estado::QUEMADURA: cout << "Quemadura (" << enemigo.contador_estado << " turnos)"; break;
            default: break;
            }
        }
        else {
            cout << "Normal";
        }
        cout << endl;

        cout << "\n¿Que haces?" << endl;
        cout << "1. Atacar" << endl;
        cout << "2. Cambiar Puchamon" << endl;
        cout << "3. Usar objeto" << endl;
        cout << "Elige (numero): ";

        int accion;
        cin >> accion;
        cin.ignore(256, '\n');

        bool turno_valido_jugador = false; // Bandera para controlar si el jugador hizo una acción válida

        if (accion == 1) {
            mostrarHabilidades(pucha_actual);
            cout << "¿Que ataque usas? (numero): ";
            int ataque_idx;
            cin >> ataque_idx;
            cin.ignore(256, '\n');

            if (ataque_idx >= 1 && ataque_idx <= pucha_actual.habilidades.size()) {
                Habilidad ataque_elegido = pucha_actual.habilidades[ataque_idx - 1];
                if (pucha_actual.puntos >= ataque_elegido.costo) { // Verificar PP antes de aplicar daño
                    aplicarDano(pucha_actual, enemigo, ataque_elegido);
                    turno_valido_jugador = true;
                }
                else {
                    cout << pucha_actual.nombre << " no tiene suficiente energia para " << ataque_elegido.nombre << "." << endl;
                }
            }
            else {
                cout << "Ataque invalido." << endl;
            }
        }
        else if (accion == 2) {
            int puchamon_anterior_idx = jugador.puchamon_actual; // Guarda el índice antes del cambio
            cambiarPuchamon(jugador);
            // Si el cambio fue a un Puchamon diferente y válido, se considera turno válido
            if (jugador.puchamon_actual != puchamon_anterior_idx && jugador.puchamones[jugador.puchamon_actual].salud > 0) {
                turno_valido_jugador = true;
            }
            else if (jugador.puchamon_actual == puchamon_anterior_idx) {
                cout << "No cambiaste de Puchamon." << endl;
                // No se consume el turno si no hubo cambio efectivo a otro Pokémon vivo
            }
        }
        else if (accion == 3) {
            // La función usarObjeto ahora maneja si el objeto se usó exitosamente
            // Por simplicidad, asumimos que si se llega a esta línea, se intentó usar un objeto
            // y el turno se consume, independientemente del éxito o no del uso del objeto.
            // Si deseas que solo se consuma el turno si el objeto se USÓ con ÉXITO,
            // la función usarObjeto debería devolver un bool. Por ahora, asumimos que intenta.
            usarObjeto(jugador);
            turno_valido_jugador = true; // Siempre consume el turno por intentar usar objeto
        }
        else {
            cout << "Opcion invalida. Pierdes turno." << endl;
        }

        // Aplicar efectos de estado del enemigo DESPUES de la acción del jugador (si el jugador hizo algo)
        if (turno_valido_jugador) { // Solo si el jugador hizo una acción que consume turno
            aplicarEfectosDeEstado(enemigo);
            if (enemigo.salud <= 0) break; // Si el enemigo cae por un efecto de estado del jugador
        }


        // --- Turno del enemigo ---
        if (enemigo.salud <= 0 || !tienePuchamonVivo(jugador)) { // Si alguien ya ganó o el jugador no tiene Puchamon
            break;
        }

        cout << "\n--- Turno de " << enemigo.nombre << " ---" << endl;
        aplicarEfectosDeEstado(pucha_actual); // Aplica efectos al Puchamon del jugador

        if (pucha_actual.salud <= 0) { // Si el Puchamon del jugador se debilita por un efecto de estado
            cout << pucha_actual.nombre << " se debilito!" << endl;
            continue; // Ir al siguiente bucle para forzar el cambio si hay mas Puchamon vivos
        }

        if (enemigo.estado_actual != Estado::ELECTROCUTADO) {
            Habilidad mejor_ataque;
            bool puede_atacar = false;
            int max_dano = -1;

            // Busca el ataque más dañino que puede usar
            for (const auto& h : enemigo.habilidades) {
                if (enemigo.puntos >= h.costo && h.dano > max_dano) {
                    max_dano = h.dano;
                    mejor_ataque = h;
                    puede_atacar = true;
                }
            }

            // Si no puede usar el más dañino, busca el primero que pueda
            if (!puede_atacar && !enemigo.habilidades.empty()) {
                for (const auto& h : enemigo.habilidades) {
                    if (enemigo.puntos >= h.costo) {
                        mejor_ataque = h;
                        puede_atacar = true;
                        break;
                    }
                }
            }

            if (puede_atacar) {
                aplicarDano(enemigo, pucha_actual, mejor_ataque);
            }
            else {
                cout << enemigo.nombre << " no tiene energia para atacar." << endl;
            }
        }
        else {
            cout << enemigo.nombre << " esta paralizado y no puede moverse." << endl;
            // El contador_estado ya se decrementa en aplicarEfectosDeEstado
        }
    }

    // Resultado final
    cout << "\n--- Fin de la batalla ---" << endl;
    if (enemigo.salud <= 0) {
        cout << "¡Derrotaste a " << enemigo.nombre << "!" << endl;
        // Recompensas por ganar
        // El operador [] de map insertará el elemento si no existe, o accederá a él si ya existe.
        // Luego, simplemente ajustamos el nombre, tipo y acumulamos la cantidad.
        // Esto asegura que la cantidad se suma, no se sobrescribe con un valor fijo si ya existía.
        jugador.mochila["Pocion"].nombre = "Pocion";
        jugador.mochila["Pocion"].tipo = TipoObjeto::CURACION;
        jugador.mochila["Pocion"].cantidad += 2; // Suma 2 a la cantidad actual (o a 0 si se acaba de crear)

        jugador.mochila["Energia X"].nombre = "Energia X";
        jugador.mochila["Energia X"].tipo = TipoObjeto::ENERGIA;
        jugador.mochila["Energia X"].cantidad += 1; // Suma 1 a la cantidad actual (o a 0 si se acaba de crear)

        cout << "Obtienes 2 Pociones y 1 Energia X!" << endl;
    }
    else {
        cout << "¡Perdiste la batalla!" << endl;
    }
    cout << "\nPresiona Enter para continuar...";
    cin.get();
}


int main() {
    // Puchamon iniciales del jugador
    vector<Puchamon> equipo_inicial = {
        {"Pichaku", 120, 120, 60, 100, // Pichaku HP, Max HP, PP, Max PP
          {{"Impactrueno", 35, Estado::ELECTROCUTADO, 30}, {"Ataque Rapido", 25, Estado::NINGUNO, 10}},
          Estado::NINGUNO, 0, true},

        {"Chambander", 110, 110, 60, 90, // Chambander HP, Max HP, PP, Max PP
          {{"Llamarada", 45, Estado::QUEMADURA, 25}, {"Ascuas", 20, Estado::NINGUNO, 10}},
          Estado::NINGUNO, 0, true},

        {"Babososur", 130, 130, 60, 80, // Babososur HP, Max HP, PP, Max PP
          {{"Latigazo", 30, Estado::VENENO, 20}, {"Hoja Afilada", 40, Estado::NINGUNO, 15}},
          Estado::NINGUNO, 0, true}
    };

    // Jefe final (Miaotu) - NOTA: Su salud y puntos se resetean al inicio de cada combate en iniciarCombate()
    Puchamon miaotu_template = { // Usamos un template para resetearlo cada vez
        "Miaotu", 500, 500, 120, 150, // Miaotu HP, Max HP, PP, Max PP
        {{"bola Psiquica", 50, Estado::NINGUNO, 30}, {"Confusion", 30, Estado::ELECTROCUTADO, 25}, {"Rayo psiquico", 60, Estado::NINGUNO, 35}},
        Estado::NINGUNO, 0, false
    };

    // Crear jugador con mochila inicial
    Jugador jugador;
    jugador.nombre = "Entrenador";
    jugador.puchamones = equipo_inicial;
    jugador.puchamon_actual = 0; // El primer Puchamon por defecto

    // Objetos iniciales en la mochila
    jugador.mochila["Pocion"] = { "Pocion", TipoObjeto::CURACION, 3 };
    jugador.mochila["Energia X"] = { "Energia X", TipoObjeto::ENERGIA, 2 };
    jugador.mochila["Piedra de Evolucion"] = { "Piedra de Evolucion", TipoObjeto::EVOLUCION, 1 };

    int opcion;
    do {
        mostrarMenuPrincipal(); // Función del archivo Menu.h
        opcion = obtenerOpcionMenu(); // Función del archivo Menu.h

        switch (opcion) {
        case 1: // Iniciar Nuevo Juego / Combatir
            iniciarCombate(jugador, miaotu_template); // Pasa una copia del template de Miaotu
            // Después del combate, el jugador puede que tenga que cambiar de Puchamon
            // Si el puchamon actual esta debilitado, forzar un cambio si hay mas disponibles
            if (jugador.puchamones[jugador.puchamon_actual].salud <= 0 && tienePuchamonVivo(jugador)) {
                cout << "Tu Puchamon actual esta debilitado. Por favor, elige otro." << endl;
                cambiarPuchamon(jugador);
            }
            break;
        case 2: // Ver Instrucciones
            cout << "--- COMO JUGAR ---" << endl;
            cout << "1. Elige sabiamente tus ataques, ya que gastan energia (PP)." << endl;
            cout << "2. Algunos ataques causan efectos de estado especiales (Veneno, Quemadura, Electrocutado) al enemigo." << endl;
            cout << "3. Puedes cambiar entre tus 3 Puchamon disponibles durante el combate." << endl;
            cout << "4. Usa objetos de tu mochila (Pociones, Energia X) para recuperar salud o energia." << endl;
            cout << "5. ¡Usa las Piedras de Evolucion para hacer que tus Puchamon sean mas fuertes!" << endl;
            cout << "6. ¡Derrota al legendario Miaotu para ganar el juego!" << endl;
            cout << "------------------" << endl;
            cout << "\nPresiona Enter para volver al menu...";
            cin.get(); // Pausa para que el usuario lea
            break;
        case 3: // Salir del Juego
            cout << "¡Gracias por jugar! ¡Hasta luego!" << endl;
            break;
        default:
            cout << "Opcion no valida. Elige 1, 2 o 3." << endl;
            cout << "\nPresiona Enter para continuar...";
            cin.get(); // Pausa para que el usuario lea
            break;
        }
    } while (opcion != 3);

    return 0;
}