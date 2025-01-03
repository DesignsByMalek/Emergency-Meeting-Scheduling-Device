
# Remote Emergency Meeting Scheduling Device

This project is an open-source IoT solution developed to address the need for efficient and reliable scheduling of emergency meetings. Designed for seamless operation, it automates the process of setting up Google Meet sessions, sending SMS triggers, and dispatching email notifications to clients and team members.

---

## Overview

The system consists of two main components:
1. **Backend Server**:
   - Developed by Karim Jibai, this handles SMS received from a GSM module.
   - Integrates with Google Forms and Sheets to manage client data.
   - Sends email notifications with meeting details.

2. **IoT Device**:
   - A physical device sends an SMS trigger to the backend server when activated.
   - Built for reliability with a battery life exceeding one year.
   - Several iterations were made to refine the design for functionality and aesthetics.
   - All digital files needed for manufacturing, including PCB design, 3D models, and schematics, are included in the repository along with a detailed project document.

---

## Features

### Backend Capabilities:
- **SMS Trigger**: Integrated with Twilio to receive and process SMS via an international number.
- **Google Workspace Integration**: Automates Google Meet scheduling and retrieves client data from Google Forms and Sheets.
- **Email Notifications**: Sends meeting details promptly via email to all participants.
- **Scalability**: Designed to handle multiple requests in real-time.

### IoT Device Highlights:
- **Battery Efficiency**: Designed to operate for over a year without maintenance.
- **Robust Communication**: Reliable GSM connectivity ensures consistent performance.
- **Refined Design**: Developed through multiple iterations for optimized functionality and aesthetics.

### Future Additions:
- Optimization for production environments.
- Additional notification and customization options.

---

## Why Use This Project?

- **Emergency Scheduling**: Automates meeting setups for critical scenarios, ensuring timely communication.
- **Customizable Backend**: Adapt the system for other use cases or triggers.
- **Complete Manufacturing Resources**: Includes all necessary files for replicating the device.
- **Educational Resource**: Learn about Twilio integration, Google Workspace APIs, and backend server design.

---

## Installation

### Prerequisites
- Python 3.8+
- Twilio account with an international number.
- Google Workspace API access (Forms and Sheets).
- Server environment (local or cloud-based).

### Setup
1. Clone the repository:
   ```bash
   git clone https://github.com/your-username/remote-emergency-meeting-scheduler.git
   cd remote-emergency-meeting-scheduler
   ```
2. Install required packages:
   ```bash
   pip install -r requirements.txt
   ```
3. Configure:
   - Add your Twilio API keys and Google Workspace credentials in `config.py`.
   - Verify server settings for your environment.

4. Run the backend server:
   ```bash
   python server.py
   ```

---

## Usage

1. Activate the trigger (SMS received via Twilio).
2. The backend processes the request and:
   - Retrieves client details from Google Forms/Sheets.
   - Schedules a Google Meet session.
   - Sends email notifications with meeting details.

---

## License

This project is licensed under the [Apache 2.0 License](LICENSE). 

### Key Points of the Apache 2.0 License:
- **Permissive Use**: Allows users to use, modify, and distribute the software in both private and commercial applications.
- **Attribution Required**: Modifications and redistributed works must provide attribution to the original authors.
- **Warranty Disclaimer**: Distributed as-is, without warranty or liability.

By using this license, we encourage innovation and collaboration while ensuring proper credit is given to contributors.

---

## Contribution

Contributions are encouraged! If you:
- Identify bugs.
- Have ideas for improvements or extensions.
- Wish to adapt the system for new use cases.

Please fork the repository and submit a pull request.

---

This repository offers a reliable solution for automating critical meeting setups and serves as a foundation for building IoT-integrated backend systems. With the inclusion of all necessary manufacturing files and a comprehensive project document, this project is ready for replication, exploration, and adaptation to your needs. Special thanks to Karim Jibai for developing the backend system that powers this solution.
