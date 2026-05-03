# 🧠 Smart ID Attendance System

A full-stack **attendance tracking system** that uses **Face Recognition, Arduino (RFID), and Google Apps Script** to automate attendance marking and store data in real-time.

---

## 📌 Overview

The Smart ID Attendance System is designed to eliminate manual attendance processes by combining **Computer Vision + IoT + Cloud Integration**.

- 🎥 Detects and recognizes faces using OpenCV  
- 🔌 Supports RFID-based attendance via Arduino  
- ☁️ Stores attendance data in Google Sheets using Apps Script  
- 🌐 Provides a Flask-based web dashboard  

---

## 🚀 Features

- 🎥 Real-time face detection using OpenCV  
- 🧠 Face recognition system  
- 📝 Automated attendance marking  
- 🌐 Web-based dashboard using Flask  
- 🔌 IoT / RFID module integration  
- ☁️ Google Sheets integration via Apps Script  
- 📸 Image capture functionality  

---

## 🛠 Tech Stack

- **Backend:** Python, Flask  
- **Computer Vision:** OpenCV  
- **Frontend:** HTML, CSS  
- **Hardware:** Arduino + RFID Module (RC522)  
- **Cloud Integration:** Google Apps Script + Google Sheets  
- **Libraries:** NumPy, face-recognition  

---

## 📂 Project Structure
smartid-attendance-system/
├── backend/ # Python backend & face recognition logic
├── templates/ # HTML templates
├── static/ # CSS, images, logo
├── screenshots/ # Project screenshots
├── arduino/ # Arduino RFID code
├── google-apps-script/ # Google Sheets integration script
├── requirements.txt
└── README.md

## 🧠 System Architecture


Camera → Face Recognition (OpenCV) → Flask Backend → Google Apps Script → Google Sheets
Arduino (RFID) → Flask Backend → Google Apps Script → Google Sheets

## 📄 Research Publication

📌 **Title:** SmartID: IoT-Based Smart Attendance System with Dual Authentication Using Face Recognition and RFID  

👨‍💻 **Authors:** Vinay Kumar Gajendra, Ritik Kumar, Vineeth Menon, Jason Ralph Das, Neha Choubey  

🏫 **Institution:** Shri Shankaracharya Technical Campus  

📚 **Journal:** International Journal of Creative Research Thoughts (IJCRT)  

📅 **Published:** March 2026  

🆔 **ISSN:** 2320-2882  

---

### 📖 Abstract

This research presents **SmartID**, an IoT-based smart attendance system integrating **face recognition and RFID-based dual authentication** to improve accuracy and security.  

The system uses:
- 📷 Face Recognition (Python, OpenCV)
- 📡 RFID Authentication
- 🌐 NodeMCU (ESP8266) for IoT communication
- ☁️ Google Sheets for cloud storage
- 📧 Email notifications for transparency  

This approach minimizes proxy attendance, automates tracking, and provides real-time data access.

---

### 🔗 Access the Paper

📥 [Download Full Research Paper](research-paper/Smart_ID_Attendance_System_Research_Paper.pdf)

---

### 🚀 Key Contributions

- 🔐 Dual Authentication (Face + RFID)
- 🌐 IoT-based real-time system using ESP8266
- ☁️ Cloud integration with Google Sheets
- 📧 Automated email alerts for attendance
- ⏱️ Login/Logout tracking with duration calculation
- 💰 Cost-effective & scalable solution

---

### 📊 System Architecture

The system integrates hardware and software components including:

- Face Recognition Module (Python)
- RFID Reader Module
- NodeMCU (ESP8266)
- Google Apps Script (Backend)
- Google Sheets (Database)

---

### 📈 Results

- ✔️ Accurate attendance tracking
- ✔️ Real-time cloud updates
- ✔️ Reduced proxy attendance
- ✔️ Efficient and scalable implementation

---

### 🔮 Future Scope

- 📱 Mobile/Web dashboard
- 🤖 Advanced face recognition models
- 🗄️ Database integration (MySQL/PostgreSQL)
- 🔍 Liveness detection for enhanced security

  
## ▶️ How to Run

### 🔹 1. Clone the repository
``bash
git clone https://github.com/your-username/smartid-attendance-system.git
cd smartid-attendance-system

###2. Install dependencies
pip install -r requirements.txt

##3. Run the Flask app
python backend/app.py

###4. Open in browser
http://127.0.0.1:5000/

###📂 Location
arduino/
⚙️ Hardware Used
Arduino Uno / NodeMCU
RFID Module (RC522)
Jumper wires
Bread Board
Buzzer
LCD Display

▶️ Steps
Open .ino file in Arduino IDE
Connect RFID module
Upload code
Monitor serial output

☁️ Google Apps Script Integration
📂 Location
google-apps-script/attendance_script.js
⚙️ Functionality
Receives attendance data from backend
Stores data in Google Sheets
Acts as a lightweight API

▶️ Setup
Open Google Apps Script
Paste the code
Deploy as Web App
Copy Web App URL
Use it in your backend

## 📸 Screenshots

### 🖥 Dashboard
![Dashboard](screenshots/SmartID%20Attendance.PNG)

### 📊 Attendance System
![Attendance](screenshots/SmartID%20Attendance2.PNG)

### 📊 Attendance View 2
![Attendance](screenshots/SmartID%20Attendance3.PNG)

### 🔌 IoT / RFID Module
![IoT Module](screenshots/IOT_Module.jpeg)

🔮 Future Improvements
🔐 User authentication system
📱 Mobile app integration
🌐 Cloud deployment (AWS / Render)
⚡ Convert backend to FastAPI
📊 Analytics dashboard

👨‍💻 Author

Vineeth Menon
🔗 [LinkedIn](https://www.linkedin.com/in/vineeth-menon-09b911287/)
