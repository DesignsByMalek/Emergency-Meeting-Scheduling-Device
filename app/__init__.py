import os
from flask import Flask
from app.routes import sms_bp
from app.config import ProductionConfig, DevelopmentConfig

def create_app() -> Flask:
    app = Flask(__name__)

    if os.getenv('FLASK_ENV', 'development') == 'production':
        app.config.from_object(ProductionConfig)
    else:
        app.config.from_object(DevelopmentConfig)

    app.register_blueprint(sms_bp)
    
    return app
