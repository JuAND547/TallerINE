from flask import Flask, request
import sqlite3

app = Flask(__name__)

# Crear la base de datos al iniciar
def inicializar_db():
    conn = sqlite3.connect("juego.db")
    cursor = conn.cursor()

    cursor.execute("""
    CREATE TABLE IF NOT EXISTS jugadores (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        nombre VARCHAR(20),
        puntaje INT
    )
    """)

    conn.commit()
    conn.close()

inicializar_db()
# Ruta para recibir datos de la ESP32
@app.route("/guardar")
def guardar():

    nombre = request.args.get("nom")
    puntaje = request.args.get("pun")

    if nombre is None or puntaje is None:
        return "Faltan parametros", 400

    conn = sqlite3.connect("juego.db")
    cursor = conn.cursor()

    cursor.execute(
        """
        INSERT INTO jugadores
        (nombre, puntaje)
        VALUES (?, ?)
        """,
        (nombre, puntaje)
    )

    conn.commit()
    conn.close()

    return "Datos guardados correctamente"

# Ruta para visualizar los datos
@app.route("/")
def mostrar_datos():

    conn = sqlite3.connect("juego.db")
    cursor = conn.cursor()

    cursor.execute("""
        SELECT id, nombre, puntaje
        FROM jugadores
        ORDER BY id DESC
    """)

    filas = cursor.fetchall()
    conn.close()

    html = """
    <!DOCTYPE html>
    <html>
    <head>
        <title>Ranking</title>
        <style>
            body {
                font-family: Arial, sans-serif;
                margin: 40px;
            }

            table {
                border-collapse: collapse;
                width: 100%;
            }

            th, td {
                border: 1px solid black;
                padding: 8px;
                text-align: center;
            }

            th {
                background-color: #e0e0e0;
            }
        </style>
    </head>
    <body>

        <h1>Ranking</h1>

        <table>
            <tr>
                <th>ID</th>
                <th>nombre</th>
                <th>puntaje</th>
            </tr>
    """

    for fila in filas:
        html += f"""
        <tr>
            <td>{fila[0]}</td>
            <td>{fila[1]}</td>
            <td>{fila[2]}</td>
        </tr>
        """

    html += """
        </table>

    </body>
    </html>
    """

    return html

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000, debug=True)