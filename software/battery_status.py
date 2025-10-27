#!/usr/bin/env python3
import serial
import re
import sys
import subprocess
import os

# UART device and battery capacity
UART_PORT = "/dev/ttyS0"
BAUDRATE = 115200
BATTERY_CAPACITY_MAH = 10000

# Regex patterns for parsing UART output
def parse_uart_line(line):
    patterns = {
        "Power_State": r"Power_State\s*:\s*(\d+)",
        "Running_State": r"Running_State\s*:\s*(\d+)",
        "Vin_Voltage": r"Vin_Voltage\(V\)\s*:\s*([\d.]+)",
        "Vout_Voltage": r"Vout_Voltage\(V\)\s*:\s*([\d.]+)",
        "Vout_Current": r"Vout_Current\(MA\)\s*:\s*([\d.]+)"
    }
    result = {}
    for key, pat in patterns.items():
        m = re.search(pat, line)
        if m:
            result[key] = float(m.group(1)) if "Voltage" in key or "Current" in key else int(m.group(1))
    return result

def get_battery_status(num_readings=5):
    ser = None
    try:
        ser = serial.Serial(UART_PORT, BAUDRATE, timeout=2)
        complete_readings = []
        current_reading = {}
        
        # Collect multiple complete readings
        for _ in range(50):  # Read up to 50 lines to get complete sets
            try:
                line = ser.readline().decode(errors='ignore')
                if not line:  # No data received
                    continue
                parsed = parse_uart_line(line)
                if parsed:  # If we got some data from this line
                    # Add parsed data to current reading
                    current_reading.update(parsed)
                    
                    # Check if we have a complete reading (all 5 values)
                    expected_keys = {"Power_State", "Running_State", "Vin_Voltage", "Vout_Voltage", "Vout_Current"}
                    if expected_keys.issubset(current_reading.keys()):
                        complete_readings.append(current_reading.copy())
                        current_reading = {}  # Start fresh for next reading
                        if len(complete_readings) >= num_readings:
                            break
            except serial.SerialException:
                break  # Stop reading on serial error
        
        # Average the readings
        if not complete_readings:
            return {}
        
        averaged = {}
        for key in expected_keys:
            if key in ["Power_State", "Running_State"]:
                # For state values, use the most recent
                averaged[key] = complete_readings[-1].get(key, 0)
            else:
                # For voltage/current, average them
                values = [r.get(key, 0) for r in complete_readings if key in r]
                averaged[key] = sum(values) / len(values) if values else 0
        
        return averaged
        
    except Exception as e:
        print(f"Error reading from UART: {e}", file=sys.stderr)
        return {}
    finally:
        if ser and ser.is_open:
            ser.close()

def estimate_percentage(status):
    # Use averaged Vin_Voltage from multiple UART readings
    vin = status.get("Vin_Voltage", 0)
    
    if vin > 4.0:
        # Device is plugged in/charging - we don't know the battery percentage
        percent = 100
    else:
        # Device is running on battery - 4.0V = 100%, 3.2V = 0%
        # Piecewise mapping based on Li-Po discharge curve
        if vin >= 4.0:
            percent = 100
        elif vin >= 3.8:
            percent = 80 + int((vin - 3.8) / 0.2 * 20)
        elif vin >= 3.7:
            percent = 60 + int((vin - 3.7) / 0.1 * 20)
        elif vin >= 3.6:
            percent = 40 + int((vin - 3.6) / 0.1 * 20)
        elif vin >= 3.4:
            percent = 20 + int((vin - 3.4) / 0.2 * 20)
        elif vin >= 3.2:
            percent = int((vin - 3.2) / 0.2 * 20)
        else:
            percent = 0
    
    percent = max(0, min(100, percent))
    return percent

def show_details(status):
    print("Battery Details:")
    print(f"Power_State: {status.get('Power_State', 'N/A')}")
    print(f"Running_State: {status.get('Running_State', 'N/A')}")
    print(f"Vin_Voltage(V): {status.get('Vin_Voltage', 'N/A')}")
    print(f"Vout_Voltage(V): {status.get('Vout_Voltage', 'N/A')}")
    print(f"Vout_Current(mA): {status.get('Vout_Current', 'N/A')}")

def debug_uart():
    """Debug function to see raw UART output"""
    ser = None
    try:
        ser = serial.Serial(UART_PORT, BAUDRATE, timeout=2)
        print("Raw UART output (10 lines):")
        for i in range(10):
            line = ser.readline().decode(errors='ignore').strip()
            if line:
                print(f"{i+1}: {line}")
                parsed = parse_uart_line(line)
                if parsed:
                    print(f"   Parsed: {parsed}")
    except Exception as e:
        print(f"Error: {e}")
    finally:
        if ser and ser.is_open:
            ser.close()

def main():
    status = get_battery_status()
    percent = estimate_percentage(status)
    if len(sys.argv) > 1 and sys.argv[1] == "details":
        show_details(status)
    elif len(sys.argv) > 1 and sys.argv[1] == "debug":
        # Debug mode - show raw UART output
        debug_uart()
    else:
        print(percent)

if __name__ == "__main__":
    main()
