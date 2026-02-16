import cv2
import serial
import numpy as np

# --- CONFIGURATION ---
LAPTOP_IP = "192.168.1.XX"  # <--- CHANGE THIS TO YOUR LAPTOP IP
UART_PORT = "/dev/ttyS0"    # Standard for Pi 3/4/5/Zero
BAUD_RATE = 115200

# 1. Initialize UART
try:
    ser = serial.Serial(UART_PORT, BAUD_RATE, timeout=1)
    print("UART initialized.")
except:
    print("Error: Could not open UART. Did you enable it in raspi-config?")

# 2. GStreamer Pipeline (RTP H264)
gst_out = (
    f"appsrc ! videoconvert ! x264enc tune=zerolatency bitrate=800 speed-preset=ultrafast ! "
    f"rtph264pay ! udpsink host={LAPTOP_IP} port=5000"
)
out = cv2.VideoWriter(gst_out, cv2.CAP_GSTREAMER, 0, 20, (640, 480), True)

# 3. Camera & AI (Haar Cascade is very fast on Pi)
cap = cv2.VideoCapture(0)
cap.set(cv2.CAP_PROP_FRAME_WIDTH, 640)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)
face_cascade = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_frontalface_default.xml')

print(f"Robot online. Streaming to {LAPTOP_IP}...")

try:
    while True:
        ret, frame = cap.read()
        if not ret: break

        # AI Detection
        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        faces = face_cascade.detectMultiScale(gray, 1.1, 4)

        for (x, y, w, h) in faces:
            # Calculate Center
            cx, cy = int(x + w/2), int(y + h/2)
            
            # Map to 0-255 for UART
            val_x = int((cx / 640) * 255)
            val_y = int((cy / 480) * 255)

            # Send UART Packet: B[XXX][YYY]E (Example: B127127E)
            # This format is very easy for an Arduino to parse
            uart_msg = f"B{val_x:03d}{val_y:03d}E\n"
            ser.write(uart_msg.encode())

            # Draw for the Laptop Feed
            cv2.rectangle(frame, (x, y), (x+w, y+h), (0, 255, 0), 2)
            cv2.putText(frame, f"X:{val_x} Y:{val_y}", (x, y-10), 
                        cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 2)

        # Send frame to GStreamer
        out.write(frame)

except KeyboardInterrupt:
    print("Stopping Robot...")
finally:
    cap.release()
    out.release()
    ser.close()