# this file deal with the workflow of user register

from flask import (
    Blueprint,
    redirect,
    render_template,
    request,
    jsonify,
    url_for
)
from flask_login import login_user
from core import db,settings
from models import User

bp = Blueprint('register', __name__, url_prefix='/register')
bp.template_folder = settings.TEMPLATE_FOLDER

@bp.route('/', methods=['POST'], strict_slashes=False)
def register():
    data = request.form
    user = User.query.filter_by(username=data['username']).first()
    if user:
        error = '用户名已存在'
        return redirect(url_for('auth.register.register_page', error=error))
    user = User(
        username=data['username'],
        password=data['password']
    )
    db.session.add(user)
    db.session.commit()
    login_user(user)
    return redirect(url_for('index'))

@bp.route('/', methods=['GET'], strict_slashes=False)
def register_page():
    title = settings.PROJECT_NAME
    error = request.args.get('error')
    return render_template('register.html', error=error, title=title)