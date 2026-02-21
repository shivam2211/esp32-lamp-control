# Lamp Switch – ESP32-C3 Bulb Controller

Wi-Fi controller for 5 bulbs or relays on an ESP32-C3. The firmware exposes a REST API, and the web UI lives elsewhere (e.g. GitHub Pages), so you can point it at any ESP32 IP.

## Hardware

- **Board:** ESP32-C3
- **GPIO:** 5 outputs on pins 1, 2, 3, 4, 8 (all safe on C3)
- **Access:** Same Wi-Fi in a browser, or from anywhere via the hosted UI

## API (ESP32 firmware)

- **Framework:** Arduino
- **Server:** WiFiServer on port 80
- **Endpoints:**
  - `GET /led1/on`, `/led1/off` through `/led5/on`, `/led5/off`
  - `GET /all/on`, `GET /all/off`
  - `GET /ping` – health check
  - `GET /` – returns curl examples
- **Serial:** 115200

## Web UI

- Hosted separately (e.g. GitHub Pages)
- Configurable ESP32 IP (default `192.168.1.11`), saved in localStorage
- TailwindCSS via CDN, DM Sans font
- Individual bulb controls plus All On / All Off, plus connection status

## Architecture

- **ESP32:** API only, no HTML baked in
- **UI:** `index.html` fetches from a configurable `baseUrl` (e.g. `http://192.168.1.11`)

## Stability (ESP32-C3)

- Uses only safe GPIOs (1, 2, 3, 4, 8)
- Keeps heap usage low with short API responses
- Non-blocking request handling
