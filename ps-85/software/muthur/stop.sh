#!/bin/bash
pkill -f "python3 lights.py"
source /home/pi/muthur/venv/bin/activate
python3 /home/pi/muthur/lights.py off
echo "MU/TH/UR CONNNECTION TERMINATED"