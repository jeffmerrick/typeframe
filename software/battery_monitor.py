#!/usr/bin/env python3
"""
Battery monitoring service with notifications
Monitors battery level and sends notifications at key thresholds
"""

import time
import serial
import re
import sys
import subprocess
import os

# Battery Configuration
UART_PORT = "/dev/ttyS0"
BAUDRATE = 115200

# Notification thresholds (in descending order)
NOTIFICATION_LEVELS = [50, 40, 30, 25, 20, 15, 10, 5]

# State file to track notifications and previous level
STATE_FILE = "/tmp/battery_monitor_state"

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

def get_battery_status(num_readings=3):
    ser = None
    try:
        ser = serial.Serial(UART_PORT, BAUDRATE, timeout=3)  # Increased timeout
        current_reading = {}
        expected_keys = {"Power_State", "Running_State", "Vin_Voltage", "Vout_Voltage", "Vout_Current"}
        
        # Try multiple times to get a complete reading
        for attempt in range(50):  # Increased attempts
            try:
                line = ser.readline().decode(errors='ignore').strip()
                if not line:
                    continue
                    
                parsed = parse_uart_line(line)
                if parsed:
                    current_reading.update(parsed)
                    # Check if we have a complete reading
                    if expected_keys.issubset(current_reading.keys()):
                        return current_reading.copy()
                        
            except serial.SerialException as e:
                print(f"Serial exception on attempt {attempt}: {e}", file=sys.stderr)
                break
            except Exception as e:
                print(f"Unexpected error on attempt {attempt}: {e}", file=sys.stderr)
                continue
                
        # If we get here, we didn't get a complete reading
        if current_reading:
            print(f"Incomplete reading after 50 attempts: {current_reading}", file=sys.stderr)
        else:
            print("No UART data received after 50 attempts", file=sys.stderr)
        return {}
        
    except Exception as e:
        print(f"Error opening UART: {e}", file=sys.stderr)
        return {}
    finally:
        if ser and ser.is_open:
            ser.close()

def estimate_percentage(status):
    vin = status.get("Vin_Voltage", 0)
    
    if vin > 4.0:
        percent = 100  # Charging/plugged in
    else:
        # Battery discharge curve mapping
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
    
    return max(0, min(100, percent))

def load_state():
    """Load previous state from file"""
    try:
        if os.path.exists(STATE_FILE):
            with open(STATE_FILE, 'r') as f:
                lines = f.read().strip().split('\n')
                notified_levels = set(map(int, lines[0].split(','))) if lines[0] else set()
                previous_percentage = int(lines[1]) if len(lines) > 1 else 100
                is_charging = lines[2] == 'True' if len(lines) > 2 else False
                return notified_levels, previous_percentage, is_charging
    except:
        pass
    return set(), 100, False

def save_state(notified_levels, percentage, is_charging):
    """Save current state to file"""
    try:
        with open(STATE_FILE, 'w') as f:
            f.write(','.join(map(str, sorted(notified_levels))) + '\n')
            f.write(str(percentage) + '\n')
            f.write(str(is_charging) + '\n')
    except Exception as e:
        print(f"Error saving state: {e}", file=sys.stderr)

def send_notification(title, message, urgency="normal"):
    """Send notification to desktop user via wfpanelctl"""
    try:
        script_path = "/home/pi/Typeframe/send_user_notification.sh"
        
        if urgency == "critical":
            cmd = ['/usr/sbin/runuser', '-u', 'pi', '--', script_path, 'critical', f"{title}: {message}"]
        else:
            cmd = ['/usr/sbin/runuser', '-u', 'pi', '--', script_path, 'notify', f"{title}: {message}"]
        
        result = subprocess.run(cmd, check=False, capture_output=True, text=True)
        print(f"Notification sent: {title} - {message}")
        if result.returncode != 0:
            print(f"Notification command returned: {result.returncode}", file=sys.stderr)
            print(f"Notification stdout: {result.stdout}", file=sys.stderr)
        if result.stderr:
            print(f"Notification stderr: {result.stderr}", file=sys.stderr)
    except Exception as e:
        print(f"Error sending notification: {e}", file=sys.stderr)

