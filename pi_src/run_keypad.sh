#!/bin/bash

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )


VENV_NAME="cyberdyne-venv"
VENV_DIR=${SCRIPT_DIR}/${VENV_NAME}

source ${VENV_DIR}/bin/activate
export PYTHONPATH="${SCRIPT_DIR}"
python3 "${SCRIPT_DIR}/keycard_lock/keycard_app.py" "$@"
