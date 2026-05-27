import serial
import sqlite3
import time

# Conexión con Arduino
arduino = serial.Serial('COM3', 9600)  # Cambia 'COM3' por tu puerto (ej. '/dev/ttyUSB0' en Linux)
time.sleep(2)  # Espera para establecer conexión

# Crear base de datos SQLite
conn = sqlite3.connect('bd')
c = conn.cursor()

print("Leyendo datos desde Arduino y guardando en SQLite...")

try:
    while True:
        if arduino.in_waiting > 0:
            dato = arduino.readline().decode('utf-8').strip()
            try:
                valor = float(dato)
                c.execute("INSERT INTO jugador (id, nombre, puntaje) VALUES (?, ?,?)", (id, nombre, puntaje))
                conn.commit()
                print(f"Guardado: {nombre} V - {puntaje}")
            except ValueError:
                print(f"Dato no válido: {dato}")
except KeyboardInterrupt:
    print("Finalizando...")

finally:MF
    arduino.close()
    conn.close()

