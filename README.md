# proyecto-final- [pensamiento algoritmico]
## 👥 Integrantes del grupo
- Angel Cortes (Angel7952) usuario de github
- Duban Vargaz (Dub4nvf-0827)usuario de github

-# Proyecto: Sistema de Parqueadero Inteligente (Python CLI) =Interfaz de línea de comando
- Simulación de mapa de parqueadero con matriz 8x8.
- Registro automático de entrada y salida de vehículos.
- Cálculo dinámico del tiempo de permanencia y tarifa.
- # Sistema de Parqueadero (Python CLI )=Interfaz de línea de comando
- ## 🚗 Funcionalidades
- mostrar la vista del parqueadero de forma una (matriz visual).
- ingresar los vehiculos al paqueadero.
-  Calcular tiempo de permanencia y tarifa al salir
- calcula la tarifa 1000 por minuto (tiempo rela).
- Control de disponibilidad de espacios.
## 📌 Desarrollos innovadores
### 1. Representación visual tipo mapa
¿Para qué se implementó?
para que el usuario pueda ver visualmente grafica para ver la disponibilidad del parqueadero.
¿Por qué se consideró necesario?
Hace más intuitiva la gestión, permitiendo ver claramente las zonas de entrada, salida, vía, lugar libre (`L`) y ocupado (`X`).
¿Cómo se implementó?
- Se usa una matriz de 8x8.
- 'E' marca la entrada, 'S' la salida.
- 'L' representa lugares libres, 'X' los ocupados.
- Se usa una función para imprimir la matriz al usuario.
### 2. Registro de tiempo con `datetime`
¿Para qué se implementó?
para cobrar de manera precisa por el tiempo de uso del parqueadero
¿Por qué se consideró necesario?
brinda realismo y permite automatizar el cobro del parqueadero
¿Cómo se implementó?
- Se guarda la hora de entrada usando `datetime.now()` se le nombro como una funcion de llamada.
- al salir, se calcula cuanto tiempo estuvo en el parqueadero, se calcula por minuto.
- - Se calcula la tarifa multiplicando por $1000 por hora (proporcionalmente). 
  



