#!/bin/sh
# daemon.sh
# Runs continuously to manage the schedule without using cron.

PYTHON_CMD=$(which python3)
GET_TIME_SCRIPT="./get_sleep_time.py"
ACTION_SCRIPT="./device_controller.py"
LOG_FILE="/var/log/schedule_daemon.log"

echo "Starting schedule daemon..." >> $LOG_FILE

while true; do
    echo "--- $(date) ---" >> $LOG_FILE
    
    # 1. Execute the main action (device_controller.py)
    # This locks schedule_app.lock, then releases it.
    $PYTHON_CMD $ACTION_SCRIPT >> $LOG_FILE 2>&1

    # 2. Get the calculated sleep time.
    # This script waits for schedule_app.lock to be free (which it will be).
    SLEEP_SECONDS=$($PYTHON_CMD $GET_TIME_SCRIPT)
    
    echo "Next execution in $SLEEP_SECONDS seconds." >> $LOG_FILE

    # 3. Wait for the calculated amount of time
    sleep $SLEEP_SECONDS
done
