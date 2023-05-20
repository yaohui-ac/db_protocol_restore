from flask_login import login_user
from core import settings
from models import User
from flask import request, Blueprint, render_template, redirect, url_for, flash

bp = Blueprint('login', __name__, url_prefix='/login')

@bp.route('/', methods=['GET', 'POST'])
def login():
    title = settings.PROJECT_NAME
    if request.method == 'GET':
        error = request.args.get('error')
        return render_template('login.html', error=error, title=title)
    elif request.method == 'POST':
        username = request.form.get('username')
        password = request.form.get('password')
        user = User.query.filter_by(username=username).first()
        if user is None or not user.validate_password(password):
            error = '用户名/密码错误'
            return redirect(url_for('auth.login.login', error=error))
        login_user(user)
        return redirect(url_for('index'))
    else:
        error = 'Invalid request method'
        return redirect(url_for('auth.login.login', error=error, title=title))