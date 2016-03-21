Q=@
BIN_DIR=bin/
PLUGINS_DIR=${BIN_DIR}plugins/
SERVER_DIR=${BIN_DIR}
CLIENT_DIR=${BIN_DIR}
BUILD_TYPE=Debug
BUILD_DIR=build/${BUILD_TYPE}/
SERVER_BIN=server/${BUILD_DIR}server
CLIENT_BIN=clients/qt_client/${BUILD_DIR}qt_client
PLUGIN_BIN=calculation_plugins/bruteforce/${BUILD_DIR}bruteforce
LOGS_DIR=${BIN_DIR}/logs/

.PHONY: run-server run-client run-test-env

all: deploy

deploy:
	${Q}echo "-------------- Deployement --------------"
	${Q}if [ -d ${BIN_DIR} ]; then rm -rf ${BIN_DIR}; fi
	${Q}if [ ! -d ${BIN_DIR} ]; then mkdir ${BIN_DIR}; fi
	${Q}if [ ! -d ${PLUGINS_DIR} ]; then mkdir ${PLUGINS_DIR}; fi
	${Q}if [ ! -d ${SERVER_DIR} ]; then mkdir ${SERVER_DIR}; fi
	${Q}if [ ! -d ${CLIENT_DIR} ]; then mkdir ${CLIENT_DIR}; fi
	${Q}if [ ! -d ${LOGS_DIR} ]; then mkdir ${LOGS_DIR}; fi
	${Q}echo "Export server..."
	${Q}cp ${SERVER_BIN} ${SERVER_DIR}
	${Q}echo "Export qt_client..."
	${Q}cp ${CLIENT_BIN} ${CLIENT_DIR}
	${Q}echo "Export bruteforce plugin..."
	${Q}cp ${PLUGIN_BIN} ${PLUGINS_DIR}
	${Q}echo "--------------- ! done ! ----------------"

run-server:
	${Q}echo "Running server..."
	${Q}echo "------------------------------------------------------------------------------------"
	${Q}./${SERVER_DIR}server 2> ${LOGS_DIR}server.log
	${Q}echo "------------------------------------------------------------------------------------"
	${Q}echo "done !"

run-client:
	${Q}echo "Running qt_client..."
	${Q}echo "------------------------------------------------------------------------------------"
	${Q}./${CLIENT_DIR}qt_client 2> ${LOGS_DIR}qt_client.log	
	${Q}echo "------------------------------------------------------------------------------------"
	${Q}echo "done !"

run-test-env: deploy
	${Q}xterm -hold -e 'make run-server' &
	${Q}xterm -hold -e 'make run-client' &


