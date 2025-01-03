import os
from dotenv import load_dotenv

load_dotenv()

class Config(object):
    SHEET_TOKEN = os.getenv('SHEET_TOKEN')
    CALENDER_TOKEN = os.getenv('CALENDER_TOKEN')
    CREDENTIAL_FILE = os.getenv('CREDENTIAL_FILE')
    GOOGLE_SHEET = os.getenv('GOOGLE_SHEET')


class DevelopmentConfig(Config):
    DEBUG: bool = True


class ProductionConfig(Config):
    DEBUG: bool = False
    TESTING: bool = False