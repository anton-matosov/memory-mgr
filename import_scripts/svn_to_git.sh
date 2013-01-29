#! /bin/sh

git svn init file://`PWD`/svn-mirror -s --prefix=svn/

git config svn.authorsfile `pwd`/svn.authors

cp -R `pwd`/.git `pwd`/.git_bak1

git svn fetch --authors-prog=`pwd`/author-generate
git svn show-ignore > .gitignore


cp -R `pwd`/.git `pwd`/.git_bak2

git svn-abandon-fix-refs

mkdir 'pwd'/.git/info/grafts
git svn-abandon-cleanup

git add .gitignore
git commit -m "finalized the import - added .gitignore file"
