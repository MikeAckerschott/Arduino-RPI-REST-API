import time
import requests
import requests_raw

ARDUINO_HTTP = "192.168.2.3:80"
RPI_HTTP = "192.168.2.2:5000"


# Configure Arduino mode
def set_arduino_mode(mode):
    try:
        content_length = len(mode)
        req = f"PUT /config/mode HTTP/1.0\r\n\Content-Length: {content_length}\r\n\r\n{mode}"
        
        print(req)
    
        response = requests_raw.raw(f"http://{ARDUINO_HTTP}", data=req)
        print(f"Arduino response: {response}")
    except requests.RequestException as e:
        print(f"Failed to set Arduino mode: {e}")


# Pull average from Arduino
def get_avg(sensor_id):
    try:
        response = requests.get(f"http://{ARDUINO_HTTP}/sensors/{sensor_id}/avg")
        response.raise_for_status()
        print(f"Arduino response: {response.json()}")
        return float(response.json())
    except requests.RequestException as e:
        print(f"Failed to fetch data from Arduino: {e}")
        return None
    
def post_to_pi(avg_sensor1, avg_sensor2):
    content_length = len(f"{avg_sensor1} {avg_sensor2}")
    req = f"\
POST /data HTTP/1.0\r\n\
Content-Length: {content_length}\r\n\
\r\n\
{avg_sensor1} {avg_sensor2}"
    response = requests_raw.raw(f"http://{RPI_HTTP}", data=req)
    print(f"Raspberry Pi response: {response}")


def main():
    set_arduino_mode("active")
    # Fill default buffers of size 12
    time.sleep(1.2)
    while True:
        sensor1_avg = get_avg(1)
        sensor2_avg = get_avg(2)
        post_to_pi(sensor1_avg, sensor2_avg)
        time.sleep(12)

if __name__ == "__main__":
    main()