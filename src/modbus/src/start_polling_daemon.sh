#!/bin/sh
while true; do

    # 1. Main action execution
    python polling_daemon.py >> modbus.log 2>> error.log

    # 2. Wait fixed 15 minutes
    sleep 900
done
