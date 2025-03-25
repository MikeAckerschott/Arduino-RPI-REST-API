from flask import Flask, request, jsonify
import sqlite3
import numpy as np
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

@app.route('/statistics', methods=['GET'])
def get_statistics():
    with sqlite3.connect('sensor_data.db') as conn:
        cursor = conn.cursor()
        cursor.execute("SELECT sensor1, sensor2 FROM measurements")
        data = cursor.fetchall()
    
    if len(data) < 3:
        return "Not enough data", 400
    
    x = np.array([d[0] for d in data])
    y = np.array([d[1] for d in data])
    
    X = np.column_stack((np.ones_like(x), x, x**2))
    coeffs, _, _, _ = np.linalg.lstsq(X, y, rcond=None)
    
    y_pred = X @ coeffs
    ss_res = np.sum((y - y_pred) ** 2)
    ss_tot = np.sum((y - np.mean(y)) ** 2)
    r2 = 1 - (ss_res / ss_tot)
    
    response = f"{coeffs[0]:.1f} {coeffs[1]:.1f} {coeffs[2]:.1f} {r2:.1f}"
    return response, 200

@app.route('/statistics', methods=['DELETE'])
def delete_statistics():
    with sqlite3.connect('sensor_data.db') as conn:
        cursor = conn.cursor()
        cursor.execute("DELETE FROM measurements")
        conn.commit()
    return "Data cleared", 200

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description="Run the Flask server.")
    parser.add_argument('--host', type=str, default='0.0.0.0', help='Host IP address to bind the server to.')
    parser.add_argument('--port', type=int, default=5000, help='Port to bind the server to.')
    args = parser.parse_args()

    init_db()
    app.run(host=args.host, port=args.port, debug=True)