def monitor_battery():
    """Main monitoring loop"""
    print("Starting battery monitoring service...")
    
    # Get initial battery status for startup notification
    initial_status = get_battery_status()
    if initial_status:
        initial_percentage = estimate_percentage(initial_status)
        send_notification("Battery Monitor", f"Started - {initial_percentage}% battery", "low")
    else:
        send_notification("Battery Monitor", "Started - Reading battery...", "low")
    
    # Rolling readings for median calculation
    voltage_history = []
    max_history = 5  # Keep last 5 readings for median
    
    # Load previous state
    notified_levels, previous_percentage, was_charging = load_state()
    
    # Track consecutive failures for retry logic
    consecutive_failures = 0
    max_consecutive_failures = 5
    
    # Track if we need to check for missed notifications after reconnection
    had_communication_failure = False
    
    try:
        while True:
            # Get battery status
            status = get_battery_status()
            if not status:
                consecutive_failures += 1
                had_communication_failure = True
                print(f"No battery data available (attempt {consecutive_failures}/{max_consecutive_failures})")
                
                if consecutive_failures >= max_consecutive_failures:
                    print("Too many consecutive failures, waiting longer before retry...")
                    time.sleep(60)  # Wait longer after multiple failures
                    consecutive_failures = 0  # Reset counter
                else:
                    time.sleep(5)  # Quick retry for initial failures
                continue
            
            # Reset failure counter on successful read
            consecutive_failures = 0
            
            current_voltage = status.get("Vin_Voltage", 0)
            
            # Add to voltage history
            voltage_history.append(current_voltage)
            if len(voltage_history) > max_history:
                voltage_history.pop(0)  # Remove oldest reading
            
            # Use median voltage for percentage calculation (immune to outliers)
            sorted_voltages = sorted(voltage_history)
            if len(sorted_voltages) % 2 == 0:
                # Even number of readings - average the middle two
                mid = len(sorted_voltages) // 2
                median_voltage = (sorted_voltages[mid-1] + sorted_voltages[mid]) / 2
            else:
                # Odd number of readings - take the middle one
                median_voltage = sorted_voltages[len(sorted_voltages) // 2]
            
            status_for_calc = status.copy()
            status_for_calc["Vin_Voltage"] = median_voltage
            
            percentage = estimate_percentage(status_for_calc)
            is_charging = median_voltage > 4.1  # Consider charging if voltage is high
            
            print(f"Battery: {percentage}% (Current: {current_voltage:.2f}V, Median: {median_voltage:.2f}V)")
            print(f"Charging state: {is_charging}, Was charging: {was_charging}")
            
            # Check if we just recovered from communication failure
            if had_communication_failure:
                print("Communication restored - checking for missed notifications")
                had_communication_failure = False
                # Force check all notification levels to catch any missed during outage
                for level in NOTIFICATION_LEVELS:
                    if percentage <= level and level not in notified_levels and not is_charging:
                        urgency = "critical" if level <= 10 else "normal"
                        send_notification(
                            "Low Battery Warning",
                            f"Battery level: {percentage}% ({median_voltage:.2f}V) - Missed during communication outage",
                            urgency
                        )
                        notified_levels.add(level)
                        print(f"Sent missed notification for {level}% threshold")
            
            # Check for charging state change
            if is_charging and not was_charging:
                print("State change detected: Plugged in")
                send_notification("Battery", "Device plugged in - Charging", "low")
                # Reset notification levels when plugged in
                notified_levels = set()
            elif not is_charging and was_charging:
                print("State change detected: Unplugged")
                send_notification("Battery", "Device unplugged - Running on battery", "low")
            
            # Only check for low battery notifications when not charging and battery is going down
            if not is_charging and percentage < previous_percentage:
                for level in NOTIFICATION_LEVELS:
                    if percentage <= level and level not in notified_levels:
                        # Determine urgency based on level
                        if level <= 10:
                            urgency = "critical"
                        elif level <= 20:
                            urgency = "normal"
                        else:
                            urgency = "low"
                        
                        send_notification(
                            "Low Battery Warning",
                            f"Battery level: {percentage}% ({median_voltage:.2f}V)",
                            urgency
                        )
                        notified_levels.add(level)
                        break  # Only notify for one level per check
            
            # Save current state
            save_state(notified_levels, percentage, is_charging)
            
            # Update previous values
            previous_percentage = percentage
            was_charging = is_charging
            
            # Check every 5 seconds for responsive monitoring
            time.sleep(5)
                
    except KeyboardInterrupt:
        print("\nStopping battery monitor...")
    except Exception as e:
        print(f"Unexpected error: {e}", file=sys.stderr)
        time.sleep(60)  # Wait before potential restart

if __name__ == "__main__":
    monitor_battery()