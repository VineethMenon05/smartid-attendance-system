import os
import cv2

# Define the directory for saving faces
faces_dir = os.path.join(os.path.dirname(os.path.abspath(__file__)), "faces")

# Create 'faces' directory if not exists
if not os.path.exists(faces_dir):
    os.makedirs(faces_dir)
    print(f"Directory '{faces_dir}' created")

# Take input for the person's name
name = input("Enter name: ").strip().upper()

# Open camera
cap = cv2.VideoCapture(0)

# Check if camera is working
if not cap.isOpened():
    print("❌ Error: Camera not detected!")
    exit()

print("\n📸 Press 'c' to capture image")
print("❌ Press 'q' to quit\n")

while True:
    ret, frame = cap.read()

    if not ret:
        print("❌ Failed to capture frame!")
        break

    # Show camera feed
    cv2.imshow("Camera - Press C to Capture", frame)

    # Detect key press (IMPORTANT FIX)
    key = cv2.waitKey(1) & 0xFF

    # Capture image
    if key == ord('c'):
        filename = os.path.join(faces_dir, f"{name}.jpg")

        # Optional overwrite warning
        if os.path.exists(filename):
            print("⚠️ Image already exists. Overwriting...")

        cv2.imwrite(filename, frame)
        print(f"✅ Image Saved: {filename}")

        # Small delay to prevent double capture
        cv2.waitKey(300)

    # Quit
    elif key == ord('q'):
        print("👋 Exiting...")
        break

# Release resources
cap.release()
cv2.destroyAllWindows()

print("\n✅ Capture completed.")
