#!/bin/bash
cd /home/pi/muthur
source venv/bin/activate
python3 lights.py &
echo "MU/TH/UR CONNECTION ESTABLISHED (PID: $!)"