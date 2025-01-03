from datetime import datetime
from flask import Blueprint, request, jsonify
from app.modules import SheetService, calender_invite

bp: Blueprint = Blueprint(name='sms', import_name=__name__, url_prefix='/emergency-button')

# POST /emergency-button/inbound-emergency
@bp.route('/inbound-emergency', methods=['POST'])
def inbound_sms():
    try:
        g = SheetService()
        from_user = request.form.get('From')
        to_user = request.form.get('To')
        body = request.form.get('Body')
        timestamp = datetime.now().isoformat()

        button_id, event_type, battery_health = str(body).split(',')

        g.append_data(sheet_name='SMS Logs', 
                       values=[[button_id, from_user, to_user, timestamp, battery_health, event_type]])
        
        if str(event_type).upper().strip() != "BATTERY_HEALTH":
            client_info_data = g.read_all_data('Client Info')              
            email = None
            for row in client_info_data:
                if row[3] == button_id:
                    email = row[4]  
                    break
            
            if email:
                calender_invite([str(email).strip() for email in str(email).split(',')])

        return jsonify({'Message': 200,
                        'Datetime': timestamp})

    except Exception as _:
        return jsonify({"Failed to process SMS": 500})
