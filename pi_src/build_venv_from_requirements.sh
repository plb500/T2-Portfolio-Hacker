#!/bin/bash

if [ "$#" -ne  "1" ]
  then
    echo "Invalid arguments"
    echo "Usage: $0 <requirements file>"
    exit 1
fi

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

VENV_NAME="cyberdyne-vewnv"
VENV_LOCATION="${SCRIPT_DIR}/${VENV_NAME}"

python3 -m venv --system-site-packages "${VENV_LOCATION}"
source "${VENV_LOCATION}/bin/activate"
pip3 install -r "$1"

echo "Installed virtual environment in: ${VENV_LOCATION}"