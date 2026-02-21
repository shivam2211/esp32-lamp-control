# Lamp Switch — ESP32-C3 Bulb Controller

Wi-Fi controller for 5 bulbs/relays using ESP32-C3. Split architecture: firmware serves a REST API; web UI is hosted separately (e.g. GitHub Pages) with configurable ESP32 IP.

## Hardware

- **Board:** ESP32-C3
- **Control:** 5 GPIO outputs (GPIO 1, 2, 3, 4, 8 — safe for ESP32-C3)
- **Access:** Browser on same Wi-Fi, or remote via hosted UI

## API (ESP32 firmware)

- **Framework:** Arduino
- **Server:** WiFiServer on port 80
- **Endpoints:**
  - `GET /led1/on`, `/led1/off` … `/led5/on`, `/led5/off`
  - `GET /all/on`, `GET /all/off`
  - `GET /ping` — health check
  - `GET /` — returns curl examples
- **Serial:** 115200

## Web UI

- Hosted separately (e.g. GitHub Pages)
- Configurable ESP32 IP (default: `192.168.1.11`), persisted in localStorage
- TailwindCSS via CDN, DM Sans font
- Per-bulb controls + All On / All Off + connection status

## Architecture

- **ESP32:** API-only, no embedded HTML
- **UI:** `index.html` — fetch to configurable `baseUrl` (e.g. `http://192.168.1.11`)

## Stability (ESP32-C3)

- Safe GPIOs only (1, 2, 3, 4, 8)
- No large heap strings; API responses are minimal
- Non-blocking request handling
