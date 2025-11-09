#!/usr/bin/env bash

# Determine the absolute path to this script's directory
TEST_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Paths to bats dependencies
SUPPORT_DIR="${TEST_DIR}/bats-support"
ASSERT_DIR="${TEST_DIR}/bats-assert"
FILE_DIR="${TEST_DIR}/bats-file"

# Verify that dependencies exist
if [ ! -d "$SUPPORT_DIR" ] || [ ! -d "$ASSERT_DIR" ] || [ ! -d "$FILE_DIR" ]; then
  echo "ERROR: Could not find bats-support, bats-assert, or bats-file directories in ${TEST_DIR}" >&2
  exit 1
fi
# Determine the project root directory (one level up from test directory)
PROJECT_ROOT="$(dirname "$TEST_DIR")"


# Load bats dependencies
load "${SUPPORT_DIR}/load.bash"
load "${ASSERT_DIR}/load.bash"
load "${FILE_DIR}/load.bash"

# Add both build/ and project root to PATH
PATH="${PROJECT_ROOT}/build:${PROJECT_ROOT}:$PATH"
export PATH

# Optional global test timeout
export BATS_TEST_TIMEOUT=10