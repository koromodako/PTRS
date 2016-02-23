Q=@
BIN_DIR="bin/"
PLUGINS_DIR="bin/plugins/"
BUILD_DIR="build/"
PLUGINS_BUILD_DIR="calculation_plugins/build/"

all: deploy

deploy:
	${Q}echo "-------------- Deployement --------------"
	${Q}rm -rf ${BIN_DIR}
	${Q}mkdir ${BIN_DIR}
	${Q}mkdir ${PLUGINS_DIR}
	${Q}echo "Exporting binaries..."
	${Q}for dir in ${BUILD_DIR}*; do echo "ln -s ${dir}/debug/${dir:6} ${PLUGINS_DIR}${dir:6}"; done
	${Q}echo "Exporting plugins..."
	${Q}for dir in ${PLUGINS_BUILD_DIR}*; do echo "ln -s ${dir}/debug/${dir:26} ${PLUGINS_DIR}${dir:26}"; done
	${Q}echo "--------------- ! done ! ----------------"

run-server: deploy
	${Q}echo "Running server..."
	${Q}echo "------------------------------------------------------------------------------------"
	${Q}./${BIN_DIR}server 2> ${BIN_DIR}server.log
	${Q}echo "------------------------------------------------------------------------------------"
	${Q}echo "done !"

run-client: deploy
	${Q}echo "Running qt_client..."
	${Q}echo "------------------------------------------------------------------------------------"
	${Q}./${BIN_DIR}qt_client 2> ${BIN_DIR}qt_client.log	
	${Q}echo "------------------------------------------------------------------------------------"
	${Q}echo "done !"

.PHONY: deploy test
