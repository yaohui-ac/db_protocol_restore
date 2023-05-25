#!/bin/bash

killall postgresql
systemctl stop postgresql
killall sniffer
killall flask
