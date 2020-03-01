#!/bin/bash
sed -n '8p' $1>$2 
sed -n -e '32p' -e '128p' -e '512p' -e '1024p' $1>>$2
