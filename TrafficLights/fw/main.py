# "defines" - always check before deploy
WIFI_SSID = "<YOUR SSID>"
WIFI_PASSWORD = "<YOUR PASSWD>"

BACKEND_URL = "http://192.168.1.156:8000"

CONTROLLER_NAME = "controller1"
NUM_LIGHTS = 2

# setup network
import machine
import network

wlan = network.WLAN()
wlan.active(True)
if not wlan.isconnected():
    print('connecting to network...')
    wlan.connect(WIFI_SSID, WIFI_PASSWORD)
    while not wlan.isconnected():
        machine.idle()
print('network config:', wlan.ipconfig('addr4'))

# sync time
from machine import RTC
import ntptime

rtc = RTC()
ntptime.settime()
print("Datetime: " + str(rtc.datetime()))

# get JSON
import requests
import json

url = BACKEND_URL + "/controllers/" + CONTROLLER_NAME

data = None

try:
    r = requests.get(url)
    if r.status_code == 200:
        data = json.loads(r.text)
        print("Received JSON:", data)
    else:
        print("HTTP Error:", r.status_code)
    r.close()  # important to free memory
except Exception as e:
    print("Request failed:", e)

# setup traffic lights

from machine import Pin, Timer
import time

class Light:
    def __init__(self, red, yellow, green):
        self.red = red
        self.yellow = yellow
        self.green = green
        self.last_outage = False

    def tick_outage(self):
        if not self.last_outage:
            self.last_outage = True
            self.red.off()
            self.yellow.on()
            self.green.off()
        else:
            self.yellow.toggle()

    def set_phase(self, phase):
        self.last_outage = False
        phase_states = {
            "red":       (1, 0, 0),
            "redyellow": (1, 1, 0),
            "yellow":    (0, 1, 0),
            "green":     (0, 0, 1),
            "off":       (0, 0, 0),
        }

        # Get the current state, default to all off if unknown
        r_state, y_state, g_state = phase_states.get(phase, (0, 0, 0))

        self.red.on() if r_state else self.red.off()
        self.yellow.on() if y_state else self.yellow.off()
        self.green.on() if g_state else self.green.off()

lights = [Light(Pin(0, Pin.OUT), Pin(2, Pin.OUT), Pin(3, Pin.OUT))]

schedule_lengths = []

if type(data['lights']) is list:
    for light in data['lights']:
        phases = light['phases']
        schedule_lengths.append(0)
        for phase in phases:
            schedule_lengths[-1] += phase['duration']
else:
    raise ValueError("Invalid data format")

print(schedule_lengths)

# setup timer and start ticking

def tick(timer):
    micropython_epoch_offset = 946684800
    epoch = time.time() + micropython_epoch_offset
    for i in range(NUM_LIGHTS):
        if i >= len(lights):
            break

        if data['outages']['enabled'] and (epoch % data['outages']['period']) < data['outages']['duration']:
            print("Outage")
            lights[i].tick_outage()
            continue

        tick = epoch % schedule_lengths[i]
        for phase in data['lights'][i]['phases']:
            if tick < phase['duration']:
                print(f"Light {i} phase: {phase['color']}")
                lights[i].set_phase(phase['color'])
                break
            tick -= phase['duration']
    # print("Tick at", rtc.datetime())

us = rtc.datetime()[7]
delay = 1000000 - (us % 1000000)  # ms until next full second
time.sleep_us(delay - 1200) # 1200 cuz it seemed like the time it will take

# Start aligned hardware timer
tim = Timer(0)
tim.init(period=1000, mode=Timer.PERIODIC, callback=tick)

while True:
    time.sleep(1)
