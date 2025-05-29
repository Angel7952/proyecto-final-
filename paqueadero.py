import time
from datetime import datetime

# Parámetros del parqueadero
filas = 8
columnas = 8

# Crear el mapa inicial (lista de listas)
mapa = [['V' for _ in range(columnas)] for _ in range(filas)]
mapa[0][0] = 'E'  # Entrada
mapa[filas-1][columnas-1] = 'S'  # Salida

# Colocar parqueaderos conectados por vía
for i in range(1, filas-1):
    mapa[i][1] = 'V'  # Vía
    mapa[i][2] = 'P'  # Parqueadero libre

# Diccionario para guardar los vehículos
vehiculos = {}

# Mostrar mapa
def mostrar_mapa():
    print("\nMapa del parqueadero:")
    for fila in mapa:
        print(" ".join(fila))

# Buscar parqueadero libre y ocuparlo
def ocupar_espacio():
    for i in range(1, filas-1):
        if mapa[i][2] == 'P':
            mapa[i][2] = 'X'
            return True
    return False

# Liberar un espacio ocupado
def liberar_espacio():
    for i in range(1, filas-1):
        if mapa[i][2] == 'X':
            mapa[i][2] = 'P'
            return True
    return False

# Ingresar vehículo
def ingresar_vehiculo():
    placa = input("Ingrese la placa del vehículo: ").upper()
    if placa in vehiculos:
        print("El vehículo ya está registrado.")
    else:
        if ocupar_espacio():
            hora_entrada = datetime.now()
            vehiculos[placa] = hora_entrada
            print(f"Vehículo {placa} ingresado a las {hora_entrada.strftime('%H:%M:%S')}.")
        else:
            print("No hay espacios disponibles.")

# Sacar vehículo
def sacar_vehiculo():
    placa = input("Ingrese la placa del vehículo: ").upper()
    if placa in vehiculos:
        hora_entrada = vehiculos[placa]
        hora_salida = datetime.now()
        tiempo = (hora_salida - hora_entrada).total_seconds() / 60  # en minutos
        tarifa = round((tiempo / 60) * 1000)  # 1000 por hora
        liberar_espacio()
        print(f"Vehículo {placa} salió a las {hora_salida.strftime('%H:%M:%S')}")
        print(f"Tiempo en parqueadero: {tiempo:.2f} minutos")
        print(f"Total a pagar: ${tarifa}")
        del vehiculos[placa]
    else:
        print("Vehículo no encontrado.")

# Menú principal
def menu():
    while True:
        print("\n--- Menú ---")
        print("1. Mostrar mapa")
        print("2. Ingresar vehículo")
        print("3. Sacar vehículo")
        print("4. Salir")
        opcion = input("Seleccione una opción: ")

        if opcion == '1':
            mostrar_mapa()
        elif opcion == '2':
            ingresar_vehiculo()
        elif opcion == '3':
            sacar_vehiculo()
        elif opcion == '4':
            print("Gracias por usar el sistema.")
            break
        else:
            print("Opción inválida. Intente de nuevo.")

# Iniciar programa
menu()
