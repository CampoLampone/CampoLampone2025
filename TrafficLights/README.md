# Traffic lights
The traffic lights of Campo Lampone's city can be controlled by any Wifi capable MCU, in the case of Campo Lampone 2025 it was ESP32-S3 with MicroPython.
All of them depend on a central server with the schedule in it, they also sync to NTP.

Every controller has it's own `CONTROLLER_NAME` under which it tries to download it's config on start.

## Server
The shedule is kept in a JSON file by the [main server](server/main.py). The server is written in Python using FastAPI.
It has a relatively friendly frontend made in React with Vite coded with the help of an LLM. Its code is [here](campo/).

Both of them can be conveniently started at the same time in Docker using:
```bash
docker compose up -d
```
That opens the backend on port `8000` and an Nginx server serving a statically built version of the frontend on the port `8080`.

## Controllers
After installing MicroPython on your Wifi capable MCU of choice, you can install `main.py` on the device using MicroPython's `mpremote` utility.
Before copying the code on the device, you should replace these placeholders in the code:
- `WIFI_SSID`
- `WIFI_PASSWORD`
- `BACKEND_URL`
- `CONTROLLER_NAME`
