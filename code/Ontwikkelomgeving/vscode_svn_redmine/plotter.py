from serial import *
from threading import Thread
import time
import sys
import matplotlib.pyplot as plt
import numpy as np

last_received = ''
data_buffer = []
max_data_points = 100

def receiving(ser):
    global last_received, data_buffer
    buffer = ''

    while True:
        try:
            buffer += ser.read(ser.inWaiting()).decode('utf-8', errors='ignore')
            if '\n' in buffer:
                last_received, buffer = buffer.split('\n')[-2:]
                try:
                    value = int(last_received)
                    if 0 <= value <= 1023:
                        data_buffer.append(value)
                        if len(data_buffer) > max_data_points:
                            data_buffer = data_buffer[-max_data_points:]
                except ValueError:
                    pass
            time.sleep(0.1)
        except SerialException as e:
            print(f"Serial error: {e}")
            break

def plot_data():
    plt.ion()
    fig, ax = plt.subplots()
    ax.set_ylim(0, 1023)
    ax.set_xlim(0, max_data_points)
    line, = ax.plot([], [], 'b-', label="Serial Data")
    ax.set_xlabel('Data Points')
    ax.set_ylabel('Value')
    ax.set_title('Real-Time Plot of Serial Data')
    ax.legend()

    while True:
        if len(data_buffer) > 0:
            line.set_xdata(np.arange(len(data_buffer)))
            line.set_ydata(data_buffer)
            plt.draw()
            plt.pause(0.1)
        time.sleep(0.1)

def main():
    try:
        ser = Serial(
            port='COM4',
            baudrate=9600,
            bytesize=EIGHTBITS,
            parity=PARITY_NONE,
            stopbits=STOPBITS_ONE,
            timeout=0.1,
            xonxoff=0,
            rtscts=0,
            interCharTimeout=None
        )

        thread = Thread(target=receiving, args=(ser,))
        thread.daemon = True
        thread.start()

        plot_thread = Thread(target=plot_data)
        plot_thread.daemon = True
        plot_thread.start()

        while True:
            time.sleep(1)

    except SerialException as e:
        print(f"Error opening serial port: {e}")
    finally:
        print("Exiting gracefully...")
        if ser.is_open:
            ser.close()

if __name__ == '__main__':
    try:
        main()
    except KeyboardInterrupt:
        print("\nProgram interrupted by user. Exiting...")
        sys.exit(0)
