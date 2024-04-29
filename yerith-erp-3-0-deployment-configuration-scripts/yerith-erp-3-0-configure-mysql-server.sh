#!/bin/bash
# @author: Dr.-Ing. XAVIER NOUNDOU

sudo service mysql stop

sudo /usr/bin/mysqld_safe &

sleep 5s

sudo expect /tmp/yerith-erp-3-0-configure-mysql-server-set-root-pwd.exp
