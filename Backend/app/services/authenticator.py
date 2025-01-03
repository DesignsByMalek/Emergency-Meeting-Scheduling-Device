'''
    To authenticate the user with google API services
'''
import os

from typing import Any
from googleapiclient.discovery import build     
from google.oauth2.credentials import Credentials
from google.auth.transport.requests import Request
from google_auth_oauthlib.flow import InstalledAppFlow

class GoogleAuthenticator(object):
    
    def __init__(self, service_name: str='', version: str='') -> object:
        self.__credential_file: str = 'credentials.json'
        if not isinstance(service_name, str) or not isinstance(version, str):
            raise ValueError('Service name and version passed must be of type str')
        
        self.__service_name = service_name
        self.__version = version

    def __authentication_process(self, token_path: str, scope: list[str]):
        creds = None
        if os.path.exists(token_path):
            creds = Credentials.from_authorized_user_file(token_path, scope)

        if not creds or not creds.valid:
            if creds and creds.expired and creds.refresh_token:
                creds.refresh(Request())
            else:
                flow = InstalledAppFlow.from_client_secrets_file(
                    self.credential_file, scope)
                creds = flow.run_local_server(port=0)
            with open(token_path, 'w') as token:
                token.write(creds.to_json())
        return creds

    @property
    def credential_file(self):
        return self.__credential_file
    
    @credential_file.setter
    def credential_file(self, path: str):
        if not isinstance(path, str):
            raise ValueError('Must place path of the credential file of type str')
        self.__credential_file = path

    @property
    def service_name(self) -> str:
        return self.__service_name
    
    @property
    def verion(self) -> str:
        return self.__version

    def get_authenticated(self, kwargs: Any):
        '''
        Pass as dict {``token_file``: ``scope``}
        :param token_file: str
        :param scope: list
        '''
        if len(kwargs) > 1:
            raise ValueError('Must pass one at a time')
        
        return self.__authentication_process(list(kwargs.keys())[0], list(kwargs.values())[0])

    def auth_build(self, name, version, cred):
        '''
        Builds the requried service
        '''
        try:
            service = build(name, version, credentials=cred)
            return service
        
        except Exception as e:
            print(f'Error in building service for {self.service_name}: {e}') 

# debug here
if __name__ == '__main__':
    auth = GoogleAuthenticator()
    auth.credential_file = r'json\credentials.json'
    auth.get_authenticated(**{
        r'json\google_calender.json': ['https://www.googleapis.com/auth/calendar'],
    })