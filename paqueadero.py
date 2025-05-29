from datetime import datetime

# Constantes
FILAS = 8
COLUMNAS = 8
TARIFA_POR_HORA = 1000

# Crear el mapa del parqueadero
parqueadero = [['V' for _ in range(COLUMNAS)] for _ in range(FILAS)]
parqueadero[0][0] = 'E'  # Entrada
parqueadero[FILAS - 1][COLUMNAS - 1] = 'S'  # Salida

# Marcar vías y lugares libres
for fila in range(1, FILAS - 1):
    parqueadero[fila][1] = 'V'
    parqueadero[fila][2] = 'L'

# Diccionario de vehículos {placa: hora_entrada}
vehiculos = {}

def mostrar_parqueadero():
    print("\nEstado del parqueadero:")
    for fila in parqueadero:
        print(" ".join(fila))

def buscar_lugar_libre():
    for fila in range(1, FILAS - 1):
        if parqueadero[fila][2] == 'L':
            return fila
    return None

def ocupar_lugar():
    fila_libre = buscar_lugar_libre()
    if fila_libre is not None:
        parqueadero[fila_libre][2] = 'X'
        return True
    return False

def liberar_lugar():
    for fila in range(1, FILAS - 1):
        if parqueadero[fila][2] == 'X':
            parqueadero[fila][2] = 'L'
            return True
    return False

def ingresar_vehiculo(placa):
    placa = placa.strip().upper()
    if not placa:
        return "Placa inválida."

    if placa in vehiculos:
        return f"El vehículo con placa {placa} ya está registrado."

    if ocupar_lugar():
        hora_entrada = datetime.now()
        vehiculos[placa] = hora_entrada
        return f"Vehículo {placa} ingresó a las {hora_entrada.strftime('%H:%M:%S')}."
    else:
        return "No hay lugares disponibles."

def sacar_vehiculo(placa):
    placa = placa.strip().upper()
    if placa not in vehiculos:
        return "Vehículo no encontrado."

    hora_entrada = vehiculos[placa]
    hora_salida = datetime.now()
    minutos = (hora_salida - hora_entrada).total_seconds() / 60
    tarifa = round((minutos / 60) * TARIFA_POR_HORA)
    liberar_lugar()
    del vehiculos[placa]

    return (
        f"Vehículo {placa} salió a las {hora_salida.strftime('%H:%M:%S')}\n"
        f"Tiempo de permanencia: {minutos:.2f} minutos\n"
        f"Total a pagar: ${tarifa}"
    )

def mostrar_menu():
    while True:
        print("\n--- Menú del Parqueadero ---")
        print("1. Mostrar estado del parqueadero")
        print("2. Ingresar vehículo")
        print("3. Sacar vehículo")
        print("4. Salir")
        opcion = input("Seleccione una opción: ")

        if opcion == '1':
            mostrar_parqueadero()
        elif opcion == '2':
            placa = input("Ingrese la placa del vehículo: ")
            print(ingresar_vehiculo(placa))
        elif opcion == '3':
            placa = input("Ingrese la placa del vehículo: ")
            print(sacar_vehiculo(placa))
        elif opcion == '4':
            print("Gracias por usar el sistema de parqueadero.")
            break
        else:
            print("Opción inválida. Intente de nuevo.")

if __name__ == "__main__":
    mostrar_menu()
