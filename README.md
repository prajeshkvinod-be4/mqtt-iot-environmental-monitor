#### MQTT-Based IoT Environmental Monitoring System

\-github.com/prajeshkvinod-be4



An end-to-end IoT telemetry pipeline engineered on native ESP-IDF (FreeRTOS). The system samples environmental metrics via analog sensors, serializes the data into JSON, and transmits it over Wi-Fi via MQTT to a Linux-hosted broker. A Python subscriber service ingests the stream and records into a MongoDB database for live inspection through MongoDB Compass.



#### Core Tech Stack



Firmware: ESP32-C3 running native ESP-IDF (C / FreeRTOS)

Operating System : Ubuntu Linux

Message Broker: Mosquitto (MQTT Broker)

Backend Ingestion: Python (`paho-mqtt` + `pymongo`)

Database: MongoDB (NoSQL Database) \& MongoDB Compass for UI inspection



#### Project Overview & End-to-End Flow

```text
[NTC & MQ-2 Sensors]
       │ (Analog Inputs)
       ▼
[ESP32-C3 Node (ESP-IDF)]
       │ (Wi-Fi / JSON Payload)
       ▼
[Mosquitto MQTT Broker (Ubuntu Linux)]
       │ (Topic Subscription)
       ▼
[Python Ingestion Backend]
       │ (PyMongo Driver)
       ▼
[MongoDB NoSQL Database] ──► [MongoDB Compass (GUI Monitoring)]
```


#### System Data Flow

1\. Sensor Layer: The ESP32-C3 reads analog output from an NTC thermistor (via voltage divider) and an MQ-2 gas sensor using the ESP-IDF ADC driver.



2\. Payload Formatting \& Transport: The readings are structured into a JSON payload and published over Wi-Fi to a Mosquitto MQTT broker running on an Ubuntu instance at the topic `sensor/environment`.



3\. Ingestion Layer: A Python subscriber client (`paho-mqtt`) listening on `sensor/environment` receives the raw JSON stream, deserializes it, and inserts documents into a local MongoDB instance using `pymongo`.



4\. Data Verification: The database collection (`esp32\_database.sensor\_data`) is monitored in real-time using MongoDB Compass.





#### Repository Structure



env\_proj\_be928/

├── docs/                       

│   ├── circuit\_diagram.png

│   ├── system\_architecture.png

│   └── mongodb\_compass.png

├── main/                       # Firmware source code (ESP-IDF)

│   ├── CMakeLists.txt          # Component build configuration

│   ├── main.c                  # Application initialization \& task scheduling

│   ├── sensor.c / .h           # ADC sampling drivers for NTC and MQ-2

│   ├── wifi.c / .h             # Wi-Fi station mode configuration

│   └── mqtt\_app.c / .h         # ESP-MQTT client implementation \& publisher

├── backend/                    # Ingestion backend

│   ├── subscriber.py           # MQTT subscriber script for MongoDB insertion

│   └── requirements.txt        # Python package dependencies

├── .gitignore                  # Exclusion rules for build artifacts and virtual envs

├── CMakeLists.txt              # Top-level ESP-IDF build file

├── sdkconfig                   # ESP-IDF configuration settings

└── README.md                   # System documentation









