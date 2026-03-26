#!/usr/bin/env bash
set -euo pipefail

# Usage:
#   ./scripts/codex_bootstrap_and_compile.sh [yaml_file]
# Default YAML:
#   hello_world_ci.yaml

YAML_FILE="${1:-hello_world_ci.yaml}"

find_ca_bundle() {
  for candidate in \
    /etc/ssl/certs/ca-certificates.crt \
    /etc/pki/tls/certs/ca-bundle.crt \
    /etc/ssl/ca-bundle.pem
  do
    if [ -f "$candidate" ]; then
      printf '%s\n' "$candidate"
      return 0
    fi
  done
  return 1
}

if ! command -v python >/dev/null 2>&1; then
  echo "python is required" >&2
  exit 1
fi

CA_BUNDLE="${CA_BUNDLE:-}"
if [ -n "$CA_BUNDLE" ] && [ ! -f "$CA_BUNDLE" ]; then
  echo "CA_BUNDLE is set but file does not exist: $CA_BUNDLE" >&2
  exit 1
fi

if [ -z "$CA_BUNDLE" ] && CA_BUNDLE="$(find_ca_bundle)"; then
  :
fi

if [ -n "$CA_BUNDLE" ]; then
  export SSL_CERT_FILE="$CA_BUNDLE"
  export REQUESTS_CA_BUNDLE="$CA_BUNDLE"
  export CURL_CA_BUNDLE="$CA_BUNDLE"
  export PIP_CERT="$CA_BUNDLE"
  git config --global http.sslCAInfo "$CA_BUNDLE" || true
  echo "Using CA bundle: $CA_BUNDLE"
else
  echo "No system CA bundle detected; continuing with defaults."
fi

python -m pip install --upgrade esphome

if [ ! -f "$YAML_FILE" ]; then
  echo "YAML file not found: $YAML_FILE" >&2
  exit 1
fi

python -m esphome compile "$YAML_FILE"
