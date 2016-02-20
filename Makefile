Q=@

all: deploy

deploy:
	${Q}echo -n "Deployement..."
	${Q}rm -rf "bin"
	${Q}mkdir bin
	${Q}ln -s ../build/server/debug/server ./bin/server
	${Q}ln -s ../build/qt_client/debug/qt_client ./bin/qt_client
	${Q}echo "done !"

run-server: deploy
	${Q}echo "Running server..."
	${Q}echo "------------------------------------------------------------------------------------"
	${Q}./bin/server 2> bin/server.log
	${Q}echo "------------------------------------------------------------------------------------"
	${Q}echo "done !"

run-client: deploy
	${Q}echo "Running qt_client..."
	${Q}echo "------------------------------------------------------------------------------------"
	${Q}./bin/qt_client 2> bin/qt_client.log	
	${Q}echo "------------------------------------------------------------------------------------"
	${Q}echo "done !"

.PHONY: deploy test
