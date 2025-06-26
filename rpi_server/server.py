from flask import Flask, request, jsonify # type: ignore
import sqlite3
import numpy as np # type: ignore
import argparse

app = Flask(__name__)

def init_db():
    with sqlite3.connect('sensor_data.db') as conn:
        cursor = conn.cursor()
        cursor.execute('''CREATE TABLE IF NOT EXISTS measurements (
                          id INTEGER PRIMARY KEY AUTOINCREMENT,
                          sensor1 REAL,
                          sensor2 REAL)''')
        conn.commit()

@app.route('/data', methods=['POST'])
def receive_data():
    try:
        data = request.data.decode('utf-8').strip()
        sensor1, sensor2 = map(float, data.split())
        with sqlite3.connect('sensor_data.db') as conn:
            cursor = conn.cursor()
            cursor.execute("INSERT INTO measurements (sensor1, sensor2) VALUES (?, ?)", (sensor1, sensor2))
            conn.commit()
        return "Data received", 201
    except Exception as e:
        return str(e), 400

#Calculate b0, b1, b2 and R2
@app.route('/statistics', methods=['GET'])
def get_statistics():
    with sqlite3.connect('sensor_data.db') as conn:
        cursor = conn.cursor()
        cursor.execute("SELECT sensor1 FROM measurements")
        sensor1 = cursor.fetchall()
        cursor.execute("SELECT sensor2 FROM measurements")
        sensor2 = cursor.fetchall()
    
    if len(sensor1) < 3 or len(sensor2) < 3:
        return "Not enough data", 400

    sensor1_values = [x[0] for x in sensor1]
    sensor2_values = [x[0] for x in sensor2]

    # sensor values
    x = np.array(sensor1_values)
    y = np.array(sensor2_values)

    # get b0 b1 and b2?
    coefficients = np.polyfit(x, y, 2)

    # predict y
    y_pred = np.polyval(coefficients, x)

    # calculate average of y
    y_average = np.mean(y)

    # totale variatie en verklaarde variatie
    totalVariance     = np.sum((y - y_average) ** 2)  
    explainedVariance = np.sum((y - y_pred) ** 2)

    #R2
    r2 = 1 - (explainedVariance / totalVariance)

    #parse to http
    result = f"{coefficients[2]:.1f} {coefficients[1]:.1f} {coefficients[0]:.1f} {r2:.1f}"
    return result, 200

@app.route('/statistics', methods=['DELETE'])
def delete_statistics():
    with sqlite3.connect('sensor_data.db') as conn:
        cursor = conn.cursor()
        cursor.execute("DELETE FROM measurements")
        conn.commit()
    return "Data cleared", 201

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description="Run the Flask server.")
    parser.add_argument('--host', type=str, default='192.168.2.2', help='Host IP address to bind the server to.')
    parser.add_argument('--port', type=int, default=5000, help='Port to bind the server to.')
    args = parser.parse_args()

    init_db()
    app.run(host=args.host, port=args.port, debug=True)
