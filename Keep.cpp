import cv2
import numpy as np
import socket

def create_bayer_pattern(image):
    bgr = cv2.split(image)
    height, width = image.shape[:2]
    bayer = np.zeros((height, width), dtype=np.uint8)
    bayer[0::2, 0::2] = bgr[2][0::2, 0::2]  # Red
    bayer[0::2, 1::2] = bgr[1][0::2, 1::2]  # Green on Red row
    bayer[1::2, 0::2] = bgr[1][1::2, 0::2]  # Green on Blue row
    bayer[1::2, 1::2] = bgr[0][1::2, 1::2]  # Blue
    return bayer

# Initialize camera
cap = cv2.VideoCapture(0)
if not cap.isOpened():
    print("Error: Could not open camera.")
    exit()

# Set up socket
ip = "192.168.1.100"  # IP address of the i.MX8MP board
port = 5005
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

while True:
    ret, frame = cap.read()
    if not ret:
        break

    bayer_frame = create_bayer_pattern(frame)
    
    # Send the frame
    sock.sendto(bayer_frame.tobytes(), (ip, port))

    # Display the frame
    cv2.imshow('Bayer Frame', bayer_frame)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# Release camera and close windows
cap.release()
cv2.destroyAllWindows()
sock.close()
