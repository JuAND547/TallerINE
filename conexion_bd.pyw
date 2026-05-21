import sqlite3
import serial
import time
sqlite3.connect('BD.db')
conn = sqlite3.connect('BD.db')
arduino = serial.Serial('COM3', 9600)  # Cambia 'COM3' por tu puerto 
c = conn.cursor()
c.execute('DROP TABLE IF EXISTS tarjeta')
c.execute('''CREATE TABLE  IF NOT EXISTS tarjeta
                (id INTEGER PRIMARY KEY AUTOINCREMENT,
                fecha TEXT,
                codigo TEXT)''')
conn.commit()
print("Leyendo datos desde Arduino y guardando en SQLite...")

try:
    while True:
        if arduino.in_waiting > 0:
            dato = arduino.readline().decode('utf-8').strip()
            try:
                valor = int(dato)
                fecha = time.strftime('%Y-%m-%d %H:%M:%S')
                c.execute("INSERT INTO tarjeta (fecha, codigo) VALUES (?, ?)", (fecha, valor))
                conn.commit()
                print(f"Guardado: {valor} - {fecha}")
                if (valor == 438515311):  # Reemplaza con el valor que se desea verificar
                    print("¡Hola!")
            except ValueError:
                c.execute("SELECT codigo FROM tarjeta")
                print(c.fetchall())
except KeyboardInterrupt:
    print("Finalizando...")

finally:
    arduino.close()
    conn.close()