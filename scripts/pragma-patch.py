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

include_guard = re.compile(r'.*HPP_INCLUDED\n')
enclosed_pragma_once = re.compile(r'\n// MS compatible.*\n\n#if.*_MSC_VER.*\n# pragma once\n#endif')

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

if __name__ == '__main__':
  for root, dirs, files in os.walk('code'):
    for file in files:
      if Path(file).suffix in ['.hpp', '.h', '.hxx']:
        patch_file(Path(root) / file)
