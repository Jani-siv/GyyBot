#!/bin/bash
sha256 $1 > sha.txt
base64 sha.txt > base1.txt
base=$(<base1.txt)

sha256 "$base" + $2 > sha.txt
base64 sha.txt > baseready.txt
rm sha.txt
rm base1.txt


