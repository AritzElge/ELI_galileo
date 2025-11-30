#!/bin/sh
while true; do
    python polling_daemon.py >> modbus.log 2>> error.log
    sleep 900
done
