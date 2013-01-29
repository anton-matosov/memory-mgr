#! /bin/sh

svnadmin create svn-mirror

echo '#!/bin/sh' > `PWD`/svn-mirror/hooks/pre-revprop-change && chmod +x svn-mirror/hooks/pre-revprop-change
# do the actual sync
svnsync init file://`PWD`/svn-mirror https://svn.code.sf.net/p/genericstl/code --username=shikin
svnsync sync file://`PWD`/svn-mirror
