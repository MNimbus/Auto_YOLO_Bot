from rpicam_apps import RPICamApp
import serial
import cv2
import numpy as np

# --- CONFIGURATION ---
LAPTOP_IP = "192.168.1.XX"  # Laptop IP
UART_PORT = "/dev/ttyS0"    # The UART out for the pi
BAUD_RATE = 115200

# --- Initialize UART ---
try:
    ser = serial.Serial(UART_PORT, BAUD_RATE, timeout=1) # Open serial lines
    print("UART initialized.")
except:
    print("Error: Could not open UART. Did you enable it in raspi-config?")

# --- GStreamer Pipeline (RTP H264) ---
gst_out = (
    f"appsrc ! videoconvert ! x264enc tune=zerolatency bitrate=800 speed-preset=ultrafast ! " #Directions for the Gstreamer pipeline
    f"rtph264pay ! udpsink host={LAPTOP_IP} port=5000"
)
out = cv2.VideoWriter(gst_out, cv2.CAP_GSTREAMER, 0, 20, (640, 480), True)

def get_steering_command(cx):
    #Screen is 640 pixels wide, we divide it into three quadrants
    if cx < 213:          # Left third of 640
        return "L"
    elif cx > 426:        # Right third of 640
        return "R"
    else:                 # Middle third
        return "F"        # F for Forward (Target is centered)

# --- AI CAMERA INITIALIZATION ---
# 'post_process_file' tells the camera chip to use your custom model
app = RPiCamApp()
app.start(
    post_process_file="/home/pi/robot_follower.json", # Link to your model config
    width=640, height=480
)

print("AI Camera Active. Tracking other robots...")

try:
    while True:
        # Get the latest frame and the AI data (metadata)
        request = app.get_request()
        if request is None: continue
        
        # 'metadata' contains the bounding boxes found by the IMX500 chip
        detections = request.get_metadata().get("object_detect.results", [])

        if not detections:
            ser.write(b"S\n") # Stop if nothing found
        else:
            # Get the first detected robot
            robot = detections[0]
            # Coordinates are usually normalized 0.0 to 1.0 by the chip
            box = robot['box'] 
            cx_percent = (box[0] + box[2]) / 2 # (xmin + xmax) / 2
            
            # Convert percentage to pixels (0.5 * 640 = 320)
            cx_pixels = cx_percent * 640
            
            command = get_steering_command(cx_pixels)
            ser.write(f"{command}\n".encode())
            
        app.release_request(request)

            # Draw for the Laptop Feed
            color = (0, 255, 0) if command == "F" else (0, 0, 255)
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