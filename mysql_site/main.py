from core import app
from models import *
from api import *
    

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000, debug=True)