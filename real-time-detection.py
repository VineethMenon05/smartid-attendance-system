import cv2
import numpy as np
import face_recognition
import os
import threading

# Define the path for training images - use current directory
path = os.path.dirname(os.path.abspath(__file__))

images = []
classNames = []

# Read the training images and store them
image_extensions = ['.jpg', '.jpeg', '.png']
for file in os.listdir(path):
    extension = os.path.splitext(file)[1].lower()
    if extension in image_extensions:
        image_path = os.path.join(path, file)
        image = cv2.imread(image_path)
        if image is not None:
            images.append(image)
            classNames.append(os.path.splitext(file)[0])

print("Loaded classes:", classNames)

# Scale factor for resizing frames
scale = 0.25
box_multiplier = 1 / scale

# Function to encode faces from images
def findEncodings(images):
    encodeList = []
    for img in images:
        img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
        encodes = face_recognition.face_encodings(img)
        if len(encodes) > 0:  # Check if at least one face is found
            encodeList.append(encodes[0])
        else:
            print("Warning: No face detected in one of the training images.")
    return encodeList

if len(images) == 0:
    print("No valid images found. Please add some images to the directory.")
    exit()

# Find encodings of training images
print('Starting encodings...')
knownEncodes = findEncodings(images)
print('Encoding Complete')

# Start video capture
cap = cv2.VideoCapture(0)
if not cap.isOpened():
    print("Error: Could not open camera.")
    exit()

# Initialize variables
face_locations = []
face_encodes = []
frame_count = 0

# Process video frames
while True:
    success, img = cap.read()
    if not success:
        print("Failed to grab frame")
        break
    
    # Resize the frame for faster processing
    small_frame = cv2.resize(img, (0, 0), None, scale, scale)
    rgb_small_frame = cv2.cvtColor(small_frame, cv2.COLOR_BGR2RGB)

    # Only process face encodings every 5 frames
    if frame_count % 5 == 0:
        face_locations = face_recognition.face_locations(rgb_small_frame, model='hog')
        face_encodes = face_recognition.face_encodings(rgb_small_frame, face_locations)
    
    # Process each detected face
    for encodeFace, faceLocation in zip(face_encodes, face_locations):
        matches = face_recognition.compare_faces(knownEncodes, encodeFace, tolerance=0.6)
        faceDis = face_recognition.face_distance(knownEncodes, encodeFace)
        
        # If there are any matches
        if len(faceDis) > 0:
            matchIndex = np.argmin(faceDis)
            name = classNames[matchIndex].upper() if matches[matchIndex] else 'Unknown'
        else:
            name = 'Unknown'

        y1, x2, y2, x1 = faceLocation
        y1, x2, y2, x1 = int(y1 * box_multiplier), int(x2 * box_multiplier), int(y2 * box_multiplier), int(x1 * box_multiplier)

        # Draw rectangle and label
        cv2.rectangle(img, (x1, y1), (x2, y2), (0, 255, 0), 2)
        cv2.rectangle(img, (x1, y2 - 20), (x2, y2), (0, 255, 0), cv2.FILLED)
        cv2.putText(img, name, (x1 + 6, y2 - 6), cv2.FONT_HERSHEY_COMPLEX, 0.5, (255, 255, 255), 2)
    
    cv2.imshow('Webcam', img)
    if cv2.waitKey(1) == ord('q'):
        break
    
    frame_count += 1

# Release resources
cap.release()
cv2.destroyAllWindows()
