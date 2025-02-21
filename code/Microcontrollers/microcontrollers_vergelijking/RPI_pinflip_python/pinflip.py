import RPi.GPIO as GPIO
import time

# Set up the GPIO mode
GPIO.setmode(GPIO.BCM)

# Set up pin 2 as an output pin
GPIO.setup(2, GPIO.OUT)

try:
    while True:
        # Toggle pin 2 between HIGH and LOW
        GPIO.output(2, GPIO.HIGH)
        GPIO.output(2, GPIO.LOW)

except KeyboardInterrupt:
    # Cleanup on program exit (to reset the GPIO pins properly)
    GPIO.cleanup()
