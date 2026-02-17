import cv2            # ADDED: Missing import
import serial
import numpy as np
from rpicam_apps import RPiCamApp # Fixed capitalization to match instantiation

# --- CONFIGURATION ---
LAPTOP_IP = "192.168.1.XX"  # <--- CHANGE TO LAPTOP IP
UART_PORT = "/dev/ttyS0"    
BAUD_RATE = 115200

# --- Initialize UART ---
try:
    ser = serial.Serial(UART_PORT, BAUD_RATE, timeout=1) 
    print("UART initialized.")
except Exception as e:
    print(f"Error: Could not open UART. {e}")

# --- GStreamer Pipeline (RTP H264) ---
gst_out = (
    f"appsrc ! videoconvert ! x264enc tune=zerolatency bitrate=800 speed-preset=ultrafast ! " 
    f"rtph264pay ! udpsink host={LAPTOP_IP} port=5000"
)
out = cv2.VideoWriter(gst_out, cv2.CAP_GSTREAMER, 0, 20, (640, 480), True)

def get_steering_command(cx):
    if cx < 213:          
        return "L"
    elif cx > 426:        
        return "R"
    else:                 
        return "F"        

# --- AI CAMERA INITIALIZATION ---
app = RPiCamApp()
app.start(
    post_process_file="/home/pi/robot_follower.json", 
    width=640, height=480
)

print(f"AI Camera Active. Streaming to {LAPTOP_IP} and Tracking...")

try:
    while True:
        request = app.get_request()
        if request is None: continue
        
        # 1. ADDED: Extract the actual picture (frame) so we can draw on it and stream it
        # The specific method to get the array depends slightly on your libcamera version, 
        # but it is usually acquired via the main stream payload:
        frame = request.make_array("main") 
        
        # 2. Get the metadata (the AI boxes)
        detections = request.get_metadata().get("object_detect.results", [])

        if not detections:
            ser.write(b"S\n") 
        else:
            robot = detections[0]
            box = robot['box'] # Format: [xmin, ymin, xmax, ymax] as percentages (0.0 to 1.0)
            
            # 3. ADDED: Convert percentage coordinates to actual pixel numbers for OpenCV
            x = int(box[0] * 640)
            y = int(box[1] * 480)
            w = int((box[2] - box[0]) * 640)
            h = int((box[3] - box[1]) * 480)
            
            cx_pixels = int(x + w/2)
            
            # Send UART Command
            command = get_steering_command(cx_pixels)
            ser.write(f"{command}\n".encode())
            
            # 4. FIXED: Draw on the frame using the corrected variables
            color = (0, 255, 0) if command == "F" else (0, 0, 255)
            cv2.rectangle(frame, (x, y), (x+w, y+h), color, 2)
            cv2.putText(frame, f"CMD: {command}", (x, y-10), 
                        cv2.FONT_HERSHEY_SIMPLEX, 0.8, color, 2)
            
        # We are done with the camera's memory buffer for this frame
        app.release_request(request)

        # 5. Send the finished picture to GStreamer
        # Make sure the frame is in BGR format for OpenCV/GStreamer if the camera gave it in RGB
        frame_bgr = cv2.cvtColor(frame, cv2.COLOR_RGB2BGR)
        out.write(frame_bgr)

except KeyboardInterrupt:
    print("Stopping Robot...")
finally:
    # FIXED: Clean up the AI camera and GStreamer, not 'cap'
    app.stop() 
    out.release()
    ser.close()