from flask import (
    Blueprint,
    redirect,
    url_for
)
from flask_login import login_required, logout_user


from core import settings

bp = Blueprint('logout', __name__)
# 设置template_folder参数

@bp.route('/', methods=['GET'], strict_slashes=False)
@login_required
def logout():
    logout_user()
    return redirect(url_for('index'))