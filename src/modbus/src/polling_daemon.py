"""
Modbus Polling Daemon

Reads device configurations from 'slaves.json' and performs a single Modbus TCP
polling cycle. Designed to be executed periodically via cron or shell script.

Functions:
    get_sensor_reg: Read holding registers from a sensor device.
    set_actuator_reg: Write a value to an actuator register.

Dependencies:
    modbus_client: Provides Modbus communication functions.
    slaves.json: Configuration file with device details.

Configuration File: slaves.json
.. code-block:: json

    [
        {
            "type": "sensor" | "actuator",
            "label": "Device name (str)",
            "ip": "IP address (str)",
            "port": "Modbus port (int)",
            "length": "Number of registers to read (int, sensor only)",
            "register_address": "Target register (int, actuator only)"
        }
    ]

Execution:
    Via cron (recommended):
        */15 * * * * python polling_daemon.py >> modbus.log 2>> error.log

    Via shell script:
        .. code-block:: sh

            #!/bin/sh
            while true; do
                python polling_daemon.py >> modbus.log 2>> error.log
                sleep 900
            done
"""

import json
from modbus_client import get_sensor_reg, set_actuator_reg

def run_polling_daemon():
    '''Start of the polling loop'''
    with open("slaves.json", "r") as f:
        devices = json.load(f)

    # Procesa cada dispositivo
    for device in devices:
        if device["type"] == "sensor":
            get_sensor_reg(device["label"], device["ip"], device["port"], device["length"])
        elif device["type"] == "actuator":
            set_actuator_reg(device["label"], device["ip"], device["port"], device["register_address"], 1)

if __name__ == "__main__":
    run_polling_daemon()
