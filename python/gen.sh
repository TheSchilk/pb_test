#!/bin/sh
protoc -I.. --python_out=. ../cmd.proto
