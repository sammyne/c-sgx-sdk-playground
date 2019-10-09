#!/bin/bash

docker run -it --rm --name sgx \
    -v ${PWD}:/workspace \
    -w /workspace \
    registry.baidu.com/bbe/ubuntu:sgx-2.6 bash
