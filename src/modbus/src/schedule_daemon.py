"""
Device Controller Script.

Reads schedule.json, determines the state, and sends an order.
Uses a JSON-specific filelock, then calls the hardware client which
manages the hardware lock internally.
...
"""

import json
import datetime
from filelock import FileLock
import os
import time
from modbus_client import set_actuator_reg

# Mutex to protect the schedule.json file ONLY
SCHEDULE_JSON_LOCK = "/tmp/schedule_app.lock" 
SCHEDULE_FILE = "schedule.json" 

def read_and_execute_schedule():
    
    entry_to_execute = None
    
    # --- 1. LOCK, READ JSON, AND RELEASE JSON MUTEX ---
    with FileLock(SCHEDULE_JSON_LOCK):
        print(f"Process {os.getpid()}: JSON Mutex acquired for reading schedule.")
        try:
            with open(SCHEDULE_FILE, 'r') as f:
                schedule_data = json.load(f)
        except (FileNotFoundError, json.JSONDecodeError) as e:
            print(f"Error reading schedule file: {e}")
            return
        
        # Logic to find the entry_to_execute for the current time
        now = datetime.datetime.now().time()
        for entry in schedule_data:
            start_time = datetime.datetime.strptime(entry['start_time'], "%H:%M").time()
            if now >= start_time:
                entry_to_execute = entry
            else:
                break
        
        print(f"Process {os.getpid()}: JSON Mutex released.")
    # The JSON mutex is released here.

    # --- 2. USE THE LOADED DATA (OUTSIDE THE JSON MUTEX) ---
    # Now call the Modbus function. This function internally handles the hardware lock.
    if entry_to_execute is not None:
        print(f"Calling set_actuator_reg (will use hardware mutex internally)...")
        set_actuator_reg(
            entry_to_execute["label"], 
            entry_to_execute["ip"], 
            entry_to_execute["port"], 
            entry_to_execute["register_address"], 
            entry_to_execute["valor"]
        )
    else:
        print("No suitable schedule entry found for the current time.")

if __name__ == "__main__":
    read_and_execute_schedule()
