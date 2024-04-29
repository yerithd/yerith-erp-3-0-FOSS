
set -x

qdbusxml2cpp -c IYRIruntimeverificationAdaptor_Interface \
	     -p src/IYRIruntimeverificationAdaptor_interface.h:src/IYRIruntimeverificationAdaptor_interface.cpp \
	        src/dbus/yri.db-runtime.verif.xml
