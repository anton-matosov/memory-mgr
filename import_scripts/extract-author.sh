#!/usr/bin/env bash
authors=$(svn log -q $@ | grep -e '^r[0-9]' | awk 'BEGIN { FS = "|" } ; { print $2 } $1 ~ /r([0-9]+000)/ { print "fetched revision " substr($1, 2) > "/dev/stderr" }' | sort | uniq)
for author in ${authors}; do
echo "${author} = ${author}_name <${author}_mail@${author}_domain.com>";
done
