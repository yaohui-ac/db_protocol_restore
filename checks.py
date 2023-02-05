#!/usr/bin/python3

import os

if os.system("which mongod") != 0:
    print("error: mongodb not exist")
    exit(1)
else:
    print("mongoDB exists -- ok")