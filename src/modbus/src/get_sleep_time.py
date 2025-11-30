#!/usr/bin/env python3
"""
Utility script to calculate the next sleep duration for the daemon.sh script.
Outputs the number of seconds to sleep to stdout.
"""

import json
import datetime
import sys
from filelock import FileLock 

# Mutex Configuration: Use the SAME lock file as device_controller.py
SCHEDULE_JSON_LOCK = "/tmp/schedule_app.lock" 
SCHEDULE_FILE = "schedule.json" 

def calculate_sleep_time():
    
    # --- WE USE THE MUTEX TO PROTECT THE READING ---
    with FileLock(SCHEDULE_JSON_LOCK):
        try:
            with open(SCHEDULE_FILE, 'r') as f:
                schedule_data = json.load(f)
        except (FileNotFoundError, json.JSONDecodeError) as e:
            print(3600) # Default to 1 hour if file is missing or corrupt
            sys.exit(0)

        # Rest of the time calculation logic (now protected)
        sorted_schedule = sorted(schedule_data, key=lambda x: datetime.datetime.strptime(x['start_time'], "%H:%M").time())
        now = datetime.datetime.now()
        sleep_seconds = 3600

        for entry in sorted_schedule:
            schedule_time = datetime.datetime.strptime(entry['start_time'], "%H:%M").time()
            next_time = datetime.datetime.combine(now.date(), schedule_time)
            if next_time <= now:
                next_time += datetime.timedelta(days=1)
            
            sleep_seconds = (next_time - now).total_seconds()
            break 

        if sleep_seconds < 10:
            sleep_seconds = 10
            
        print(int(sleep_seconds))
    # The lock is automatically released here when exiting the 'with FileLock' block

if __name__ == "__main__":
    calculate_sleep_time()
