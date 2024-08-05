#!/usr/bin/env python

import re
import os
from pathlib import Path

"""
This script is used to patch the include guards in the header files of the project.
It replaces the include guards with #pragma once and removes the old include guards.

Example:
```
#ifndef MEMORY_MGR_TYPE_INFO_HPP_INCLUDED
#define MEMORY_MGR_TYPE_INFO_HPP_INCLUDED

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

... // code


#endif // #ifndef MEMORY_MGR_TYPE_INFO_HPP_INCLUDED
```

Will be patched to:
```
#pragma once

... // code
```
"""

include_guard = re.compile(r'.*(_HPP_INCLUDED|_HEADER|_HPP)[\n]*')
enclosed_pragma_once = re.compile(r'(\n// MS compatible.*\n\n)*#if.*_MSC_VER.*\n#\spragma once\n#endif')
pragma_once_only = re.compile(r'#pragma once')

is_verbose = False

def verbose(*args, **kwargs):
  if is_verbose:
    print(*args, **kwargs)

def patch_file(file_path):
  try:
    with open(file_path, 'r') as f:
      data = f.read()
  except Exception as e:
    print(f'Error reading {file_path}: {e}')
    return

  if enclosed_pragma_once.search(data):
    print(f'Patching {file_path}')
    data = enclosed_pragma_once.sub('#pragma once', data)
    data = include_guard.sub('', data)

    with open(file_path, 'w') as f:
      f.write(data)
  else:
    verbose(f'File {file_path} already patched')


# Puts pragma once at the top of the file if it's not there
# doesn't look good with license comments
def ensure_pragma_once(file_path):
  try:
    with open(file_path, 'r') as f:
      data = f.read()
  except Exception as e:
    print(f'Error reading {file_path}: {e}')
    return

  if not pragma_once_only.search(data):
    print(f'Patching {file_path} with pragma once')
    data = '#pragma once\n' + data
    with open(file_path, 'w') as f:
      f.write(data)

if __name__ == '__main__':
  for root, dirs, files in os.walk('code'):
    for file in files:
      if Path(file).suffix in ['.hpp', '.h', '.hxx']:
        patch_file(Path(root) / file)
