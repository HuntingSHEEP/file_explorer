#!/usr/bin/env bash

gcc gt4.c -o SheepVIEW `pkg-config --cflags --libs gtk+-3.0` && ./SheepVIEW