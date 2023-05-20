from datetime import datetime
from sqlalchemy import Column, Integer, DateTime, String, ForeignKey, Text, Boolean

class BaseModel(object):
    id = Column(Integer, primary_key=True, autoincrement=True)
    created_at = Column(DateTime, nullable=False, default=datetime.now)
    updated_at = Column(DateTime, nullable=False, default=datetime.now, onupdate=datetime.now)

    def __init__(self):
        self.created_at = datetime.now()
        self.updated_at = datetime.now()

    
    def fields_with_value(self,fields = None):
        if fields is None:
            fields = self.form_fields()

        for item in fields:
            if hasattr(self,item['name']):
                item['value'] = getattr(self,item['name'])
        return fields
    
    def form_fields(self):
        return self.fields_with_value()
    
    @classmethod
    def form_fields(cls):
        return []
    
    @classmethod
    def search_form_fields(cls, params_value={}):
        return []
        

    def detail_dict(self):
        return self.fields_with_value()
    
    def update(self, data):
        for key in data:
            if hasattr(self,key):
                setattr(self,key,data[key] or None)
        self.updated_at = datetime.now()

    def edit_form_fields(self):
        return self.fields_with_value()
    
    @classmethod
    def add_form_fields(cls):
        return cls.form_fields()
    
    def set_attrs(self, attrs):
        for key in attrs:
            if hasattr(self,key):
                setattr(self,key,attrs[key] or None)
        self.updated_at = datetime.now()