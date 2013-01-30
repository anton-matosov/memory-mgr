#! /bin/sh

svnadmin create svn-mirror

echo '#!/bin/sh' > `PWD`/svn-mirror/hooks/pre-revprop-change && chmod +x svn-mirror/hooks/pre-revprop-change
# do the actual sync
svnsync init file://`PWD`/svn-mirror https://memory-mgr.svn.sourceforge.net/svnroot/memory-mgr
svnsync sync file://`PWD`/svn-mirror
