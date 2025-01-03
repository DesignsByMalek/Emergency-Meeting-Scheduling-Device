from flask import current_app
from app.services import GoogleAuthenticator

def google_spreadsheet():
    auth = GoogleAuthenticator()
    auth.credential_file = current_app.config['CREDENTIAL_FILE']

    creds = auth.get_authenticated({
        current_app.config['SHEET_TOKEN']: ["https://www.googleapis.com/auth/spreadsheets"]
    })
    service = auth.auth_build("sheets", "v4", cred=creds)

    return creds, service
    

class GoogleSheets(object):
    def __init__(self):
        self.SPREADSHEET_ID = current_app.config['GOOGLE_SHEET']  
        self.__creds, self.__service = google_spreadsheet()

    @property
    def creds(self):
        return self.__creds
    
    @property
    def service(self):
        return self.__service
    
    def get_total_rows(self, sheet_name: str):
        """Fetches the total number of rows in the specified sheet."""
        try:
            sheet_metadata = self.__service.spreadsheets().get(spreadsheetId=self.SPREADSHEET_ID).execute()
            sheet = next((s for s in sheet_metadata['sheets'] if s['properties']['title'] == sheet_name), None)
            if sheet:
                return sheet['properties']['gridProperties']['rowCount']
            
            return 0
        except Exception as err:
            print(f"An error occurred: {err}")
            return 0
    
    def read_data(self, sheet_name, range_):
        """
        Read data from a specific sheet and range.
        :param sheet_name: Name of the sheet (Client Info, SMS Logs)
        :param range_: The range of data to read (e.g., 'A1:C10')
        :return: Data from the sheet
        """
        range_name = f"{sheet_name}!{range_}"
        result = self.service.spreadsheets().values().get(
            spreadsheetId=self.SPREADSHEET_ID, range=range_name
        ).execute()
        return result.get('values', [])

    def read_all_data(self, sheet_name: str):
        """Dynamically reads all data from the specified sheet."""
        total_rows = self.get_total_rows(sheet_name)
        if total_rows > 0:
            range = f'A1:E{total_rows}'
            return self.read_data(sheet_name, range)
        else:
            print(f"No rows found for {sheet_name}")
            return []

    def write_data(self, sheet_name, range_, values):
        """
        Write data to a specific sheet and range.
        :param sheet_name: Name of the sheet (Client Info, SMS Logs)
        :param range_: The range of data to write (e.g., 'A1')
        :param values: The values to write (list of lists)
        """
        range_name = f"{sheet_name}!{range_}"
        body = {
            'values': values
        }
        self.service.spreadsheets().values().update(
            spreadsheetId=self.SPREADSHEET_ID, range=range_name,
            valueInputOption="RAW", body=body
        ).execute()

    def append_data(self, sheet_name, values):
        """
        Append data to the sheet without overwriting. Data will be appended as rows.
        :param sheet_name: Name of the sheet (Client Info, SMS Logs)
        :param values: The values to append (list of lists)
        """
        # Get the last row of the sheet to determine the next empty row
        range_name = f"{sheet_name}!A1"  # Start from A1 to determine last used row
        result = self.service.spreadsheets().values().get(
            spreadsheetId=self.SPREADSHEET_ID, range=range_name
        ).execute()
        
        rows = result.get('values', [])
        # Find the next empty row based on the current number of rows in the sheet
        next_row = len(rows) + 1  # The next row to write in
        
        # Dynamically generate the range to append data, e.g., "A{next_row}:Z{next_row}"
        range_to_append = f"{sheet_name}!A{next_row}"

        # Append the data
        body = {
            'values': values
        }
        self.service.spreadsheets().values().append(
            spreadsheetId=self.SPREADSHEET_ID, range=range_to_append,
            valueInputOption="RAW", body=body
        ).execute()