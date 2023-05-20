from flask import redirect, url_for
from flask_login import LoginManager
from .app import app

# set login_manager for flask_login
login_manager = LoginManager()

# login_manager.init_app(app)

# set login_view for flask_login
login_manager.login_view = 'auth_login'

# set login_message for flask_login
login_manager.login_message = 'Please login to access this page'

# set login_message_category for flask_login
login_manager.login_message_category = 'info'

# confifure flask_login to use the user_loader callback from models
from models import User

@login_manager.user_loader
def load_user(user_id):
    return User.query.get(int(user_id))

# configure flask_login to use the unauthorized callback from api/auth/login.py
@login_manager.unauthorized_handler
def unauthorized():
    return redirect(url_for('auth.login.login'))