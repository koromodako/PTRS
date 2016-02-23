#!/bin/bash

SRC_DIR=${1}
DEST_DIR=${2}
BUILD_TYPE=${3}
DEPTH=${4}
LEN=${#SRC_DIR}

if [[ "${SRC_DIR}" != "" ]] && [[ "${DEST_DIR}" != "" ]]; then
	for folder in ${SRC_DIR}*; do
		echo -n "Exporting ${DEST_DIR}${folder:${LEN}}..."
		cp ${folder}/${BUILD_TYPE}/${folder:${LEN}} ${DEST_DIR}${folder:${LEN}} && echo "done !"
	done
fi
