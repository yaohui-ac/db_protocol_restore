from flask import (
    Blueprint,
    redirect,
    render_template,
    url_for
)
from flask_login import login_required

bp = Blueprint(__name__.split('.')[-1], __name__)

@bp.route('/', methods=['GET'], strict_slashes=False)
@login_required
def users():
    return render_template('v1/users.html')