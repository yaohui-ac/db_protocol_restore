#!/bin/bash
systemctl stop postgresql
systemctl start postgresql
python3 ./mysql_site/utils/clean_database.py
