#!/usr/bin/env bash

gcc gt4.c -o frontend `pkg-config --cflags --libs gtk+-3.0` && ./frontend