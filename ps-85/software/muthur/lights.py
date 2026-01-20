import time
import random
import board
import busio
from adafruit_is31fl3731.charlie_bonnet import CharlieBonnet as Display
import sys

i2c = busio.I2C(board.SCL, board.SDA)
display = Display(i2c)

MIN_BRIGHT = 1
MAX_BRIGHT = 20

if len(sys.argv) > 1 and sys.argv[1] == "off":
    i2c = busio.I2C(board.SCL, board.SDA)
    display = Display(i2c)
    for x in range(display.width):
        for y in range(display.height):
            display.pixel(x, y, 0)
    sys.exit(0)


# ----- cluster generation -----

clusters = []  # each: {pixels, on, next}

num_clusters = int((display.width * display.height) / 4)

for _ in range(num_clusters):
    cx = random.randrange(display.width)
    cy = random.randrange(display.height)

    size = random.randint(3, 7)
    pixels = []

    for _ in range(size):
        x = min(display.width - 1, max(0, cx + random.randint(-1, 1)))
        y = min(display.height - 1, max(0, cy + random.randint(-1, 1)))
        pixels.append((x, y))

    on = random.random() < 0.6

    hold = random.uniform(2.0, 6.0) if on else random.uniform(0.5, 2.0)

    clusters.append({
        "pixels": list(set(pixels)),
        "on": on,
        "next": time.monotonic() + hold,
        "brightness": [random.randint(MIN_BRIGHT, MAX_BRIGHT) for _ in pixels]
    })

# ----- initial draw -----

for c in clusters:
    for (x, y), b in zip(c["pixels"], c["brightness"]):
        display.pixel(x, y, b if c["on"] else 0)

# ----- main loop -----

while True:
    now = time.monotonic()

    for c in clusters:
        if now >= c["next"]:
            c["on"] = not c["on"]
            # Set the next activation time
            hold = random.uniform(2.0, 6.0) if c["on"] else random.uniform(0.5, 2.0)
            c["next"] = now + hold
            # Update the display for this cluster
            for (x, y), b in zip(c["pixels"], c["brightness"]):
                display.pixel(x, y, b if c["on"] else 0)
    time.sleep(0.05)  # Small delay to avoid high CPU usage