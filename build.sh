#!/bin/bash
#
# Copyright (c) 2013-2021 Igor Pecovnik, igor.pecovnik@gma**.com
#

SRC="$(dirname "$(realpath "${BASH_SOURCE[0]}")")"
EXTER="${SRC}/external"

cd "${SRC}" || exit

source "${SRC}"/scripts/general.sh

prepare_host_basic

toolchain="${SRC}/toolchains/bin"

CONFIG="userpatches/config-build.conf"
CONFIG_FILE="$(realpath "${CONFIG}")"
CONFIG_PATH=$(dirname "${CONFIG_FILE}")

display_alert "Using config file" "${CONFIG_FILE}" "info"
pushd "${CONFIG_PATH}" > /dev/null || exit

source "${CONFIG_FILE}"
popd > /dev/null || exit

USERPATCHES_PATH="${CONFIG_PATH}"

[[ $# == 1 ]] && export $1 
[[ $# == 2 ]] && export $1 && export $2

source "${SRC}"/scripts/main.sh
