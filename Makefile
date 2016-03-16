Q=@
BIN_DIR=bin/
PLUGINS_DIR=bin/plugins/
BUILD_DIR=build/
PLUGINS_BUILD_DIR=calculation_plugins/build/
BUILD_TYPE=debug

all: deploy

deploy:
	${Q}echo "-------------- Deployement --------------"
	${Q}rm -rf ${BIN_DIR}
	${Q}mkdir ${BIN_DIR}
	${Q}mkdir ${PLUGINS_DIR}
	${Q}echo "Export binaries..."
	${Q}./export.sh ${BUILD_DIR} ${BIN_DIR} ${BUILD_TYPE} ../
	${Q}echo "Export plugins..."
	${Q}./export.sh ${PLUGINS_BUILD_DIR} ${PLUGINS_DIR} ${BUILD_TYPE} ../../
	${Q}echo "--------------- ! done ! ----------------"

run-server:
	${Q}echo "Running server..."
	${Q}echo "------------------------------------------------------------------------------------"
	${Q}./${BIN_DIR}server 2> ${BIN_DIR}server.log
	${Q}echo "------------------------------------------------------------------------------------"
	${Q}echo "done !"

run-client:
	${Q}echo "Running qt_client..."
	${Q}echo "------------------------------------------------------------------------------------"
	${Q}./${BIN_DIR}qt_client 2> ${BIN_DIR}qt_client.log	
	${Q}echo "------------------------------------------------------------------------------------"
	${Q}echo "done !"

run-test-env: deploy
	${Q}xterm -hold -e 'make run-server' &
	${Q}xterm -hold -e 'make run-client' &

.PHONY: deploy test
