from flask_sqlalchemy import SQLAlchemy
from .app import app
from .settings import settings

app.config['SQLALCHEMY_DATABASE_URI'] = 'postgresql://%s:%s@%s:%s/%s' % (
    settings.POSTGRESQL_USER,
    settings.POSTGRESQL_PASSWORD,
    settings.POSTGRESQL_HOST,
    settings.POSTGRESQL_PORT,
    settings.POSTGRESQL_DB
)

db = SQLAlchemy(app)

