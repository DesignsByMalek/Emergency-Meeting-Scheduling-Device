from flask import current_app
from app.services import GoogleAuthenticator
from datetime import timedelta, datetime, timezone

def set_calendar(invitees: list[str]):
    auth = GoogleAuthenticator()

    auth.credential_file = current_app.config['CREDENTIAL_FILE']  
    creds = auth.get_authenticated({
        current_app.config['CALENDER_TOKEN']: ['https://www.googleapis.com/auth/calendar']
    })

    service = auth.auth_build('calendar', 'v3', cred=creds)
    
    now = datetime.now(timezone.utc)
    duration = timedelta(hours=1)
    time_zone = 'UTC'  # Replace with your time zone if needed

    start_time = now
    end_time = start_time + duration
    attendees = [{'email': email} for email in invitees]

    # Create event details
    event = {
        'summary': 'Emergency Meeting',
        'location': 'Google Meet',
        'description': 'Emergency Button has been pressed. Emergency Meeting has been set!',
        'start': {
            'dateTime': start_time.isoformat(),
            'timeZone': time_zone,
        },
        'end': {
            'dateTime': end_time.isoformat(),
            'timeZone': time_zone,
        },
        'attendees': attendees,  
        'reminders': {
            'useDefault': False,
            'overrides': [
                {'method': 'email', 'minutes': 30},
                {'method': 'popup', 'minutes': 10},
            ],
        },
        'conferenceData': {
            'createRequest': {
                'conferenceSolutionKey': {'type': 'hangoutsMeet'},
                'requestId': 'some-random-string'
            }
        }
    }

    event = service.events().insert(
        calendarId='primary',
        body=event,
        conferenceDataVersion=1,
        sendUpdates='all'
    ).execute()