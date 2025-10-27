#!/usr/bin/env python3
"""
NeoPixel battery status indicator for GPIO18
Shows battery level with color gradient and blinking for low battery
"""

import time
import board
import neopixel
import serial
import re
import sys

# NeoPixel Configuration
PIXEL_PIN = board.D18  # GPIO18
NUM_PIXELS = 1
ORDER = neopixel.RGB  # Try RGB instead of GRB

# Battery Configuration (imported from battery_status.py)
UART_PORT = "/dev/ttyS0"
BAUDRATE = 115200

# Initialize the NeoPixel
pixels = neopixel.NeoPixel(PIXEL_PIN, NUM_PIXELS, brightness=0.01, auto_write=False, pixel_order=ORDER)

# Battery reading functions (from battery_status.py)
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
        ser = serial.Serial(UART_PORT, BAUDRATE, timeout=2)
        current_reading = {}
        expected_keys = {"Power_State", "Running_State", "Vin_Voltage", "Vout_Voltage", "Vout_Current"}
        for _ in range(30):  # Try up to 30 lines to get a complete reading
            try:
                line = ser.readline().decode(errors='ignore')
                if not line:
                    continue
                parsed = parse_uart_line(line)
                if parsed:
                    current_reading.update(parsed)
                    if expected_keys.issubset(current_reading.keys()):
                        return current_reading.copy()
            except serial.SerialException:
                break
        return {}
    except Exception as e:
        print(f"Error reading from UART: {e}", file=sys.stderr)
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

# LED Color Functions
def percentage_to_color(percentage):
    """
    Convert battery percentage to RGB color
    100-15%: Green to Red gradient
    15-0%: Red (will blink)
    """
    if percentage <= 15:
        return (255, 0, 0)  # Red for low battery
    
    # Green to Red gradient for 15-100%
    # Normalize to 0-1 range for gradient calculation
    normalized = (percentage - 15) / 85  # 85 = 100-15
    
    # Green (0, 255, 0) to Red (255, 0, 0)
    # At 100% (normalized=1): full green (0, 255, 0)
    # At 15% (normalized=0): full red (255, 0, 0)
    red = int(255 * (1 - normalized))
    green = int(255 * normalized)
    blue = 0
    
    return (red, green, blue)

def set_battery_light(percentage):
    """Set the LED color based on battery percentage"""
    color = percentage_to_color(percentage)
    pixels.fill(color)
    pixels.show()

def blink_low_battery():
    """Blink red for low battery (15% and below)"""
    # Red on
    pixels.fill((255, 0, 0))
    pixels.show()
    time.sleep(0.5)
    
    # Off
    pixels.fill((0, 0, 0))
    pixels.show()
    time.sleep(0.5)

def test_colors():
    """Test the color gradient"""
    print("Testing color gradient...")
    test_percentages = [100, 75, 50, 25, 15, 10, 5, 0]
    
    for percent in test_percentages:
        color = percentage_to_color(percent)
        print(f"{percent}% -> RGB{color}")
        pixels.fill(color)
        pixels.show()
        time.sleep(1)
    
    # Turn off
    pixels.fill((0, 0, 0))
    pixels.show()

def monitor_battery():
    """Continuously monitor battery and update LED"""
    print("Starting battery monitoring...")
    print("Press Ctrl+C to stop")
    
    try:
        while True:
            # Get battery status
            status = get_battery_status()
            if not status:
                print("No battery data available")
                time.sleep(5)
                continue
            
            percentage = estimate_percentage(status)
            voltage = status.get("Vin_Voltage", 0)
            
            print(f"Battery: {percentage}% ({voltage:.2f}V)")
            
            if percentage <= 15:
                # Low battery - blink red
                print("LOW BATTERY - Blinking")
                blink_low_battery()
            else:
                # Normal battery - show gradient color
                set_battery_light(percentage)
                time.sleep(2)  # Update every 2 seconds for normal levels
                
    except KeyboardInterrupt:
        print("\nStopping battery monitor...")
    finally:
        # Turn off LED
        pixels.fill((0, 0, 0))
        pixels.show()

if __name__ == "__main__":
    if len(sys.argv) > 1 and sys.argv[1] == "test":
        test_colors()
    else:
        monitor_battery()