import hashlib
import uuid
from flask_login import UserMixin
from .base_model import BaseModel
from sqlalchemy import Column, Integer, DateTime, String, ForeignKey, Text, Boolean, Date
from core.db import db

# user表
class User(db.Model, BaseModel, UserMixin):
    __table_name__ = "user"
    id = Column(Integer, primary_key=True, autoincrement=True)
    username = Column(String(20), nullable=False, unique=True)
    password = Column(String(500), nullable=False)

    is_active = Column(Boolean, default=True)

    def __init__(self, username, password):
        self.username = username
        self.set_password(password)

    # validate user password
    def validate_password(self, password):
        salt, hash = self.password.split(".")
        return hash == hashlib.sha512(salt.encode() + password.encode()).hexdigest() 
    
    # set user password
    def set_password(self, password):
        salt = uuid.uuid4().hex
        hash = hashlib.sha512(salt.encode() + password.encode()).hexdigest()
        self.password = salt + "." + hash

    def get_id(self):
        return self.id

    @property
    def is_admin(self):
        # 物业公司员工均为管理员
        return self.employee or self.username == "admin"