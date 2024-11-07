#!/usr/bin/env sh

kill -9 "$(cat simple-server.pid)"
rm simple-server.pid
