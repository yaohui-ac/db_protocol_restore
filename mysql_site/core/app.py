import flask
from .settings import settings

app = flask.Flask(__name__)

app.config['DEBUG'] = settings.DEBUG

# SECRET_KEY
app.config['SECRET_KEY'] = settings.SECRET_KEY

# set the static folder
app.static_folder = settings.STATIC_FOLDER