import time
from datetime import datetime

# Dimensiones del parqueadero
filas = 8
columnas = 8

# Crear el mapa del parqueadero (una matriz de texto)
parqueadero = [['V' for _ in range(columnas)] for _ in range(filas)]
parqueadero[0][0] = 'E'  # Entrada
parqueadero[filas-1][columnas-1] = 'S'  # Salida

# Colocar parqueaderos conectados por vías
for fila in range(1, filas-1):
    parqueadero[fila][1] = 'V'  # Vía
    parqueadero[fila][2] = 'L'  # Lugar libre de parqueo (antes 'P')

# Diccionario para registrar vehículos
vehiculos = {}

# Mostrar el mapa
def mostrar_parqueadero():
    print("\nEstado del parqueadero:")
    for fila in parqueadero:
        print(" ".join(fila))

# Buscar un lugar libre y marcarlo como ocupado
def ocupar_lugar():
    for fila in range(1, filas-1):
        if parqueadero[fila][2] == 'L':
            parqueadero[fila][2] = 'X'
            return True
    return False

# Liberar un lugar ocupado
def liberar_lugar():
    for fila in range(1, filas-1):
        if parqueadero[fila][2] == 'X':
            parqueadero[fila][2] = 'L'
            return True
    return False

# Registrar entrada de vehículo
def ingresar_vehiculo():
    placa = input("Ingrese la placa del vehículo: ").upper()
    if placa in vehiculos:
        print("Ese vehículo ya está registrado.")
    else:
        if ocupar_lugar():
            hora_entrada = datetime.now()
            vehiculos[placa] = hora_entrada
            print(f"Vehículo {placa} ingresó a las {hora_entrada.strftime('%H:%M:%S')}.")
        else:
            print("No hay lugares disponibles.")

# Registrar salida de vehículo
def sacar_vehiculo():
    placa = input("Ingrese la placa del vehículo: ").upper()
    if placa in vehiculos:
        hora_entrada = vehiculos[placa]
        hora_salida = datetime.now()
        tiempo_en_minutos = (hora_salida - hora_entrada).total_seconds() / 60
        tarifa = round((tiempo_en_minutos / 60) * 1000)  # $1000 por hora
        liberar_lugar()
        print(f"Vehículo {placa} salió a las {hora_salida.strftime('%H:%M:%S')}")
        print(f"Tiempo de permanencia: {tiempo_en_minutos:.2f} minutos")
        print(f"Total a pagar: ${tarifa}")
        del vehiculos[placa]
    else:
        print("Vehículo no encontrado.")

# Menú del sistema
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
            ingresar_vehiculo()
        elif opcion == '3':
            sacar_vehiculo()
        elif opcion == '4':
            print("Gracias por usar el sistema de parqueadero.")
            break
        else:
            print("Opción inválida. Intente de nuevo.")

# Iniciar el programa
mostrar_menu()