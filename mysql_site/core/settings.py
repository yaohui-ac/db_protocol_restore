import os

# 配置信息
class Settings(object):

    # 项目名称
    PROJECT_NAME = 'MYSQL网络报文还原系统'

    # 项目版本
    PROJECT_VERSION = '1.0.0'

    # 项目描述
    PROJECT_DESCRIPTION = '网络报文还原系统'

    # POSTGRESQL数据库配置
    POSTGRESQL_HOST = os.environ.get('POSTGRESQL_HOST', '127.0.0.1')
    POSTGRESQL_PORT = os.environ.get('POSTGRESQL_PORT', 5432)
    POSTGRESQL_USER = os.environ.get('POSTGRESQL_USER', 'postgres')
    POSTGRESQL_PASSWORD = os.environ.get('POSTGRESQL_PASSWORD','postgres')
    POSTGRESQL_DB = os.environ.get('POSTGRESQL_DB', 'mysql_protocol_restore')

    DEBUG = True

    # SECRET_KEY
    SECRET_KEY = os.environ.get('SECRET_KEY','f298460206214c3a81b58590a2a6560b')

    # BASE_DIR
    BASE_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

    # TEMPLATE_FOLDER
    TEMPLATE_FOLDER = os.path.join(BASE_DIR, 'templates')

    STATIC_FOLDER = os.path.join(BASE_DIR, 'static')
    CSV_FOLDER = os.path.join(STATIC_FOLDER, 'csv')

settings = Settings()