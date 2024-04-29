#!/bin/bash

rm yerith-erp-3.0-academic/DEBIAN/postinst
yerith-create-dpkg_yerith-erp-3-0.sh -e -v "academic"
